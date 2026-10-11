#!/usr/bin/env python3
"""Generate ORIGINAL low-poly skinned glTF characters with 19 keyed motion clips.

No Blender, network, paid assets or AI generation required. Each GLTF embeds its
mesh, skin, skeleton, time samplers and animated quaternion keyframes. The
source-authored models are previsualization assets, NOT completed Unreal assets.
"""
from __future__ import annotations
import argparse, base64, json, math, struct
from pathlib import Path

KINDS = {
 "Resident": [
  ("root",None,(0,0,0),None),
  ("torso","root",(0,1.18,0),(.27,.45,.18)),
  ("head","torso",(0,.63,0),(.22,.24,.23)),
  ("left_arm","torso",(-.4,.20,0),(.13,.4,.14)),
  ("right_arm","torso",(.4,.20,0),(.13,.4,.14)),
  ("left_leg","root",(-.19,.50,0),(.16,.5,.16)),
  ("right_leg","root",(.19,.50,0),(.16,.5,.16)),
 ],
 "Beast": [
  ("root",None,(0,0,0),None),
  ("chest","root",(0,.76,0),(.78,.38,.32)),
  ("head","chest",(0,.10,.70),(.32,.31,.38)),
  ("front_left","chest",(-.52,-.37,.54),(.14,.43,.16)),
  ("front_right","chest",(.52,-.37,.54),(.14,.43,.16)),
  ("rear_left","root",(-.49,.43,-.47),(.18,.48,.17)),
  ("rear_right","root",(.49,.43,-.47),(.18,.48,.17)),
 ],
 "Wraith": [
  ("root",None,(0,0,0),None),
  ("core","root",(0,1.05,0),(.42,.64,.38)),
  ("eye","core",(0,.10,.43),(.19,.19,.15)),
  ("left_tendril","core",(-.57,-.16,0),(.2,.55,.19)),
  ("right_tendril","core",(.57,-.16,0),(.2,.55,.19)),
 ],
}
COLORS={"Resident":(0.5,0.37,0.28,1),"Beast":(.19,.23,.29,1),
        "Wraith":(.18,.44,.55,1)}
MATERIALS={
 "MI_AshBone":((.56,.36,.19,1),.91,.02),
 "MI_PaleGlass":((.35,.69,.83,1),.17,.48),
 "MI_BellPatina":((.38,.49,.38,1),.65,.67),
 "MI_ArchiveInk":((.21,.17,.29,1),.8,.1),
 "MI_SaltFoam":((.2,.57,.6,1),.48,.18),
 "MI_KilnSlag":((.44,.19,.12,1),.93,.23)
}
BOX_FACES=[
 ((0,0,1),[(0,0,1),(1,0,1),(1,1,1),(0,1,1)]),
 ((0,0,-1),[(1,0,0),(0,0,0),(0,1,0),(1,1,0)]),
 ((1,0,0),[(1,0,1),(1,0,0),(1,1,0),(1,1,1)]),
 ((-1,0,0),[(0,0,0),(0,0,1),(0,1,1),(0,1,0)]),
 ((0,1,0),[(0,1,1),(1,1,1),(1,1,0),(0,1,0)]),
 ((0,-1,0),[(0,0,0),(1,0,0),(1,0,1),(0,0,1)])
]

def angle_quat(axis, degrees):
    radians=math.radians(degrees)*.5
    s=math.sin(radians)
    return tuple(axis[i]*s for i in range(3))+(math.cos(radians),)

class Packed:
    def __init__(self):
        self.binary=bytearray()
        self.views=[]
        self.accessors=[]
    def add(self, records, dtype, kind, target=None):
        raw=b"".join(struct.pack("<"+dtype,*r) for r in records)
        while len(self.binary)%4:self.binary.append(0)
        offset=len(self.binary); self.binary.extend(raw)
        view={"buffer":0,"byteOffset":offset,"byteLength":len(raw)}
        if target is not None:view["target"]=target
        self.views.append(view)
        fmt={"f":5126,"H":5123,"I":5125}
        count=len(records)
        accessor={"bufferView":len(self.views)-1,"componentType":fmt[dtype[-1]],
                  "count":count,"type":kind}
        if kind=="VEC3" and dtype=="3f" and records:
            accessor["min"]=[min(x[i] for x in records) for i in range(3)]
            accessor["max"]=[max(x[i] for x in records) for i in range(3)]
        if kind=="SCALAR" and records:
            accessor["min"]=[min(x[0] for x in records)]
            accessor["max"]=[max(x[0] for x in records)]
        self.accessors.append(accessor)
        return len(self.accessors)-1

