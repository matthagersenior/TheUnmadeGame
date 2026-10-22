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
  # Stable, UV-free stone/wood tonal modulation. Unlike the older single
  # RGBA value for every vertex, sloped surfaces now read under lighting.
  # No random state, external texture fetch, or loss of original palette.
  verts=np.asarray(mesh.vertices,dtype=float)
  grain=(np.sin(verts[:,0]*1.713+verts[:,2]*2.411+verts[:,1]*.71)
        +.42*np.cos(verts[:,0]*3.079-verts[:,2]*.963+verts[:,1]*1.409))
  tone=np.clip(.955+.067*grain,.83,1.07)
  rgb=np.clip(np.asarray(color,dtype=float)[None,:]*tone[:,None],0,255).astype(np.uint8)
  mesh.visual.vertex_colors=np.column_stack((rgb,np.full(len(verts),255,dtype=np.uint8)))
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
 def eye(x,y,z,r=.075):
  # Layered, forward-facing eye geometry reads as a face at model scale and
  # survives the GLB's vertex-color material path without texture dependence.
  k.orb('eye_socket',(x,y,z-r*.34),(r*1.38,r*1.28,r*.54),BLACK,2)
  k.orb('eye_iris',(x,y,z),(r,r,r*.62),trim,2)
  k.orb('eye_pupil',(x,y,z+r*.34),(r*.47,r*.54,r*.36),light,2)
  k.orb('eye_glint',(x-r*.20,y+r*.24,z+r*.56),(r*.18,r*.20,r*.13),WHITE,1)
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
   spine_row(-.57,5,.25,bodyY+.36)
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
# These route graphs are deliberately different physical layouts, not a shared
# town-square grid with new building props. They are authored preview layouts,
# NOT validated Unreal navigation, collision, or canon world coordinates.
REALM_PATHS={
 "ThreefoldReach":(((-12,-8),(-6,-4),(0,0),(8,8)),((-12,8),(-5,3),(0,0),(11,-8)),((-6,10),(-2,4),(0,0),(4,-3))),
 "WidowedRain":(((-12,8),(-9,8),(-9,-8),(5,-8),(5,6)),((-9,-3),(-3,-3),(1,2),(10,2)),((-1,-11),(-1,-4),(4,-4),(10,-9))),
 "HearthBeneath":(((-11,0),(-6,0),(-4,-4),(0,-6),(5,-3),(7,1)),((0,-12),(0,-6),(4,-1),(7,5),(12,7)),((-7,10),(-7,4),(-3,2),(3,2),(8,8))),
 "TidalLedger":(((-11,-10),(-9,-1),(-10,9)),((-3,-11),(-2,-1),(-3,10)),((4,-10),(4,-1),(7,10)),((-10,0),(-3,2),(4,1),(12,2))),
 "SkyBelow":(((-12,-9),(-5,-9),(2,-5),(8,1)),((-10,9),(-2,6),(5,8),(11,5)),((-5,-9),(-2,0),(5,8))),
 "CinderSpine":(((-12,-10),(-5,-10),(-5,-5),(4,-5),(4,1),(11,1)),((-10,9),(-10,4),(-2,4),(-2,9),(8,9)),((-5,-5),(-2,0),(-2,4))),
 "HundredUnlived":(((-12,-8),(-8,-8),(-8,-3),(-2,-3),(1,-8),(10,-8)),((-10,7),(-5,7),(-5,2),(2,2),(2,9)),((-2,-3),(4,-3),(7,2),(12,2))),
 "OrchardOfKings":(((0,0),(-5,-3),(-11,-9)),((0,0),(6,-2),(11,-7)),((0,0),(2,6),(7,10)),((0,0),(-4,6),(-9,11))),
 "FirstAbsence":(((-11,-8),(-4,-8),(-4,-2)),((4,2),(4,8),(11,8)),((-3,10),(-3,3),(3,-3),(10,-3)),((-10,2),(-6,2),(-6,-5)))
}
# Eleven distinct settlement plans. City identifiers here are asset IDs, not a
# replacement for canonical world-place IDs or quest/SaveGame references.
CITY_SPECS={
 "Crossings":{"realm":"ThreefoldReach","routes":(((-12,-6),(-4,-2),(0,0),(5,6)),((-11,8),(-4,4),(0,0),(11,-7)),((0,0),(9,2),(12,7)))},
 "Bellwold":{"realm":"ThreefoldReach","routes":(((-11,9),(-7,6),(-2,2),(1,-5)),((-8,-10),(-3,-7),(1,-5),(8,-7)),((1,-5),(5,0),(10,3)))},
 "Paperhaven":{"realm":"ThreefoldReach","routes":(((-12,-8),(-6,-6),(-3,-1),(0,8)),((-7,10),(0,8),(7,8),(11,3)),((-3,-1),(5,-3),(11,-9)))},
 "Saltwake":{"realm":"WidowedRain","routes":(((-11,9),(-7,5),(-7,-10)),((-7,5),(2,5),(5,0),(5,-9)),((5,0),(11,0),(11,9)))},
 "Cinderhold":{"realm":"HearthBeneath","routes":(((-11,-8),(-5,-5),(-2,0),(4,3)),((0,-11),(-2,0),(2,7),(10,9)),((-10,7),(-4,5),(2,7)))},
 "Drevlach":{"realm":"TidalLedger","routes":(((-12,-9),(-9,0),(-11,10)),((-2,-10),(0,0),(-1,10)),((8,-9),(7,1),(10,10)),((-10,0),(0,0),(7,1)))},
 "Orravane":{"realm":"SkyBelow","routes":(((-11,9),(-5,5),(2,8),(10,3)),((-11,-7),(-3,-9),(3,-3),(11,-8)),((-5,5),(-3,-2),(3,-3)))},
 "Vathless":{"realm":"CinderSpine","routes":(((-12,-10),(-6,-10),(-6,-4),(0,-4),(0,3)),((-11,9),(-3,9),(-3,4),(4,4),(11,9)),((0,3),(7,2),(7,-5),(11,-5)))},
 "Eillun":{"realm":"HundredUnlived","routes":(((-11,9),(-6,7),(-4,1),(2,4),(9,9)),((-10,-8),(-4,-5),(0,-9),(8,-7)),((-4,1),(2,-2),(8,-7)))},
 "Tharniv":{"realm":"OrchardOfKings","routes":(((-12,8),(-7,5),(-4,1),(0,-2)),((0,-2),(4,2),(8,9)),((0,-2),(5,-6),(11,-10)),((0,-2),(-6,-8)))},
 "Auvren":{"realm":"FirstAbsence","routes":(((-11,9),(-8,4),(-2,4)),((2,-3),(8,-3),(11,-8)),((-8,-10),(-3,-5),(2,-3)),((4,10),(5,4),(11,4)))}
}

def _road_mesh(k,realm,routes,prefix,base,trim):
 # A physically oriented strip follows each segment rather than reusing a
 # single center-line grid in every realm.
 y=3.2 if realm=="SkyBelow" else .105
 width={"ThreefoldReach":1.45,"WidowedRain":1.65,"HearthBeneath":1.35,
        "TidalLedger":1.05,"SkyBelow":1.05,"CinderSpine":1.2,
        "HundredUnlived":1.32,"OrchardOfKings":1.08,"FirstAbsence":.95}[realm]
 for road_id,points in enumerate(routes):
  for part,((x1,z1),(x2,z2)) in enumerate(zip(points,points[1:])):
   dx,dz=x2-x1,z2-z1;length=math.hypot(dx,dz)
   if length<=.01:raise ValueError("Zero street segment")
   strip=trimesh.creation.box(extents=(width,.11,length))
   strip.apply_transform(trimesh.transformations.rotation_matrix(math.atan2(dx,dz),[0,1,0]))
   strip.apply_translation(((x1+x2)*.5,y,(z1+z2)*.5))
   k.add(strip,f'{prefix}_route_{realm}_{road_id}_{part}',trim if road_id%2 else base)
   for xx,zz in ((x1,z1),(x2,z2)):
    k.box(f'{prefix}_junction_{realm}',(xx,y+.018,zz),(.29,.055,.34),WHITE if road_id==0 else trim)
  # A path that forks visibly has more than one junction, not a corridor texture.
  for vertex,(x,z) in enumerate(points[1:-1]):
   k.box(f'{prefix}_route_marker_{realm}',(x,y+.25,z),(.24,.46,.24),GOLD if vertex%2 else trim)

