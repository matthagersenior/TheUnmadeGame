#!/usr/bin/env python3
"""Source-linked, reproducible Unreal production work orders; never certifies playability.

All output is derived from committed canon and authoring records. This tool
does not create assets, invoke Unreal, award rewards, or change save state.
"""
from __future__ import annotations

import argparse
import csv
import io
import json
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCES = {
    "ledger": "docs/canon/dependency-ledger.json",
    "campaign": "Authoring/main_quest_interaction_contracts.json",
    "atlas": "Authoring/lived_universe_atlas.json",
    "cities": "Authoring/city_assembly_kits_v1.json",
    "residents": "Authoring/resident_return_encounters.json",
    "codex": "Authoring/ability_codex.json",
}
REALMS = (
    "ThreefoldReach", "WidowedRain", "HearthBeneath", "TidalLedger",
    "SkyBelow", "CinderSpine", "HundredUnlived", "OrchardOfKings",
    "FirstAbsence",
)
FIELDS = ("type", "realm", "id", "name", "source", "production_action",
          "engine_acceptance", "status")
TASKS = {
    "REALM": ("Build an authored navigable realm with distinct weather, ecology and life.",
              "Actual spawn, movement, alternate routes, hazard cues and saved return tested."),
    "CITY": ("Replace noncolliding reference guides with usable architecture, doors, interiors, NPC navigation and local sound.",
             "Walk every intended entrance and public path on keyboard/controller; measure performance."),
    "MAIN_STEP": ("Stage a physical actor, local dialogue, evidence trigger and true saved-state interaction.",
                  "Trigger, failure, retry and cold-save reload proven in PIE for both resolutions."),
    "CLUE": ("Build an examinable world object tied to firsthand evidence, journal and accessibility text.",
             "Discover only on valid physical interaction; preserve provenance after reload."),
    "SIDE_STORY": ("Implement optional branch, safe retry, protected choice and isolated save-backed aftermath.",
                   "Both care and truth outcomes tested without forcing the main campaign."),
    "RESIDENT": ("Provide distinct visible resident, schedule, interaction and situated memory.",
                 "Sightline/rumor distinction, consent, return visit and save/load manually tested."),
    "QUEST": ("Bind canonical quest prerequisites, physical interactions, rewards and unique consequence.",
              "Both eligible resolutions and failed-write recovery work without invented progress."),
    "BOSS": ("Author readable silhouette, tells, hit geometry, distinct phases, counterplay and valid mercy/defeat.",
             "Arena entry, telegraphs, optional nonviolence, reward and reload verified."),
    "ITEM": ("Provide inventory/icon/mesh or material presentation and enforce the canonical acquisition and use rules.",
             "No duplicate grants; equipment/consumption and rollback proven in the live UI."),
    "SKILL": ("Expose earned learning path, cost, limitation, tell and physical gameplay feedback.",
              "No automatic learning from mere sight; acquisition, use and reload proven."),
    "CODEX": ("Reconcile documented prerequisites with a discoverable learning source or mark as non-playable.",
              "Definition/proposal never mistaken for an unlocked power; runtime requirements audited."),
}

def read_sources(root: Path) -> dict:
    data = {}
    for key, relative in SOURCES.items():
        data[key] = json.loads((root / relative).read_text(encoding="utf-8"))
    return data

