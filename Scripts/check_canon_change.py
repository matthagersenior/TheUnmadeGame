#!/usr/bin/env python3
"""Block unregistered new game/lore assets and stale codex chapters in PRs.

This is a *change impact* gate complementing validate_canon_encyclopedia.py,
not a standalone mechanical or Unreal-runtime validation.
"""
from __future__ import annotations
import argparse
import json
from pathlib import Path, PurePosixPath
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = "docs/canon/SOURCE_COVERAGE.json"
INDEX = "docs/canon/encyclopedia/INDEX.json"
CHAPTER_DIR = "docs/canon/encyclopedia/"
ALLOWED_CLASSES = {"canon", "implementation", "noncanonical_infrastructure"}

# Authoritative registries. Any edit to the contents of one of these must
# update the corresponding human-readable Codex chapter in the SAME PR.
AUTHORITIES = {
    "Authoring/ability_codex.json": {"05_ABILITIES_SKILLS_AND_RULES.md"},
    "docs/canon/dependency-ledger.json": {
        "03_BESTIARY_BOSSES_AND_GUARDIANS.md",
        "04_ARMORY_ITEMS_AND_CRAFTING.md",
        "06_QUESTS_STORY_AND_ENDINGS.md",
    },
    "Authoring/remembered_roads_v13.json": {"02_CHARACTERS_NPCS_AND_RELATIONSHIPS.md"},
    "Authoring/resident_return_encounters.json": {"02_CHARACTERS_NPCS_AND_RELATIONSHIPS.md"},
    "Authoring/world_content_pack.json": {
        "01_WORLDS_AND_PLACES.md", "02_CHARACTERS_NPCS_AND_RELATIONSHIPS.md"
    },
    "Authoring/lived_universe_atlas.json": {"01_WORLDS_AND_PLACES.md"},
    "Authoring/transmedia_storyworld_v1.json": {
        "01_WORLDS_AND_PLACES.md",
        "02_CHARACTERS_NPCS_AND_RELATIONSHIPS.md",
        "07_LORE_MYSTERIES_AND_CONTINUITY.md",
    },
    "Authoring/main_quest_interaction_contracts.json": {
        "06_QUESTS_STORY_AND_ENDINGS.md"
    },
    "Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h": {
        "03_BESTIARY_BOSSES_AND_GUARDIANS.md"
    },
    "Source/TheUnmadeGame/Public/World/UnmadeFactionChronicleRules.h": {
        "08_FACTIONS_SOCIAL_SYSTEMS_AND_ECONOMIES.md"
    },
    "Source/TheUnmadeGame/Public/World/UnmadeLivingEconomyRules.h": {
        "08_FACTIONS_SOCIAL_SYSTEMS_AND_ECONOMIES.md"
    },
}

# New C++ source, new real content assets, original authoring masters, and new
# authored literary manuscripts must be registered. Exclude generated outputs:
# regenerating data should not demand an independent canonical feature ID.
def requires_registration(path: str) -> bool:
    if path.startswith("Source/TheUnmadeGame/") and path.endswith((".h", ".cpp")):
        return not path.endswith("/UnmadeCodexGenerated.h")
    if path.startswith("Authoring/") and not path.startswith("Authoring/generated/"):
        return path.endswith((".json", ".md", ".csv"))
    if path.startswith("Content/"):
        return path.endswith((".uasset", ".umap", ".wav", ".mp3", ".png", ".fbx", ".glb", ".json"))
    if path.startswith("docs/lore/"):
        return path.endswith((".md", ".json"))
    return False


def valid_path(path: object) -> bool:
    if not isinstance(path, str) or not path or "\\" in path:
        return False
    p = PurePosixPath(path)
    return (not p.is_absolute() and ".." not in p.parts and
            path == p.as_posix() and not path.startswith("./"))