def _terrain(k,realm,base,trim,city=None):
 # Region-specific silhouette including physically raised quarry/suspended
 # zones; no one-size 28 x 25 foundation beneath identical corners.
 terrain={
  "ThreefoldReach":[(-7,-5,13,13,-.24),(5,5,17,16,-.28),(-2,-9,11,7,-.37)],
  "WidowedRain":[(-9,2,9,23,-.40),(4,-7,17,8,-.4),(8,7,9,12,-.4)],
  "HearthBeneath":[(-7,-3,11,20,-.45),(5,4,16,14,-.53),(5,-9,12,6,-.5)],
  "TidalLedger":[(-10,-1,6,25,-.36),(-1,2,8,25,-.4),(9,-2,7,26,-.36)],
  "SkyBelow":[(-9,-7,8,9,2.0),(-2,2,10,9,2.6),(8,7,9,8,3.1),(7,-9,10,6,2.3)],
  "CinderSpine":[(-9,0,9,26,-.3),(-1,-5,9,13,1.3),(8,4,9,22,2.2)],
  "HundredUnlived":[(-7,-7,14,12,-.3),(4,-6,17,12,-.3),(-4,6,17,13,-.4),(8,7,11,12,-.2)],
  "OrchardOfKings":[(0,0,19,18,-.4),(-9,-6,7,12,-.3),(9,8,8,11,-.3)],
  "FirstAbsence":[(-8,-5,8,14,.45),(6,5,11,14,1.2),(-6,8,7,8,1.7),(8,-8,8,8,.8)]
 }[realm]
 for i,(x,z,w,d,elevation) in enumerate(terrain):
  k.box(f'landform_{realm}_terrace_{i}',(x,elevation,z),(w,.6,d),base if i%2==0 else trim)
 # No invisible navigation claim: these are diorama landmasses only.

def _shelter(k,x,z,base,trim,light,kind="refuge"):
 k.box(kind+'_walls',(x,1.45,z),(3.9,2.9,3.3),base)
 k.box(kind+'_dark_entry',(x,.9,z+1.68),(.9,1.8,.08),DARK)
 for offset in (-1.3,1.3):
  k.box(kind+'_windows',(x+offset,1.85,z+1.70),(.43,.48,.09),light)
 k.box(kind+'_roof_left',(x-.92,3.05,z),(2.18,.32,3.55),trim)
 k.box(kind+'_roof_right',(x+.92,3.15,z),(2.12,.32,3.55),trim)
 k.box(kind+'_soot_band',(x,1.15,z+1.75),(2.7,.15,.08),DARK)

def _tall_frame(k,x,z,h,color,tag):
 for d in (-1.05,1.05):k.box(tag+'_upright',(x+d,h*.5,z),(.24,h,.34),color)
 k.box(tag+'_lintel',(x,h,z),(2.38,.30,.42),color)

def _regional_signature(k,realm,base,trim,light):
 # Nine materially different macro geometries, traversals and verticalities.
 if realm=="ThreefoldReach":
  _shelter(k,-8,-5,base,trim,light,"refuge_split_roof")
  _shelter(k,7,6,base,trim,light,"carter_kitchen")
  for x,z in ((-3,2),(3,1)):
   _tall_frame(k,x,z,5.7,trim,"bridge_ledger_gate")
   k.beam("surveyed_rope_span",(x-1,3.5,z),(x+1,3.5,z),.10,light)
  k.box("conflicting_mile_stone",(0,.5,0),(.85,1.1,.72),WHITE)
  for i,z in enumerate((-9,-4,5,10)):
   k.box("refuge_lantern_post",(9,1.15,z),(.18,2.2,.18),DARK)
   k.orb("held_lantern_wick",(9,2.4,z),(.18,.28,.18),GOLD,1)
 elif realm=="WidowedRain":
  for x,z in ((-9,-6),(-8,5),(5,-7),(8,5)):
   k.beam("dry_dock_rib",(x-.8,.2,z),(x,2.9,z+1),.15,trim)
   k.beam("dry_dock_rib",(x+.8,.2,z),(x,2.9,z+1),.15,trim)
   k.box("upturned_hull_planking",(x,3.05,z),(3.4,.25,1.7),base)
  for i,z in enumerate(range(-9,10,3)):
   k.beam("empty_harbor_pier",(-1.5,.3,z),(-1.5,.3,z+2.8),.18,trim)
   k.box("rain_catcher_basin",(4,.8,z),(2.5,.3,1.1),light)
  _tall_frame(k,9,-2,6.1,GOLD,"tide_gauge_gate")
 elif realm=="HearthBeneath":
  for r in (2.4,4.3,6.2):
   k.ring("sunken_communal_kiln_court",(0,.05,0),r,.22,trim,plane="xz")
  for x,z in ((-8,7),(7,-7)):
   k.box("basalt_house_ribs",(x,1.65,z),(3.8,3.2,3.1),base)
   k.taper("braided_kiln_chimney",(x,2.7,z),(x+.3,7.3,z),.62,.32,trim)
   k.orb("borrowed_hearth_coals",(x+.3,7.55,z),(.51,.19,.46),light,2)
  for x in (-7,-4,4,7):
   k.box("ash_filter_sieve",(x,.6,5),(1.35,1,.25),light)
  for z in (-8,-5,-2,2,6):
   k.box("heated_basal_channel",(2,.06,z),(1.2,.18,1.7),GOLD)
 elif realm=="TidalLedger":
  for x in (-6.5,2.5):
   for z in (-9,-4,1,6):
    k.box("inkstone_lock_bank",(x,.36,z),(.6,.82,2.8),base)
  for x,z in ((-3,-6),(4,4),(-2,9)):
   _tall_frame(k,x,z,3.9,trim,"portable_counting_bridge")
   k.box("counterweight_ledger",(x,2.4,z+1.1),(1.45,2.1,.20),light)
  for i in range(8):
   k.box("forward_backward_tide_date",(-10+i*2.4,1.15,-10),(.24,.4,.14),GOLD)
 elif realm=="SkyBelow":
  for x,z,h in ((-9,-7,6.0),(-2,3,8.8),(8,7,10.4),(7,-8,7.5)):
   k.box("inverted_terrace",(x,h,z),(5,.35,4.2),trim)
   k.box("inhabited_hanging_floor",(x,h-.95,z),(3.4,1.6,2.8),base)
   for side in (-1,1):
    k.beam("choir_suspension_cable",(x+side*2,h,z),(x+side*.6,h+7.2,z),.09,light)
   k.ring("audible_counterweight",(x,h-2.1,z),.62,.10,GOLD,plane="xz")
  for a,b in (((-9,6,-7),(-2,8.8,3)),((-2,8.8,3),(8,10.4,7)),((8,10.4,7),(7,7.5,-8))):
   k.beam("singing_span_between_homes",a,b,.16,WHITE)
 elif realm=="CinderSpine":
  for i,(x,z,y) in enumerate(((-9,-9,3),(-9,0,5),(-9,8,4),(1,-6,4),(6,2,6),(10,10,8))):
   k.box("bearing_quarry_shelf",(x,y*.4,z),(5.3,y,4.0),trim if i%2 else base)
   k.box("family_wall_with_inlaid_names",(x,y*.7,z+2),(4.5,.7,.12),WHITE)
   k.taper("bone_seam_column",(x-1,y,z),(x-.45,y+3,z+.4),.30,.05,light)
  for z in (-10,-7,-4,-1,2,5,8):
   k.box("quarry_switchback_step",(2,.27,z),(2.7,.53,.8),trim)
 elif realm=="HundredUnlived":
  for i,(x,z) in enumerate(((-9,-7),(-3,4),(8,-4),(7,9))):
   _tall_frame(k,x,z,3.3+i*.4,trim,"consenting_overlapping_doorstep")
   k.box("unregistered_house_wall",(x+1.1,1.2,z-1),(2.6,2.2,2.2),base)
   k.box("household_reversible_sign",(x-.8,2,z+.3),(1.2,.24,.2),light)
  for x,z in ((0,-8),(4,2),(-5,8)):
   k.box("public_uncounted_sleeping_mat",(x,.25,z),(1.6,.19,.90),WHITE)
  k.box("unsigned_civic_refuge_wall",(-1,2.2,0),(3.5,4.2,.25),trim)
 elif realm=="OrchardOfKings":
  for x,z,r in ((-8,-8,.8),(-8,8,1.1),(8,-8,1.2),(7,8,.93),(0,0,1.5)):
   k.taper("grafted_root_trunk",(x,0,z),(x+.15,4+r*2,z),r*.54,r*.31,base)
   for j in range(6):
    a=2*math.pi*j/6;tip=(x+math.cos(a)*2.7,6+r,z+math.sin(a)*2.5)
    k.taper("petition_bark_branch",(x,3.5+r,z),tip,.27,.045,trim,9)
    k.orb("uncrowned_petition_fruit",tip,(.24,.34,.22),light,1)
  k.ring("empty_civic_crown_seat",(0,.68,0),2.1,.18,GOLD,plane="xz")
  for i in range(8):
   a=2*math.pi*i/8
   k.box("root_vote_pedestal",(5*math.cos(a),.30,5*math.sin(a)),(.6,.6,.6),WHITE)
 elif realm=="FirstAbsence":
  for x,z,h in ((-9,-8,6.1),(-2,7,4.8),(9,5,8.4),(6,-8,7.7)):
   _tall_frame(k,x,z,h,trim,"two_contradictory_foundation_thresholds")
   k.box("unoccupied_threshold_stone",(x,.85,z+1.2),(3.1,.25,.46),WHITE)
  for i,(x,z) in enumerate(((-6,0),(-4,-5),(3,1),(9,-1))):
   k.box("witness_plateau_marker",(x,1.4+i*.45,z),(2.3,.23,1.9),base)
   k.box("limestone_room_outside_itself",(x,2.6+i*.45,z),(1.9,1.3,.20),light)

