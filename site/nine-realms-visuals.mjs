// Source-authored browser-only world art, hazard grammar and guardian counterplay.
// Motifs derive from the nine realm architecture and sensory cues in the campaign source.
// These are 2D presentation prototypes; they do not claim Unreal assets or collision parity.
const scenes={
 ThreefoldReach:{color:"#d9bc82",deep:"#243348",motif:"roads",hazard:{x:55,y:58,rx:12,ry:9,shape:"cross",period:5100,warning:.60,active:.80,label:"SPLIT ROAD"},attack:"fork",start:{x:77,y:16},bossMark:"arch"},
 WidowedRain:{color:"#8ec2c4",deep:"#15303b",motif:"tide",hazard:{x:64,y:57,rx:24,ry:7,shape:"laneX",period:5500,warning:.63,active:.83,label:"STORM SLUICE"},attack:"sweepX",start:{x:85,y:22},bossMark:"wave"},
 HearthBeneath:{color:"#efa65e",deep:"#31211d",motif:"kiln",hazard:{x:54,y:59,rx:13,ry:13,shape:"ellipse",period:4900,warning:.57,active:.78,label:"VENT BURST"},attack:"burst",start:{x:72,y:21},bossMark:"flame"},
 TidalLedger:{color:"#9ac5c1",deep:"#14272b",motif:"ledger",hazard:{x:62,y:57,rx:7,ry:25,shape:"laneY",period:5600,warning:.62,active:.82,label:"DEBT TIDE"},attack:"sweepY",start:{x:81,y:23},bossMark:"ledger"},
 SkyBelow:{color:"#d2c0e7",deep:"#29253b",motif:"choir",hazard:{x:56,y:58,rx:20,ry:6,shape:"ring",period:5300,warning:.61,active:.82,label:"COUNTERWEIGHT PULSE"},attack:"ring",start:{x:73,y:19},bossMark:"bell"},
 CinderSpine:{color:"#d6c7b0",deep:"#302b34",motif:"quarry",hazard:{x:56,y:56,rx:6,ry:25,shape:"laneY",period:5700,warning:.64,active:.84,label:"SHEAR LINE"},attack:"fissure",start:{x:83,y:19},bossMark:"seam"},
 HundredUnlived:{color:"#d0c0df",deep:"#1e2b40",motif:"doorsteps",hazard:{x:56,y:57,rx:18,ry:13,shape:"grid",period:5200,warning:.60,active:.80,label:"CENSUS LATTICE"},attack:"target",start:{x:70,y:21},bossMark:"lattice"},
 OrchardOfKings:{color:"#b4d38c",deep:"#182d23",motif:"graft",hazard:{x:56,y:57,rx:20,ry:8,shape:"cross",period:5400,warning:.62,active:.82,label:"ROOT RISE"},attack:"roots",start:{x:79,y:18},bossMark:"branch"},
 FirstAbsence:{color:"#d8d5ec",deep:"#201e30",motif:"threshold",hazard:{x:57,y:57,rx:6,ry:24,shape:"doubleLane",period:5800,warning:.65,active:.85,label:"THRESHOLD SHIFT"},attack:"door",start:{x:86,y:20},bossMark:"doors"}
};
const fallback={color:"#d8c69f",deep:"#182332",motif:"roads",hazard:{x:55,y:58,rx:12,ry:9,shape:"cross",period:5100,warning:.6,active:.8,label:"WORLD SURGE"},attack:"burst",start:{x:77,y:16},bossMark:"arch"};
export function realmVisual(id){return scenes[id]||fallback;}
export function guardianStart(id){return {...realmVisual(id).start};}
export function hazardState(id,now){
 const h=realmVisual(id).hazard,p=((now%h.period)+h.period)%h.period/h.period;
 return {...h,warning:p>=h.warning&&p<h.active,active:p>=h.active};
}
export function insideHazard(pos,h){
 const dx=pos.x-h.x,dy=pos.y-h.y,ax=Math.abs(dx),ay=Math.abs(dy);
 if(h.shape==="laneX")return ax<=h.rx&&ay<=h.ry;
 if(h.shape==="laneY")return ay<=h.ry&&ax<=h.rx;
 if(h.shape==="cross")return (ax<=h.rx&&ay<=h.ry*2.1)||(ay<=h.ry&&ax<=h.rx*2.1);
 if(h.shape==="ring"){const d=Math.hypot(dx,dy);return Math.abs(d-h.rx)<=h.ry;}
 if(h.shape==="grid")return (ax<=h.rx&&ay<=h.ry)||(ax<=h.rx&&Math.abs(ay-h.ry*.65)<2)||(ay<=h.ry&&Math.abs(ax-h.rx*.65)<2);
 if(h.shape==="doubleLane")return ay<=h.ry&&(Math.abs(dx-h.rx*.55)<h.ry*.34||Math.abs(dx+h.rx*.55)<h.ry*.34);
 return (dx*dx)/(h.rx*h.rx)+(dy*dy)/(h.ry*h.ry)<=1;
}
const X=(w,p)=>w*p/100,Y=(h,p)=>h*p/100;
function path(ctx,w,h,points){
 ctx.beginPath();points.forEach((v,i)=>i?ctx.lineTo(X(w,v[0]),Y(h,v[1])):ctx.moveTo(X(w,v[0]),Y(h,v[1])));
}
function drawArch(ctx,x,y,s,color){
 ctx.save();ctx.strokeStyle=color;ctx.lineWidth=3*s;ctx.beginPath();ctx.moveTo(x-13*s,y+17*s);ctx.lineTo(x-13*s,y-5*s);ctx.quadraticCurveTo(x,y-22*s,x+13*s,y-5*s);ctx.lineTo(x+13*s,y+17*s);ctx.stroke();ctx.restore();
}
export function drawRealmScene(ctx,w,h,id,now,choice,palette){
 const art=realmVisual(id),accent=art.color,deep=art.deep;
 const seed=Object.keys(scenes).indexOf(id);
 ctx.save();ctx.globalAlpha=.35;ctx.lineWidth=2;ctx.strokeStyle=accent;ctx.fillStyle=accent;
 // Each set is a different physical contradiction, not a palette swap.
 if(art.motif==="roads"){
  path(ctx,w,h,[[7,89],[24,68],[39,51],[51,46],[70,31],[92,16]]);ctx.stroke();
  path(ctx,w,h,[[38,51],[27,38],[18,25]]);ctx.stroke();
  for(const [x,y] of [[18,25],[51,46],[92,16]]){ctx.fillRect(X(w,x)-4,Y(h,y)-13,8,26);ctx.beginPath();ctx.arc(X(w,x),Y(h,y)-13,6,Math.PI,0);ctx.stroke();}
 }else if(art.motif==="tide"){
  for(let i=0;i<4;i++){let y=25+i*12;ctx.beginPath();ctx.moveTo(0,Y(h,y));ctx.quadraticCurveTo(w*.25,Y(h,y-5),w*.5,Y(h,y));ctx.quadraticCurveTo(w*.75,Y(h,y+5),w,Y(h,y));ctx.stroke();}
  for(const x of [18,31,72,84]){ctx.fillRect(X(w,x)-3,Y(h,34),6,Y(h,48)-Y(h,34));ctx.beginPath();ctx.moveTo(X(w,x)-7,Y(h,34));ctx.lineTo(X(w,x),Y(h,29));ctx.lineTo(X(w,x)+7,Y(h,34));ctx.stroke();}
  ctx.strokeRect(X(w,73),Y(h,26),w*.1,h*.14);
 }else if(art.motif==="kiln"){
  for(const [x,y,r] of [[22,27,11],[76,30,13],[86,75,9]]){ctx.beginPath();ctx.ellipse(X(w,x),Y(h,y),w*r/100,h*r/100,0,0,Math.PI*2);ctx.stroke();ctx.beginPath();ctx.arc(X(w,x),Y(h,y),Math.min(w,h)*r/210,Math.PI,0);ctx.stroke();}
  for(const x of [20,28,75,82]){ctx.fillRect(X(w,x)-5,Y(h,42),10,Y(h,18));ctx.beginPath();ctx.arc(X(w,x),Y(h,42),5,Math.PI,0);ctx.stroke();}
  path(ctx,w,h,[[24,43],[38,49],[60,49],[78,42]]);ctx.stroke();
 }else if(art.motif==="ledger"){
  ctx.lineWidth=7;ctx.strokeRect(X(w,16),Y(h,25),w*.13,h*.58);ctx.strokeRect(X(w,72),Y(h,19),w*.11,h*.61);
  ctx.lineWidth=2;for(let i=0;i<5;i++){const y=31+i*9;ctx.beginPath();ctx.moveTo(0,Y(h,y));ctx.lineTo(w,Y(h,y));ctx.stroke();}
  for(const x of [35,53,68]){ctx.fillRect(X(w,x)-9,Y(h,48),18,5);ctx.beginPath();ctx.arc(X(w,x),Y(h,48),3,0,Math.PI*2);ctx.fill();}
 }else if(art.motif==="choir"){
  for(let i=0;i<5;i++){const x=18+i*16,top=19+(i%2)*6;ctx.beginPath();ctx.moveTo(X(w,x),Y(h,top));ctx.lineTo(X(w,x),Y(h,50+i%2*9));ctx.stroke();ctx.fillRect(X(w,x)-8,Y(h,50+i%2*9),16,4);}
  for(const [x,y] of [[28,65],[65,29],[83,60]]){ctx.beginPath();ctx.ellipse(X(w,x),Y(h,y),12,5,0,0,Math.PI*2);ctx.stroke();}
 }else if(art.motif==="quarry"){
  for(let i=0;i<7;i++){const y=19+i*11;ctx.beginPath();ctx.moveTo(0,Y(h,y));ctx.lineTo(X(w,28),Y(h,y+(i%2)*3));ctx.lineTo(X(w,51),Y(h,y-2));ctx.lineTo(w,Y(h,y+4));ctx.stroke();}
  path(ctx,w,h,[[65,12],[59,27],[64,41],[55,55],[60,72],[50,90]]);ctx.stroke();
  ctx.fillRect(X(w,18),Y(h,76),w*.19,h*.04);ctx.fillRect(X(w,68),Y(h,33),w*.2,h*.035);
 }else if(art.motif==="doorsteps"){
  for(const [x,y,s] of [[17,30,1],[29,50,.75],[77,26,1.2],[84,65,.85]]){ctx.lineWidth=3;drawArch(ctx,X(w,x),Y(h,y),s,accent);}
  for(let i=0;i<5;i++){const x=40+i*5,y=29+(i%2)*13;ctx.beginPath();ctx.arc(X(w,x),Y(h,y),4,0,Math.PI*2);ctx.stroke();}
  path(ctx,w,h,[[14,78],[31,60],[42,44],[63,57],[87,40]]);ctx.stroke();
 }else if(art.motif==="graft"){
  path(ctx,w,h,[[51,99],[48,75],[37,60],[31,46]]);ctx.stroke();
  path(ctx,w,h,[[48,75],[61,58],[71,36],[83,25]]);ctx.stroke();
  path(ctx,w,h,[[38,61],[26,55],[17,42]]);ctx.stroke();
  for(const [x,y] of [[17,42],[31,46],[83,25],[71,36]]){ctx.beginPath();ctx.arc(X(w,x),Y(h,y),7,0,Math.PI*2);ctx.stroke();}
  path(ctx,w,h,[[12,82],[36,77],[61,83],[88,76]]);ctx.stroke();
 }else{
  drawArch(ctx,X(w,29),Y(h,44),1.45,accent);drawArch(ctx,X(w,71),Y(h,39),1.2,accent);
  path(ctx,w,h,[[30,59],[42,50],[57,51],[70,57]]);ctx.stroke();
  for(const [x,y] of [[21,30],[79,26],[50,72]]){ctx.beginPath();ctx.arc(X(w,x),Y(h,y),5,0,Math.PI*2);ctx.stroke();}
 }
 // Realm-specific ambient movement.
 ctx.globalAlpha=.3;
 for(let i=0;i<14;i++){
  const phase=((now*(art.motif==="tide" ? .028 : art.motif==="kiln" ? .018 : .012)+i*137)%1000)/1000;
  const x=((i*71+seed*43)%100)*w/100,y=((phase*103+i*19)%100)*h/100;
  ctx.beginPath();
  if(art.motif==="tide"){ctx.moveTo(x,y);ctx.lineTo(x+5,y+10);ctx.stroke();}
  else if(art.motif==="kiln"){ctx.arc(x,y,1.5+(i%3),0,Math.PI*2);ctx.fill();}
  else if(art.motif==="graft"){ctx.moveTo(x,y);ctx.lineTo(x+5,y-3);ctx.lineTo(x+9,y+2);ctx.closePath();ctx.fill();}
  else if(art.motif==="ledger"||art.motif==="doorsteps"){ctx.fillRect(x,y,4,6);}
  else{ctx.arc(x,y,1+(i%2),0,Math.PI*2);ctx.fill();}
 }
 ctx.globalAlpha=.9;ctx.fillStyle=palette[2];ctx.font="600 12px Georgia";ctx.textAlign="left";
 ctx.fillText(id==="FirstAbsence"?"TWO FOUNDING THRESHOLDS":id==="HundredUnlived"?"A REFUGE WITHOUT A CENSUS":id==="CinderSpine"?"THE QUARRY REMEMBERS":id==="SkyBelow"?"THE BRIDGE NEEDS ITS REST":id==="TidalLedger"?"A DEBT CAN PRECEDE ITS DEBTOR":id==="HearthBeneath"?"HEAT IS HELD IN COMMON":id==="WidowedRain"?"THE SEA IS ABSENT; THE PIER IS NOT":id==="OrchardOfKings"?"NO CROWN OWNS THE ROOTS":"THREE ROADS, NO AGREED MAP",w*.1,h*.08);
 // Saved choice appears in the realm's own visual language.
 if(choice){ctx.globalAlpha=.7;ctx.strokeStyle=choice==="care"?"#e6bd77":"#b8d9e1";ctx.lineWidth=4;ctx.setLineDash(choice==="truth"?[5,4]:[]);ctx.beginPath();ctx.arc(X(w,86),Y(h,48),17+Math.sin(now/260)*2,0,Math.PI*2);ctx.stroke();ctx.setLineDash([]);}
 ctx.restore();
}
export function drawHazardField(ctx,w,h,id,now){
 const q=hazardState(id,now),x=X(w,q.x),y=Y(h,q.y),sx=w/100,sy=h/100;
 ctx.save();ctx.strokeStyle=q.active?"#ff8b78":q.warning?"#f8d17e":"rgba(230,235,221,.4)";
 ctx.fillStyle=q.active?"rgba(245,91,72,.20)":q.warning?"rgba(242,198,108,.12)":"rgba(225,238,228,.035)";
 ctx.lineWidth=q.active?4:2;ctx.setLineDash(q.warning?[7,5]:[]);
 if(q.shape==="laneX"){ctx.fillRect(x-q.rx*sx,y-q.ry*sy,2*q.rx*sx,2*q.ry*sy);ctx.strokeRect(x-q.rx*sx,y-q.ry*sy,2*q.rx*sx,2*q.ry*sy);}
 else if(q.shape==="laneY"){ctx.fillRect(x-q.rx*sx,y-q.ry*sy,2*q.rx*sx,2*q.ry*sy);ctx.strokeRect(x-q.rx*sx,y-q.ry*sy,2*q.rx*sx,2*q.ry*sy);}
 else if(q.shape==="cross"){ctx.beginPath();ctx.moveTo(x-q.rx*sx,y);ctx.lineTo(x+q.rx*sx,y);ctx.moveTo(x,y-q.ry*sy);ctx.lineTo(x,y+q.ry*sy);ctx.stroke();}
 else if(q.shape==="ring"){ctx.beginPath();ctx.ellipse(x,y,q.rx*sx,q.rx*sx,0,0,Math.PI*2);ctx.stroke();ctx.beginPath();ctx.ellipse(x,y,(q.rx-q.ry)*sx,(q.rx-q.ry)*sx,0,0,Math.PI*2);ctx.stroke();}
 else if(q.shape==="grid"){for(let i=-1;i<=1;i++){ctx.beginPath();ctx.moveTo(x+i*q.rx*sx/2,y-q.ry*sy);ctx.lineTo(x+i*q.rx*sx/2,y+q.ry*sy);ctx.moveTo(x-q.rx*sx,y+i*q.ry*sy/2);ctx.lineTo(x+q.rx*sx,y+i*q.ry*sy/2);ctx.stroke();}}
 else if(q.shape==="doubleLane"){for(const k of [-1,1]){const xx=x+k*q.rx*sx*.55;ctx.beginPath();ctx.moveTo(xx,y-q.ry*sy);ctx.lineTo(xx,y+q.ry*sy);ctx.stroke();}}
 else{ctx.beginPath();ctx.ellipse(x,y,q.rx*sx,q.ry*sy,0,0,Math.PI*2);ctx.fill();ctx.stroke();}
 ctx.setLineDash([]);ctx.textAlign="center";ctx.fillStyle=q.active?"#ffd2c4":q.warning?"#ffe4aa":"#f5ead3";ctx.font="bold 10px system-ui";ctx.fillText(q.active?"DANGER · "+q.label:q.warning?"READ THE CUE · "+q.label:q.label,x,y-8);
 ctx.restore();return q;
}
export function drawWitness(ctx,x,y,id,palette,focused=false){
 const art=realmVisual(id),s=focused?1.2:1;
 ctx.save();ctx.translate(x,y);ctx.scale(s,s);
 ctx.fillStyle="rgba(0,0,0,.38)";ctx.beginPath();ctx.ellipse(0,14,13,5,0,0,Math.PI*2);ctx.fill();
 ctx.fillStyle=art.deep;ctx.beginPath();ctx.moveTo(-10,2);ctx.lineTo(-8,-7);ctx.lineTo(-5,-12);ctx.lineTo(5,-12);ctx.lineTo(9,-6);ctx.lineTo(11,12);ctx.lineTo(-11,12);ctx.closePath();ctx.fill();
 ctx.fillStyle="#d7b892";ctx.beginPath();ctx.arc(0,-7,5.5,0,Math.PI*2);ctx.fill();
 ctx.strokeStyle=art.color;ctx.lineWidth=2;
 if(art.motif==="tide"){ctx.beginPath();ctx.arc(0,-8,8,Math.PI,Math.PI*2);ctx.stroke();ctx.beginPath();ctx.moveTo(11,1);ctx.lineTo(15,11);ctx.stroke();}
 else if(art.motif==="kiln"){ctx.beginPath();ctx.moveTo(-7,-12);ctx.lineTo(0,-18);ctx.lineTo(7,-12);ctx.stroke();ctx.beginPath();ctx.arc(10,3,3,0,Math.PI*2);ctx.stroke();}
 else if(art.motif==="ledger"){ctx.strokeRect(9,-2,7,11);ctx.beginPath();ctx.moveTo(10,1);ctx.lineTo(15,1);ctx.moveTo(10,4);ctx.lineTo(15,4);ctx.stroke();}
 else if(art.motif==="choir"){ctx.beginPath();ctx.arc(0,-13,9,Math.PI,Math.PI*2);ctx.stroke();ctx.beginPath();ctx.moveTo(-13,-1);ctx.quadraticCurveTo(0,5,13,-1);ctx.stroke();}
 else if(art.motif==="quarry"){ctx.beginPath();ctx.moveTo(-8,-13);ctx.lineTo(-2,-18);ctx.lineTo(7,-14);ctx.stroke();ctx.beginPath();ctx.moveTo(11,3);ctx.lineTo(16,8);ctx.stroke();}
 else if(art.motif==="doorsteps"){drawArch(ctx,0,-4,.38,art.color);ctx.beginPath();ctx.arc(13,7,3,0,Math.PI*2);ctx.stroke();}
 else if(art.motif==="graft"){ctx.beginPath();ctx.moveTo(-7,-14);ctx.quadraticCurveTo(-14,-18,-14,-9);ctx.moveTo(5,-15);ctx.quadraticCurveTo(13,-20,14,-11);ctx.stroke();}
 else if(art.motif==="threshold"){drawArch(ctx,-4,-4,.45,art.color);drawArch(ctx,7,-4,.32,palette[2]);}
 else{ctx.beginPath();ctx.moveTo(-9,-12);ctx.lineTo(-12,-18);ctx.lineTo(-4,-15);ctx.moveTo(9,-12);ctx.lineTo(12,-18);ctx.lineTo(4,-15);ctx.stroke();}
 ctx.restore();
}
export function drawGuardian(ctx,x,y,id,scale=1,warning=false){
 const a=realmVisual(id),s=scale;
 ctx.save();ctx.translate(x,y);ctx.scale(s,s);
 ctx.fillStyle="rgba(0,0,0,.38)";ctx.beginPath();ctx.ellipse(0,13,16,6,0,0,Math.PI*2);ctx.fill();
 ctx.fillStyle=a.deep;ctx.beginPath();ctx.moveTo(-13,11);ctx.lineTo(-11,-5);ctx.lineTo(-6,-13);ctx.lineTo(6,-13);ctx.lineTo(12,-5);ctx.lineTo(14,11);ctx.closePath();ctx.fill();
 ctx.strokeStyle=warning?"#fff0b7":a.color;ctx.fillStyle=a.color;ctx.lineWidth=2;
 if(a.bossMark==="arch"){drawArch(ctx,0,-1,.75,a.color);ctx.beginPath();ctx.moveTo(-12,5);ctx.lineTo(0,-1);ctx.lineTo(12,5);ctx.stroke();}
 else if(a.bossMark==="wave"){ctx.beginPath();ctx.moveTo(-14,5);ctx.quadraticCurveTo(-8,-7,0,5);ctx.quadraticCurveTo(8,17,14,2);ctx.stroke();ctx.beginPath();ctx.arc(0,-8,5,Math.PI,0);ctx.stroke();}
 else if(a.bossMark==="flame"){ctx.beginPath();ctx.moveTo(-8,-6);ctx.quadraticCurveTo(-12,-16,-2,-21);ctx.quadraticCurveTo(-5,-11,2,-14);ctx.quadraticCurveTo(12,-11,7,-3);ctx.stroke();ctx.beginPath();ctx.arc(0,4,4,0,Math.PI*2);ctx.fill();}
 else if(a.bossMark==="ledger"){ctx.strokeRect(-8,-13,16,22);for(let i=0;i<3;i++){ctx.beginPath();ctx.moveTo(-5,-8+i*5);ctx.lineTo(5,-8+i*5);ctx.stroke();}ctx.beginPath();ctx.moveTo(-13,-5);ctx.lineTo(-9,-5);ctx.moveTo(9,5);ctx.lineTo(13,5);ctx.stroke();}
 else if(a.bossMark==="bell"){ctx.beginPath();ctx.arc(0,-3,10,Math.PI,0);ctx.lineTo(9,6);ctx.lineTo(-9,6);ctx.closePath();ctx.stroke();ctx.beginPath();ctx.moveTo(0,6);ctx.lineTo(0,12);ctx.stroke();ctx.arc(0,-14,3,0,Math.PI*2);ctx.stroke();}
 else if(a.bossMark==="seam"){ctx.beginPath();ctx.moveTo(-5,-16);ctx.lineTo(1,-7);ctx.lineTo(-4,1);ctx.lineTo(5,8);ctx.lineTo(1,16);ctx.stroke();ctx.beginPath();ctx.moveTo(-13,-4);ctx.lineTo(-5,-4);ctx.moveTo(5,6);ctx.lineTo(13,6);ctx.stroke();}
 else if(a.bossMark==="lattice"){ctx.strokeRect(-12,-12,24,23);ctx.beginPath();for(let i=-1;i<=1;i++){ctx.moveTo(i*6,-12);ctx.lineTo(i*6,11);ctx.moveTo(-12,i*6);ctx.lineTo(12,i*6);}ctx.stroke();ctx.beginPath();ctx.arc(0,-2,3,0,Math.PI*2);ctx.stroke();}
 else if(a.bossMark==="branch"){ctx.beginPath();ctx.moveTo(0,9);ctx.lineTo(0,-4);ctx.lineTo(-9,-14);ctx.moveTo(0,-4);ctx.lineTo(9,-15);ctx.moveTo(-5,-10);ctx.lineTo(-13,-9);ctx.moveTo(5,-10);ctx.lineTo(13,-9);ctx.stroke();ctx.beginPath();ctx.arc(0,-16,2,0,Math.PI*2);ctx.stroke();}
 else{drawArch(ctx,-6,-2,.56,a.color);drawArch(ctx,7,-3,.48,paletteSafe(a.color));}
 ctx.restore();
}
function paletteSafe(c){return c==="#d8d5ec"?"#f5e8c8":"#d8d5ec";}
export function drawGuardianTelegraph(ctx,w,h,id,battle){
 const kind=realmVisual(id).attack,bx=X(w,battle.x),by=Y(h,battle.y),sx=w/100,sy=h/100,t=battle.target||battle;
 ctx.save();ctx.strokeStyle="#ffd38a";ctx.fillStyle="rgba(255,177,110,.12)";ctx.lineWidth=4;ctx.setLineDash([8,5]);
 if(kind==="sweepX"){ctx.fillRect(0,by-8*sy,w,16*sy);ctx.strokeRect(0,by-8*sy,w,16*sy);}
 else if(kind==="sweepY"||kind==="fissure"){ctx.fillRect(bx-5*sx,0,10*sx,h);ctx.strokeRect(bx-5*sx,0,10*sx,h);}
 else if(kind==="ring"){ctx.beginPath();ctx.arc(bx,by,Math.min(w,h)*.15,0,Math.PI*2);ctx.stroke();ctx.beginPath();ctx.arc(bx,by,Math.min(w,h)*.20,0,Math.PI*2);ctx.stroke();}
 else if(kind==="target"){const tx=X(w,t.x),ty=Y(h,t.y);ctx.beginPath();ctx.arc(tx,ty,Math.min(w,h)*.10,0,Math.PI*2);ctx.fill();ctx.stroke();ctx.beginPath();ctx.moveTo(tx-12,ty);ctx.lineTo(tx+12,ty);ctx.moveTo(tx,ty-12);ctx.lineTo(tx,ty+12);ctx.stroke();}
 else if(kind==="roots"||kind==="fork"){ctx.beginPath();ctx.moveTo(0,by);ctx.lineTo(w,by);ctx.moveTo(bx,0);ctx.lineTo(bx,h);ctx.stroke();ctx.beginPath();ctx.moveTo(bx,by);ctx.lineTo(bx+24*sx,by-20*sy);ctx.lineTo(bx+36*sx,by-20*sy);ctx.stroke();}
 else if(kind==="door"){const tx=X(w,t.x);ctx.beginPath();ctx.moveTo(tx-5*sx,0);ctx.lineTo(tx-5*sx,h);ctx.moveTo(tx+5*sx,0);ctx.lineTo(tx+5*sx,h);ctx.stroke();}
 else{ctx.beginPath();ctx.arc(bx,by,Math.min(w,h)*.16,0,Math.PI*2);ctx.stroke();}
 ctx.setLineDash([]);ctx.restore();
}
export function guardianStrikeHits(id,pos,battle){
 const kind=realmVisual(id).attack,dx=pos.x-battle.x,dy=pos.y-battle.y,ax=Math.abs(dx),ay=Math.abs(dy),d=Math.hypot(dx,dy);
 if(kind==="sweepX")return ay<8&&ax<48;
 if(kind==="sweepY"||kind==="fissure")return ax<5&&ay<34;
 if(kind==="ring")return d>12&&d<24;
 if(kind==="target"){const t=battle.target||battle;return Math.hypot(pos.x-t.x,pos.y-t.y)<10;}
 if(kind==="roots")return (ax<5&&ay<34)||(ay<5&&ax<34);
 if(kind==="fork")return (ax<6&&ay<30)||(ay<6&&ax<30);
 if(kind==="door"){const t=battle.target||battle;return Math.abs(pos.x-t.x)<5&&Math.abs(pos.y-t.y)<34;}
 if(kind==="sweepY")return ax<7&&ay<32;
 return d<17;
}
export function guardianCue(id){
 const k=realmVisual(id).attack;
 return k==="sweepX"?"Wave line · step clear or guard":k==="sweepY"?"Debt lane · move sideways":k==="ring"?"Resonant ring · leave its radius":k==="fissure"?"Quarry fissure · cross the line":k==="target"?"Census mark · leave the target":k==="roots"?"Root cross · move between arms":k==="fork"?"Forked road · leave both arms":k==="door"?"Threshold beam · step out of the opening":"Vent burst · retreat or guard";
}
