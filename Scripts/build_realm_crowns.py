#!/usr/bin/env python3
"""Nine lore-matched, independently importable low-poly realm crown glTF meshes.

This ports the distinctive landmark themes of the older, draft-based visual
branch into a completely separate pipeline on current main. Meshes are
noncolliding, non-authoritative preview art, NOT an authored playable level.
"""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path

import build_realm_geometry as G

ROOT=Path(__file__).resolve().parents[1]
# Each visual motif is tied to an established realm. Do not add story claims.
DESIGNS=(
    ("CharterArch","The Three-Span Charter","Three unequal road supports"),
    ("TideGauge","The Dry-Tide Gauge","Waterline that never agrees with itself"),
    ("EmberFlue","The Borrowed Hearth Vent","Two common furnace mouths"),
    ("LedgerChain","The Foreclosed Oath Pier","Unclaimed stacked debt slates"),
    ("ChoirWeight","The Choir Counterweight","Suspended inverted witness weight"),
    ("SplitSeam","The Twin Ancestral Seam","Nonmatching quarry layers"),
    ("OccupiedBed","The Occupied Blank Bed","Shelter without a published name"),
    ("RootGraft","The Unclaimed Graft","Public branching root supports"),
    ("PairedThreshold","The Double Honest Threshold","Two unfinished doorways"),
)

def geometry(family:str)->G.Mesh:
    m=G.Mesh()
    # Small common ground footprint to preserve a legible reference scale.
    m.box(0,0,0,5,.25,3.2,"stone")
    if family=="CharterArch":
        for x,h,w in ((-1.75,3.9,.50),(0,5.8,.42),(1.75,4.6,.60)):
            m.box(x,.25,0,w,h,.65,"structure")
            m.box(x,.25+h,0,w*1.65,.25,.9,"accent")
        m.box(0,2.65,-.10,3.8,.23,.35,"accent")
        m.ring(0,4.2,.42,.72,.53,.20,"accent")
    elif family=="TideGauge":
        for x,h,r in ((-1.6,5.4,.30),(1.55,4.5,.36)):
            m.cylinder(x,.25,0,r,h,"structure")
            for k in range(1,6):
                m.box(x,.25+k*h/6,.38,.75,.09,.14,"accent")
        m.box(0,1.2,0,4.5,.20,.40,"stone")
        m.box(0,3.7,0,4.5,.17,.40,"stone")
        m.ring(0,3.7,.65,1.08,.88,.12,"accent")
    elif family=="EmberFlue":
        for x,h in ((-1.25,5.7),(1.32,4.6)):
            m.box(x,.25,0,1.15,h,.98,"structure")
            m.box(x,.25+h,0,1.45,.40,1.24,"accent")
            m.box(x,.25+h*.68,0,1.35,.16,1.3,"stone")
        m.box(0,.48,-.40,4,.8,1.25,"accent")
        m.box(0,1.35,.4,3.1,.31,.53,"stone")
    elif family=="LedgerChain":
        for x,h in ((-1.4,2.1),(-.3,3.9),(.9,5.2)):
            m.box(x,.25,0,1.38,h,.42,"stone")
            m.box(x,.25+h,0,1.55,.25,1.25,"accent")
        for i in range(3):
            m.ring(-.65+i*.75,4.5-i*.62,.48,.38,.24,.12,"structure",12)
        m.box(0,1.5,-.7,4.2,.28,.24,"structure")
    elif family=="ChoirWeight":
        m.box(-1.85,.25,0,.55,5.35,.50,"structure")
        m.box(1.85,.25,0,.55,5.35,.50,"structure")
        m.box(0,5.55,0,4.1,.35,.65,"accent")
        m.box(0,2.6,0,.20,2.9,.30,"structure")
        m.box(0,1.58,0,2.3,.72,.84,"stone")
        m.cylinder(0,.65,.04,.64,.68,"accent",10)
        m.ring(0,3.7,.58,1.0,.78,.16,"accent")
    elif family=="SplitSeam":
        m.box(-1.36,.25,0,2.1,4.75,1.05,"stone")
        m.box(1.35,.25,0,2.0,5.65,1.15,"structure")
        for z in (1.0,2.2,3.4,4.6):
            m.box(-1.37,z,.61,2.2,.16,.12,"accent")
        for z in (1.4,2.7,3.9,5.15):
            m.box(1.35,z,.62,2.22,.12,.14,"accent")
        m.box(0,.26,0,.24,4.25,.39,"accent")
    elif family=="OccupiedBed":
        for x in (-1.85,1.85):
            m.box(x,.25,0,.36,5.5,.51,"structure")
            m.box(x,5.7,0,.65,.42,.80,"accent")
        m.box(0,.67,0,3.42,.40,1.95,"stone")
        m.box(0,1.14,-.66,1.0,.25,.55,"accent")
        m.box(0,2.15,.52,3.2,.16,.22,"structure")
        m.ring(0,4.15,.60,.72,.45,.13,"accent")
    elif family=="RootGraft":
        m.cylinder(0,.25,0,.69,4.4,"structure",10)
        for x,h in ((-1.55,2.8),(1.65,3.4)):
            m.cylinder(x,.27,.18,.32,h,"stone",9)
            m.box(x/2,2.2,.18,abs(x)+.28,.38,.65,"accent")
        for x,h in ((-.9,5.0),(.85,5.25)):
            m.cylinder(x,3.1,.0,.29,h-3.1,"accent",9)
        m.box(0,5.05,0,3.05,.22,.42,"structure")
    elif family=="PairedThreshold":
        for x,z,h in ((-1.35,.15,5.3),(1.45,-.42,6.1)):
            m.box(x-.62,.25,z,.34,h,.45,"structure")
            m.box(x+.62,.25,z,.34,h,.45,"structure")
            m.box(x,.25+h,z,1.76,.40,.70,"accent")
            m.box(x,.33,z,1.15,.15,.61,"stone")
        m.ring(-1.30,4.2,.65,.58,.41,.12,"accent")
        m.ring(1.45,4.85,.13,.56,.39,.13,"stone")
    else:
        raise ValueError("Unknown authored skyline family: "+str(family))
    return m


