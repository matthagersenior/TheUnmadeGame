import test from "node:test";
import assert from "node:assert/strict";
import {readFileSync} from "node:fs";
import {fileURLToPath} from "node:url";
import {dirname,resolve} from "node:path";
import {
  newCampaign,restoreCampaign,MAX_HEALTH,campaignProgress,canOpenMechanism,
  discover,speak,activateMechanism,commitChoice,sideAction,resolveGuardian,travel,markEndingPreview
} from "../nine-realms-rules.mjs";

const root=resolve(dirname(fileURLToPath(import.meta.url)),"..");
const rows=JSON.parse(readFileSync(resolve(root,"nine-realms-data.json"),"utf8")).realms;
const first=rows[0];
const fresh=()=>newCampaign(rows);
const prime=(s,r)=>{
  const c=s.chapters[r.id];
  assert.equal(activateMechanism(s,r).ok,false,"no story skip without firsthand evidence");
  assert.equal(discover(s,r,r.evidence[0].id).ok,true);
  assert.equal(discover(s,r,r.evidence[1].id).ok,true);
  assert.equal(speak(s,r).ok,true);
  assert.equal(canOpenMechanism(c),true);
  assert.equal(activateMechanism(s,r).ok,true);
};

test("Nine distinct authored realm scenarios have local grounded evidence and outcomes",()=>{
  assert.equal(rows.length,9);
  assert.equal(new Set(rows.map(r=>r.id)).size,9);
  assert.equal(new Set(rows.map(r=>r.boss)).size,9);
  for(const r of rows){
    assert.equal(r.evidence.length,3);
    assert.equal(new Set(r.evidence.map(e=>e.id)).size,3);
    assert.equal(r.side.steps.length,5);
    assert.ok(r.choices.care.after&&r.choices.truth.after);
    assert.ok(r.witness.id&&r.mechanism.id&&r.guardian.mercy);
  }
});
test("Witness words or visiting cannot fabricate inspected clue",()=>{
  const s=fresh();speak(s,first);
  assert.equal(s.chapters[first.id].evidence.length,0);
  assert.equal(canOpenMechanism(s.chapters[first.id]),false);
  assert.equal(activateMechanism(s,first).ok,false);
  assert.equal(commitChoice(s,first,"truth").ok,false);
  assert.equal(discover(s,first,"invalid-id").ok,false);
});
test("Two distinct physically examined clues and a witnessed account are required",()=>{
  const s=fresh();
  discover(s,first,first.evidence[0].id);
  discover(s,first,first.evidence[0].id);
  speak(s,first);
  assert.equal(s.chapters[first.id].evidence.length,1);
  assert.equal(canOpenMechanism(s.chapters[first.id]),false);
  discover(s,first,first.evidence[1].id);
  assert.equal(activateMechanism(s,first).ok,true);
});
test("Witness-first path requires returning with physical evidence before a world change",()=>{
  const s=fresh(),r=first;
  speak(s,r);
  discover(s,r,r.evidence[0].id);discover(s,r,r.evidence[1].id);
  assert.equal(canOpenMechanism(s.chapters[r.id]),false);
  assert.equal(activateMechanism(s,r).ok,false);
  speak(s,r);
  assert.equal(s.chapters[r.id].techniques.understood,true);
  assert.equal(activateMechanism(s,r).ok,true);
});
test("Every realm commits exactly one care or truth choice; guardian and side content remain optional",()=>{
  for(const r of rows)for(const choice of ["care","truth"]){
    const s=fresh();
    prime(s,r);
    assert.equal(commitChoice(s,r,choice).ok,true,r.id+" "+choice);
    assert.equal(commitChoice(s,r,choice==="care"?"truth":"care").ok,false);
    assert.equal(s.chapters[r.id].choice,choice);
    assert.equal(s.chapters[r.id].side.choice,null);
    assert.equal(s.chapters[r.id].guardian,null);
  }
});
test("Optional stories require local decision, five steps, then commit once",()=>{
  const s=fresh(),r=first;
  assert.equal(sideAction(s,r).ok,false);
  prime(s,r);commitChoice(s,r,"care");
  assert.equal(sideAction(s,r,"truth").step,1,"a command cannot skip five physical steps");
  for(let i=2;i<=5;i++)assert.equal(sideAction(s,r).step,i);
  assert.equal(sideAction(s,r).decision,true);
  assert.equal(sideAction(s,r,"truth").ok,true);
  assert.equal(sideAction(s,r,"care").repeat,true);
  assert.equal(s.chapters[r.id].side.choice,"truth");
  assert.equal(s.chapters[r.id].choice,"care");
});
test("Guardians require evidence and testimony but do not grant main story progression",()=>{
  for(const r of rows){
    const s=fresh();
    assert.equal(resolveGuardian(s,r,"mercy").ok,false);
    discover(s,r,r.evidence[0].id);discover(s,r,r.evidence[1].id);speak(s,r);
    assert.equal(resolveGuardian(s,r,"mercy").ok,true);
    assert.equal(s.chapters[r.id].choice,null);
    assert.equal(s.chapters[r.id].techniques.mastered,true);
    assert.equal(resolveGuardian(s,r,"defeat").ok,false);
  }
});
test("Travel never auto-completes chapters, and reentry preserves consequences",()=>{
  const s=fresh();prime(s,first);commitChoice(s,first,"truth");
  const r=rows[4];const dest=travel(s,rows,r.id);
  assert.equal(dest.ok,true);
  assert.equal(s.health,MAX_HEALTH);
  assert.equal(s.chapters[r.id].choice,null);
  assert.equal(s.chapters[r.id].evidence.length,0);
  travel(s,rows,first.id);
  assert.equal(s.chapters[first.id].choice,"truth");
  assert.equal(s.chapters[first.id].evidence.length,2);
  assert.equal(travel(s,rows,"nonexistent").ok,false);
});
test("Four browser profiles are independent and remain round-trip consistent",()=>{
  const a=newCampaign(rows,1),b=newCampaign(rows,2);
  prime(a,first);commitChoice(a,first,"care");
  assert.equal(b.chapters[first.id].choice,null);
  const saved=restoreCampaign(JSON.parse(JSON.stringify(a)),rows);
  assert.deepEqual(saved,a);
  assert.equal(saved.profile,1);
});
test("Malformed and unsupported saves rejected; no unresolved pending choice restored",()=>{
  const s=fresh();
  assert.equal(restoreCampaign({...s,version:900},rows),null);
  assert.equal(restoreCampaign({...s,current:"not-a-realm"},rows),null);
  assert.equal(restoreCampaign({...s,chapters:{}},rows),null);
  const corrupt=structuredClone(s);corrupt.chapters[first.id].evidence=null;
  assert.equal(restoreCampaign(corrupt,rows),null);
  const extraneous=structuredClone(s);
  extraneous.chapters[first.id].evidence=["spoofed",first.evidence[0].id,first.evidence[0].id];
  extraneous.pendingCommit="truth";
  const clean=restoreCampaign(extraneous,rows);
  assert.deepEqual(clean.chapters[first.id].evidence,[first.evidence[0].id]);
  assert.equal(clean.pendingCommit,undefined);
});
test("Epilogue preview requires nine settled local choices and is not a canonical boss receipt",()=>{
  const s=fresh();
  assert.equal(markEndingPreview(s,"held").ok,false);
  for(const r of rows){prime(s,r);commitChoice(s,r,"care");}
  assert.equal(campaignProgress(s).completed,9);
  assert.equal(markEndingPreview(s,"many").ok,true);
  assert.equal(s.endingPreview,"many");
  assert.equal(s.journey.some(e=>e.includes("browser-only final boss defeated")),false);
});