def _city_signature(k,city,base,trim,light):
 # A settlement has its own civic centerpiece. It is not a recolored regional
 # overview or a reusable clone of another city's square.
 if city=="Crossings":
  for x,z in ((-7,3),(3,-4),(7,6)):
   _tall_frame(k,x,z,4.0,trim,"identity_crossings_bridge_ledger")
  for i,(x,z) in enumerate(((-4,-6),(3,2),(9,-2))):
   k.box("identity_crossings_three_different_knots",(x,.6,z),(1+i*.27,.26,.45),GOLD)
 elif city=="Bellwold":
  _shelter(k,-4,-3,base,trim,light,"identity_bellwold_refuge_kettle_kitchen")
  k.box("identity_bellwold_shared_kettle",(-3.1,1.8,-1.25),(.55,.45,.45),GOLD)
  for x in (-4.2,-3.3,-2.4):
   k.box("identity_bellwold_one_uncounted_cup",(x,.9,-1.12),(.2,.24,.22),WHITE)
  # The bell never existed: an empty cross-yoke, absolutely no bell geometry.
  for x in (3.1,5.9):
   k.taper("identity_bellwold_green_copper_tower",(x,0,-5),(x,8.6,-5),.62,.46,trim)
  k.beam("identity_bellwold_uncast_yoke",(3.1,8.2,-5),(5.9,8.2,-5),.16,GOLD)
  k.box("identity_bellwold_absent_casting_seam",(4.5,7.2,-5),(.6,.12,.13),DARK)
  for z in (-9,-6,-2,2,6):
   k.box("identity_bellwold_refuge_lantern_staff",(8,1.3,z),(.18,2.4,.18),DARK)
   k.orb("identity_bellwold_persistent_lantern",(8,2.6,z),(.2,.31,.19),light,1)
 elif city=="Paperhaven":
  for x,z,h in ((-7,-5,4.8),(-4,6,6.2),(7,5,8.2)):
   k.box("identity_paperhaven_chalk_archive_vault",(x,h*.5,z),(4.2,h,3.2),WHITE)
   k.box("identity_paperhaven_narrow_roof_archive",(x,h+.5,z),(4.4,.85,3.5),trim)
  k.box("identity_paperhaven_cross_index_desk",(3,.9,-5),(3.6,1.3,2.1),base)
  for i in range(8):
   k.box("identity_paperhaven_inkless_witness_slip",(1.8+i*.24,1.61,-4.7),(.17,.06,.29),light)
 elif city=="Saltwake":
  for x,z in ((-8,-4),(-2,5),(7,1)):
   for dx in (-1,0,1):
    k.beam("identity_saltwake_upturned_hull_ribs",(x+dx*.65,.4,z-1),(x,3.1,z+.4),.13,trim)
   k.box("identity_saltwake_rain_slate_catch",(x,3.2,z),(3.3,.18,2),light)
  k.box("identity_saltwake_public_storm_sluice",(9,.18,-7),(3.7,.34,4.9),GOLD)
 elif city=="Cinderhold":
  for i,(x,z) in enumerate(((-8,4),(1,-6),(9,3))):
   k.ring("identity_cinderhold_household_hearth",(x,.3,z),1.55+i*.16,.18,GOLD,plane="xz")
   k.taper("identity_cinderhold_heat_vent",(x,0,z),(x,4.8+i*.6,z),.54,.21,trim)
  for z in range(-7,8,2):
   k.box("identity_cinderhold_shared_heat_allotment_channel",(4,.2,z),(1.5,.18,1.5),light)
 elif city=="Drevlach":
  for x,z in ((-8,6),(4,-7),(8,7)):
   k.box("identity_drevlach_inkstone_lockhouse",(x,1.8,z),(3.9,3.6,3.5),base)
   k.box("identity_drevlach_double_read_contract_board",(x,2.2,z+1.9),(2.0,1.1,.21),GOLD)
  _tall_frame(k,0,0,8.4,trim,"identity_drevlach_future_tide_clock")
  k.ring("identity_drevlach_time_debt_wheel",(0,6.5,0),1.9,.13,light,plane="xy")
 elif city=="Orravane":
  for x,z,h in ((-8,2,6.1),(2,5,9.0),(8,-7,7.7)):
   k.box("identity_orravane_inverted_terrace",(x,h,z),(4.5,.4,3.8),trim)
   k.beam("identity_orravane_restable_singer_cable",(x-1.7,h,z),(x-1,15,z),.13,light)
   k.beam("identity_orravane_restable_singer_cable",(x+1.7,h,z),(x+1,15,z),.13,light)
  k.ring("identity_orravane_missing_choir_tone",(0,5,0),3.0,.16,GOLD,plane="xy")
 elif city=="Vathless":
  for i,(x,z) in enumerate(((-8,-5),(-4,7),(5,-6),(9,7))):
   k.box("identity_vathless_family_bearing_stone",(x,2.1+i*.25,z),(3.6,4.1+i*.5,3.1),base)
   for j in range(4):k.box("identity_vathless_inlaid_ancestral_name",(x-1+j*.63,1.65,z+1.61),(.42,.13,.10),WHITE)
  k.box("identity_vathless_quarry_witness_shelter",(0,1.4,0),(3.5,2.8,2.4),trim)
 elif city=="Eillun":
  for x,z,rot in ((-9,-6,1),(-3,5,-1),(7,-3,1),(10,7,-1)):
   _tall_frame(k,x,z,4.0+rot*.3,light,"identity_eillun_overlapping_consensual_threshold")
   k.box("identity_eillun_unsigned_household_lamp",(x+rot,2.3,z+.4),(.37,.45,.34),GOLD)
  k.box("identity_eillun_unrecorded_refuge_hall",(0,1.9,-3),(6.1,3.8,4),trim)
 elif city=="Tharniv":
  for i,(x,z) in enumerate(((-8,8),(8,-7),(-8,-8),(7,8))):
   k.taper("identity_tharniv_petition_bark_trunk",(x,0,z),(x-.2,7,z),.7,.4,base)
   for j in range(4):
    a=2*math.pi*j/4
    k.taper("identity_tharniv_grafted_civic_branch",(x,5,z),(x+2.1*math.cos(a),8.6,z+2*math.sin(a)),.2,.04,trim)
  k.box("identity_tharniv_uncrowned_plinth",(0,.8,0),(3.7,1.6,3.2),WHITE)
  k.ring("identity_tharniv_law_without_author",(0,1.8,0),1.35,.10,GOLD,plane="xz")
 elif city=="Auvren":
  for x,z,h in ((-7,5,7.3),(7,-4,6.1)):
   _tall_frame(k,x,z,h,WHITE,"identity_auvren_two_founding_thresholds")
  k.box("identity_auvren_northern_witness_plateau",(0,1.8,9),(7.1,1.7,4.6),trim)
  for x,z in ((-4,-5),(4,5),(0,-9)):
   k.box("identity_auvren_public_testimony_stone",(x,1.2,z),(2.5,2.4,.5),light)