def build_crowns(root:Path=ROOT):
    cities,worlds=G.read_data(root)
    if len(DESIGNS)!=9:
        raise ValueError("Nine canonical realm concepts required")
    by_realm={r:next((c for c in cities if c["realm"]==r),None)
              for r in G.REALMS}
    if not all(by_realm.values()):
        raise ValueError("Missing canonical city anchors")
    output={}
    rows=[]
    for realm,(family,motif,description) in zip(G.REALMS,DESIGNS):
        city=by_realm[realm]
        model_name="SM_UnmadeRealmCrown_"+realm
        filename=model_name+".gltf"
        mesh=geometry(family)
        payload,triangles=G.gltf_bytes(mesh,model_name,realm)
        G.validate_model(payload)
        if filename in output:raise ValueError("Generated duplicate crown file")
        output[filename]=payload
        at=city["origin_cm"]
        rows.append({
            "stable_id":"visual.realm_crown."+realm,
            "realm":realm,
            "city_id":city["id"],
            "source":"Authoring/city_assembly_kits_v1.json",
            "visual_family":family,
            "motif":motif,
            "description":description,
            "file":filename,
            "triangles":triangles,
            "ue_position_cm":[at[0]-4100,at[1]+3600,at[2]+25],
            "ue_rotation_deg":[0,0,0],
            "ue_scale":[1,1,1],
            "sha256":hashlib.sha256(payload).hexdigest(),
            "collision":"none",
            "quest_authority":False,
            "status":"SOURCE_MESH_ENGINE_UNVERIFIED",
        })
    if len(rows)!=9 or len({r["visual_family"] for r in rows})!=9:
        raise ValueError("Nine distinct source-backed realm visual kits required")
    return {
        "schema_version":1,
        "purpose":"Nine original noncolliding skyline prototypes; distinct geometry, not shipped levels",
        "asset_count":9,
        "asset_status":"SOURCE_MESH_ENGINE_UNVERIFIED",
        "coordinate_system":"glTF Y-up, units meters; source placement Unreal XYZ centimeters",
        "rules":[
          "Preview-only landmarks may not block travel, E-interactions or projectile sight",
          "World and quest states, artifact identity, enemy behavior and save files unchanged",
          "Site offsets are authoring previews, not navigable gameplay landmarks",
          "Imported meshes remain separate from existing 193 static assets",
          "Historical illustrated releases, Unreal rendering and packaging unverified",
        ],
        "assets":rows,
    },output


def main():
    p=argparse.ArgumentParser()
    p.add_argument("--out-dir",type=Path)
    p.add_argument("--check",action="store_true")
    p.add_argument("--verify-output",action="store_true")
    args=p.parse_args()
    if args.verify_output and not args.out_dir:
        p.error("--verify-output requires --out-dir")
    manifest,files=build_crowns()
    expected={**files,"manifest.json":(json.dumps(manifest,indent=2)+"\n").encode()}
    if args.verify_output:
        if not args.out_dir.is_dir():
            raise ValueError("Missing crown output")
        if {p.name for p in args.out_dir.iterdir() if p.is_file()} != set(expected):
            raise ValueError("Crown asset inventory drift")
        for name,data in expected.items():
            if (args.out_dir/name).read_bytes()!=data:
                raise ValueError("Crown output byte mismatch: "+name)
    elif args.out_dir and not args.check:
        args.out_dir.mkdir(parents=True,exist_ok=True)
        for name,data in expected.items():
            (args.out_dir/name).write_bytes(data)
    print("PASS: nine distinct source-built realm crowns; no Unreal import or gameplay authority")


if __name__=="__main__":
    main()
