#!/usr/bin/env python3
"""Create tangible low-poly geometry files for The Unmade without Unreal.

Builds 88 distinct canonical city-module static meshes and 27 physical-clue
meshes as glTF 2.0 assets with embedded vertex/index buffers and PBR colors.
Each file is independent; stable IDs and source positions are retained in a
manifest. Geometry is ORIGINAL PROTOTYPE ART, not an Unreal-imported, authored
level, gameplay trigger, collision map or final production mesh.

No network, Blender, external packages or commercial assets are required.
"""
from __future__ import annotations

import argparse
import base64
import hashlib
import json
import math
import re
import struct
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CITIES = "Authoring/city_assembly_kits_v1.json"
ATLAS = "Authoring/lived_universe_atlas.json"
REALMS = ("ThreefoldReach", "WidowedRain", "HearthBeneath", "TidalLedger",
          "SkyBelow", "CinderSpine", "HundredUnlived", "OrchardOfKings",
          "FirstAbsence")
# Prototype art palette only; not an added canon claim.
COLORS = {
 "ThreefoldReach": ((.42,.35,.27,1),(.68,.56,.34,1),(.20,.48,.45,1)),
 "WidowedRain": ((.26,.38,.45,1),(.57,.66,.65,1),(.26,.59,.72,1)),
 "HearthBeneath": ((.41,.30,.27,1),(.63,.47,.34,1),(.83,.38,.19,1)),
 "TidalLedger": ((.29,.33,.43,1),(.52,.57,.62,1),(.67,.54,.26,1)),
 "SkyBelow": ((.35,.34,.51,1),(.66,.61,.75,1),(.43,.71,.78,1)),
 "CinderSpine": ((.36,.31,.29,1),(.68,.52,.40,1),(.79,.37,.20,1)),
 "HundredUnlived": ((.31,.41,.45,1),(.62,.71,.69,1),(.76,.54,.49,1)),
 "OrchardOfKings": ((.35,.45,.31,1),(.72,.66,.44,1),(.58,.31,.39,1)),
 "FirstAbsence": ((.32,.35,.47,1),(.75,.76,.82,1),(.39,.63,.77,1)),
}
MATERIALS = ("stone", "structure", "accent")

def read_data(root: Path = ROOT):
    cities=json.loads((root/CITIES).read_text(encoding="utf-8"))["cities"]
    atlas=json.loads((root/ATLAS).read_text(encoding="utf-8"))["worlds"]
    if len(cities)!=11 or len(atlas)!=9:
        raise ValueError("Source city/realm count changed")
    if [a["id"] for a in atlas]!=list(REALMS):
        raise ValueError("Canonical realm order changed")
    realm_counts=Counter(x["realm"] for x in cities)
    if realm_counts!=Counter({REALMS[0]:3,**{r:1 for r in REALMS[1:]}}):
        raise ValueError("Unexpected city-to-realm coverage")
    if any(len(c["modules"])!=8 for c in cities):
        raise ValueError("City module count changed")
    if any(len(x["mystery"]["evidence"])!=3 for x in atlas):
        raise ValueError("Physical clue count changed")
    return cities,atlas

def normal(a,b,c):
    ux,uy,uz=(b[i]-a[i] for i in range(3))
    vx,vy,vz=(c[i]-a[i] for i in range(3))
    x,y,z=(uy*vz-uz*vy,uz*vx-ux*vz,ux*vy-uy*vx)
    length=math.sqrt(x*x+y*y+z*z)
    if length<1e-8:
        raise ValueError("Degenerate mesh face")
    return (x/length,y/length,z/length)

