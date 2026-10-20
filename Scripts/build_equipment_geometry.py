#!/usr/bin/env python3
"""Build source-linked prototype equipment mesh assets, without Unreal.

Exactly 60 main ItemCatalog entries + 18 regional equipment variants. Every
file is real glTF triangle geometry, not a screenshot, blueprint or fake item
unlock. Uses existing geometry kernel; all silhouette choices are temporary
visual production drafts and do not change provenance, rarity or save data.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
LEDGER="docs/canon/dependency-ledger.json"
GEOM_SCRIPT=Path(__file__).with_name("build_realm_geometry.py")
spec=importlib.util.spec_from_file_location("unmade_geometry_shared",GEOM_SCRIPT)
G=importlib.util.module_from_spec(spec)
spec.loader.exec_module(G)

def decorate(mesh,seed,width):
    # Stable individuality: geometry varies, not item stats or ability effects.
    marks=1+seed%3
    for i in range(marks):
        mesh.box(-width*.30+i*width*.23,.14,.37,.12,.10,.12,"accent")

def build_weapon(name,seed):
    m=G.Mesh()
    size=1+(.06*(seed%5))
    n=name.casefold()
    if "spear" in n or "lance" in n or "pike" in n:
        m.cylinder(0,0,0,.085,2.06*size,"structure",10)
        m.roof(0,2.02*size,0,.34,.43,.23,"accent")
        m.box(0,.17,0,.18,.15,.18,"stone")
    elif any(word in n for word in ("maul","hammer","mace","club")):
        m.cylinder(0,0,0,.095,1.65*size,"structure",10)
        m.box(0,1.39*size,0,.77,.37,.34,"stone")
        m.box(0,1.62*size,0,.73,.13,.34,"accent")
    elif any(word in n for word in ("staff","rod","scepter","wand")):
        m.cylinder(0,0,0,.081,1.9*size,"structure",10)
        m.cylinder(0,1.70*size,0,.25,.30,"accent",10)
        m.ring(0,1.84*size,0,.32,.24,.10,"structure")
    elif any(word in n for word in ("bow","crossbow")):
        m.cylinder(0,.15,0,.07,1.45*size,"structure",10)
        m.box(0,1.21*size,0,.95,.11,.12,"stone")
        m.box(0,.37,0,.95,.11,.12,"stone")
        m.box(0,.8,0,.09,.92,.06,"accent")
    else:
        m.box(0,0,0,.18,.79,.20,"structure")
        m.box(0,.71,0,.72,.12,.24,"accent")
        m.box(0,.82,0,.22,1.16*size,.12,"stone")
        m.roof(0,1.85*size,0,.27,.42,.13,"accent")
        m.cylinder(0,0,0,.14,.13,"stone",8)
    decorate(m,seed,.75)
    return m

def build_armor(name,seed):
    m=G.Mesh()
    size=1+(.05*(seed%5))
    m.box(0,.18,0,1.12,1.18*size,.33,"stone")
    m.roof(0,1.36*size,0,.85,.33,.40,"structure")
    for x in (-.58,.58):
        m.box(x,1.04*size,0,.38,.30,.52,"structure")
        m.box(x,.62,0,.12,.57,.41,"accent")
    m.box(0,.20,.19,.82,.19,.13,"accent")
    m.ring(0,.92,.27,.18,.12,.09,"accent")
    if "mantle" in name.casefold() or "cloak" in name.casefold():
        m.box(0,.19,-.27,1.36,1.46*size,.15,"structure")
    decorate(m,seed,.85)
    return m

def build_charm(name,seed):
    m=G.Mesh()
    width=1+(.1*(seed%4))
    m.ring(0,.90,0,.47*width,.31*width,.20,"structure")
    m.cylinder(0,.20,0,.28,.60,"stone",10)
    m.ring(0,.94,.16,.21,.12,.10,"accent")
    m.box(0,.33,.30,.33,.19,.10,"accent")
    decorate(m,seed,.80)
    return m

def build_potion(name,seed):
    m=G.Mesh()
    m.cylinder(0,.0,0,.30,.13,"stone",12)
    m.cylinder(0,.12,0,.30,.59,"accent",12)
    m.cylinder(0,.69,0,.12,.24,"structure",10)
    m.cylinder(0,.93,0,.21,.13,"stone",10)
    m.box(0,.26,.29,.31,.19,.07,"structure")
    decorate(m,seed,.55)
    return m

def build_material(name,seed):
    m=G.Mesh()
    text=name.casefold()
    if any(x in text for x in ("ink","water","oil")):
        m=build_potion(name,seed)
    elif any(x in text for x in ("glass","crystal","seed")):
        m.cylinder(0,0,0,.41,.20,"stone",8)
        m.roof(0,.20,0,.70,1.18,.55,"accent")
        m.ring(0,.36,.28,.22,.13,.06,"structure",10)
    elif any(x in text for x in ("herb","root","flower")):
        m.cylinder(0,0,0,.08,1.1,"structure",8)
        for x in (-.31,.31):
            m.box(x,.42,0,.52,.12,.25,"accent")
    else:
        m.box(0,0,0,1.05,.46,.77,"stone")
        m.box(0,.46,0,.82,.24,.59,"structure")
        m.box(0,.72,0,.65,.18,.46,"accent")
    return m

def make_mesh(record,kind):
    # Both regional weapon/armor rewards and ItemCatalog items have immutable IDs.
    key=str(record["id"])
    seed=int(hashlib.sha256(key.encode()).hexdigest()[:8],16)
    if kind=="Weapon":return build_weapon(record["name"],seed)
    if kind=="Armor":return build_armor(record["name"],seed)
    if kind=="Charm":return build_charm(record["name"],seed)
    if kind=="Consumable":return build_potion(record["name"],seed)
    if kind=="Material":return build_material(record["name"],seed)
    raise ValueError("Unknown canonical gear type: "+kind)

def make_assets(root:Path=ROOT):
    ledger=json.loads((root/LEDGER).read_text(encoding="utf-8"))
    items=ledger["items"]
    regional=ledger["regional_gear"]
    if len(items)!=60 or len(regional)!=18:
        raise ValueError("Canonical item/regional gear count changed")
    records=[]
    for item in items:
        if item["kind"] not in ("Weapon","Armor","Charm","Consumable","Material"):
            raise ValueError("Unsupported item kind "+str(item["kind"]))
        records.append((item,item["kind"],"ThreefoldReach","item_catalog"))
    for gear in regional:
        kind={"weapon":"Weapon","armor":"Armor"}.get(gear["slot"])
        if kind is None or gear["realm"] not in G.REALMS:
            raise ValueError("Unknown regional gear kind or realm")
        records.append((gear,kind,gear["realm"],"regional_gear"))
    ids=[x[0]["id"] for x in records]
    if len(set(ids))!=78:raise ValueError("Duplicate canonical equipment stable ID")
    manifest=[]
    files={}
    for item,kind,realm,group in records:
        name="SM_UnmadeGear_"+G.slug(item["id"])+".gltf"
        mesh=make_mesh(item,kind)
        content,triangles=G.gltf_bytes(mesh,name.removesuffix(".gltf"),realm)
        G.validate_model(content)
        if name in files:raise ValueError("Duplicate generated equipment filename")
        files[name]=content
        manifest.append({
          "id":item["id"],"name":item["name"],"kind":kind,
          "realm":realm if group=="regional_gear" else None,
          "group":group,"source":LEDGER,"file":name,
          "acquisition":item.get("acquisition"),
          "availability":item.get("availability","ledger_regional_gear"),
          "exclusive_with":item.get("exclusive_with"),
          "triangles":triangles,
          "sha256":hashlib.sha256(content).hexdigest(),
          "state":"SOURCE_PREVIEW_ONLY_NOT_UNREAL_IMPORTED_OR_EQUIPPED"})
    return {"schema":1,"asset_count":78,"main_catalog":60,"regional_gear":18,
      "warning":"Static prototype props only; not rigged wearable meshes, grants or verified Unreal assets",
      "records":manifest},files

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--out-dir",type=Path)
    ap.add_argument("--check",action="store_true")
    ap.add_argument("--verify-output",action="store_true")
    args=ap.parse_args()
    if args.verify_output and not args.out_dir:ap.error("--verify-output requires --out-dir")
    manifest,files=make_assets()
    meta=(json.dumps(manifest,indent=2,ensure_ascii=False)+"\n").encode()
    if args.verify_output:
        expected={**files,"manifest.json":meta}
        actual={p.name for p in args.out_dir.iterdir() if p.is_file()}
        if actual!=set(expected):raise ValueError("Equipment file set drift")
        for name,data in expected.items():
            if (args.out_dir/name).read_bytes()!=data:
                raise ValueError("Generated equipment byte drift: "+name)
    elif args.out_dir and not args.check:
        args.out_dir.mkdir(parents=True,exist_ok=True)
        for name,data in files.items(): (args.out_dir/name).write_bytes(data)
        (args.out_dir/"manifest.json").write_bytes(meta)
    print("PASS: 60 main and 18 regional gear models generated (78 original glTF); "
          "Unreal import, attachment and wearable rigging are unverified.")
    return 0

if __name__=="__main__":
    raise SystemExit(main())