# Districts are independently located within each town rather than repeating
# a square of four buildings around the same axial road. Tuples are
# (district x, district z, distinct habitable buildings, local frontage angle).
# These are deterministic concept-art placements, not canonical map coordinates.
CITY_DISTRICTS={
 "Crossings":((-14,-9,5,.18),(-4,11,5,1.45),(11,-12,5,.9),(15,8,5,2.25)),
 "Bellwold":((-11,-9,5,.28),(-10,8,5,2.00),(6,10,5,.90),(13,-8,5,2.65)),
 "Paperhaven":((-12,-10,5,.02),(-6,11,5,.56),(5,-7,5,1.24),(13,8,5,2.33)),
 "Saltwake":((-13,9,5,1.42),(-11,-8,5,1.58),(2,-12,5,.22),(12,6,5,2.02)),
 "Cinderhold":((-11,0,5,.2),(-5,-11,5,.97),(9,-8,5,1.8),(10,10,5,2.48)),
 "Drevlach":((-12,-8,5,1.58),(-12,9,5,1.4),(1,-11,5,1.25),(12,8,5,1.87)),
 "Orravane":((-12,5,5,2.3),(-4,-11,5,.85),(8,8,5,1.02),(13,-7,5,2.67)),
 "Vathless":((-13,-10,5,.30),(-9,5,5,.99),(4,-7,5,1.68),(12,10,5,2.48)),
 "Eillun":((-12,8,5,2.8),(-8,-9,5,1.66),(7,3,5,2.0),(13,-11,5,.12)),
 "Tharniv":((-11,10,5,1.9),(-12,-8,5,.25),(8,-10,5,2.2),(12,9,5,3.02)),
 "Auvren":((-11,-8,5,.52),(-12,11,5,1.96),(7,7,5,2.45),(12,-9,5,.78))
}
REGION_HABITAT={
 "ThreefoldReach":"Crossings","WidowedRain":"Saltwake","HearthBeneath":"Cinderhold",
 "TidalLedger":"Drevlach","SkyBelow":"Orravane","CinderSpine":"Vathless",
 "HundredUnlived":"Eillun","OrchardOfKings":"Tharniv","FirstAbsence":"Auvren"
}

def _dwelling(k,city,idx,x,z,base,trim,light):
 # Materials, floors, and structure change according to the settlement's
 # trade and social use. Every house has a real independent body and roof.
 # No house position is a claim about playable navigation.
 offset=(idx*7)%5
 w=2.25+.24*(idx%3);h=2.3+.36*(idx%4);d=2.20+.28*(idx%2)
 y=0.0
 if city=="Orravane":y=4.6+1.55*(idx%3)
 elif city=="Vathless":y=.55*(idx%4)
 elif city=="Auvren":y=.50*(idx%3)
 elif city=="Cinderhold":y=-.28
 tag="fabric_"+city.lower()
 k.box(tag+"_ground_pad_"+str(idx),(x,y-.31,z),(w+1.1,.25,d+1.0),base)
 if city=="Tharniv":
  k.taper(tag+"_living_root_"+str(idx),(x,y-.15,z),(x+.16,y+h+.7,z),.62,.3,base)
  for j in range(3):
   angle=2*math.pi*j/3+.1*idx
   end=(x+2*math.cos(angle),y+h+2,z+2*math.sin(angle))
   k.taper(tag+"_canopy_bough_"+str(idx),(x,y+h-.1,z),end,.19,.04,trim,8)
  y+=.35
 if city=="Saltwake":
  k.box(tag+"_dry_hull_keel_"+str(idx),(x,y+.22,z),(w+.6,.32,d+.7),trim)
  y+=.45
 if city=="Vathless":
  k.box(tag+"_step_bearing_stone_"+str(idx),(x,y-.12,z),(w+.45,.32,d+.32),WHITE)
 if city=="Orravane":
  for side in (-1,1):
   k.beam(tag+"_suspension_tether_"+str(idx),
     (x+side*.85,y+h,z),(x+side*1.20,y+h+4.5,z),.068,light)
 if city=="Auvren":
  k.box(tag+"_unmatched_threshold_"+str(idx),(x+.6,y+.72,z+d*.53),(.17,1.5,.24),WHITE)
 k.box(tag+"_habitation_shell_"+str(idx),(x,y+h*.50,z),(w,h,d),base if idx%2 else trim)
 door_side=(-1 if idx%2 else 1)
 k.box(tag+"_threshold_"+str(idx),(x+door_side*.40,y+.84,z+d*.51),(.6,1.52,.09),DARK)
 for side in (-1,1):
  k.box(tag+"_lit_window_"+str(idx),(x+side*w*.33,y+max(h*.75,1.9),z+d*.515),(.31,.39,.11),light)
 roofline=y+h+.27
 if city in ("Crossings","Bellwold","Eillun"):
  # Split gables make timber/refuge/overlapping houses recognizable in silhouette.
  k.box(tag+"_roof_detail_gable_left_"+str(idx),(x-w*.25,roofline,z),(w*.57,.34,d+.48),trim)
  k.box(tag+"_roof_detail_gable_right_"+str(idx),(x+w*.25,roofline+.18,z),(w*.60,.25,d+.47),base)
  if city=="Bellwold":
   k.box(tag+"_refuge_chimney_"+str(idx),(x-w*.26,roofline+.65,z-.36),(.36,1.0,.45),WHITE)
   k.box(tag+"_shelter_lantern_"+str(idx),(x+w*.28,y+1.7,z+d*.58),(.23,.38,.18),GOLD)
  if city=="Eillun":k.box(tag+"_consent_notice_not_name_"+str(idx),(x,y+1.8,z+d*.56),(.7,.2,.12),WHITE)
  if city=="Crossings":k.beam(tag+"_route_knot_beam_"+str(idx),(x-1,roofline+.35,z),(x+1,roofline+.25,z),.11,GOLD)
 elif city=="Paperhaven":
  k.box(tag+"_roof_detail_archive_cap_"+str(idx),(x,roofline+.38,z),(w+.24,.62,d+.26),WHITE)
  k.box(tag+"_roof_detail_ink_flue_"+str(idx),(x+.65,roofline+.95,z-.2),(.30,.72,.43),DARK)
  for j in range(3):
   k.box(tag+"_public_ledger_slit_"+str(idx),(x-.65+j*.65,y+1.22,z+d*.56),(.35,.11,.07),GOLD)
 elif city=="Saltwake":
  for side in (-1,1):
   k.taper(tag+"_roof_detail_inverted_hull_rib_"+str(idx),
     (x+side*(w*.57),roofline-.06,z),(x,roofline+1.15,z),.17,.045,trim)
  k.box(tag+"_roof_detail_rain_catch_"+str(idx),(x,roofline+1.09,z),(w*.56,.16,d+.62),light)
 elif city=="Cinderhold":
  k.box(tag+"_roof_detail_basalt_vent_"+str(idx),(x,roofline+.22,z),(w+.3,.47,d+.2),DARK)
  k.taper(tag+"_heat_chimney_"+str(idx),(x+.45,roofline,z),(x+.3,roofline+2.0,z),.32,.2,trim)
  k.orb(tag+"_public_hearth_glow_"+str(idx),(x+.3,roofline+2.03,z),(.22,.18,.2),GOLD,1)
 elif city=="Drevlach":
  k.box(tag+"_roof_detail_lockhouse_slate_"+str(idx),(x,roofline+.18,z),(w+.3,.34,d+.34),trim)
  k.box(tag+"_counting_ledger_table_"+str(idx),(x-.5,y+1.64,z+d*.56),(.55,.75,.13),WHITE)
  k.beam(tag+"_tide_debt_pole_"+str(idx),(x+.7,y+.1,z+1),(x+.7,roofline+1.1,z+1),.05,GOLD)
 elif city=="Orravane":
  k.box(tag+"_roof_detail_hanging_terrace_"+str(idx),(x,roofline,z),(w+.9,.23,d+.65),trim)
  k.box(tag+"_acoustic_counterweight_"+str(idx),(x,y-.7,z),(.34,.8,.37),GOLD)
  k.beam(tag+"_bridge_anchor_"+str(idx),(x-.9,y,z-.5),(x-1.7,y-.25,z-2),.08,WHITE)
 elif city=="Vathless":
  k.box(tag+"_roof_detail_quarry_shelf_"+str(idx),(x,roofline+.14,z),(w+.48,.4,d+.3),trim)
  for n in range(3):
   k.box(tag+"_consented_family_inlay_"+str(idx),(x-.7+n*.62,y+1.35,z+d*.56),(.42,.12,.10),WHITE)
 elif city=="Tharniv":
  k.box(tag+"_roof_detail_petition_canopy_"+str(idx),(x,roofline+.15,z),(w+1.2,.26,d+1.0),light)
  for n in (-1,1):
   k.beam(tag+"_root_veranda_"+str(idx),(x+n*.9,y+.1,z+d*.52),(x+n*.8,roofline,z+d*.52),.12,base)
 elif city=="Auvren":
  k.box(tag+"_roof_detail_open_limestone_lintel_"+str(idx),(x,roofline,z),(w+.5,.18,d+.4),WHITE)
  k.box(tag+"_witness_bench_"+str(idx),(x-.6,y+.36,z+d*.7),(1.3,.2,.44),trim)

 _architecture_finish(k,city,idx,x,y,z,w,d,h,base,trim,light)
 _district_surface().finish_dwelling(k,city,idx,x,y,z,w,h,d)
 return y