def verify(data: dict) -> None:
    ledger, campaign, atlas = data["ledger"], data["campaign"], data["atlas"]
    expected = list(REALMS)
    if [x["id"] for x in campaign["quests"]] != expected:
        raise ValueError("Nine main quests do not match canonical realm ordering")
    if [x["id"] for x in atlas["worlds"]] != expected:
        raise ValueError("Lived-universe atlas realm ordering changed")
    if len(data["cities"]["cities"]) != 11:
        raise ValueError("The eleven-city staging scope changed")
    if sorted(x["realm"] for x in data["cities"]["cities"]) != sorted(
            [expected[0]] * 3 + expected[1:]):
        raise ValueError("City-to-realm staging coverage does not match canon")
    if not data["residents"]["residents"] or not data["codex"]["records"]:
        raise ValueError("Resident or codex source is empty")
    for collection, key in (
        (ledger["quests"], "quests"), (ledger["bosses"], "bosses"),
        (ledger["items"], "items"), (ledger["skills"], "skills"),
        (data["residents"]["residents"], "residents"),
        (data["codex"]["records"], "codex"),
    ):
        ids = [x["id"] for x in collection]
        if len(set(ids)) != len(ids) or any(not str(x).strip() for x in ids):
            raise ValueError("Duplicate or empty stable ID in " + key)
    boss_ids = {x["id"] for x in ledger["bosses"]}
    for q in ledger["quests"]:
        if q.get("boss") and q["boss"] not in boss_ids:
            raise ValueError("Quest references missing boss: " + q["id"])
    for quest in campaign["quests"]:
        steps = quest["steps"]
        if len(steps) != 6 or len({s["id"] for s in steps}) != 6:
            raise ValueError("Main campaign requires six distinct steps: " + quest["id"])
        for step in steps:
            for field in ("id", "trigger", "interaction", "objective",
                          "failure", "recovery", "saved_as"):
                if not str(step.get(field, "")).strip():
                    raise ValueError("Unstaged main quest field: " + quest["id"] + "/" + field)
    if len({r["id"] for r in atlas["worlds"]}) != 9:
        raise ValueError("Nine unique realm dossiers required")
    for realm in atlas["worlds"]:
        clues = realm["mystery"]["evidence"]
        if len(clues) != 3 or len({c["id"] for c in clues}) != 3:
            raise ValueError("Three distinct physical clues required: " + realm["id"])
        if not realm["side"]["id"] or not realm["side"]["save"]:
            raise ValueError("Optional-story ownership missing: " + realm["id"])
    needed = {"quest:campaign:" + key for key in REALMS}
    known = {q["id"] for q in ledger["quests"]}
    if not needed.issubset(known):
        raise ValueError("Canonical campaign quests missing from dependency ledger")

def row(kind: str, realm: str, ident: str, name: str, source: str) -> dict:
    action, acceptance = TASKS[kind]
    return dict(type=kind, realm=realm, id=ident, name=str(name),
                source=source, production_action=action,
                engine_acceptance=acceptance, status="UNREAL_NOT_VERIFIED")

def make_orders(data: dict) -> list[dict]:
    orders = []
    add = orders.append
    atlas_source = SOURCES["atlas"]
    for realm in data["atlas"]["worlds"]:
        key = realm["id"]
        add(row("REALM", key, key, realm["title"], atlas_source))
        for clue in realm["mystery"]["evidence"]:
            add(row("CLUE", key, clue["id"], clue["site"], atlas_source))
        side = realm["side"]
        add(row("SIDE_STORY", key, side["id"], side["name"], atlas_source))
    for city in data["cities"]["cities"]:
        add(row("CITY", city["realm"], city["id"], city["name"], SOURCES["cities"]))
    for quest in data["campaign"]["quests"]:
        for step in quest["steps"]:
            add(row("MAIN_STEP", quest["id"], quest["chapter"] + "/" + step["id"],
                    step["objective"], SOURCES["campaign"]))
    for person in data["residents"]["residents"]:
        add(row("RESIDENT", person["realm"], person["id"], person["name"],
                SOURCES["residents"]))
    for kind, key in (("QUEST", "quests"), ("BOSS", "bosses"),
                      ("ITEM", "items"), ("SKILL", "skills")):
        for record in data["ledger"][key]:
            add(row(kind, str(record.get("realm") or "cross-realm"),
                    record["id"], record["name"], SOURCES["ledger"]))
    for record in data["codex"]["records"]:
        add(row("CODEX", str(record.get("realm") or "cross-realm"),
                record["id"], record["name"], SOURCES["codex"]))
    if len(orders) != len({(o["type"], o["id"]) for o in orders}):
        raise ValueError("Work-order identity collision")
    return orders

def line(value: object) -> str:
    """Single-line markdown text, not executable markup from untrusted authoring."""
    return str(value).replace("\r", " ").replace("\n", " ").replace("|", "/").strip()