def finish(doc,packed):
    doc["buffers"]=[{"byteLength":len(packed.binary),
      "uri":"data:application/octet-stream;base64,"+
       base64.b64encode(packed.binary).decode("ascii")}]
    doc["bufferViews"]=packed.views;doc["accessors"]=packed.accessors
    return doc

def make_actor(kind):
    specs=KINDS[kind]
    ids={v[0]:i for i,v in enumerate(specs)}
    nodes=[]
    absolute={}
    for name,parent,xyz,_ in specs:
        origin=absolute[parent] if parent else (0,0,0)
        absolute[name]=tuple(origin[i]+xyz[i] for i in range(3))
        nodes.append({"name":name,"translation":list(xyz)})
    for name,parent,_,_ in specs:
        if parent:nodes[ids[parent]].setdefault("children",[]).append(ids[name])
    pos=[];normal=[];joints=[];weights=[];indices=[]
    for name,parent,xyz,half in specs:
        if half is None:continue
        bone=ids[name];center=absolute[name]
        start=len(pos)
        for normal3,face in BOX_FACES:
            for x,y,z in face:
                pos.append(tuple(center[i]+(v*2-1)*half[i] for i,v in enumerate((x,y,z))))
                normal.append(normal3)
                joints.append((bone,0,0,0))
                weights.append((1.,0.,0.,0.))
            k=start;start+=4
            indices.extend([k,k+1,k+2,k,k+2,k+3])
    packed=Packed()
    attr={
        "POSITION":packed.add(pos,"3f","VEC3",34962),
        "NORMAL":packed.add(normal,"3f","VEC3",34962),
        "JOINTS_0":packed.add(joints,"4H","VEC4",34962),
        "WEIGHTS_0":packed.add(weights,"4f","VEC4",34962)}
    index=packed.add([(x,) for x in indices],"H","SCALAR",34963)
    inverse=[]
    for name,_,_,_ in specs:
        x,y,z=absolute[name]
        inverse.append((1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.,-x,-y,-z,1.))
    inverse_id=packed.add(inverse,"16f","MAT4")
    times=[0.,.25,.5,.75,1.]
    time_id=packed.add([(t,) for t in times],"f","SCALAR")
    animations=[]
    for clip in ["Idle","Walk","Attack","Gesture","Speak","Guard","Stagger","Death","Cast","Inspect","Repair","Release","Ranged","Crush","Sweep","Seal","Yield","Dodge","Parry"]:
        samplers=[];channels=[]
        for name,_,_,_ in specs[1:]:
            if clip=="Walk" and ("leg" in name or "front" in name or "rear" in name):
                degrees=[0,19,0,-19,0]
            elif clip=="Walk" and ("arm" in name or "tendril" in name):
                degrees=[0,-15,0,15,0]
            elif clip=="Attack" and name in ("head","eye"):
                degrees=[0,0,26,8,0]
            elif clip=="Attack" and ("arm" in name or "front" in name or "tendril" in name):
                degrees=[0,28,-48,-15,0]
            elif clip=="Gesture" and (name=="right_arm" or name=="right_tendril" or name=="front_right"):
                degrees=[0,-32,-48,-26,0]
            elif clip=="Speak" and name in ("head","eye"):
                degrees=[0,9,-5,7,0]
            elif clip=="Idle" and name in ("torso","chest","core"):
                degrees=[0,3,0,-3,0]
            elif clip in ("Guard","Stagger","Death","Cast","Inspect","Repair","Release","Ranged","Crush","Sweep","Seal","Yield","Dodge","Parry"):
                poses={"Guard":[-28,-28,-28,-28,-28],"Stagger":[0,-24,13,-6,0],
                  "Death":[0,15,48,78,88],"Cast":[0,-25,-65,-40,0],"Inspect":[0,12,25,12,0],
                  "Repair":[0,38,-30,38,0],"Release":[0,-30,-45,-20,0],"Ranged":[0,-45,-45,12,0],
                  "Crush":[0,45,65,-55,0],"Sweep":[0,-35,55,30,0],"Seal":[0,-35,-60,-35,0],
                  "Yield":[0,15,35,35,35],"Dodge":[0,30,55,20,0],"Parry":[0,-30,24,-12,0]}
                degrees=poses[clip]
                if "leg" in name or "rear" in name:degrees=[d*.25 for d in degrees]
            else:
                continue
            axis=(0,1,0) if clip=="Sweep" else (1,0,0) if name not in ("core","torso","chest") else (0,0,1)
            output=packed.add([angle_quat(axis,d) for d in degrees],"4f","VEC4")
            si=len(samplers)
            samplers.append({"input":time_id,"output":output,"interpolation":"LINEAR"})
            channels.append({"sampler":si,"target":{"node":ids[name],"path":"rotation"}})
        animations.append({"name":"AN_Unmade"+kind+"_"+clip,"samplers":samplers,
                           "channels":channels})
    doc={"asset":{"version":"2.0","generator":"The Unmade original deterministic rig builder"},
         "scene":0,"scenes":[{"nodes":[0,len(nodes)]}],
         "nodes":nodes+[{"name":"Mesh_"+kind,"mesh":0,"skin":0}],
         "skins":[{"joints":list(range(len(specs))),"skeleton":0,
                   "inverseBindMatrices":inverse_id}],
         "meshes":[{"name":"SK_Unmade"+kind,
                    "primitives":[{"attributes":attr,"indices":index,"material":0}]}],
         "materials":[{"name":"M_Unmade"+kind,"pbrMetallicRoughness":{
             "baseColorFactor":COLORS[kind],"metallicFactor":0.0,"roughnessFactor":.86}}],
         "animations":animations}
    return finish(doc,packed)