def _sculpted_pitch(k,name,x,y,z,w,d,rise,material):
 # Six-vertex gabled prism: genuinely sloped roof planes, with closed gables.
 # Replaces one-size-fits-all flat roof silhouettes without requiring Blender.
 a=w*.55;b=d*.56
 vertices=[
  (x-a,y,z-b),(x,y+rise,z-b),(x+a,y,z-b),
  (x-a,y,z+b),(x,y+rise,z+b),(x+a,y,z+b)]
 faces=[[0,2,1],[3,4,5],[0,1,4],[0,4,3],
        [1,2,5],[1,5,4],[0,3,5],[0,5,2]]
 mesh=trimesh.Trimesh(vertices=vertices,faces=faces,process=False)
 k.add(mesh,name,material)

def _architecture_finish(k,city,idx,x,y,z,w,d,h,base,trim,light):
 """Specific shaped exterior structure. No generic single-town house grammar.

 Details are visual-only geometry, not interactive game props, approved
 canonical interior layouts or pathfinding-ready structures.
 """
 identity={
  "Crossings":"triple_way_charter","Bellwold":"refuge_patched_stone",
  "Paperhaven":"archive_buttress","Saltwake":"ship_hull_curvature",
  "Cinderhold":"heat_channel_exhaust","Drevlach":"counterweight_lockhouse",
  "Orravane":"suspended_walkway","Vathless":"quarry_switchback",
  "Eillun":"consent_threshold_screen","Tharniv":"grafted_root_vault",
  "Auvren":"limestone_witness_frame"
 }[city]
 tag="architecture_"+identity+"_"+str(idx)+"_"
 # Tilted roof is a separately triangulated object. The rise and eave rhythm
 # varies by settlement and neighborhood, not by a common material swap.
 roof_base=y+h+.13
 rise=.72+.19*(idx%4)
 if city in ("Saltwake","Tharniv","Orravane"):rise=1.05+.11*(idx%5)
 if city in ("Paperhaven","Drevlach","Vathless"):rise=.48+.17*(idx%3)
 _sculpted_pitch(k,tag+"nonrectangular_roof_gable",x,roof_base,z,w,d,rise,trim)
 k.beam(tag+"ridge_support",(x,roof_base+rise,z-d*.56),
        (x,roof_base+rise,z+d*.56),.09,light)
 for side in (-1,1):
  k.beam(tag+"sloping_rafter",(x,roof_base+rise,z),
         (x+side*w*.55,roof_base,z),.075,base)
  k.box(tag+"wall_trim_plaster",(x+side*(w*.5+.04),y+h*.7,z),
        (.09,.22,d*.8),WHITE if idx%3==0 else trim)
 # Architectural completion uses eight or more city-specific medium-scale
 # feature types; the city should remain recognizable with palette removed.
 if city=="Crossings":
  for side in (-1,1):
   k.beam(tag+"forked_timber_rib",(x+side*w*.42,y+.25,z+.70),
          (x+side*w*.61,roof_base+.27,z-.30),.11,GOLD)
   k.box(tag+"route_notice_lintel",(x+side*.64,y+1.85,z+d*.62),
         (.83,.23,.18),WHITE)
  k.box(tag+"charter_porch",(x,y+.15,z+d*.77),(w*.84,.3,1.03),base)
  k.beam(tag+"bridging_survey_rail",(x-w*.4,y+1.4,z+d*.9),
         (x+w*.42,y+1.45,z+d*.9),.09,trim)
  k.box(tag+"three_way_chalk_mark",(x,y+.33,z+d*.86),(.65,.08,.42),WHITE)
  k.box(tag+"refuge_side_awning",(x-w*.47,y+2.0,z),(1.05,.15,1.55),trim)
 elif city=="Bellwold":
  k.box(tag+"refuge_lean_to",(x+w*.63,y+.93,z+.2),(1.25,1.72,d*.68),base)
  k.beam(tag+"patched_roof_support",(x+w*.58,y+1.95,z-d*.28),
         (x+w*1.0,y+1.62,z+d*.25),.11,trim)
  k.box(tag+"copper_lantern_hood",(x-w*.31,y+1.94,z+d*.62),(.62,.18,.52),GOLD)
  k.box(tag+"covered_guest_veranda",(x,y+.29,z+d*.85),(w*.84,.22,1.25),WHITE)
  for side in (-1,1):
   k.beam(tag+"weathered_refuge_knee_brace",(x+side*w*.38,y+.4,z+d*.76),
          (x+side*w*.27,y+2.18,z+d*.61),.13,trim)
  k.box(tag+"smoke_baffle",(x-w*.3,roof_base+.78,z-.5),(.44,.39,.48),DARK)
  k.box(tag+"community_wash_rack",(x+w*.52,y+.57,z-.6),(.15,1.08,1.56),WHITE)
 elif city=="Paperhaven":
  for side in (-1,1):
   k.taper(tag+"archive_buttress_pier",
       (x+side*(w*.65),y,z),(x+side*w*.52,roof_base+.7,z),.37,.14,WHITE)
   k.box(tag+"index_slit_frame",(x+side*w*.22,y+1.68,z+d*.6),
         (.23,.98,.13),trim)
  k.box(tag+"stacked_chalk_folio",(x,y+2.2,z+d*.67),(w*.64,.18,.24),WHITE)
  k.box(tag+"narrow_archive_terrace",(x,roof_base+.19,z-d*.55),
        (w+.48,.24,.72),trim)
  k.box(tag+"vault_corner_cap",(x-w*.39,roof_base+.75,z+d*.3),(.28,.65,.28),GOLD)
  k.box(tag+"public_index_shelf",(x,y+.70,z+d*.73),(w*.62,.15,.35),base)
 elif city=="Saltwake":
  # Hull form: keel plus multiple curved-looking spaced ribs across pitched roof.
  for side in (-1,0,1):
   zz=z+side*d*.42
   k.beam(tag+"upturned_hull_port_rib",(x-w*.56,roof_base,zz),
          (x,roof_base+rise*1.38,zz),.125,trim)
   k.beam(tag+"upturned_hull_starboard_rib",(x,roof_base+rise*1.38,zz),
          (x+w*.56,roof_base,zz),.125,light)
  k.box(tag+"inverted_keel",(x,roof_base+rise*1.22,z),
        (.23,.14,d+.67),DARK)
  k.box(tag+"tide_catch_gutter",(x+w*.54,roof_base-.28,z),
        (.27,.24,d+.41),GOLD)
  k.beam(tag+"rope_brace",(x+w*.50,y+1.93,z-.4),
         (x+w*.96,y+.21,z+.35),.07,WHITE)
  k.box(tag+"harbor_drying_frame",(x-w*.55,y+1.12,z+d*.68),
        (1.4,1.35,.12),trim)
 elif city=="Cinderhold":
  k.box(tag+"heat_channel_apron",(x,y+.17,z+d*.75),(w*.85,.22,1.27),DARK)
  for side in (-1,1):
   k.box(tag+"basalt_corner_rib",(x+side*w*.50,y+1.5,z),
         (.35,2.9,.36),base)
   k.beam(tag+"exhaust_tension_brace",(x+side*.75,roof_base+.26,z),
          (x+side*1.16,y+1.1,z+.25),.11,trim)
  k.taper(tag+"sintered_kiln_chimney",(x+w*.22,roof_base,z),
          (x+w*.29,roof_base+2.6,z),.48,.31,DARK)
  k.ring(tag+"chimney_collar",(x+w*.29,roof_base+2.3,z),.40,.07,GOLD,plane="xz")
  k.box(tag+"heat_warning_sill",(x,y+.68,z+d*.59),(w*.6,.17,.3),light)
 elif city=="Drevlach":
  k.box(tag+"canal_lock_house_platform",(x,y+.26,z+d*.62),
        (w+.7,.35,1.60),trim)
  for side in (-1,1):
   k.beam(tag+"counterweight_suspended_cord",
          (x+side*.76,roof_base+1.15,z),
          (x+side*.72,y+.45,z),.07,GOLD)
   k.box(tag+"ledger_weight",(x+side*.72,y+.56,z),(.32,.77,.34),DARK)
  k.ring(tag+"debt_balance_wheel",(x,roof_base+.87,z),.59,.09,WHITE,plane="xy")
  k.box(tag+"date_inscribed_lock_sluice",(x,y+.46,z+d*.78),
        (w*.5,.25,.3),GOLD)
  k.box(tag+"public_double_read_panel",(x+w*.52,y+1.55,z),
        (.15,1.3,1.11),WHITE)
 elif city=="Orravane":
  # Distinct suspended habitation with overhead support geometry and balcony.
  k.box(tag+"hanging_public_walkway",(x,y-.35,z+d*.75),
        (w+1.10,.22,1.5),WHITE)
  for side in (-1,1):
   k.beam(tag+"diagonal_tension_cable",
          (x+side*w*.54,roof_base,z),
          (x+side*w*.84,roof_base+5.0,z-.82),.085,light)
   k.beam(tag+"walkway_guardrail",(x+side*w*.50,y+.78,z+d*.7),
          (x+side*w*.50,y+.78,z+d*1.2),.09,trim)
  k.box(tag+"acoustic_balcony",(x,y+1.02,z+d*.72),
        (w*.92,.21,.84),base)
  k.ring(tag+"counterweight_tuner",(x,y-.85,z),.42,.08,GOLD,plane="xz")
  k.box(tag+"choir_rest_platform",(x-w*.55,y+.1,z),
        (.65,.30,1.05),WHITE)
 elif city=="Vathless":
  for step in range(3):
   k.box(tag+"quarry_cut_switchback_step",
         (x+w*.7+step*.34,y+.19+step*.36,z+step*.31),
         (1.05,.35,1.07),trim if step%2 else base)
  for side in (-1,1):
   k.taper(tag+"bearing_stone_buttress",
         (x+side*w*.52,y+.15,z),
         (x+side*w*.55,roof_base-.1,z),.33,.18,WHITE)
  k.box(tag+"ancestral_ledger_frieze",(x,y+2.1,z+d*.59),
        (w*.76,.30,.16),GOLD)
  k.beam(tag+"safe_quarry_rail",(x-w*.55,y+1.05,z+d*.83),
         (x+w*.55,y+1.05,z+d*.83),.10,trim)
  k.box(tag+"shored_family_wall",(x-w*.6,y+1.0,z-d*.20),
        (.28,1.95,d*.47),base)
 elif city=="Eillun":
  # The overlapping thresholds are visibly reversible, not census scanners.
  for side in (-1,1):
   k.beam(tag+"privacy_screen_post",(x+side*.92,y,z+d*.92),
          (x+side*.92,y+2.4,z+d*.92),.09,trim)
   k.box(tag+"consent_folded_screen",(x+side*.59,y+1.12,z+d*.94),
         (.41,1.92,.15),WHITE)
  k.box(tag+"reversible_unmarked_sill",(x,y+.17,z+d*.91),
        (w*.92,.29,.48),base)
  k.box(tag+"protected_guest_awning",(x,roof_base-.56,z+d*.8),
        (w*.85,.15,.94),GOLD)
  k.box(tag+"quiet_community_cup_board",(x+w*.61,y+.86,z+.20),
        (.22,1.1,.85),light)
  k.beam(tag+"intersecting_unnumbered_lintel",(x-w*.65,y+2.54,z+d*.93),
         (x+w*.65,y+2.54,z+d*.93),.12,trim)
 elif city=="Tharniv":
  for side in (-1,1):
   k.taper(tag+"grafted_root_truss",
        (x+side*w*.46,y,z),
        (x+side*.14,roof_base+1.30,z),.29,.08,base)
   k.beam(tag+"living_arched_branch",(x+side*w*.52,y+2.0,z+d*.37),
         (x+side*w*.68,roof_base+1.5,z),.12,trim)
  k.box(tag+"petition_civic_arbor",(x,roof_base+.76,z),
        (w+1.0,.17,d+.76),light)
  k.box(tag+"root_step_veranda",(x,y+.12,z+d*.86),
        (w*.85,.26,1.1),base)
  k.orb(tag+"grafted_public_fruit",(x+w*.59,roof_base+1.43,z),
        (.34,.45,.30),GOLD,1)
  k.box(tag+"uncrowned_threshold_witness",(x-w*.46,y+.65,z+d*.63),
        (.34,1.2,.22),WHITE)
 elif city=="Auvren":
  for side in (-1,1):
   k.box(tag+"discontinuous_limestone_arch",(x+side*w*.54,y+1.8,z),
         (.37,3.6,.36),WHITE)
   k.beam(tag+"witness_diagonal_frame",
         (x+side*w*.45,y+.3,z),
         (x+side*.20,roof_base+1.0,z),.15,trim)
  k.box(tag+"paired_threshold_crown",(x,roof_base+.72,z),
        (w+.86,.38,.53),WHITE)
  k.box(tag+"unresolved_ground_stair",(x,y+.18,z+d*.86),
        (w*.72,.34,1.05),base)
  k.box(tag+"public_origin_witness_plate",(x,y+1.52,z+d*.58),
        (1.03,.73,.15),GOLD)
  k.beam(tag+"unlinked_second_entrance",(x+w*.55,y,z-.4),
         (x+w*.55,y+2.7,z-.4),.09,WHITE)

