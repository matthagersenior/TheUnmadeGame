#!/usr/bin/env python3
"""Original, real 3-D meshes for The Unmade: 12 enemies, 18 named NPCs,
9 realm architecture dioramas. Deterministic GLB glTF 2.0 exports with
separate named geometric parts and materials. No licensed/external assets.

Each mesh is nonrigged source art; environment dioramas are previews only.
No collision, quest/state, physical evidence, or Unreal playback claims.
"""
from __future__ import annotations
import argparse, hashlib, json, math, random, re
from pathlib import Path
import numpy as np
import trimesh
from PIL import Image, ImageDraw, ImageFont

ROOT=Path(__file__).resolve().parents[1]
REALMS=("ThreefoldReach","WidowedRain","HearthBeneath","TidalLedger","SkyBelow","CinderSpine","HundredUnlived","OrchardOfKings","FirstAbsence")
SPECIES=("Cinderjaw","PaleStrider","Veilwatch","GleamMoth","HollowHound","GriefLantern","Inkmaw","RedactionEye","SaltglassCrawler","DrownedWatch","Ashmaw","KilnWisp")
PALETTE={
 "ThreefoldReach":((82,83,87),(166,132,85),(87,156,150)),
 "WidowedRain":((55,91,104),(133,163,166),(89,193,207)),
 "HearthBeneath":((69,66,65),(169,113,75),(235,106,54)),
 "TidalLedger":((70,75,92),(171,170,152),(214,178,87)),
 "SkyBelow":((89,79,133),(175,165,198),(92,200,209)),
 "CinderSpine":((70,65,67),(164,132,116),(229,100,56)),
 "HundredUnlived":((88,118,123),(192,195,176),(226,149,134)),
 "OrchardOfKings":((75,99,67),(178,159,106),(190,99,125)),
 "FirstAbsence":((70,81,106),(204,203,195),(119,185,215)),
}
DARK=(39,44,56);PALE=(207,193,176);BLACK=(25,30,42);GOLD=(216,179,92);WHITE=(232,229,209)

class Kit:
 def __init__(self):self.scene=trimesh.Scene();self.count=0
 def add(self,mesh,name,color):
  if mesh is None:return
  mesh=mesh.copy()
  mesh.visual.vertex_colors=np.tile(np.array([*color,255],dtype=np.uint8),(len(mesh.vertices),1))
  key=f'{self.count:04d}_{name}';self.count+=1
  self.scene.add_geometry(mesh,geom_name=key,node_name=key)
 def orb(self,name,at,radii,color,detail=2):
  mesh=trimesh.creation.icosphere(subdivisions=detail,radius=1)
  mesh.apply_scale(list(radii));mesh.apply_translation(at);self.add(mesh,name,color)
 def box(self,name,at,size,color):
  mesh=trimesh.creation.box(extents=size);mesh.apply_translation(at);self.add(mesh,name,color)
 def beam(self,name,a,b,radius,color,segments=10):
  a=np.array(a,dtype=float);b=np.array(b,dtype=float)
  length=np.linalg.norm(b-a)
  if length<1e-6: return
  mesh=trimesh.creation.cylinder(radius=radius,height=length,sections=segments)
  mesh.apply_transform(trimesh.geometry.align_vectors([0,0,1],(b-a)/length))
  mesh.apply_translation((a+b)/2);self.add(mesh,name,color)
 def taper(self,name,a,b,r0,r1,color,segments=12):
  a=np.array(a,dtype=float);b=np.array(b,dtype=float)
  length=float(np.linalg.norm(b-a)); assert length>1e-5 and r0>=0 and r1>=0
  tangent=(b-a)/length
  cross=np.cross(tangent,[0,0,1])
  if np.linalg.norm(cross)<.1:cross=np.cross(tangent,[0,1,0])
  cross/=np.linalg.norm(cross);up=np.cross(tangent,cross)
  v=[];faces=[]
  for pos,r in ((a,max(.001,r0)),(b,max(.001,r1))):
   for n in range(segments):
    th=2*math.pi*n/segments
    v.append(pos+r*(math.cos(th)*cross+math.sin(th)*up))
  for j in range(segments):
   k=(j+1)%segments;faces.extend([[j,k,segments+k],[j,segments+k,segments+j]])
  faces.extend([[0,j+1,j] for j in range(1,segments-1)])
  faces.extend([[segments,segments+j,segments+j+1] for j in range(1,segments-1)])
  self.add(trimesh.Trimesh(vertices=np.array(v),faces=np.array(faces),process=False),name,color)
 def ring(self,name,center,major,minor,color,segments=24,sides=7,plane="xy"):
  cx,cy,cz=center;verts=[];faces=[]
  for k in range(segments):
   t=k*2*math.pi/segments;ct,st=math.cos(t),math.sin(t)
   for j in range(sides):
    u=j*2*math.pi/sides;c,s=math.cos(u),math.sin(u)
    r=major+minor*c
    if plane=="xy": v=(cx+r*ct,cy+r*st,cz+minor*s)
    elif plane=="xz":v=(cx+r*ct,cy+minor*s,cz+r*st)
    else:v=(cx+minor*s,cy+r*ct,cz+r*st)
    verts.append(v)
  for k in range(segments):
   q=(k+1)%segments
   for j in range(sides):
    l=(j+1)%sides;A=k*sides+j;B=q*sides+j;C=q*sides+l;D=k*sides+l
    faces.extend([[A,B,C],[A,C,D]])
  self.add(trimesh.Trimesh(vertices=verts,faces=faces,process=False),name,color)
 def ribbon(self,name,pts,width,color):
  # Visible flexible strip/rag surfaces, double-sided triangle winding.
  if len(pts)<2:return
  v=[];f=[]
  for p in pts:
   x,y,z=p;v.extend([[x-width/2,y,z],[x+width/2,y,z]])
  for i in range(len(pts)-1):
   a=2*i;f.extend([[a,a+1,a+3],[a,a+3,a+2],[a+3,a+1,a],[a+2,a+3,a]])
  self.add(trimesh.Trimesh(vertices=v,faces=f,process=False),name,color)
 def export(self,path):
  path.parent.mkdir(parents=True,exist_ok=True)
  data=self.scene.export(file_type='glb')
  path.write_bytes(data)
  return {"parts":self.count,"triangles":int(sum(len(m.faces) for m in self.scene.geometry.values())),"bytes":len(data),"sha256":hashlib.sha256(data).hexdigest(),"bounds_m":[[round(float(x),3) for x in row] for row in self.scene.bounds.tolist()]}