def bullet(label: str, content: object) -> str:
    return "- **" + label + ":** " + line(content) + "\n"

def realm_dossier(world: dict, campaign: dict, data: dict) -> str:
    realm = world["id"]
    out = ["# " + line(world["title"]), "",
           "Production handoff only: no Unreal build or gameplay certification.", "",
           "## World identity", ""]
    for field in ("ordinary", "architecture", "custom", "taboo", "governance",
                  "labor", "trade", "ecology", "weather"):
        out.append(bullet(field.replace("_", " ").title(), world[field]).rstrip())
    out += ["", "## Sensory and accessibility", ""]
    for label, value in world["sensory"].items():
        out.append(bullet(label.title(), value).rstrip())
    out += ["", "## Firsthand investigation and physical clues", "",
            bullet("Question", world["mystery"]["question"]).rstrip(),
            bullet("Do not reveal early", world["mystery"]["do_not_reveal"]).rstrip()]
    for clue in world["mystery"]["evidence"]:
        out += ["", "### " + line(clue["id"]) + " — " + line(clue["site"]), "",
                bullet("Player action", clue["verb"]).rstrip(),
                bullet("Observed result", clue["discovery"]).rstrip()]
    out += ["", "## Main chapter: six physically staged beats", ""]
    for number, step in enumerate(campaign["steps"], 1):
        out += ["", "### " + str(number) + ". " + line(step["id"]), "",
                bullet("Real trigger", step["trigger"]).rstrip(),
                bullet("Interaction", step["interaction"]).rstrip(),
                bullet("Objective", step["objective"]).rstrip(),
                bullet("Speaker", step.get("speaker", "Authoritative source state")).rstrip(),
                bullet("Failure", step["failure"]).rstrip(),
                bullet("Recovery", step["recovery"]).rstrip(),
                bullet("Saved as", step["saved_as"]).rstrip()]
        if step.get("line"):
            out.append(bullet("Authored line", step["line"]).rstrip())
    out += ["", "## Saved outcomes", ""]
    for key, value in campaign["world_after_save"].items():
        out.append(bullet(key.title(), value).rstrip())
    side = world["side"]
    out += ["", "## Optional side story: " + line(side["name"]), "",
            bullet("Canonical ID", side["id"]).rstrip(),
            bullet("Unlock", side["unlock"]).rstrip(),
            bullet("Save authority", side["save"]).rstrip()]
    for n, step in enumerate(side["steps"], 1):
        out.append(str(n) + ". " + line(step))
    for key, value in side["branches"].items():
        out.append(bullet("Outcome " + key, value).rstrip())
    out += ["", "## Art, sound and environment source briefs", ""]
    for asset in world["asset"]:
        out.append("- " + line(asset))
    out += ["", "## Other registered regional work", "",
            "Every record below must be evaluated in the live game before it is marked playable.", ""]
    for key, label in (("quests", "Quests"), ("bosses", "Bosses"), ("skills", "Learned skills")):
        out += ["", "### " + label]
        matches = [r for r in data["ledger"][key] if r.get("realm") == realm]
        if not matches:
            out.append("- No additional realm-keyed records; inspect the global index.")
        for record in matches:
            out.append("- " + line(record["id"]) + " — " + line(record["name"]))
    out += ["", "## Required Unreal acceptance", "",
            "- Traverse the location physically with collision, NavMesh, signage and a safe fallback.",
            "- Play both mutually exclusive chapter outcomes in separate clean saves.",
            "- Witness only nearby facts; do not expose protected identities or omniscient NPC knowledge.",
            "- Validate objective UI, controller focus, subtitle and non-audio alternatives.",
            "- Reload after each key choice and force a failed save write; never award a ghost reward.",
            "- Return on a later day and after the final world decision; inspect changed physical states.",
            "- Record build SHA, screenshots, performance and a complete walkthrough receipt.", ""]
    return "\n".join(out)