class Mesh:
    def __init__(self):
        self.triangles={m:[] for m in MATERIALS}
    def face(self,vertices,material="stone"):
        if material not in self.triangles or len(vertices)<3:
            raise ValueError("Invalid mesh face")
        a=tuple(vertices[0])
        for n in range(1,len(vertices)-1):
            tri=(a,tuple(vertices[n]),tuple(vertices[n+1]))
            self.triangles[material].append((tri,normal(*tri)))
    def box(self,x,y,z,w,h,d,material="stone"):
        if min(w,h,d)<=0:raise ValueError("Box must have positive dimensions")
        a,b=x-w/2,x+w/2
        low,high=y,y+h
        back,front=z-d/2,z+d/2
        faces=(
            [(a,low,front),(b,low,front),(b,high,front),(a,high,front)],
            [(b,low,back),(a,low,back),(a,high,back),(b,high,back)],
            [(b,low,front),(b,low,back),(b,high,back),(b,high,front)],
            [(a,low,back),(a,low,front),(a,high,front),(a,high,back)],
            [(a,high,front),(b,high,front),(b,high,back),(a,high,back)],
            [(a,low,back),(b,low,back),(b,low,front),(a,low,front)],
        )
        for f in faces:self.face(f,material)
    def roof(self,x,y,z,w,h,d,material="structure"):
        # Solid pitched roof: ridge is along Z; raised ridge gives a distinct silhouette.
        a,b=x-w/2,x+w/2
        back,front=z-d/2,z+d/2
        p=(x,y+h,front);q=(x,y+h,back)
        l=(a,y,front);r=(b,y,front)
        lb=(a,y,back);rb=(b,y,back)
        for f in ([l,r,p],[rb,lb,q],[l,p,q,lb],[p,r,rb,q],[r,l,lb,rb]):
            self.face(f,material)
    def cylinder(self,x,y,z,r,h,material="structure",sides=12):
        if r<=0 or h<=0:raise ValueError("Cylinder dimensions invalid")
        for i in range(sides):
            u,v=2*math.pi*i/sides,2*math.pi*(i+1)/sides
            a=(x+r*math.cos(u),y,z+r*math.sin(u))
            b=(x+r*math.cos(v),y,z+r*math.sin(v))
            c=(b[0],y+h,b[2]);d=(a[0],y+h,a[2])
            self.face([a,b,c,d],material)
            self.face([(x,y,z),b,a],material)
            self.face([(x,y+h,z),d,c],material)
    def ring(self,x,y,z,outer,inner,depth,material="accent",sides=16):
        if not (0<inner<outer):raise ValueError("Invalid ring radii")
        for i in range(sides):
            u,v=2*math.pi*i/sides,2*math.pi*(i+1)/sides
            def pt(r,ang,offset):
                return (x+r*math.cos(ang),y+r*math.sin(ang),z+offset)
            o1,o2=pt(outer,u,depth/2),pt(outer,v,depth/2)
            i1,i2=pt(inner,u,depth/2),pt(inner,v,depth/2)
            ob1,ob2=pt(outer,u,-depth/2),pt(outer,v,-depth/2)
            ib1,ib2=pt(inner,u,-depth/2),pt(inner,v,-depth/2)
            for f in ([o1,o2,i2,i1],[ob2,ob1,ib1,ib2],
                      [o1,ob1,ob2,o2],[i2,ib2,ib1,i1]):
                self.face(f,material)