def district_house_positions(plan):
 """Stable non-colliding footprint centers for region and city blockouts.

 These distances are exterior-shell spacing in a source-art diorama, not
 validated pathfinding, building clearance or finished Unreal collision.
 """
 positions=[]
 for district,(cx,cz,count,heading) in enumerate(plan):
  for j in range(count):
   radius=3.75+.50*((j+district)%3)
   angle=heading+2*math.pi*j/count+(.11 if j%2 else 0)
   for attempt in range(120):
    # Rotating candidates around the district, then widening the local ring,
    # preserves different street fabrics without placing buildings inside one another.
    theta=angle+(attempt%8)*.074
    r=radius+(attempt//8)*.42
    x=cx+r*math.cos(theta);z=cz+r*math.sin(theta)
    if all(math.hypot(x-px,z-pz)>=3.30 for px,pz in positions):
     positions.append((round(x,4),round(z,4)))
     break
   else:raise ValueError("Not enough room for inhabited district footprint")
 return positions

def _streetlife(k,city,district,x,z,trim,light):
 """Small source-lore civic props, not quest actors or interactive evidence."""
 t="streetlife_"+city.lower()+"_"+str(district)+"_"
 y=4.65 if city=="Orravane" else .12
 k.box(t+"public_pause_seat",(x-1.4,y+.22,z+1.1),(1.75,.26,.55),trim)
 k.box(t+"seat_support",(x-1.4,y-.01,z+1.1),(1.25,.36,.40),DARK)
 if city in ("Bellwold","Crossings","Eillun"):
  # A guest is sheltered before being asked for a name.
  k.box(t+"shared_kettle_stand",(x,y+.52,z),(.96,.82,.82),trim)
  k.orb(t+"ordinary_public_kettle",(x,y+1.05,z),(.43,.29,.40),DARK,1)
  k.box(t+"unregistered_cup",(x+.62,y+.75,z+.32),(.19,.20,.19),WHITE)
  if city=="Bellwold":
   k.beam(t+"lantern_tender_staff",(x-.8,y,z-1.2),(x-.8,y+2.4,z-1.2),.07,trim)
   k.orb(t+"stable_lantern",(x-.8,y+2.42,z-1.2),(.20,.29,.2),GOLD,1)
  elif city=="Crossings":
   k.box(t+"disagreeing_waystones",(x-.75,y+.50,z-1.0),(.49,.83,.42),WHITE)
   k.box(t+"alternate_route_mark",(x-.23,y+.65,z-1.0),(.34,.35,.42),GOLD)
  else:k.box(t+"unsigned_welcome_table",(x-.8,y+.56,z-1.0),(1.38,.22,.8),WHITE)
 elif city=="Paperhaven":
  k.box(t+"chalk_public_index",(x,y+1.1,z),(1.95,1.9,.35),WHITE)
  for a in range(3):
   k.box(t+"ledger_leaf_"+str(a),(x-.62+a*.59,y+.8,z+.2),(.35,.48,.04),trim)
  k.box(t+"ink_and_public_papers",(x+.9,y+.28,z+1.1),(1.2,.24,.8),GOLD)
 elif city=="Saltwake":
  k.box(t+"dry_water_catcher",(x,y+.35,z),(1.75,.55,1.28),trim)
  k.box(t+"public_rain_basin",(x,y+.67,z),(1.58,.14,1.13),light)
  for side in (-1,1):
   k.ring(t+"coiled_rope_"+str(side),(x+side*.85,y+.20,z-.95),.30,.08,WHITE,plane="xz")
 elif city=="Cinderhold":
  k.box(t+"ash_sieve",(x,y+.55,z),(1.5,.15,1.1),trim)
  k.box(t+"heat_transfer_channel",(x+.9,y+.13,z-1),(1.3,.20,.35),GOLD)
  k.taper(t+"communal_braided_furnace_flue",(x-.8,y,z-.4),(x-.6,y+2.1,z-.4),.33,.14,DARK)
 elif city=="Drevlach":
  k.box(t+"double_read_debt_slate",(x,y+1.1,z),(1.8,1.9,.14),trim)
  k.box(t+"public_accounting_table",(x+.8,y+.56,z+.94),(1.44,.16,.9),WHITE)
  k.ring(t+"tide_clock_marker",(x-.9,y+1.55,z),.55,.07,GOLD,plane="xy")
 elif city=="Orravane":
  k.beam(t+"restate_support_note_rail",(x-1.0,y,z),(x+1.0,y+.55,z),.13,WHITE)
  k.box(t+"rest_platform",(x,y+.2,z),(2.6,.21,1.85),trim)
  k.ring(t+"choir_counterweight",(x,y-1.2,z),.52,.09,GOLD,plane="xz")
 elif city=="Vathless":
  k.box(t+"quarry_safety_brace",(x,y+.55,z),(1.8,.34,.65),WHITE)
  k.beam(t+"supporting_rail",(x-1,y,z-1),(x+1.2,y+1.4,z-1),.16,trim)
  k.box(t+"family_stone_unfilled_seam",(x+.4,y+.9,z+.8),(1.25,1.0,.18),GOLD)
 elif city=="Tharniv":
  k.taper(t+"grafted_public_petition_root",(x,y,z),(x-.1,y+2.7,z),.47,.18,trim)
  for j in range(3):
   a=2*math.pi*j/3
   k.beam(t+"petition_branch_"+str(j),(x,y+2.0,z),
    (x+1.8*math.cos(a),y+2.7,z+1.6*math.sin(a)),.09,WHITE)
  k.box(t+"empty_public_witness_seat",(x+1,y+.45,z+1.1),(1.0,.4,.8),GOLD)
 elif city=="Auvren":
  k.box(t+"witness_threshold_base",(x,y+.3,z),(2.35,.53,1.9),WHITE)
  for side in (-1,1):
   k.beam(t+"paired_different_thresholds_"+str(side),
    (x+side*.90,y,z),(x+side*.90,y+2.4,z),.16,trim)
  k.box(t+"witness_record_without_answer",(x,y+2.55,z),(2.1,.21,.41),GOLD)

_DISTRICT_SURFACE_MODULE=None
def _district_surface():
 global _DISTRICT_SURFACE_MODULE
 if _DISTRICT_SURFACE_MODULE is None:
  import importlib.util
  loc=Path(__file__).with_name('district_surface_detail.py')
  spec=importlib.util.spec_from_file_location('unmade_district_surface_detail',loc)
  module=importlib.util.module_from_spec(spec)
  spec.loader.exec_module(module)
  _DISTRICT_SURFACE_MODULE=module
 return _DISTRICT_SURFACE_MODULE

_TERRAIN_JOINERY_MODULE=None
def _terrain_joinery():
 global _TERRAIN_JOINERY_MODULE
 if _TERRAIN_JOINERY_MODULE is None:
  import importlib.util
  source=Path(__file__).with_name('terrain_street_joinery.py')
  spec=importlib.util.spec_from_file_location('unmade_terrain_street_joinery',source)
  module=importlib.util.module_from_spec(spec)
  spec.loader.exec_module(module)
  _TERRAIN_JOINERY_MODULE=module
 return _TERRAIN_JOINERY_MODULE

def _build_habited_districts(k,city,realm,plan,roads,base,trim,light):
 spurs=[];positions=district_house_positions(plan);index=0
 for district,(cx,cz,count,heading) in enumerate(plan):
  # All homes sit within a visual district foundation; a separate neighborhood
  # spur joins the authored approach road. This does not promise Unreal traversal.
  floor_y=4.12 if city=="Orravane" else -.42
  # The load-bearing base is buried inside the terrain apron; the old
  # exposed 14.2m square pad formed a visible hard cliff around each district.
  k.box("district_ground_"+city.lower()+"_"+str(district),
        (cx,floor_y-.2,cz),(13.1,.2,13.1),base if district%2 else trim)
  _district_surface().finish_district_terrain(k,city,district,cx,cz,floor_y)
  endpoints=[p for route in roads for p in (route[0],route[-1])]
  connection=min(endpoints,key=lambda p:(p[0]-cx)**2+(p[1]-cz)**2)
  if math.hypot(connection[0]-cx,connection[1]-cz)<.1:
   spurs.append(((cx,cz),(cx+3*math.cos(heading),cz+3*math.sin(heading))))
  else:spurs.append((connection,(cx,cz)))
  _streetlife(k,city,district,cx,cz,trim,light)
  for j in range(count):
   x,z=positions[index]
   house_y=_dwelling(k,city,index,x,z,base,trim,light)
   width=2.25+.24*(index%3);depth=2.20+.28*(index%2)
   _terrain_joinery().join_house_to_relief(k,city,index,x,house_y,z,width,depth,
       district,cx,cz,floor_y,_district_surface().terrain_surface_height)
   index+=1
 _road_mesh(k,realm,spurs,"neighborhood",base,light)

def _regional_district_plan(realm):
 # Smaller previews remain inhabited, while city-level views show all four
 # districts at higher building density and their own civic micro-topology.
 roads=REALM_PATHS[realm]
 # The orchard is a radial road system: all routes BEGIN at the public empty seat.
 # District viewpoints belong at different branch destinations, not atop each other.
 points=[route[-1] if realm=='OrchardOfKings' else route[0] for route in roads[:3]]
 if len(points)<3:raise ValueError("Three independent regional approaches required")
 return tuple((float(x)*1.22,float(z)*1.18,4,(i*.81+.27)) for i,(x,z) in enumerate(points))

def _add_region_hero_layer(k,city):
 global _REALM_HERO_MODULE
 if _REALM_HERO_MODULE is None:
  import importlib.util
  module_path=Path(__file__).with_name('realm_hero_pbr.py')
  spec=importlib.util.spec_from_file_location('unmade_realm_hero_pbr',module_path)
  module=importlib.util.module_from_spec(spec)
  spec.loader.exec_module(module)
  _REALM_HERO_MODULE=module
 _REALM_HERO_MODULE.add_unique_hero(k,city)
 return _REALM_HERO_MODULE

_REALM_HERO_MODULE=None

def build_environment(realm):
 if realm not in REALM_PATHS:raise ValueError("Unknown canonical realm "+str(realm))
 k=Kit();base,trim,light=PALETTE[realm]
 _terrain(k,realm,base,trim)
 _road_mesh(k,realm,REALM_PATHS[realm],"realm",base,trim)
 _regional_signature(k,realm,base,trim,light)
 _build_habited_districts(k,REGION_HABITAT[realm],realm,_regional_district_plan(realm),REALM_PATHS[realm],base,trim,light)
 _add_region_hero_layer(k,REGION_HABITAT[realm])
 return k

_BELLWOLD_HERO_MODULE=None
def _add_bellwold_hero_layer(k):
 global _BELLWOLD_HERO_MODULE
 if _BELLWOLD_HERO_MODULE is None:
  import importlib.util
  module_path=Path(__file__).with_name('bellwold_hero_pbr.py')
  spec=importlib.util.spec_from_file_location('unmade_bellwold_hero_pbr',module_path)
  module=importlib.util.module_from_spec(spec)
  spec.loader.exec_module(module)
  _BELLWOLD_HERO_MODULE=module
 _BELLWOLD_HERO_MODULE.add_bellwold_hero(k)

def build_city_environment(city):
 if city not in CITY_SPECS:raise ValueError("Unknown canonical settlement "+str(city))
 spec=CITY_SPECS[city];realm=spec["realm"];base,trim,light=PALETTE[realm]
 k=Kit()
 _terrain(k,realm,base,trim,city)
 _road_mesh(k,realm,spec["routes"],"city",base,light)
 _regional_signature(k,realm,base,trim,light)
 _city_signature(k,city,base,trim,light)
 _build_habited_districts(k,city,realm,CITY_DISTRICTS[city],spec['routes'],base,trim,light)
 if city=='Bellwold':_add_bellwold_hero_layer(k)
 else:_add_region_hero_layer(k,city)
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
 for city,spec in CITY_SPECS.items():specs.append(('City',city,'city:'+city,spec['realm'],build_city_environment(city)))
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
  if info['triangles']<150 or info['parts']<({'Creature':5,'NPC':10,'Environment':15,'City':23}[group]):raise ValueError('Too little genuine model geometry: '+name)
  building_shells=sum('_habitation_shell_' in name for name in k.scene.geometry)
  district_count=(len(CITY_DISTRICTS[stable.removeprefix('city:')]) if group=='City' else len(_regional_district_plan(realm)) if group=='Environment' else 0)
  district_views=([[round(float(x),3),round(4.9 if realm=='SkyBelow' else 1.3,3),round(float(z),3)] for x,z,_,_ in (CITY_DISTRICTS[stable.removeprefix('city:')] if group=='City' else _regional_district_plan(realm))] if group in ('City','Environment') else [])
  pbr_materials=sorted({g.visual.material.name for g in k.scene.geometry.values() if getattr(getattr(g,'visual',None),'kind',None)=='texture' and hasattr(g.visual,'material') and g.visual.material is not None and getattr(g.visual.material,'baseColorTexture',None) is not None})
  hero_city=(stable.removeprefix('city:') if group=='City' else REGION_HABITAT[realm] if group=='Environment' else None)
  if hero_city=='Bellwold':hero_label,hero_focus='Empty bell yoke',[4.5,7.4,-5]
  elif hero_city:
   if _REALM_HERO_MODULE is None:raise ValueError('Region hero module not loaded')
   hero_label,hero_focus=_REALM_HERO_MODULE.FOCUS[hero_city]
   hero_focus=list(hero_focus)
  else:hero_label,hero_focus='',[]
  textured_roofs=sum('_district_pbr_pitched_roof_' in key for key in k.scene.geometry)
  sculpted_districts=sum('_district_terrain_relief_' in key for key in k.scene.geometry)
  blended_edges=sum('_terrain_blend_apron_' in key for key in k.scene.geometry)
  grounded_homes=sum('_terrain_joinery_support_' in key for key in k.scene.geometry)
  graded_walkways=sum('_terrain_joinery_walkway_' in key for key in k.scene.geometry)
  entries.append(dict(category=group,stable_id=stable,realm=realm,file=group+'/'+name,**info,building_shells=building_shells,district_count=district_count,district_views=district_views,pbr_materials=pbr_materials,hero_label=hero_label,hero_focus=hero_focus,textured_roofs=textured_roofs,sculpted_districts=sculpted_districts,blended_edges=blended_edges,grounded_homes=grounded_homes,graded_walkways=graded_walkways,mesh_type='static_visual_reference',gameplay_authority=False,rigged=False,unreal_import_verified=False))
 manifest={'schema':1,'title':'The Unmade — real source-authored 3D character, creature and realm scene meshes','generator':'Scripts/build_hero_models.py','asset_count':len(entries),'categories':{'NPC':18,'Creature':12,'Environment':9,'City':11},'format':'GLB (glTF 2.0), binary static meshes, vertex colors, and locally generated source-generated realm, city, roof and terrain UV/PBR albedo and normal textures','units':'meters, Y-up, +Z forward','authority':'Non-authoritative visual meshes only. No quest progress, collision, skinned animation, or verified UE import','assets':entries}
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
 print('PASS: %d real GLB meshes: 18 NPCs, 12 unique creatures, 9 realms and 11 cities; %d triangles'%(manifest['asset_count'],sum(a['triangles'] for a in manifest['assets'])))
if __name__=='__main__':main()