def build(root: Path = ROOT) -> tuple[dict, dict[str, str]]:
    data = read_sources(root)
    verify(data)
    orders = make_orders(data)
    summary = dict(Counter(order["type"] for order in orders))
    lines = ["# First-PC production package", "",
             "Generated from canon; no game assets, Unreal compilation or playtests are certified.",
             "", "## Work-order counts", ""]
    for key in TASKS:
        lines.append("- " + key + ": " + str(summary.get(key, 0)))
    lines += ["", "## Operating rules", "",
              "1. Keep the existing native gameplay and save authority; visual actor guides cannot complete a quest.",
              "2. Begin with START_THE_UNMADE.cmd and inspect TestReports failures before importing art.",
              "3. Use realm dossiers for physical interactions, witnesses, failure recovery and branch continuity.",
              "4. Filter work-orders.csv by type and realm. A row remains UNREAL_NOT_VERIFIED until actual engine evidence exists.",
              "5. Preserve consent, costs, evidence provenance, OPEN mysteries, distinct NPC memory and old saves.",
              "6. Build and validate one full Threefold Reach loop before scaling the remaining realms.",
              "7. Produce two-save clean walkthroughs for every realm; then verify complete alternate endings.",
              "8. Do not claim release readiness until compile, PIE, build, controller, assets, long saves, performance and packaging pass.",
              "", "## Generated files", "",
              "- work-orders.csv: traceable object-level staging and acceptance work",
              "- canon-index.md: entire registered quest/boss/item/skill/ability catalog",
              "- realm-*.md: nine full local implementation dossiers",
              "", "## Original input sources", ""]
    lines.extend("- " + source for source in SOURCES.values())
    files = {"README.md": "\n".join(lines) + "\n"}
    buffer = io.StringIO(newline="")
    writer = csv.DictWriter(buffer, fieldnames=FIELDS, lineterminator="\n")
    writer.writeheader()
    writer.writerows(orders)
    files["work-orders.csv"] = buffer.getvalue()
    index = ["# Canonical implementation index", "",
             "These are authoring records. Neither registry membership nor source-rule status proves playable content.", ""]
    for kind, key in (("QUEST", "quests"), ("BOSS", "bosses"),
                      ("ITEM", "items"), ("SKILL", "skills")):
        records = data["ledger"][key]
        index += ["", "## " + kind + " (" + str(len(records)) + ")", ""]
        index.extend("- " + line(r["id"]) + " — " + line(r["name"]) +
                     " [source: " + line(r.get("source", SOURCES["ledger"])) + "]"
                     for r in records)
    index += ["", "## ABILITY CODEX (" + str(len(data["codex"]["records"])) + ")", ""]
    index.extend("- " + line(r["id"]) + " — " + line(r["name"]) +
                 " [definition status: " + line(r.get("implementation", "unknown")) + "]"
                 for r in data["codex"]["records"])
    files["canon-index.md"] = "\n".join(index) + "\n"
    for world, main in zip(data["atlas"]["worlds"], data["campaign"]["quests"]):
        files["realm-" + world["id"] + ".md"] = realm_dossier(world, main, data)
    return {"summary": summary, "total_work_orders": len(orders),
            "realm_count": 9, "source_files": list(SOURCES.values())}, files

def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="Validate authoring without writes")
    parser.add_argument("--out-dir", type=Path,
                        help="Write generated work orders and dossiers to this directory")
    args = parser.parse_args()
    try:
        receipt, files = build()
        if args.out_dir and not args.check:
            args.out_dir.mkdir(parents=True, exist_ok=True)
            for name, contents in files.items():
                (args.out_dir / name).write_text(contents, encoding="utf-8")
        print("PASS: " + str(receipt["total_work_orders"]) +
              " canonical source-linked production work orders in 9 realms")
        print("NOTE: no Unreal compilation, editor import or gameplay certification.")
        if args.out_dir and not args.check:
            print("Generated package: " + str(args.out_dir))
        return 0
    except (OSError, ValueError, KeyError, TypeError, json.JSONDecodeError) as exc:
        print("FAIL: " + str(exc))
        return 1

if __name__ == "__main__":
    raise SystemExit(main())