def module_geometry(kind,profile,scale):
    if kind not in {"landmark","bridge","civic","social","architecture","path","evidence"}:
        raise ValueError("Unsupported city module kind "+kind)
    w,d,h=(max(.6,float(scale[0])),max(.65,float(scale[1])),
           max(1.2,float(scale[2])))
    m=Mesh()
    if kind=="bridge":
        m.box(0,.10,0,w,.17,d,"stone")
        m.box(-w*.42,.28,0,.12,.53,d,"structure")
        m.box(w*.42,.28,0,.12,.53,d,"structure")
        for zz in (-d*.32,0,d*.32):
            m.box(0,0,zz,w*.96,.16,.13,"accent")
        m.box(0,-.22,-d*.38,w*.32,.25,.22)
        m.box(0,-.22,d*.38,w*.32,.25,.22)
    elif kind in ("civic","architecture"):
        m.box(0,0,0,w,h*.70,d,"stone")
        m.roof(0,h*.70,0,w*1.10,h*.20,d*1.09,"structure")
        m.box(0,.02,d*.502,w*.21,h*.45,.08,"accent")
        for x in (-w*.38,w*.38):
            m.box(x,0,d*.54,w*.12,h*.68,.14,"structure")
        if kind=="civic":
            m.box(0,0,d*.64,w*.9,.24,d*.21,"structure")
            m.ring(0,h*.43,d*.55,min(w,h)*.16,min(w,h)*.10,.08)
        else:
            m.box(0,h*.29,-d*.54,w*.53,h*.10,.12,"accent")
            m.box(-w*.26,h*.22,-d*.54,w*.1,h*.45,.12,"structure")
    elif kind=="social":
        m.box(0,0,0,w,.13,d,"stone")
        for x in (-w*.42,w*.42):
            for z in (-d*.42,d*.42):
                m.cylinder(x,0,z,.08,h*.68,"structure",8)
        m.roof(0,h*.7,0,w*1.02,h*.27,d*1.02,"accent")
        m.box(0,.18,0,w*.52,.22,d*.25,"structure")
    elif kind=="path":
        for i in range(7):
            offset=(i-3)*w*.14
            m.box(offset,0,math.sin(i*1.3)*.10,w*.11,.09,d*.65,"stone")
        m.box(0,.05,-d*.39,w,.07,.08,"accent")
        m.box(0,.05,d*.39,w,.07,.08,"accent")
    elif kind=="landmark":
        m.box(0,0,0,w*.8,.26,d*.75,"stone")
        m.box(0,.26,0,w*.46,h*.48,d*.40,"structure")
        m.roof(0,.26+h*.48,0,w*.54,h*.24,d*.49,"accent")
        if profile in {"vacant_bell","upside_down_choir","before_arrival"}:
            m.ring(0,h*.74,d*.29,min(w,h)*.30,min(w,h)*.21,.15,"accent")
        else:
            m.cylinder(0,h*.72,0,min(w,d)*.15,h*.19,"accent",10)
        m.box(0,.39,d*.25,w*.24,.11,.10,"accent")
    else: # evidence
        m.box(0,0,0,w*.8,.22,d*.75,"stone")
        m.box(0,.22,0,w*.45,h*.69,d*.30,"structure")
        m.ring(0,.42+h*.37,d*.18,min(w,h)*.17,min(w,h)*.10,.09,"accent")
        m.box(0,h*.25,d*.18,w*.31,.07,.07,"accent")
    return m

def clue_geometry(site,number):
    s=site.casefold()
    m=Mesh()
    m.box(0,0,0,1.55,.18,1.25)
    if any(token in s for token in ("bell","ring","wheel","counterweight","loom")):
        m.box(-.53,.18,0,.15,1.22,.16,"structure")
        m.box(.53,.18,0,.15,1.22,.16,"structure")
        m.box(0,1.32,0,1.22,.13,.16,"structure")
        m.ring(0,.85,.02,.40,.28,.12,"accent")
    elif any(token in s for token in ("bench","stool","desk","register","oven","press")):
        m.box(0,.64,0,1.25,.16,.75,"structure")
        for x in (-.47,.47):
            for z in (-.25,.25):
                m.box(x,.18,z,.13,.48,.13,"structure")
        m.box(0,.86,-.13,.85,.08,.42,"accent")
    elif any(token in s for token in ("bridge","gate","threshold","door","footprint","stair")):
        m.box(-.51,.18,0,.24,1.45,.30,"structure")
        m.box(.51,.18,0,.24,1.45,.30,"structure")
        m.box(0,1.50,0,1.16,.20,.35,"accent")
        m.box(0,.19,0,.58,.12,.33,"accent")
    elif "tree" in s or "bark" in s:
        m.cylinder(0,.18,0,.23,1.56,"structure")
        for x,z in ((-.37,.24),(.45,0),(-.09,-.39)):
            m.cylinder(x,.92,z,.20,.66,"accent",8)
    else:
        m.box(0,.18,0,.60,1.16,.46,"structure")
        m.roof(0,1.34,0,.69,.26,.54,"accent")
        m.box(0,.65,.26,.42,.11,.08,"accent")
        m.box(0,.88,.26,.30,.07,.08,"accent")
    # Three distinct placement marks per realm; purely visual, no unlock.
    for i in range(number):
        m.box(-.45+i*.40,.18,.50,.15,.12,.12,"accent")
    return m

