// Browser-only campaign state and progression rules.
// This module deliberately cannot mutate Unreal save data or award canon quest flags.
export const SAVE_VERSION = 1;
export const MAX_HEALTH = 6;
export const normalize = (v) => String(v ?? "").replace(/[^a-zA-Z0-9_-]/g, "");
export function chapterState() {
  return { evidence: [], spoken: false, visits: 0, mechanism: false, choice: null,
    side: { stage: 0, choice: null }, guardian: null, techniques: { observed: false, understood: false, experimented: false, mastered: false } };
}
export function newCampaign(realms, profile = 1) {
  if (!Array.isArray(realms) || realms.length !== 9) throw new Error("Nine authored realms required");
  return { version: SAVE_VERSION, profile, current: realms[0].id, position: { x: 12, y: 78 },
    health: MAX_HEALTH, chapters: Object.fromEntries(realms.map(r => [r.id, chapterState()])),
    journey: [], endingPreview: null, revision: 0 };
}
export function restoreCampaign(raw, realms) {
  if (!raw || raw.version !== SAVE_VERSION || !raw.chapters || !realms.every(r => raw.chapters[r.id])) return null;
  const clean = newCampaign(realms, Number(raw.profile) || 1);
  if (!realms.some(r => r.id === raw.current)) return null;
  clean.current = raw.current;
  clean.health = Math.max(1, Math.min(MAX_HEALTH, Number(raw.health) || MAX_HEALTH));
  clean.position = {
    x: Math.max(4, Math.min(96, Number(raw.position?.x) || 12)),
    y: Math.max(4, Math.min(96, Number(raw.position?.y) || 78)) };
  for (const r of realms) {
    const old = raw.chapters[r.id], c = clean.chapters[r.id];
    if (!Array.isArray(old.evidence)) return null;
    c.evidence = [...new Set(old.evidence.filter(id => r.evidence.some(e => e.id === id)))];
    c.spoken = old.spoken === true; c.visits = Math.min(10000, Math.max(0, Number(old.visits) || 0));
    c.mechanism = old.mechanism === true;
    c.choice = ["care","truth"].includes(old.choice) ? old.choice : null;
    c.side.stage = Math.max(0, Math.min(5, Number(old.side?.stage) || 0));
    c.side.choice = ["care","truth"].includes(old.side?.choice) && c.side.stage === 5 ? old.side.choice : null;
    c.guardian = ["mercy","defeat"].includes(old.guardian) ? old.guardian : null;
    c.techniques = {
      observed: !!old.techniques?.observed, understood: !!old.techniques?.understood,
      experimented: !!old.techniques?.experimented, mastered: !!old.techniques?.mastered };
    // No unresolved confirmation, temporary combat result, or spoiler from a saved payload is trusted.
  }
  clean.journey = Array.isArray(raw.journey) ? raw.journey.filter(e => typeof e === "string").slice(-100) : [];
  clean.endingPreview = raw.endingPreview === "held" || raw.endingPreview === "many" ? raw.endingPreview : null;
  clean.revision = Math.max(0, Number(raw.revision) || 0);
  return clean;
}
export function campaignProgress(s) {
  return { completed: Object.values(s.chapters).filter(c => c.choice).length,
    side: Object.values(s.chapters).filter(c => c.side.choice).length,
    guardians: Object.values(s.chapters).filter(c => c.guardian).length,
    studied: Object.values(s.chapters).filter(c => c.techniques.mastered).length };
}
export function canOpenMechanism(c) {
  return c.spoken && c.evidence.length >= 2;
}
export function canConfrontGuardian(c) {
  return c.evidence.length >= 2 && c.spoken;
}
export function journal(s, message) {
  s.journey.push(message);
  if (s.journey.length > 100) s.journey.shift();
  s.revision += 1;
}
export function discover(s, realm, evidenceId) {
  const c = s.chapters[realm.id];
  if (!c) return {ok:false, text:"Unknown realm"};
  const e = realm.evidence.find(item => item.id === evidenceId);
  if (!e) return {ok:false, text:"This is not a physical clue in this realm."};
  if (c.evidence.includes(e.id)) return {ok:true, repeat:true, text:e.discovery};
  c.evidence.push(e.id);
  c.techniques.observed = true;
  journal(s, realm.title+": inspected "+e.site);
  return {ok:true, text:e.discovery};
}
export function speak(s, realm) {
  const c=s.chapters[realm.id];
  if (!c.spoken) {
    c.spoken=true;
    if(c.techniques.observed)c.techniques.understood=true;
    journal(s, realm.title+": heard testimony from "+realm.witness.id);
  }
  return {ok:true, text:c.choice?realm.witness.return:realm.witness.line};
}
export function activateMechanism(s,realm) {
  const c=s.chapters[realm.id];
  if(c.choice)return {ok:false,text:"The local choice is already recorded. Return to inspect its effect."};
  if(!canOpenMechanism(c))return {ok:false,text:"The mechanism requires two physically examined clues and a local witness."};
  c.mechanism=true;
  c.techniques.experimented=true;
  journal(s,realm.title+": activated "+realm.mechanism.id);
  return {ok:true,text:realm.mechanism.line};
}
export function commitChoice(s,realm,choice) {
  const c=s.chapters[realm.id];
  if(!["care","truth"].includes(choice))return {ok:false,text:"Invalid choice"};
  if(c.choice)return {ok:false,text:"This realm already has a saved outcome. You can revisit it."};
  if(!c.mechanism||!canOpenMechanism(c))return {ok:false,text:"The physical mechanism and testimony must be completed before any commitment."};
  c.choice=choice;
  journal(s,realm.title+": "+choice+" outcome committed");
  return {ok:true,text:realm.choices[choice].after};
}
export function sideAction(s,realm,commit=null) {
  const c=s.chapters[realm.id];
  if(c.side.choice)return {ok:true,repeat:true,text:realm.side.return};
  if(!c.choice)return {ok:false,text:"This optional story becomes available after the local first-story choice."};
  if(c.side.stage<5){
    const step=c.side.stage;
    c.side.stage+=1;
    journal(s,realm.title+": optional story step "+(step+1));
    return {ok:true,step:c.side.stage,text:realm.side.steps[step],decision:c.side.stage===5};
  }
  if(["care","truth"].includes(commit)) {
    c.side.choice=commit;
    journal(s,realm.title+": optional "+realm.side.name+" / "+commit);
    return {ok:true,text:realm.side.branches[commit]};
  }
  return {ok:true,decision:true,text:"Choose the optional resolution; the main campaign remains independent."};
}
export function resolveGuardian(s,realm,outcome) {
  const c=s.chapters[realm.id];
  if(c.guardian)return {ok:false,text:"This encounter has already resolved."};
  if(!canConfrontGuardian(c))return {ok:false,text:"Read two physical clues and hear the local witness first."};
  if(!["mercy","defeat"].includes(outcome))return {ok:false,text:"Invalid guardian result"};
  c.guardian=outcome;c.techniques.mastered=true;
  journal(s,realm.title+": guardian "+outcome+"; studied "+realm.technique);
  return {ok:true,text:outcome==="mercy"?realm.guardian.mercy:realm.guardian.defeat};
}
export function travel(s,realms,target) {
  if(!realms.some(r=>r.id===target))return {ok:false,text:"No such authored realm."};
  const leaving=s.current;
  s.current=target;
  s.position={x:12,y:78};
  s.health=MAX_HEALTH;
  s.chapters[target].visits+=1;
  if(leaving!==target)journal(s,"Atlas crossing: "+leaving+" → "+target);
  return {ok:true,text:"Arrived in "+realms.find(r=>r.id===target).title};
}
export function markEndingPreview(s,variant) {
  if(!["held","many"].includes(variant))return {ok:false,text:"Invalid preview"};
  if(campaignProgress(s).completed!==9)return {ok:false,text:"Explore and commit a local story in all nine realms first."};
  s.endingPreview=variant;
  journal(s,"Browser-only ending preview: "+variant);
  return {ok:true,text:"This is an authored world-state preview, not an implemented Unreal final encounter."};
}