def validate_manifest(manifest: dict, index: dict, root: Path | None = None) -> list[str]:
    errors = []
    if manifest.get("schema") != 1:
        errors.append("SOURCE_COVERAGE.schema must be 1")
    records = manifest.get("entries")
    if not isinstance(records, list):
        return errors + ["SOURCE_COVERAGE.entries must be a list"]
    keys = {item["key"] for item in index.get("entries", [])}
    seen = set()
    for row in records:
        if not isinstance(row, dict):
            errors.append("SOURCE_COVERAGE entry must be an object")
            continue
        path = row.get("path")
        if not valid_path(path) or not requires_registration(path):
            errors.append(f"invalid covered source path: {path!r}")
            continue
        if path in seen:
            errors.append(f"duplicate covered source path: {path}")
        seen.add(path)
        klass = row.get("classification")
        if klass not in ALLOWED_CLASSES:
            errors.append(f"{path}: invalid canon classification")
            continue
        if root is not None and not (root / path).is_file():
            errors.append(f"{path}: registration points to nonexistent file")
        linked = row.get("codex_keys")
        if not isinstance(linked, list) or len(set(linked)) != len(linked):
            errors.append(f"{path}: codex_keys must be a unique ID array")
            continue
        if klass != "noncanonical_infrastructure" and not linked:
            errors.append(f"{path}: gameplay/story additions require canonical keys")
        if klass == "noncanonical_infrastructure":
            rationale = row.get("rationale")
            if linked or not isinstance(rationale, str) or len(rationale.strip()) < 30:
                errors.append(f"{path}: a noncanonical classification needs a meaningful reason and no canon keys")
        for key in linked:
            if key not in keys:
                errors.append(f"{path}: unknown codex key {key!r}")
    return errors


def check_changes(changed: list[tuple[str, str]], manifest: dict,
                  index: dict) -> list[str]:
    errors = validate_manifest(manifest, index)
    records = {entry["path"]: entry for entry in manifest.get("entries", [])
               if isinstance(entry, dict) and valid_path(entry.get("path"))}
    paths = {path for _, path in changed}
    for status, path in changed:
        if status in ("A", "R") and requires_registration(path):
            if path not in records:
                errors.append(
                    f"UNREGISTERED GAME ADDITION {path}: add an entry in {MANIFEST} "
                    "with canonical codex_keys or an explicit infrastructure rationale"
                )
        if path in AUTHORITIES and status in ("A", "M", "R"):
            chapters = AUTHORITIES[path]
            if not any(CHAPTER_DIR + page in paths for page in chapters):
                errors.append(
                    f"STALE CANON {path}: update at least one affected "
                    f"encyclopedia volume in the same PR ({', '.join(sorted(chapters))})"
                )
    return errors


def changed_files(base: str) -> list[tuple[str, str]]:
    if not base or base.startswith("-"):
        raise ValueError("provide an existing trusted Git base ref")
    result = subprocess.run(
        ["git", "diff", "--name-status", "-M", base + "...HEAD"],
        cwd=ROOT, capture_output=True, text=True, check=True,
    )
    changed = []
    for line in result.stdout.splitlines():
        fields = line.split("\t")
        status = fields[0][0]
        if status == "D":
            continue
        if status == "R" and len(fields) == 3:
            changed.append(("R", fields[2]))
        elif status in ("A", "M", "C") and len(fields) == 2:
            changed.append(("A" if status == "C" else status, fields[1]))
    return changed


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--base", default=None,
                        help="PR base commit or origin/branch for three-dot diff")
    args = parser.parse_args()
    try:
        manifest = json.loads((ROOT / MANIFEST).read_text(encoding="utf-8"))
        index = json.loads((ROOT / INDEX).read_text(encoding="utf-8"))
        if args.base:
            changed = changed_files(args.base)
            errors = check_changes(changed, manifest, index)
        else:
            changed = []
            errors = validate_manifest(manifest, index, ROOT)
    except (OSError, ValueError, KeyError, TypeError, subprocess.CalledProcessError) as e:
        print(f"CANON CHANGE ERROR: {e}", file=sys.stderr)
        return 1
    if errors:
        for error in errors:
            print(f"CANON CHANGE REJECTED: {error}", file=sys.stderr)
        return 1
    print(f"PASS: {len(manifest['entries'])} registered source additions, "
          f"{len(changed)} inspected changed files, no undocumented canon expansion")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