def make_material_atlas():
    packed=Packed()
    pos=[];norm=[];indices=[]
    primitives=[]
    mats=list(MATERIALS)
    for i,name in enumerate(mats):
        first=len(pos)
        center=(i*1.45,0,0)
        for n,face in BOX_FACES:
            for x,y,z in face:
                pos.append((center[0]+x, y, z))
                norm.append(n)
            start=first;first+=4
            indices.extend([start,start+1,start+2,start,start+2,start+3])
        # Give each color its own indexed primitive for PBR material import.
        # mesh vertices are shared; six separate material primitives use distinct ranges.
        end=len(indices)
        ia=packed.add([(z,) for z in indices[-36:]],"H","SCALAR",34963)
        primitives.append({"attributes":None,"indices":ia,"material":i})
    # All six primitives share one vertex buffer and each index slice.
    pa=packed.add(pos,"3f","VEC3",34962)
    na=packed.add(norm,"3f","VEC3",34962)
    for prim in primitives:prim["attributes"]={"POSITION":pa,"NORMAL":na}
    materials=[{"name":name,"pbrMetallicRoughness":{
        "baseColorFactor":list(col),"metallicFactor":metal,"roughnessFactor":rough}}
        for name,(col,rough,metal) in MATERIALS.items()]
    return finish({"asset":{"version":"2.0","generator":"The Unmade materials atlas"},
      "scene":0,"scenes":[{"nodes":[0]}],"nodes":[{"mesh":0,"name":"MaterialSwatches"}],
      "meshes":[{"name":"M_UnmadeSixRegions","primitives":primitives}],
      "materials":materials},packed)

def validate(path):
    doc=json.loads(path.read_text(encoding="utf-8"))
    assert doc["asset"]["version"]=="2.0"
    blob=base64.b64decode(doc["buffers"][0]["uri"].split(",",1)[1])
    assert len(blob)==doc["buffers"][0]["byteLength"]
    for accessor in doc["accessors"]:
        view=doc["bufferViews"][accessor["bufferView"]]
        assert view["byteOffset"]+view["byteLength"]<=len(blob)
    for skin in doc.get("skins",[]):
        assert len(skin["joints"])>=4
    for anim in doc.get("animations",[]):
        assert anim["samplers"] and anim["channels"]
    return {"path":path.name,"bytes":path.stat().st_size,
            "joints":len(doc.get("skins",[{"joints":[]}])[0]["joints"]),
            "animation_clips":[a["name"] for a in doc.get("animations",[])]}

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--out",type=Path,required=True)
    args=ap.parse_args()
    args.out.mkdir(parents=True,exist_ok=True)
    for kind in KINDS:
        p=args.out/("SK_Unmade"+kind+".gltf")
        p.write_text(json.dumps(make_actor(kind),separators=(",",":")),encoding="utf-8")
        print(json.dumps(validate(p)))
    p=args.out/"UnmadeSixRegionPBR.gltf"
    p.write_text(json.dumps(make_material_atlas(),separators=(",",":")),encoding="utf-8")
    print(json.dumps(validate(p)))
if __name__=="__main__":
    main()