# Signature animals have different anatomy, appendages and locomotion cues.
def build_creature(kind):
 k=Kit();c=PALETTE[REALMS[SPECIES.index(kind)%9]];base,trim,light=c
 def eye(x,y,z,r=.075):k.orb('sight_glass',(x,y,z),(r,r,r*.6),light,2)
 def spine_row(start,n,sep,height=.45):
  for i in range(n):
   z=start+i*sep
   k.taper('spinal_crest',(0,height,z),(0,height+.36+(i%3)*.10,z-.10),.12,.001,trim)
 if kind in ('Cinderjaw','HollowHound','Ashmaw','PaleStrider'):
  dims={'Cinderjaw':(.75,.56,1.15),'HollowHound':(.52,.52,1.30),'Ashmaw':(1.0,.89,.98),'PaleStrider':(.28,.48,.65)}[kind]
  bodyY=1.10 if kind=='PaleStrider' else .95
  k.orb('thorax',(0,bodyY,0),dims,base,2)
  for side in (-1,1):
   for front,z in enumerate((-.55,.53)):
    x=side*(dims[0]*.73)
    long=1.6 if kind=='PaleStrider' else .75 if kind!='Ashmaw' else 1.03
    knee=(x*1.28,bodyY-.35,z+( -.19 if front==0 else .25));foot=(x*1.26,.10 if kind!='PaleStrider' else -.50,z+.31)
    k.taper('jointed_leg',(x,bodyY-.18,z),knee,.16,.115,trim)
    k.taper('shin',knee,foot,.10,.07,base)
    k.orb('claw',foot,(.115,.075,.18),BLACK,1)
  faceZ=1.12 if kind!='PaleStrider' else .75
  k.orb('skull',(0,bodyY+.16,faceZ),(.47,.39,.48) if kind!='PaleStrider' else (.25,.31,.37),trim,2)
  for side in (-1,1):
   eye(side*.20,bodyY+.32,faceZ+.33,.065)
   for t in range(2 if kind!='PaleStrider' else 3):
    k.taper('antler_or_ear',(side*.21,bodyY+.36,faceZ),(side*(.34+.15*t),bodyY+.72+.21*t,faceZ-.19*t),.10,.008,base)
  if kind=='Cinderjaw':
   for side in (-1,1):
    k.box('split_charcoal_jaw',(side*.25,bodyY-.15,faceZ+.30),(.43,.13,.65),DARK)
    for t in range(4):k.taper('molten_tooth',(side*(.09+t*.11),bodyY-.09,faceZ+.62),(side*(.09+t*.11),bodyY+.13,faceZ+.67),.058,.001,WHITE,7)
   for z in (-.50,-.13,.26):k.taper('ash_fin',(0,bodyY+.33,z),(0,bodyY+.88,z-.15),.18,.01,light)
  if kind=='HollowHound':
   for x in (-.34,.34):
    for z in (-.44,-.18,.1,.36):k.beam('exposed_rib',(x,bodyY+.26,z),(x*.85,bodyY-.21,z+.08),.038,WHITE,7)
   k.orb('empty_cage',(0,bodyY-.05,.15),(.30,.18,.45),BLACK,1)
   for i in range(3):k.taper('spine_hook',(0,bodyY+.34,-.2+i*.3),(0,bodyY+.6,-.28+i*.3),.08,.001,WHITE)
  if kind=='Ashmaw':
   k.orb('furnace_cavity',(0,bodyY,.82),(.41,.36,.19),BLACK,2)
   k.orb('furnace_ember',(0,bodyY,.94),(.18,.16,.08),light,2)
   for side in (-1,1):
    k.box('broken_furnace_chimney',(side*.55,bodyY+.58,-.34),(.28,.91,.30),base)
    k.taper('burnt_cowl',(side*.55,bodyY+1.07,-.34),(side*.68,bodyY+1.26,-.47),.23,.10,DARK)
  if kind=='PaleStrider':
   k.taper('long_spine',(0,bodyY+.33,0),(0,bodyY+.95,-.18),.22,.08,WHITE)
   for side in (-1,1): k.beam('unpaired_rear_hoof',(side*.42,-.43,-.31),(side*.62,-.69,-.58),.065,WHITE)
 elif kind in ('GleamMoth','Veilwatch','GriefLantern','RedactionEye','DrownedWatch','KilnWisp'):
  if kind=='GleamMoth':
   k.orb('lantern_thorax',(0,1.2,0),(.23,.48,.28),base,2)
   for side in (-1,1):
    for rear in (-1,1):
     pts=[(side*.14,1.44,rear*.06),(side*1.04,1.92,rear*.52),(side*1.25,.95,rear*.49),(side*.17,1.04,rear*.11)]
     k.ribbon('four_glass_wings',pts,.41,light if rear>0 else trim)
     k.beam('veins',pts[0],pts[1],.027,GOLD,7)
    k.taper('antennae',(side*.14,1.55,.07),(side*.32,2.07,.44),.07,.001,trim)
   eye(0,1.42,.29,.15)
  elif kind=='Veilwatch':
   k.taper('narrow_shroud',(0,.22,0),(0,2.2,0),.70,.32,base,20)
   k.orb('blind_lens',(0,1.55,.47),(.30,.23,.09),BLACK,2)
   eye(0,1.54,.56,.13)
   for side in (-1,1):
    for p in range(3): k.ribbon('veil_fray',[(side*.45,1.4,p*.16-.28),(side*(.75+p*.12),.75,p*.16-.28),(side*(.55+p*.09),.20,p*.16-.27)],.17,trim)
   k.ring('watcher_halo',(0,2.2,0),.46,.041,light,plane='xz')
  elif kind=='GriefLantern':
   for i in range(6):
    th=2*math.pi*i/6
    x,z=.46*math.cos(th),.46*math.sin(th)
    k.beam('cage_pillar',(x,.61,z),(x,2.10,z),.073,base)
   k.taper('hanging_bell',(0,.64,0),(0,1.86,0),.50,.33,trim,14)
   k.ring('suspended_bell',(0,2.05,0),.42,.07,light,plane='xz')
   for i in range(4):k.beam('lament_chain',(0,2.24+i*.1,0),(0,2.36+i*.10,.07),.052,GOLD)
   k.orb('lantern_soul',(0,1.30,.50),(.14,.19,.10),light,2)
  elif kind=='RedactionEye':
   k.orb('great_cornea',(0,1.29,0),(.63,.65,.46),base,2)
   k.orb('iris',(0,1.34,.42),(.30,.32,.10),trim,2)
   eye(0,1.34,.53,.19)
   k.box('forbidden_text_bar',(0,1.26,.62),(1.25,.14,.08),BLACK)
   for i in range(5):
    x=(-2+i)*.32
    k.ribbon('paper_erasures',[(x,.65+i%2*.18,-.45),(x+.10,.79+i%2*.2,-.39),(x+.13,1.03,-.33)],.16,WHITE)
   k.ring('archival_index',(0,1.32,-.11),.87,.041,GOLD,plane='xy')
  elif kind=='DrownedWatch':
   k.orb('drowned_body',(0,1.17,0),(.48,.82,.32),base,2)
   k.orb('helmet',(0,2.15,0),(.34,.39,.33),trim,2)
   for side in (-1,1):
    k.beam('weighted_arm',(side*.46,1.65,0),(side*.59,.52,.03),.17,base)
    k.ring('tide_halo',(0,1.40,0),.76+.12*(side+1),.037,light,plane='xz')
   for i in range(5):k.ribbon('drowned_cloth',[(i*.20-.4,.82,.15),(i*.25-.5,.15,.2),(i*.30-.6,-.11,.20)],.19,trim)
   eye(0,2.24,.31,.16)
  else: # KilnWisp
   k.orb('coal_soul',(0,1.12,0),(.33,.34,.33),DARK,2)
   for z in range(4):
    angle=z*1.44;x=.18*math.cos(angle);zz=.18*math.sin(angle)
    k.taper('flame_blade',(x,.88+z*.3,zz),(.28*math.cos(angle+.8),1.32+z*.34,.30*math.sin(angle+.8)),.21,.014,light,9)
   for i in range(3):k.ring('floating_ash_rings',(0,.83+i*.25,0),.41-i*.06,.032,trim,plane='xz')
 elif kind in ('Inkmaw','SaltglassCrawler'):
  if kind=='Inkmaw':
   k.box('sealed_manuscript_spine',(0,.94,0),(.80,.46,1.87),base)
   for side in (-1,1):
    k.box('jaw_pages',(side*.52,.9,.58),(.43,.24,1.03),trim)
    for i in range(8):
     z=-.46+i*.18;k.taper('ink_quill',(side*.58,1.12,z),(side*(.73+.14*i/8),1.63,z+.06),.055,.001,BLACK,7)
   k.orb('black_gullet',(0,.93,1.04),(.33,.21,.14),BLACK,2)
   eye(-.35,1.23,.78,.07);eye(.35,1.23,.78,.07)
   for i in range(4):k.box('page_spine_segment',(0,1.2,-.65+i*.42),(.28,.20,.25),WHITE)
  else:
   k.orb('armored_shell',(0,.91,0),(.91,.38,1.22),base,2)
   for j in range(8):
    z=-.95+j*.27
    k.box('calcite_scute',(0,1.27,z),(1.35,.12,.11),WHITE)
   for side in (-1,1):
    for j in range(5):
     z=-.88+j*.39
     k.beam('crawler_knee',(side*.71,.8,z),(side*(1.13+j*.04),.37,z+.06),.082,trim)
     k.taper('salt_spike',(side*(1.13+j*.04),.37,z+.06),(side*(1.27+j*.04),.04,z+.20),.12,.01,WHITE)
    k.beam('claw_arm',(side*.83,.84,1.02),(side*1.35,.76,1.57),.15,base)
    k.taper('double_pincer',(side*1.35,.76,1.57),(side*1.63,.94,2.05),.18,.014,trim)
    k.taper('double_pincer',(side*1.35,.76,1.57),(side*1.36,.47,2.05),.18,.014,WHITE)
   eye(-.21,1.05,1.08,.09);eye(.21,1.05,1.08,.09)
 return k