def gltf_bytes(mesh: Mesh,name:str,realm:str):
    blob=bytearray()
    views=[];accessors=[];primitives=[]
    def add(rows,fmt,kind,target=None):
        while len(blob)%4:blob.append(0)
        start=len(blob)
        for record in rows:blob.extend(struct.pack("<"+fmt,*record))
        v={"buffer":0,"byteOffset":start,"byteLength":len(blob)-start}
        if target is not None:v["target"]=target
        views.append(v)
        comp={"3f":5126,"I":5125}[fmt]
        accessor={"bufferView":len(views)-1,"componentType":comp,"count":len(rows),"type":kind}
        if kind=="VEC3":
            accessor["min"]=[min(r[i] for r in rows) for i in range(3)]
            accessor["max"]=[max(r[i] for r in rows) for i in range(3)]
        elif kind=="SCALAR":
            accessor["min"]=[min(r[0] for r in rows)]
            accessor["max"]=[max(r[0] for r in rows)]
        accessors.append(accessor)
        return len(accessors)-1
    total_triangles=0
    for mat,triangles in mesh.triangles.items():
        if not triangles:continue
        positions=[];normals=[]
        for tri,n in triangles:
            positions.extend(tri); normals.extend([n]*3)
        nverts=len(positions)
        vertex_idx=add(positions,"3f","VEC3",34962)
        normal_idx=add(normals,"3f","VEC3",34962)
        index_idx=add([(n,) for n in range(nverts)],"I","SCALAR",34963)
        primitives.append({"attributes":{"POSITION":vertex_idx,"NORMAL":normal_idx},
                           "indices":index_idx,"mode":4,
                           "material":MATERIALS.index(mat)})
        total_triangles+=len(triangles)
    if not primitives or total_triangles<10:
        raise ValueError("Empty or malformed generated model "+name)
    colors=COLORS[realm]
    doc={
      "asset":{"version":"2.0","generator":"The Unmade source-authored prototype geometry v1"},
      "scene":0,"scenes":[{"nodes":[0]}],
      "nodes":[{"name":name,"mesh":0}],
      "meshes":[{"name":name,"primitives":primitives}],
      "materials":[{"name":"M_"+realm+"_"+mat,
        "doubleSided":True,
        "pbrMetallicRoughness":{"baseColorFactor":colors[i],
                                "roughnessFactor":.78,
                                "metallicFactor":.07}}
        for i,mat in enumerate(MATERIALS)],
      "buffers":[{"byteLength":len(blob),
                  "uri":"data:application/octet-stream;base64,"+
                        base64.b64encode(blob).decode("ascii")}],
      "bufferViews":views,"accessors":accessors
    }
    return (json.dumps(doc,sort_keys=True,separators=(",",":"),ensure_ascii=False)+"\n").encode("utf-8"),total_triangles

def slug(name:str)->str:
    return re.sub(r"[^A-Za-z0-9_]+","_",name).strip("_")

def build(root:Path=ROOT):
    cities,atlas=read_data(root)
    artifacts={}
    entries=[]
    for city in cities:
        for module in city["modules"]:
            ident=city["id"]+"/"+module["slug"]
            file="SM_Unmade_"+slug(ident)+".gltf"
            geom=module_geometry(module["kind"],city["profile"],module["scale"])
            payload,triangles=gltf_bytes(geom,slug(file[:-5]),city["realm"])
            if file in artifacts:raise ValueError("Duplicate generated file "+file)
            artifacts[file]=payload
            origin=city["origin_cm"]
            local=module["local_cm"]
            # Existing staging markers are centered cubes; asset origins use bases.
            base_z=round(origin[2]+local[2]-float(module["scale"][2])*50,3)
            placement=[origin[0]+local[0],origin[1]+local[1],base_z]
            entries.append({"stable_id":ident,"realm":city["realm"],
                "city_id":city["id"],"kind":module["kind"],
                "source":CITIES,"source_profile":city["profile"],
                "file":file,"ue_position_cm":placement,"ue_rotation_deg":[0,0,0],
                "ue_scale":[1,1,1],"triangles":triangles,
                "sha256":hashlib.sha256(payload).hexdigest(),
                "status":"SOURCE_MESH_ENGINE_UNVERIFIED"})
    for world in atlas:
        for number,clue in enumerate(world["mystery"]["evidence"],1):
            ident=clue["id"]
            file="SM_Unmade_"+slug(ident)+".gltf"
            payload,triangles=gltf_bytes(clue_geometry(clue["site"],number),
                                         slug(file[:-5]),world["id"])
            if file in artifacts:raise ValueError("Duplicate generated file "+file)
            artifacts[file]=payload
            entries.append({"stable_id":ident,"realm":world["id"],
                "kind":"physical_clue","site":clue["site"],
                "interaction":clue["verb"],"discovery":clue["discovery"],
                "source":ATLAS,"file":file,"ue_position_cm":None,
                "ue_rotation_deg":[0,0,0],"ue_scale":[1,1,1],
                "triangles":triangles,"sha256":hashlib.sha256(payload).hexdigest(),
                "status":"SOURCE_MESH_ENGINE_UNVERIFIED"})
    if len(entries)!=115 or len({x["stable_id"] for x in entries})!=115:
        raise ValueError("Expected 88 city modules and 27 unique clue props")
    manifest={
      "schema_version":1,
      "purpose":"Real low-poly glTF prototype geometry, not Unreal imported/playtested assets",
      "coordinate_system":"glTF Y-up in meters; placement values are Unreal XYZ centimeters",
      "rules":["No mesh is an interactable quest actor by itself",
               "Clue meshes have deliberately unspecified UE placement until local evidence sites are built",
               "Neither static mesh nor guide marker grants rewards, progress or witness memory",
               "Do not overwrite artists' meshes during import; verify collision and material mapping in editor"],
      "asset_count":len(entries),"city_module_count":88,"clue_count":27,
      "assets":entries,
    }
    return manifest,artifacts

