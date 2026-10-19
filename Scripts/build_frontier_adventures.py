#!/usr/bin/env python3
"""Validate the historical frontier release against Codex-owned expeditions.

This compatibility script no longer writes the live ExpeditionRegistry.h:
the Codex's deterministic generator owns executable expedition rule tables.
"""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT))
from Scripts.generate_unmade_codex import REALMS,generate_expeditions

LEGACY=ROOT/"Authoring/frontier_adventures_v14.json"
CODEX=ROOT/"Authoring/ability_codex.json"
TARGET=ROOT/"Source/TheUnmadeGame/Public/World/UnmadeCodexExpeditionGenerated.inc"
WRAPPER=ROOT/"Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h"
FIELDS=("city","frontier","refuge","dungeon","boss","weapon","armor","technique",
        "kill_return","mercy_return","people","clues")

def build():
    legacy=json.loads(LEGACY.read_text(encoding="utf-8"))["arcs"]
    canon=json.loads(CODEX.read_text(encoding="utf-8"))
    records={row["id"]:row for row in canon["records"]}
    if len(legacy)!=9 or [x["realm"] for x in legacy]!=list(REALMS):
        raise ValueError("historical expedition realm indices have drifted")
    for index,record in enumerate(legacy):
        key=record["realm"]
        codex_row=records["journey.expedition."+key]
        rule=codex_row["rule_contract"]
        if rule["source_index"]!=index or rule["realm"]!="realm."+key:
            raise ValueError("Codex quest identity/ordinal drift for "+key)
        for field in FIELDS:
            if rule[field]!=record[field]:
                raise ValueError("historical release and canonical Codex disagree on "+key+"."+field)
        if codex_row["summary"]!=record["brief"]:
            raise ValueError("Codex quest briefing drift for "+key)
        if rule["world_origin"]!={"x":record["x"],"y":record["y"]}:
            raise ValueError("Codex physical quest origin drift for "+key)
    if '#include "World/UnmadeCodexExpeditionGenerated.inc"' not in WRAPPER.read_text(encoding="utf-8"):
        raise ValueError("Expedition registry is no longer wired to the Codex")
    return generate_expeditions(canon)

if __name__=="__main__":
    p=argparse.ArgumentParser()
    p.add_argument("--check",action="store_true")
    a=p.parse_args()
    output=build()
    if a.check:
        if not TARGET.is_file() or TARGET.read_text(encoding="utf-8")!=output:
            raise SystemExit("FAIL: Codex-generated expedition registry drift")
    else:
        TARGET.write_text(output,encoding="utf-8")
    print("PASS: nine Codex-owned expeditions match historic realm, boss, witness, clue and outcome data")
