import {MAX_HEALTH,newCampaign,restoreCampaign,campaignProgress,canOpenMechanism,canConfrontGuardian,discover,speak,activateMechanism,commitChoice,sideAction,resolveGuardian,travel,markEndingPreview,journal} from "./nine-realms-rules.mjs";
import {drawRealmScene,drawHazardField,hazardState,insideHazard,drawWitness,drawGuardian,drawGuardianTelegraph,guardianStart,guardianStrikeHits,guardianCue} from "./nine-realms-visuals.mjs";

const $=id=>document.getElementById(id);
const canvas=$("world"),ctx=canvas.getContext("2d"),modal=$("modal");
const palettes=[
  ["#172737","#786b68","#e3c98c"],["#102936","#4d7883","#b7d4d0"],
  ["#2d211f","#985637","#edb16b"],["#10252c","#5f8791","#bec9dc"],
  ["#29273c","#8c83ab","#e2c28f"],["#2c2838","#827580","#d2c9d0"],
  ["#16263b","#55689c","#ebd0ae"],["#192c23","#657c4b","#d8ba93"],
  ["#1b1827","#76768b","#ede7dc"]
];
const evidenceCoords=[[21,61],[65,31],[77,75]];
const sideCoords=[[17,23],[37,17],[60,15],[87,78],[44,72]];
const SAVE_PREFIX="the-unmade-nine-realm-play-v1-";
let data,realms,s,profile=1,battle=null,pendingChoice=null,storyBeat=null;
let time=0,last=0,damageCooldown=0,attackCooldown=0,abilityCooldown=0,toastUntil=0,toastText="";
let sizes={w:800,h:600},keyboard=new Set(),touch=new Set(),guardHold=false,focusPaused=false;
const clamp=(n,min,max)=>Math.min(max,Math.max(min,n));
const distance=(a,b)=>Math.hypot(a.x-b.x,a.y-b.y);
const format=(s,n=23)=>s.length>n?s.slice(0,n-1)+"…":s;
const realm=()=>realms.find(r=>r.id===s.current);
const chapter=()=>s.chapters[s.current];
const rec=(id)=>realms.findIndex(r=>r.id===id);
const xy=(p)=>({x:p.x*sizes.w/100,y:p.y*sizes.h/100});
const profileKey=(p)=>SAVE_PREFIX+"profile-"+p;
function inform(message,duration=3500){toastText=message;toastUntil=time+duration;$("feedback").textContent=message;}
function backupAndSave(){
  if(!s)return;
  try{
    const key=profileKey(profile),old=localStorage.getItem(key);
    if(old&&restoreCampaign(JSON.parse(old),realms))localStorage.setItem(key+"-backup",old);
    localStorage.setItem(key,JSON.stringify(s));
    $("saveStatus").textContent="Profile "+profile+" saved locally · rev "+s.revision;
  }catch(err){$("saveStatus").textContent="Storage unavailable; this session may not persist. Check private mode or browser storage.";}
}
function loadProfile(number){
  profile=number;
  let stored=null;
  try{localStorage.setItem(SAVE_PREFIX+"active",String(number));stored=JSON.parse(localStorage.getItem(profileKey(number))||"null");}catch{}
  s=restoreCampaign(stored,realms)||newCampaign(realms,number);
  s.profile=number; s.chapters[s.current].visits=Math.max(1,s.chapters[s.current].visits);
  pendingChoice=null;battle=null; sync(); inform("Profile "+number+" · "+realm().title,5500);
}
function commitUpdate(){backupAndSave();sync();}
function sync(){
  const r=realm(),c=chapter(),p=campaignProgress(s);
  $("location").textContent=r.title+" · "+r.city+" · Profile "+profile;
  $("health").textContent="Resolve "+("●".repeat(s.health))+("○".repeat(MAX_HEALTH-s.health));
  $("completion").textContent="Main choices "+p.completed+"/9 · optional "+p.side+"/9 · guardians "+p.guardians+"/9";
  $("quest").textContent=c.choice?"Return visit: "+r.choices[c.choice].after:
    !c.spoken?"Talk to the local witness.":
    c.evidence.length<2?"Inspect physical evidence: "+c.evidence.length+"/2 needed.":
    !c.techniques.understood?"Return to the witness to interpret your examined evidence.":
    !c.mechanism?"Activate the highlighted physical mechanism.":
    "Commit one local outcome at the mechanism, after reading both previews.";
}
function worldNodes(){
  const r=realm(),c=chapter();
  const nodes=[
    {kind:"atlas",label:"Atlas crossing",x:12,y:78,icon:"◇"},
    {kind:"witness",label:c.choice?"Returning witness":"Local witness",x:42,y:44,icon:"◆"},
    {kind:"mechanism",label:c.choice?"Changed public place":"Public mechanism",x:85,y:48,icon:"◈"},
    {kind:"refuge",label:r.refuge,x:13,y:91,icon:"✚"}
  ];
  r.evidence.forEach((e,i)=>nodes.push({kind:"evidence",id:e.id,label:e.site,x:evidenceCoords[i][0],y:evidenceCoords[i][1],icon:c.evidence.includes(e.id)?"✓":"✦"}));
  if(!c.side.choice&&c.choice)nodes.push({kind:"side",label:r.side.name+" · "+Math.min(5,c.side.stage+1)+"/5",x:sideCoords[Math.min(4,c.side.stage)][0],y:sideCoords[Math.min(4,c.side.stage)][1],icon:"✧"});
  if(!c.guardian)nodes.push({kind:"guardian",label:r.boss,x:77,y:12,icon:"⚔"});
  return nodes;
}
function nearNode(){
  return worldNodes().map(n=>({...n,dist:distance(s.position,n)})).sort((a,b)=>a.dist-b.dist)[0];
}
function objectiveNode(){
  const c=chapter(),nodes=worldNodes();
  if(c.choice)return nodes.find(n=>n.kind==="witness");
  if(!c.spoken)return nodes.find(n=>n.kind==="witness");
  if(c.evidence.length<2)return nodes.find(n=>n.kind==="evidence"&&!c.evidence.includes(n.id));
  if(!c.techniques.understood)return nodes.find(n=>n.kind==="witness");
  return nodes.find(n=>n.kind==="mechanism");
}
function para(txt){
  const p=document.createElement("p");p.textContent=txt;return p;
}
function openDialog(title,paragraphs,actions=[],overline="FIELD NOTES"){
  focusPaused=true;keyboard.clear();touch.clear();guardHold=false;
  $("modalTitle").textContent=title;$("modalOverline").textContent=overline;
  const body=$("modalBody");body.replaceChildren();
  for(const p of (Array.isArray(paragraphs)?paragraphs:[paragraphs])){if(p)body.append(para(p));}
  const choices=$("modalChoices");choices.replaceChildren();
  for(const option of actions){
    const button=document.createElement("button");button.textContent=option.label;
    button.disabled=!!option.disabled;
    button.addEventListener("click",()=>{if(option.close!==false)closeDialog();option.action?.();});
    choices.append(button);
  }
  modal.classList.add("show");modal.focus();
}
function closeDialog(){
  focusPaused=false;pendingChoice=null;
  modal.classList.remove("show");
  $("interact").focus({preventScroll:true});
}
function atlasDialog(){
  const actions=realms.map(r=>{
    const c=s.chapters[r.id];
    return {label:r.title+" · "+(c.choice?c.choice.toUpperCase()+" resolved":"Unresolved")+(r.id===s.current?" · current":""),action:()=>{
      const previous=s.current;
      const result=travel(s,realms,r.id);
      if(result.ok){battle=null;commitUpdate();inform(result.text+(previous===r.id?" (revisited)":""));}
    }};
  });
  openDialog("Atlas of Nine Realms",[
    "Each location is drawn from authored world and quest sources. The browser preview permits direct atlas travel so you can inspect all nine; it does not claim to implement the canonical Unreal passage gates.",
    "Visits preserve inspected clues, witnesses, optional outcomes and guardian decisions. Nothing here writes to the Unreal SaveGame."],actions,"NONLINEAR WORLD MAP");
}
function showJournal(){
  const r=realm(),c=chapter(),p=campaignProgress(s);
  const evidence=c.evidence.map(id=>r.evidence.find(e=>e.id===id)?.site||id).join(" · ")||"No physical clues examined.";
  const study=c.techniques;
  const text=[
    "Realm: "+r.title+" / "+r.city,
    "Evidence: "+evidence,
    "Witness: "+(c.spoken?"Heard locally":"Not interviewed")+" · Mechanism: "+(c.mechanism?"Activated":"Not activated"),
    "Committed first-story result: "+(c.choice||"None")+" · Return visits: "+Math.max(0,c.visits-1),
    "Optional quest: "+r.side.name+" · "+c.side.stage+"/5 steps · "+(c.side.choice||"unresolved"),
    "Guardian: "+r.boss+" · "+(c.guardian||"unresolved"),
    "Learning: Observation "+(study.observed?"✓":"—")+" · Understanding "+(study.understood?"✓":"—")+" · Experimentation "+(study.experimented?"✓":"—")+" · Mastery "+(study.mastered?"✓":"—"),
    "Campaign-wide: "+p.completed+"/9 local outcomes; "+p.side+" optional resolutions; "+p.guardians+" guardians resolved; "+p.studied+" studied techniques.",
    "Recent witnessed activities: "+(s.journey.slice(-8).join(" | ")||"None")
  ];
  const actions=p.completed===9?[
    {label:"Preview Held Morning consequence language",action:()=>endingPreview("held")},
    {label:"Preview Many Mornings consequence language",action:()=>endingPreview("many")}
  ]:[];
  openDialog("Journal & Consequences",text,actions,"SAVED LOCAL PROFILE");
}
function endingPreview(variant){
  const result=markEndingPreview(s,variant);
  if(!result.ok){inform(result.text);return;}
  commitUpdate();
  openDialog(variant==="held"?"Held Morning · preview":"Many Mornings · preview",[
    "These are the nine authored realm-level aftermaths for the selected world-state motif. This is NOT the canonical final boss or a completed ending.",
    ...realms.map(r=>r.title+": "+s.chapters[r.id].choice+" / "+r.return_after[variant]),
    result.text],[],"SOURCE-CONSTRAINED AFTERMATH");
}
function profilesDialog(){
  let choices=[1,2,3,4].map(num=>({label:"Open profile "+num+(num===profile?" · current":""),action:()=>loadProfile(num)}));
  openDialog("Four Independent Save Profiles",[
    "Each profile persists only in this browser/device. Switching profiles does not overwrite the others.",
    "The previous saved state is retained as a local backup when a new good revision is written. There is no cloud account or Unreal save import."],choices,"SAVE SLOTS");
}
function helpDialog(){
  openDialog("How to Play",[
    "Move with WASD or arrow keys. On mobile, use the direction pad. Stand near a diamond/star marker and press E or INTERACT. The target label tells you what is nearby.",
    "Follow the dotted NEXT trail for the saved main-story step: speak to the local witness, physically inspect at least two different marked clues, return to the witness to interpret them, then use the public mechanism. Read both care and truth consequences and confirm one choice twice. Return later to inspect how the place changed.",
    "Guardians are OPTIONAL. Stand near the guardian and interact to begin. Space / ATTACK strikes in range. Each realm has a distinct tell: read the HUD cue and leave the marked line, ring, target or impact area, or Shift / GUARD through the strike. Spare a weakened guardian with E / INTERACT; another strike defeats it. No guardian is required for any main-story choice.",
    "Master one source-named technique by resolving a studied guardian. Press Q / STUDY COUNTER during later encounters to stun briefly. This counter mechanic is a browser balance experiment, not an Unreal rite grant.",
    "After each realm choice, a short captioned aftermath plays over the changed place; press Escape or Skip scene to continue. Find the optional story's next active marker afterward. Five physical stops lead to a separate care/truth resolution. Completing it never gates another realm.",
    "Environmental danger marks its own shape and warning interval. Cross during the quiet phase or guard the pulse, then move clear. The atlas permits non-linear visits to every authored realm. Journal shows local and across-realm progress. Rest at a refuge to restore resolve, with no re-award of evidence or committed outcomes."],[],"KEYBOARD & TOUCH");
}
function inspect(node){
  const r=realm(),c=chapter();
  if(node.kind==="atlas"){atlasDialog();return;}
  if(node.kind==="refuge"){rest();return;}
  if(node.kind==="evidence"){
    const e=r.evidence.find(x=>x.id===node.id),result=discover(s,r,node.id);
    commitUpdate();
    openDialog(e.site,[e.verb,result.text,result.repeat?"Recorded earlier. No repeated progression reward.":"New physical observation recorded."],[],"EXAMINED EVIDENCE");return;
  }
  if(node.kind==="witness"){
    const result=speak(s,r);commitUpdate();
    openDialog("Witness · "+r.city,[r.witness.id,result.text,"The resident speaks only from the authored local testimony. This does not count as inspecting a physical clue."],[],"HEARD TESTIMONY");return;
  }
  if(node.kind==="mechanism"){
    if(c.choice){openDialog("World That Remembers",[r.choices[c.choice].after,"Witness: "+r.witness.return,"Your local decision remains recorded on this profile."],[],"RETURNED CONSEQUENCE");return;}
    const result=activateMechanism(s,r);
    if(!result.ok){inform(result.text,6000);return;}
    commitUpdate();
    mechanismDialog(result.text);return;
  }
  if(node.kind==="side"){optionalDialog();return;}
  if(node.kind==="guardian"){guardianDialog();return;}
}
function mechanismDialog(opening){
  const r=realm(),c=chapter();
  if(c.choice)return;
  openDialog("A Choice With Witnesses",[
    opening,
    "Care — "+r.choices.care.objective+" "+r.choices.care.line,
    "Truth — "+r.choices.truth.objective+" "+r.choices.truth.line,
    "This decision is irreversible within the current local profile. Confirmation is required TWICE. It does not silently expose private identities."],[
    {label:"Preview and choose CARE",action:()=>confirmMain("care",1)},
    {label:"Preview and choose TRUTH",action:()=>confirmMain("truth",1)}
  ],"PHYSICAL INTERVENTION");
}
function confirmMain(choice,stage){
  const r=realm(),x=r.choices[choice];
  const warning=stage===1?"First confirmation. Nothing has been committed.":"Second and final confirmation. This will be saved and cannot be undone on this profile.";
  openDialog(stage===1?"Confirm "+choice.toUpperCase()+" (1/2)":"Commit "+choice.toUpperCase()+" (2/2)",[
    x.objective,x.line,"Visible public aftermath: "+x.after,warning
  ],[
    {label:stage===1?"Continue to FINAL confirmation":"COMMIT "+choice.toUpperCase()+" and save",action:()=>{
      if(stage===1){confirmMain(choice,2);return;}
      const result=commitChoice(s,r,choice);
      if(result.ok){commitUpdate();inform("Saved "+choice+" in "+r.title,6500);startStoryBeat(r,choice,result.text);}
      else inform(result.text);
    }},
    {label:"Cancel without a commitment",action:()=>inform("No choice was committed.")}
  ],"DELIBERATE SAVE BOUNDARY");
}
function startStoryBeat(r,choice,resultText){
  closeDialog();storyBeat={started:time,r,choice,resultText};
  const button=$("skipCinematic");button.hidden=false;button.focus({preventScroll:true});
}
function finishStoryBeat(){
  if(!storyBeat)return;
  const beat=storyBeat;storyBeat=null;$("skipCinematic").hidden=true;
  focusPaused=false;inform("The place remembers your "+beat.choice+" choice. "+beat.r.witness.return,5000);
  $("interact").focus({preventScroll:true});
}
function drawStoryBeat(ctx,w,h){
  if(!storyBeat)return;
  const beat=storyBeat,elapsed=time-beat.started,phase=Math.min(2,Math.floor(elapsed/2500));
  const lines=[beat.r.choices[beat.choice].after,beat.r.witness.return,beat.resultText];
  ctx.save();ctx.fillStyle="rgba(4,7,13,.82)";const bar=Math.max(48,h*.13);
  ctx.fillRect(0,0,w,bar);ctx.fillRect(0,h-bar,w,bar);
  ctx.fillStyle="rgba(10,13,20,.78)";ctx.fillRect(w*.08,h*.69,w*.84,h*.18);
  ctx.strokeStyle=beat.choice==="care"?"#e8bd79":"#b8d9e1";ctx.lineWidth=2;
  ctx.beginPath();ctx.moveTo(w*.12,h*.67);ctx.lineTo(w*.88,h*.67);ctx.stroke();
  ctx.textAlign="center";ctx.fillStyle="#f2d39a";ctx.font="600 12px system-ui";
  ctx.fillText(beat.choice==="care"?"AFTERMATH · A PROMISE GIVEN":"AFTERMATH · A CLAIM MADE PUBLIC",w/2,h*.74);
  ctx.fillStyle="#fff4dc";ctx.font="500 "+Math.max(15,Math.min(22,w/34))+"px Georgia";
  wrapCanvasText(ctx,lines[phase],w/2,h*.80,w*.72,Math.max(20,Math.min(28,w/30)));
  ctx.fillStyle="rgba(255,255,255,.62)";ctx.font="11px system-ui";ctx.fillText("SCENE "+(phase+1)+" / 3",w/2,h*.94);
  if(elapsed>=7500)finishStoryBeat();
  ctx.restore();
}
function wrapCanvasText(ctx,text,x,y,maxWidth,lineHeight){
  const words=String(text||"").split(/\s+/);let line="",rows=[];
  for(const word of words){const next=line?line+" "+word:word;if(ctx.measureText(next).width>maxWidth&&line){rows.push(line);line=word;}else line=next;}
  if(line)rows.push(line);const shown=rows.slice(0,3),start=y-(shown.length-1)*lineHeight/2;
  shown.forEach((row,i)=>ctx.fillText(row,x,start+i*lineHeight));
}
function optionalDialog(){
  const r=realm(),c=chapter();
  if(c.side.stage<5){
    const n=c.side.stage+1,explanation=r.side.steps[c.side.stage];
    openDialog(r.side.name+" · Step "+n+"/5",[
      r.side.objective,
      explanation,
      "You reached the currently marked physical site. Record the encounter once; then a different route marker appears."],[
      {label:"Perform this local step",action:()=>{
        const result=sideAction(s,r);if(result.ok){commitUpdate();inform("Optional step "+result.step+"/5 recorded");if(result.decision)optionalDialog();}
      }}
    ],"INDEPENDENT OPTIONAL STORY");
  }else{
    openDialog("Resolve · "+r.side.name,[
      "CARE: "+r.side.branches.care,
      "TRUTH: "+r.side.branches.truth,
      "Optional local story choices are independent of main-story route and all guardian encounters."],[
      {label:"Save CARE outcome",action:()=>resolveSide("care")},
      {label:"Save TRUTH outcome",action:()=>resolveSide("truth")}
    ],"OPTIONAL AFTERMATH");
  }
}
function resolveSide(choice){
  const result=sideAction(s,realm(),choice);if(result.ok){commitUpdate();
    openDialog("An Ordinary Place Remembers",[result.text,realm().side.return],[],"OPTIONAL RESOLUTION");
  }else inform(result.text);
}
function guardianDialog(){
  const r=realm(),c=chapter();
  if(c.guardian){inform("Guardian already resolved as "+c.guardian);return;}
  if(!canConfrontGuardian(c)){inform("The optional guardian needs two physically examined clues and local testimony.");return;}
  if(battle){if(battle.hp<=1&&distance(s.position,battle)<20){
      const result=resolveGuardian(s,r,"mercy");if(result.ok){battle=null;commitUpdate();openDialog("Mercy Accepted",[result.text,"Studied technique: "+r.technique],[],"OPTIONAL NONVIOLENT RESOLUTION");}
    }else inform("Keep distance from the warning ring; weaken the guardian before mercy.");return;}
  openDialog(r.boss,[
    r.brief,
    ...r.guardian.clues,
    "This is a separate browser encounter with telegraphed attacks, movement, guard, optional mercy and recorded outcomes—not the complete Unreal boss choreography."],[
    {label:"Begin confrontation (optional)",action:()=>{
      battle={...guardianStart(r.id),hp:5,max:5,attackTime:time+1500,warningAt:time+1000,stunnedUntil:0,attacked:false,target:{...s.position}};
      inform("Confrontation started. SPACE to strike · SHIFT to guard · E to spare when weakened.",5000);sync();
    }}
  ],"OPTIONAL GUARDIAN");
}
function attack(){
  if(focusPaused||!battle||time<attackCooldown)return;
  attackCooldown=time+470;
  if(distance(s.position,battle)>18){inform("Move closer to the guardian to land a strike.",850);return;}
  battle.hp--;
  if(battle.hp<=0){
    const r=realm(),result=resolveGuardian(s,r,"defeat");
    battle=null;commitUpdate();
    openDialog("Guardian Confrontation Resolved",[result.text,"Studied technique: "+r.technique],[],"OPTIONAL VIOLENT RESOLUTION");
  }else inform(battle.hp===1?"Guardian yields: E / INTERACT to offer mercy, or strike again.":"Attack landed. Watch the telegraphed counter.",1900);
}
function studyCounter(){
  if(focusPaused||!battle||time<abilityCooldown)return;
  const p=campaignProgress(s);
  if(p.studied<1){inform("Master a studied guardian technique first.");return;}
  abilityCooldown=time+6000;
  battle.stunnedUntil=time+1750;
  battle.attackTime=Math.max(battle.attackTime,time+2100);
  inform("Earned study counter: guardian staggered. Recharges in 6 seconds.",2200);
}
function rest(){
  s.health=MAX_HEALTH;s.position={x:13,y:89};battle=null;
  commitUpdate();inform("You recovered at "+realm().refuge+". Evidence and choices remain saved.");
}
function interact(){
  if(focusPaused)return;
  // A wounded guardian moves toward the player: offer mercy at its ACTUAL position.
  if(battle&&battle.hp<=1&&distance(s.position,battle)<20){guardianDialog();return;}
  const node=nearNode();
  if(!node||node.dist>15){inform("Approach a glowing field marker to interact.");return;}
  inspect(node);
}
function resetDialog(){
  openDialog("Reset ONLY Profile "+profile,[
    "This deletes the active profile's current browser-local campaign state. It does not change other profiles, repository data, Unreal files, or GitHub.",
    "The browser-local prior backup is kept, but restoration from it is not automated. This operation is not reversible through the UI."],[
    {label:"Delete this profile and start new",action:()=>{
      try{localStorage.removeItem(profileKey(profile));}catch{}
      s=newCampaign(realms,profile);battle=null;commitUpdate();inform("Reset only profile "+profile);
    }}
  ],"DESTRUCTIVE LOCAL ACTION");
}
function draw(){
  if(!s||!ctx)return;
  const r=realm(),c=chapter(),idx=rec(r.id),p=palettes[idx],w=sizes.w,h=sizes.h;
  const grd=ctx.createLinearGradient(0,0,w,h);grd.addColorStop(0,p[0]);grd.addColorStop(.6,p[1]);grd.addColorStop(1,p[0]);ctx.fillStyle=grd;ctx.fillRect(0,0,w,h);
  drawRealmScene(ctx,w,h,r.id,time,c.choice,p);
  ctx.save();
  ctx.fillStyle="rgba(0,0,0,.25)";ctx.fillRect(w*.09,h*.04,w*.8,27);
  ctx.fillStyle=p[2];ctx.font="600 13px Georgia";ctx.fillText(r.culture,w*.11,h*.04+18);
  if(c.choice){
    const m=xy({x:85,y:48});ctx.lineWidth=5;ctx.strokeStyle=c.choice==="care"?"#dfb68d":"#c3e1e3";
    ctx.beginPath();ctx.arc(m.x,m.y,16+Math.sin(time/250)*2,0,Math.PI*2);ctx.stroke();
    ctx.fillStyle=c.choice==="care"?"#e5b56d":"#b9d2e3";ctx.font="bold 11px system-ui";
    ctx.textAlign="center";ctx.fillText(c.choice.toUpperCase()+" REMAINS",m.x,m.y+30);
  }
  const hazard=drawHazardField(ctx,w,h,r.id,time);
  const nodes=worldNodes(),questTarget=objectiveNode();
  if(questTarget){
    const from=xy(s.position),to=xy(questTarget);
    ctx.save();ctx.strokeStyle="rgba(248,220,164,.53)";ctx.lineWidth=2;ctx.setLineDash([4,9]);
    ctx.beginPath();ctx.moveTo(from.x,from.y);ctx.lineTo(to.x,to.y);ctx.stroke();ctx.setLineDash([]);
    ctx.fillStyle="#f7d99f";ctx.font="bold 9px system-ui";ctx.textAlign="center";ctx.fillText("NEXT",to.x,to.y-23);
    ctx.restore();
  }
  nodes.forEach(n=>{
    const loc=xy(n),dist=distance(s.position,n),rad=dist<15?16:12,isNext=questTarget&&questTarget.kind===n.kind&&questTarget.id===n.id;
    if(isNext){ctx.save();ctx.strokeStyle="#f4d493";ctx.lineWidth=2;ctx.setLineDash([4,4]);ctx.beginPath();ctx.arc(loc.x,loc.y,rad+8+Math.sin(time/210)*2,0,Math.PI*2);ctx.stroke();ctx.setLineDash([]);ctx.restore();}
    if(n.kind==="witness"){drawWitness(ctx,loc.x,loc.y,r.id,p,dist<15,time);}
    else if(n.kind==="guardian"){drawGuardian(ctx,loc.x,loc.y,r.id,dist<15?1.12:.92);}
    else{ctx.shadowColor=p[2];ctx.shadowBlur=dist<15?22:8;
      ctx.beginPath();ctx.fillStyle=n.kind==="side"?"#a48cda":n.kind==="mechanism"?"#e0b273":"#d0d4cb";
      ctx.arc(loc.x,loc.y,rad,0,Math.PI*2);ctx.fill();ctx.shadowBlur=0;
      ctx.textAlign="center";ctx.font="bold 16px system-ui";ctx.fillStyle="#13202b";ctx.fillText(n.icon,loc.x,loc.y+5);}
    ctx.font=(dist<15?"bold ":"")+"11px system-ui";const label=format(n.label,24);
    const width=ctx.measureText(label).width+11;
    ctx.fillStyle="rgba(10,17,27,.85)";ctx.fillRect(clamp(loc.x-width/2,0,w-width),loc.y+rad+3,width,18);
    ctx.fillStyle="#f8ebd1";ctx.fillText(label,clamp(loc.x,w-width/2,width/2),loc.y+rad+16);
  });
  if(battle){
    const loc=xy(battle),warning=time>battle.warningAt&&time<battle.attackTime;
    if(warning)drawGuardianTelegraph(ctx,w,h,r.id,battle);
    drawGuardian(ctx,loc.x,loc.y,r.id,1.28,warning);
    if(warning){ctx.fillStyle="#f9d8c1";ctx.font="bold 12px system-ui";ctx.textAlign="center";ctx.fillText("GUARD OR EVADE",loc.x,loc.y-28);}
  }
  const user=xy(s.position);
  ctx.shadowColor="#030810";ctx.shadowBlur=13;
  ctx.fillStyle="#0c1220";ctx.beginPath();ctx.ellipse(user.x,user.y+11,12,5,0,0,Math.PI*2);ctx.fill();
  ctx.shadowBlur=0;ctx.fillStyle="#d5c199";ctx.beginPath();ctx.arc(user.x,user.y-3,8,0,Math.PI*2);ctx.fill();
  ctx.fillStyle=guardHold?"#8cc5d9":"#603c63";ctx.beginPath();ctx.moveTo(user.x-9,user.y+2);ctx.lineTo(user.x+9,user.y+2);ctx.lineTo(user.x+13,user.y+18);ctx.lineTo(user.x-13,user.y+18);ctx.fill();
  ctx.strokeStyle="#e6ca93";ctx.lineWidth=2;ctx.beginPath();ctx.moveTo(user.x,user.y+4);ctx.lineTo(user.x,user.y+14);ctx.stroke();
  ctx.restore();
  drawStoryBeat(ctx,w,h);
  $("bossHud").style.display=battle?"block":"none";
  if(battle){$("bossName").textContent=r.boss;$("bossFill").style.width=(battle.hp/battle.max*100)+"%";
    $("bossCue").textContent=battle.hp<=1?"Guardian weakened · E to offer mercy":time>battle.warningAt?guardianCue(r.id):"Study its tell · strike in range";}
}
function resize(){
  const rect=canvas.getBoundingClientRect(),dpr=Math.min(2,window.devicePixelRatio||1);
  sizes.w=Math.max(1,rect.width);sizes.h=Math.max(1,rect.height);
  canvas.width=Math.round(sizes.w*dpr);canvas.height=Math.round(sizes.h*dpr);
  ctx.setTransform(dpr,0,0,dpr,0,0);
}
function tick(now){
  if(!last)last=now;
  const dt=Math.min(.05,(now-last)/1000);last=now;time=now;
  if(s&&!focusPaused&&!storyBeat){
    let dx=0,dy=0;
    if(keyboard.has("a")||keyboard.has("arrowleft")||touch.has("left"))dx--;
    if(keyboard.has("d")||keyboard.has("arrowright")||touch.has("right"))dx++;
    if(keyboard.has("w")||keyboard.has("arrowup")||touch.has("up"))dy--;
    if(keyboard.has("s")||keyboard.has("arrowdown")||touch.has("down"))dy++;
    const length=Math.hypot(dx,dy),guard=guardHold||keyboard.has("shift");
    if(length>0){
      const speed=guard?12:25;
      s.position.x=clamp(s.position.x+dx/length*dt*speed,4,96);
      s.position.y=clamp(s.position.y+dy/length*dt*speed,4,96);
    }
    const hazard=hazardState(realm().id,time);
    if(hazard.active&&insideHazard(s.position,hazard)&&time>damageCooldown){
      if(!guard){s.health--;inform(hazard.label+" caught you. Read its warning and cross during the lull.",2100);}
      else inform("Guarded "+hazard.label+". Move clear before the next pulse.",1500);
      damageCooldown=time+1800;
    }
    if(battle){
      if(time>battle.stunnedUntil&&distance(battle,s.position)>8){
        const sx=s.position.x-battle.x,sy=s.position.y-battle.y,d=Math.hypot(sx,sy)||1;
        battle.x+=sx/d*dt*6;battle.y+=sy/d*dt*6;
      }
      if(time>=battle.attackTime&&time>battle.stunnedUntil){
        const struck=guardianStrikeHits(realm().id,s.position,battle);
        if(struck&&time>damageCooldown){
          if(guard){inform("Guardian's "+guardianCue(realm().id)+" blocked. Counter now!",1600);}
          else{s.health--;inform("Guardian struck. "+guardianCue(realm().id)+".",1900);}
          damageCooldown=time+1000;
        }
        battle.attackTime=time+2600;battle.warningAt=battle.attackTime-750;battle.target={...s.position};
      }
    }
    if(s.health<=0){
      s.health=MAX_HEALTH;s.position={x:13,y:89};battle=null;
      inform("You recovered at the refuge. Existing choices and discoveries were preserved.",5500);
      commitUpdate();
    }
    $("target").textContent=(()=>{
      const node=nearNode();return node.dist<15?"E / INTERACT · "+node.label+" ("+Math.round(node.dist)+"m)":"Seek a glowing location · "+realm().hazard;
    })();
  }
  if(s)draw();
  if(time>toastUntil&&!focusPaused&&toastText){$("feedback").textContent="WASD / arrows · E interact · Space attack · Shift guard · Q counter";toastText="";}
  requestAnimationFrame(tick);
}
function wire(){
  $("atlasBtn").onclick=atlasDialog;$("journalBtn").onclick=showJournal;$("profileBtn").onclick=profilesDialog;
  $("helpBtn").onclick=helpDialog;$("restBtn").onclick=rest;$("clearBtn").onclick=resetDialog;
  $("interact").onclick=()=>modal.classList.contains("show")?closeDialog():interact();
  $("attackTouch").onclick=attack;$("skillTouch").onclick=studyCounter;
  $("closeModal").onclick=closeDialog;$("skipCinematic").onclick=finishStoryBeat;
  window.addEventListener("keydown",e=>{
    const k=e.key.toLowerCase();
    if(k==="escape"){if(storyBeat)finishStoryBeat();else if(focusPaused)closeDialog();return;}
    // Let the browser activate focused modal buttons with Space / Enter.
    if(focusPaused)return;
    if(["arrowup","arrowdown","arrowleft","arrowright"," ","shift"].includes(k))e.preventDefault();
    keyboard.add(k);
    if(k==="e")interact();
    if(k===" ")attack();
    if(k==="q")studyCounter();
    if(k==="j")showJournal();
    if(k==="m")atlasDialog();
  });
  window.addEventListener("keyup",e=>keyboard.delete(e.key.toLowerCase()));
  window.addEventListener("blur",()=>{keyboard.clear();touch.clear();guardHold=false;});
  for(const btn of document.querySelectorAll("[data-key]")){
    const dir=btn.dataset.key;
    const down=e=>{e.preventDefault();touch.add(dir);btn.classList.add("active");try{btn.setPointerCapture(e.pointerId);}catch{}};
    const up=e=>{e.preventDefault();touch.delete(dir);btn.classList.remove("active");};
    btn.addEventListener("pointerdown",down);btn.addEventListener("pointerup",up);
    btn.addEventListener("pointercancel",up);btn.addEventListener("lostpointercapture",up);
  }
  const guard=$("guardTouch");
  guard.addEventListener("pointerdown",e=>{e.preventDefault();guardHold=true;guard.setPointerCapture(e.pointerId)});
  for(const event of ["pointerup","pointercancel","lostpointercapture"])guard.addEventListener(event,()=>{guardHold=false;});
  window.addEventListener("resize",resize);resize();
}
async function start(){
  try{
    const response=await fetch("./nine-realms-data.json",{cache:"no-cache"});
    if(!response.ok)throw new Error("authored scenarios could not load ("+response.status+")");
    data=await response.json();realms=data.realms;
    if(!Array.isArray(realms)||realms.length!==9||realms.some(r=>r.evidence?.length!==3||r.side?.steps?.length!==5))
      throw new Error("nine-realm authoring parity failed");
    try{profile=clamp(Number(localStorage.getItem(SAVE_PREFIX+"active"))||1,1,4);}catch{profile=1;}
    wire();loadProfile(profile);requestAnimationFrame(tick);
  }catch(err){
    $("feedback").textContent="Unable to start: "+err.message;
    $("quest").textContent="The nine-realm authored data is unavailable; gameplay has not started.";
  }
}
start();