def validate_model(payload:bytes):
    doc=json.loads(payload)
    if doc["asset"]["version"]!="2.0" or not doc["meshes"][0]["primitives"]:
        raise ValueError("Invalid glTF")
    raw=base64.b64decode(doc["buffers"][0]["uri"].split(",",1)[1],validate=True)
    if len(raw)!=doc["buffers"][0]["byteLength"]:raise ValueError("Invalid glTF buffer")
    for v in doc["bufferViews"]:
        if v["byteOffset"]%4 or v["byteOffset"]+v["byteLength"]>len(raw):
            raise ValueError("Invalid glTF buffer-view boundary")
    for p in doc["meshes"][0]["primitives"]:
        pos=doc["accessors"][p["attributes"]["POSITION"]]
        inds=doc["accessors"][p["indices"]]
        if pos["count"]<=0 or inds["count"]%3 or not inds["count"]:
            raise ValueError("Invalid glTF triangle count")
        iv=doc["bufferViews"][inds["bufferView"]]
        array=struct.unpack_from("<"+"I"*inds["count"],raw,iv["byteOffset"])
        if max(array)>=pos["count"]:
            raise ValueError("Out-of-bounds glTF mesh index")
    return True

def main():
    p=argparse.ArgumentParser()
    p.add_argument("--out-dir",type=Path)
    p.add_argument("--check",action="store_true",
                   help="Validate sources and geometry without filesystem writes")
    p.add_argument("--verify-output",action="store_true",
                   help="Verify exact generated bytes in --out-dir")
    args=p.parse_args()
    if args.verify_output and not args.out_dir:p.error("--verify-output requires --out-dir")
    manifest,files=build()
    for content in files.values():validate_model(content)
    generated=json.dumps(manifest,indent=2,ensure_ascii=False)+"\n"
    if args.verify_output:
        expected={**files,"manifest.json":generated.encode("utf-8")}
        actual={p.name for p in args.out_dir.iterdir() if p.is_file()}
        if actual!=set(expected):raise ValueError("Prototype geometry output file drift")
        for name,content in expected.items():
            if (args.out_dir/name).read_bytes()!=content:
                raise ValueError("Prototype geometry output byte drift: "+name)
    elif args.out_dir and not args.check:
        args.out_dir.mkdir(parents=True,exist_ok=True)
        for name,content in files.items():
            (args.out_dir/name).write_bytes(content)
        (args.out_dir/"manifest.json").write_text(generated,encoding="utf-8")
    print("PASS: 88 individual city modules and 27 clue props; "
          "115 original glTF meshes; no Unreal imports or gameplay verified.")
    if args.out_dir and not args.check:
        print("Generated at "+str(args.out_dir))
    return 0

if __name__=="__main__":
    raise SystemExit(main())
