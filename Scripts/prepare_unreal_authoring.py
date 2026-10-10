#!/usr/bin/env python3
"""Offline-verified Unreal DataTable staging plan, no pip dependencies.

The generated Unreal CSVs are read-only production references. Creating/importing
them must never write quest state, existing gameplay maps, or SaveGame files.
"""
from __future__ import annotations
import argparse
import csv
import hashlib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GENERATED = ROOT / "Authoring" / "generated"
DEST = "/Game/UnmadeProduction/Data"
# Stable import destination and named FTableRowBase USTRUCT from the C++ module.
# Seven actual checked-in tables; do not invent an eighth cinematic or NPC pack.
TABLES = (
    ("npcs_unreal.csv", "DT_UnmadeNPCs", "UnmadeNpcAuthoringRow", 18),
    ("realms_unreal.csv", "DT_UnmadeRealms", "UnmadeRealmAuthoringRow", 9),
    ("abilities_unreal.csv", "DT_UnmadeAbilities", "UnmadeRiteAuthoringRow", 10),
    ("cinematic_shots_unreal.csv", "DT_UnmadeCinematicShots", "UnmadeCinematicShotAuthoringRow", 16),
    ("cinematic_realms_unreal.csv", "DT_UnmadeCinematicRealms", "UnmadeCinematicRealmAuthoringRow", 9),
    ("main_quest_steps_unreal.csv", "DT_UnmadeMainQuestSteps", "UnmadeQuestStepAuthoringRow", 54),
    ("resident_returns_unreal.csv", "DT_UnmadeResidentReturns", "UnmadeResidentReturnAuthoringRow", 82),
)

HEADER_FOR = {
    "UnmadeNpcAuthoringRow": "UnmadeAuthoringRows.h",
    "UnmadeRealmAuthoringRow": "UnmadeAuthoringRows.h",
    "UnmadeRiteAuthoringRow": "UnmadeAuthoringRows.h",
    "UnmadeCinematicShotAuthoringRow": "UnmadeCinematicAuthoringRows.h",
    "UnmadeCinematicRealmAuthoringRow": "UnmadeCinematicAuthoringRows.h",
    "UnmadeQuestStepAuthoringRow": "UnmadeQuestSceneRows.h",
    "UnmadeResidentReturnAuthoringRow": "UnmadeQuestSceneRows.h",
}

def authoring_plan(root=ROOT) -> dict:
    generated = root / "Authoring" / "generated"
    headers_dir = root / "Source" / "TheUnmadeGame" / "Public" / "Authoring"
    tables = []
    for filename, asset, struct, count in TABLES:
        file = generated / filename
        if not file.is_file():
            raise ValueError(f"Missing generated Unreal CSV {filename}")
        raw = file.read_bytes()
        if b"\x00" in raw:
            raise ValueError(f"Invalid NUL in {filename}")
        text = raw.decode("utf-8-sig")
        with file.open(encoding="utf-8-sig", newline="") as stream:
            rows = list(csv.DictReader(stream))
        if len(rows) != count:
            raise ValueError(f"{filename}: {len(rows)} rows, expected {count}")
        keys = rows[0].keys() if rows else []
        if not keys or "Name" not in keys:
            raise ValueError(f"{filename}: missing Unreal row Name field")
        if any(not value or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", value)
               for value in [x["Name"] for x in rows]):
            raise ValueError(f"{filename}: unsafe Unreal DataTable row ID")
        if len({row["Name"] for row in rows}) != count:
            raise ValueError(f"{filename}: duplicate DataTable row name")
        if any(None in row or None in row.values() for row in rows):
            raise ValueError(f"{filename}: malformed CSV column widths")
        header = (headers_dir / HEADER_FOR[struct]).read_text(encoding="utf-8")
        body = header.split(f"struct THEUNMADEGAME_API F{struct}",1)
        if len(body) != 2:
            raise ValueError(f"Missing compiled USTRUCT F{struct}")
        body = body[1].split("};",1)[0]
        properties = set(re.findall(r"UPROPERTY\([^)]*\)\s*(?:FString|float|int32|bool)\s+(\w+)",body))
        if not set(keys).difference({"Name"}).issubset(properties):
            raise ValueError(f"{filename}: CSV fields drifted from compiled {struct}")
        tables.append({
            "source": f"Authoring/generated/{filename}",
            "asset": f"{DEST}/{asset}",
            "row_struct": f"/Script/TheUnmadeGame.{struct}",
            "rows": count,
            "sha256": hashlib.sha256(raw).hexdigest(),
            "columns": list(keys),
        })
    return {
        "schema_version": 1, "mode": "editor-authoring-data-only",
        "data_folder": DEST, "tables": tables,
        "total_rows": sum(table["rows"] for table in tables),
        "expected_staging_actors": 52,
        "staging_map": "/Game/UnmadeProduction/Maps/Unmade_AuthoringStaging",
        "source_write_policy": "Never mutate source masters or generated CSVs from the Unreal Editor",
        "state_write_policy": "Never touch gameplay SaveGame or player progression",
    }

def main():
    p=argparse.ArgumentParser()
    p.add_argument("--check",action="store_true",help="validate only, no files written")
    p.add_argument("--json",action="store_true",help="print validated manifest JSON")
    args=p.parse_args()
    data=authoring_plan()
    if args.json: print(json.dumps(data,indent=2))
    else: print(f"PASS: {len(data['tables'])} editor tables / {data['total_rows']} authored rows / "
                f"{data['expected_staging_actors']} dedicated staging references")
    return 0

if __name__=="__main__":
    raise SystemExit(main())