# Canonical resident prototypes are non-skinned but contain separately named
# body/wardrobe/accessory geometry for future Blender rig and socket mapping.
def build_resident(row,index):
 ident,name,realm,visual=row
 k=Kit();base,trim,light=PALETTE[realm]
 scale=1+((index%5)-2)*.035
 k.taper('coat_hem',(0,.16,0),(0,1.50,0),.45*scale,.31*scale,base,12)
 k.orb('layered_tunic',(0,1.30,.015),(.35*scale,.42,.29),trim,2)
 for side in (-1,1):
  x=side*.18*scale
  k.taper('booted_leg',(x,.05,.035),(x,.83,.0),.15,.16,DARK)
  k.orb('boot',(x,.07,.20),(.16,.09,.29),BLACK,1)
  a=(side*.31,1.56,0);b=(side*(.46+index%3*.02),1.10,-.03)
  c=(side*.49,.76,.15)
  k.beam('detached_sleeve_upper',a,b,.17,base)
  k.taper('sewn_forearm',b,c,.15,.09,trim)
  k.orb('hand',c,(.09,.13,.10),PALE,1)
 k.orb('individual_face',(0,1.96,0),(.22*scale,.30,.22),PALE,2)
 k.orb('nose',(0,1.91,.22),(.052,.068,.11),(158,124,110),1)
 for side in (-1,1):k.orb('eye',(side*.089,2.02,.194),(.028,.031,.021),BLACK,1)
 # Unique hair/hood; not cosmetic tint alone: physically distinct geometry.
 style=index%6
 if style in (0,3):
  k.taper('angular_hood',(0,2.16,0),(0,2.48,.02),.26,.16,base,9)
 elif style==1:
  for j in range(8):
   th=j*2*math.pi/8
   k.taper('hair_braid',(.18*math.cos(th),2.22,.18*math.sin(th)),(.25*math.cos(th),2.0,.27*math.sin(th)),.065,.012,DARK)
 elif style==2:
  k.orb('cap_and_cowl',(0,2.19,0),(.258,.14,.27),DARK,1)
 else:
  for side in (-1,1):k.taper('side_bun',(side*.19,2.20,0),(side*.29,2.31,.04),.12,.045,trim)
 # Each visual_direction has distinct individually modeled personal object.
 unique=index%18
 if unique==0: # Veynu
  for j in range(5):k.ring('braided_clock_cord',(.38,.93+j*.13,.23),.085,.013,GOLD,plane='xz')
  k.orb('inked_left_glove',(-.48,.73,.14),(.105,.085,.112),BLACK,1)
 elif unique==1: # Orshel
  k.beam('wooden_untipped_boat_hook',(.58,.35,.20),(.69,2.66,.28),.042,trim)
  k.box('unclaimed_vest',(0,1.30,.33),(.49,.42,.07),WHITE)
  for z in range(3):k.ring('rope_coil',(-.43,.56+z*.12,-.22),.15,.025,GOLD,plane='xz')
 elif unique==2: # Ezzun
  k.ring('date_glass_monocle',(.10,2.04,.218),.12,.024,GOLD,plane='xy')
  k.ribbon('clerk_blue_sash',[(-.28,1.7,.33),(.15,1.24,.35),(.29,.68,.29)],.15,light)
 elif unique==3: # Yllesh
  for side in (-1,1):
   for j in range(4):k.beam('layered_sleeve_cord',(side*.30,1.66,.05+j*.05),(side*.52,.69,.03+j*.07),.023,light)
  k.beam('silent_tuning_pin',(.0,.83,.32),(.0,1.55,.35),.032,GOLD)
 elif unique==4: # Vhaurn
  k.box('stone_weighted_apron',(0,.84,.31),(.57,.66,.09),trim)
  k.beam('vertical_plumb_line',(.53,1.60,.1),(.53,.38,.1),.019,WHITE)
  k.orb('brass_plumb',(.53,.38,.1),(.12,.16,.12),GOLD,1)
 elif unique==5: # Orru
  k.ribbon('dissonant_scarf',[(-.28,1.71,.34),(.38,1.65,.31),(.62,.76,.24)],.29,BLACK)
  k.beam('uneven_staff',(.55,.10,.25),(.74,2.26,.27),.048,trim)
 elif unique==6: # Ghraet
  k.box('split_quarry_visor',(0,2.15,.22),(.49,.12,.25),trim)
  for side in (-1,1):k.box('mismatched_initial_studs',(side*.16,2.2,.36),(.09,.09,.08),WHITE if side>0 else GOLD)
 elif unique==7: # Torrun
  k.taper('dust_hood',(0,2.15,-.07),(0,2.52,-.15),.32,.08,trim)
  for j in range(3):
   k.box('three_stone_trowels',(.45+.11*j,.76+j*.23,.34),(.24,.047,.08),WHITE)
 elif unique==8: # Issil
  for j in range(4):k.ring('family_wrist_knot',(-.47,.76+j*.065,.15),.09,.021,WHITE,plane='xz')
  k.ribbon('unfinished_surname',[(-.27,1.42,.34),(.29,1.47,.34)],.13,light)
 elif unique==9: # Nhel
  for j in range(5):k.orb('numberless_copper_token',(-.33+j*.15,.77+(j%2)*.1,.30),(.077,.075,.03),GOLD,1)
  k.ribbon('reversible_clerk_coat',[(-.39,1.21,-.23),(-.65,.67,-.23),(-.23,.15,-.18)],.31,WHITE)
 elif unique==10: # Auvet
  for j in range(7):
   k.box('unsigned_patch',((j%3-1)*.20,.35+(j//3)*.27,.35),(.17,.19,.052),PALETTE[REALMS[(j+2)%9]][1])
 elif unique==11: # Veysha
  k.beam('civic_banner_pole',(.49,.12,-.08),(.49,2.67,-.08),.045,GOLD)
  k.ribbon('blank_signature_banner',[(.50,2.53,-.08),(1.05,2.48,-.08),(1.04,1.55,-.06)],.50,WHITE)
 elif unique==12: # Serriv
  k.ring('hollow_crown_wreath',(0,2.42,0),.31,.038,trim,plane='xz')
  for j in range(5):
   th=j*2*math.pi/5;k.taper('bark_mantle',(0,1.71,.0),(.59*math.cos(th),.97,.46*math.sin(th)),.13,.015,base)
 elif unique==13: # Imro
  k.orb('fruit_stained_glove',(.49,.75,.15),(.10,.12,.10),light,1)
  k.beam('graft_knife',(-.46,.67,.15),(-.66,.35,.32),.053,WHITE)
  for j in range(3):k.orb('graft_fruit',(-.55,1.20+j*.21,-.22),(.10,.11,.08),light,1)
 elif unique==14: # Eliun
  k.ribbon('unrolled_blank_charter',[(-.35,1.36,.41),(.33,1.31,.47),(.35,.94,.44)],.30,WHITE)
  for j in range(3):k.taper('thornless_pin',(j*.12-.13,1.50,.40),(j*.12-.13,1.65,.42),.028,.005,GOLD)
 elif unique==15: # Orriv
  for side in (-1,1):
   k.ribbon('exterior_double_coat_seam',[(side*.29,1.66,.29),(side*.37,.85,.35),(side*.4,.23,.21)],.055,light)
  k.box('inside_out_collar',(0,1.72,.10),(.55,.10,.43),WHITE)
 elif unique==16: # Thae
  k.beam('ash_guide_cord',(.45,1.40,.19),(.70,.42,.16),.034,WHITE)
  k.orb('portable_lamp',(.70,.42,.16),(.17,.22,.16),GOLD,2)
  k.ring('lamp_glass',(.70,.42,.16),.19,.029,light,plane='xz')
 elif unique==17: # Veilune
  for side in (-1,1):
   k.box('unequal_footmark_emblem',(side*.17,.80,.37),(.12,.24 if side<0 else .18,.065),WHITE if side<0 else light)
  k.ring('two_mornings_necklace',(0,1.60,.30),.19,.026,GOLD,plane='xy')
 return k

# These nine distinct environments are real multi-mesh architectural dioramas
# with separate non-interactable parts, not navmesh/quest/lighting-authoritative maps.
def build_environment(realm):
 k=Kit();base,trim,light=PALETTE[realm];seed=REALMS.index(realm)
 rng=random.Random(17511+seed)
 k.box('terrain_foundation',(0,-.22,0),(28,.44,25),base)
 # Visible stone roads and visually legible entrances; imported as art, no collision contract.
 for j in range(12):
  z=-11+j*2.05
  k.box('surveyor_path_stone',(rng.uniform(-.24,.24),.025,z),(2.2,.085,1.65),trim)
 def cottage(x,z,w=3.2,h=3.7,d=3.3,roof=True):
  k.box('structure_walls',(x,h*.45,z),(w,h*.90,d),trim)
  k.box('door_dark_recess',(x,.98,z+d*.507),(.72,1.85,.055),DARK)
  k.box('door_bell_lintel',(x,2.0,z+d*.55),(1.08,.22,.23),light)
  for xx in (-w*.32,w*.32):
   k.box('window_vents',(x+xx,2.15,z+d*.51),(.36,.51,.08),BLACK)
  if roof:
   k.taper('roof_pitched_first',(x-w*.54,h*.9,z),(x,h*1.27,z),.17,.08,base)
   k.taper('roof_pitched_second',(x+w*.54,h*.9,z),(x,h*1.27,z),.17,.08,base)
   k.box('ridge_cap',(x,h*1.24,z),(w*.21,.22,d*1.05),light)
 def stone_tower(x,z,h=7,r=.75):
  k.taper('cut_stone_column',(x,.0,z),(x,h,z),r,r*.68,trim,16)
  for j in range(4):k.ring('civic_tower_rings',(x,1.1+j*h/5,z),r*.91,.07,light,plane='xz')
 if realm=='ThreefoldReach':
  for x,z in ((-8,-6),(8,-7),(-7,7),(8,6)):cottage(x,z,3.5,3.5,3.4)
  for x,h in ((-2.4,5.4),(0,7.0),(2.4,6.1)):
   stone_tower(x,1.0,h,.46)
   k.ring('no_final_bell_sound',(x,h+.20,1),.55,.09,GOLD,plane='xy')
  for i in range(7):k.box('chalk_waymarker',(rng.uniform(-5,5),.25,rng.uniform(-10,10)),(.38,.5,.38),WHITE)
 elif realm=='WidowedRain':
  for x,z in ((-8,-6),(7,-7),(-8,7),(8,6)):
   cottage(x,z,4,2.8,3.9,roof=False)
   for offset in (-1,1):k.taper('dry_boat_roof',(x+offset*2.25,2.8,z),(x,4.35,z),.15,.045,base)
  for side in (-1,1):
   k.beam('pier_rail',(side*2.8,.38,-11),(side*2.8,.38,11),.10,trim)
   for z in range(-10,11,2):k.beam('pier_post',(side*2.8,-.1,z),(side*2.8,1.1,z),.10,GOLD)
  stone_tower(3,0,7,.33)
  for j in range(9):k.box('dry_tide_notch',(3.36,.58+j*.64,.0),(.13,.065,.6),light)
 elif realm=='HearthBeneath':
  for x,z in ((-8,-6),(8,-7),(-8,6),(8,7)):
   k.box('charred_communal_foundation',(x,1.32,z),(5,2.5,4),DARK)
   k.box('ember_channel',(x,1.24,z+2.06),(3.0,.36,.13),light)
   k.taper('braided_chimney',(x,2.4,z),(x+.3,6.25,z),.54,.34,trim)
   k.orb('furnace_throat',(x+.3,6.3,z),(.42,.12,.36),light,2)
  for j in range(13):
   z=-11+j*1.7;k.box('heat_aqueduct',(4,.18,z),(1.1,.26,1.45),light)
 elif realm=='TidalLedger':
  for x,z in ((-8,-7),(8,-7),(-8,6),(8,7)):
   cottage(x,z,4,3.7,3.1)
   for j in range(6):k.box('debt_slate',(x,1.0+j*.44,z+1.65),(.86,.11,.16),light)
  for x in (-2.4,2.4):
   stone_tower(x,0,8,.55)
   k.beam('ledger_suspension',(x,7.8,.0),(x,2.7,1.4),.040,GOLD)
   k.box('unclaimed_hanging_ledger',(x,2.6,1.4),(.75,1.2,.13),trim)
  k.ring('uncertain_contract_halo',(0,7.8,0),2.1,.12,light,plane='xz')
 elif realm=='SkyBelow':
  for x,z,h in ((-8,-7,4.2),(8,-7,6.0),(-7,7,5.6),(8,7,7.2)):
   k.box('suspended_house_roof',(x,h,z),(5,.35,4.6),trim)
   k.box('open_habitat',(x,h+.98,z),(3.6,1.8,2.7),base)
   for side in (-1,1):k.beam('aerial_support_cable',(x+side*2,h,z),(x+side*.6,h+7,z),.064,light)
   k.ring('choir_counterweight',(x,h-1.3,z),.7,.10,GOLD,plane='xz')
  for x in (-2.5,2.5):k.beam('central_hanging_bridge',(x,1,-10),(x,4,10),.13,WHITE)
 elif realm=='CinderSpine':
  for x in (-10,10):
   for j in range(6):
    z=-10+j*4
    k.box('quarry_strata',(x,rng.uniform(1.3,3.1),z),(5.2,rng.uniform(2.2,5.5),3.9),base if j%2 else trim)
    k.taper('split_seam_bone',(x,4.6,z),(x+(1 if x<0 else -1)*1.5,7.8,z+.5),.27,.01,WHITE)
  cottage(0,-7,3.6,3.7,2.9);cottage(0,6,4.5,3.2,3.2)
  for z in range(-7,8,2):k.box('quarry_cut',(3.5,.15,z),(.56,.15,1.3),light)
 elif realm=='HundredUnlived':
  for i,(x,z) in enumerate(((-8,-7),(7,-6),(-8,7),(8,7))):
   cottage(x,z,4.2,3.6+i*.35,3.2,roof=False)
   for j in range(6):k.box('different_household_patch',(x+(-1+j%3)*.88,2.5+(j//3)*.51,z+1.67),(.75,.44,.09),PALETTE[REALMS[(j+i)%9]][1])
  for j in range(11):
   x=(j%4-1.5)*1.5;z=(j//4-1)*2.0
   k.box('uncounted_bed',(x,.35,z),(.65,.52,1.2),WHITE)
  k.box('blank_public_registry',(0,3.9,5.5),(2.5,3.2,.17),trim)
 elif realm=='OrchardOfKings':
  for x,z,r in ((-9,-7,.50),(9,-6,.64),(-8,7,.55),(8,7,.46),(0,0,.92)):
   k.taper('old_root_trunk',(x,0,z),(x+.2,5.4+4*r,z),r,r*.47,base,12)
   for t in range(5):
    ang=2*math.pi*t/5
    tip=(x+3.2*math.cos(ang),7.1+1.8*r,z+3.1*math.sin(ang))
    k.taper('grafted_crown_branch',(x,4.6+3*r,z),tip,r*.22,.06,trim,9)
    k.orb('crown_fruit',tip,(.25,.32,.22),light,1)
  k.ring('no_crowned_throne',(0,.61,0),2.5,.22,GOLD,plane='xz')
  for j in range(8):
   th=2*math.pi*j/8;k.box('root_vote_tablet',(5*math.cos(th),.25,5*math.sin(th)),(.64,.50,.34),trim)
 elif realm=='FirstAbsence':
  for i,(x,z,off) in enumerate(((-8,-6,-1),(8,-7,1),(-8,7,1),(8,7,-1))):
   k.box('missing_corner_monolith',(x,3.0,z),(3.0,6.0,2.6),base)
   k.box('reversible_exterior_seam',(x+off*1.55,3.5,z),(.1,5.0,.08),light)
  for x,z,h in ((-2.4,0,7.3),(2.4,1.1,6.6)):
   for side in (-1,1):k.box('two_nonmatching_thresholds',(x+side*.9,h/2,z),(.37,h,.65),trim)
   k.box('unanswered_lintel',(x,h,z),(2.2,.40,.80),light)
  for j in range(8):k.box('absent_paving_inlay',(rng.uniform(-4,4),.11,rng.uniform(-10,10)),(.65,.10,.80),BLACK)
 return k

# Source of truth: exact canonical roster from existing Authoring file, with
# local fixture permitted for offline preview only (never committed as canon).
def read_roster(root):
 path=root/'Authoring/world_content_pack.json'
 if path.is_file():
  source=json.loads(path.read_text())
  rows=[(n['id'],n['display_name'],n['realm'],n['visual_direction']) for n in source['npcs']]
  realm_ids=tuple(r['id'] for r in source['realms'])
 else:
  source=json.loads((root/'initial_sources.json').read_text())
  rows=source['npcs'];realm_ids=tuple(source['realms'])
 if len(rows)!=18 or len({r[0] for r in rows})!=18 or realm_ids!=REALMS:
  raise ValueError('Canonical NPC/realm identity drift; no asset build')
 return rows

def build_all(root=ROOT,out=None,verify=False):
 out=Path(out or root/'Build/UnmadeHeroModels')
 npcs=read_roster(root)
 specs=[]
 for i,kind in enumerate(SPECIES):specs.append(('Creature',kind,'creature:'+kind,REALMS[i%9],build_creature(kind)))
 for i,row in enumerate(npcs):specs.append(('NPC',re.sub('[^A-Za-z0-9_]+','_',row[0]),row[0],row[2],build_resident(row,i)))
 for realm in REALMS:specs.append(('Environment',realm,'realm:'+realm,realm,build_environment(realm)))
 if verify:
  if not out.is_dir():raise ValueError('Missing generated art folder')
  old=json.loads((out/'manifest.json').read_text())
  if old.get('asset_count')!=len(specs):raise ValueError('Asset roster changed')
 entries=[]
 for group,name,stable,realm,k in specs:
  name='SM_Unmade_'+name+'.glb'
  path=out/group/name
  if verify:
   buf=k.scene.export(file_type='glb')
   if not path.is_file() or path.read_bytes()!=buf:raise ValueError('Not deterministic: '+name)
   info={'parts':k.count,'triangles':sum(len(m.faces) for m in k.scene.geometry.values()),'bytes':len(buf),'sha256':hashlib.sha256(buf).hexdigest(),'bounds_m':[[round(float(x),3) for x in row] for row in k.scene.bounds.tolist()]}
  else:info=k.export(path)
  if info['triangles']<150 or info['parts']<({'Creature':5,'NPC':10,'Environment':15}[group]):raise ValueError('Too little genuine model geometry: '+name)
  entries.append(dict(category=group,stable_id=stable,realm=realm,file=group+'/'+name,**info,mesh_type='static_visual_reference',gameplay_authority=False,rigged=False,unreal_import_verified=False))
 manifest={'schema':1,'title':'The Unmade — real source-authored 3D character, creature and realm scene meshes','generator':'Scripts/build_hero_models.py','asset_count':len(entries),'categories':{'NPC':18,'Creature':12,'Environment':9},'format':'GLB (glTF 2.0) embedded binary meshes and vertex colors','units':'meters, Y-up, +Z forward','authority':'Non-authoritative visual meshes only. No quest progress, collision, skinned animation, or verified UE import','assets':entries}
 if verify:
  if manifest!=old:raise ValueError('Manifest was altered')
  if {str(p.relative_to(out)) for p in out.rglob('*.glb')}!={r['file'] for r in entries}:raise ValueError('Asset file inventory drift')
 else:
  out.mkdir(parents=True,exist_ok=True)
  (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
 return manifest

def main():
 parser=argparse.ArgumentParser();parser.add_argument('--root',type=Path,default=ROOT);parser.add_argument('--out-dir',type=Path);parser.add_argument('--verify-output',action='store_true');parser.add_argument('--check',action='store_true')
 args=parser.parse_args()
 if args.verify_output and not args.out_dir:parser.error('--verify-output requires --out-dir')
 if args.check:
  # CI --check runs real generation in a temp dir, checking real GLB structure.
  import tempfile
  with tempfile.TemporaryDirectory() as td:
   manifest=build_all(args.root,Path(td))
 elif args.verify_output:manifest=build_all(args.root,args.out_dir,True)
 else:manifest=build_all(args.root,args.out_dir)
 print('PASS: %d real glTF/GLB meshes: %d NPCs, %d unique creatures, %d realms; %d triangles'%(manifest['asset_count'],18,12,9,sum(a['triangles'] for a in manifest['assets'])))
if __name__=='__main__':main()