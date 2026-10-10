#!/usr/bin/env python3
"""Unreal Editor-only projection of 11 distinguishable city proxy layouts.

Dry-run is offline, without UE. Mutations require a dedicated map and
the first-PC batch runner's explicit approve environment variable.
"""
from __future__ import annotations
import importlib.util
import os
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
MAP="/Game/UnmadeProduction/Maps/Unmade_CityAssemblyStaging"

def builder():
    spec=importlib.util.spec_from_file_location("unmade_city_assembly",
                ROOT/"Scripts/build_city_assembly.py")
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module

def read_plan():
    plan=builder().build()
    if plan["editor_map"]!=MAP or plan["actor_count"]!=402:
        raise ValueError("City assembly staging plan drift")
    return plan

def apply_in_editor(plan):
    if os.environ.get("UNMADE_EDITOR_APPROVED")!="1":
        raise RuntimeError("User-approved first-PC batch required")
    import unreal
    world=unreal.EditorLevelLibrary.get_editor_world()
    if not world or "Unmade_CityAssemblyStaging" not in world.get_name():
        raise RuntimeError("Refusing to edit non-staging game map")
    existing={a.get_actor_label():a for a in
              unreal.EditorLevelLibrary.get_all_level_actors()}
    cube=unreal.load_asset("/Engine/BasicShapes/Cube.Cube")
    if not cube:raise RuntimeError("Unreal built-in cube missing")
    created=skipped=0
    for item in plan["actors"]:
        label=item["label"]
        if label in existing:
            skipped+=1
            # Do not rewrite custom artist modifications on reimport.
            continue
        p=item["position_cm"]
        s=item["scale"]
        actor=unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.StaticMeshActor,unreal.Vector(*p))
        if not actor: raise RuntimeError("Failed to create city guide "+label)
        actor.set_actor_label(label)
        comp=actor.get_component_by_class(unreal.StaticMeshComponent)
        if comp:
            comp.set_static_mesh(cube)
            comp.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        actor.set_actor_scale3d(unreal.Vector(*s))
        # No scripted quest events, E pickups, main-quest actors or SaveGame.
        created+=1
    if created:unreal.EditorLevelLibrary.save_current_level()
    if created+skipped!=plan["actor_count"]:
        raise RuntimeError("Incomplete city assembly")
    unreal.log(f"UNMADE 11-CITY ASSEMBLY: created={created} existing={skipped}")
    return created,skipped

if __name__=="__main__":
    plan=read_plan()
    print(f"DRY RUN ONLY: {plan['actor_count']} distinct source-bound markers across {plan['cities']} cities; no UE writes")
