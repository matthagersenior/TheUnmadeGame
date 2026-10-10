"""Enforce lore-source co-updates on every gameplay source commit.

This is intentionally *not* proof the illustrated PDF was regenerated.
The downloadable visual package remains a separately verified dated release.
"""
from __future__ import annotations
import subprocess
import sys

CANON="docs/lore/living-world-continuity.md"
LEDGER="docs/lore/CONTINUITY_LOG.md"

def check_changes(changed: list[str]) -> list[str]:
    gameplay=sorted(p for p in changed
                    if p.startswith("Source/TheUnmadeGame/") and
                    p.endswith((".h",".cpp",".uproject",".uasset",".umap")))
    if not gameplay:
        return []
    missing=[p for p in (CANON,LEDGER) if p not in changed]
    return missing

def main() -> int:
    parent=subprocess.run(
        ["git","rev-parse","--verify","HEAD^"],
        stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL
    )
    if parent.returncode:
        print("Lore sync cannot inspect parent; first commit or shallow checkout.")
        return 1
    diff=subprocess.run(
        ["git","diff","--name-only","HEAD^","HEAD"],
        capture_output=True,text=True,check=True
    )
    changed=diff.stdout.splitlines()
    missing=check_changes(changed)
    if missing:
        print("Gameplay-source commit missing lore updates:",", ".join(missing))
        print("Update both canonical lore and continuity ledger in the SAME commit.")
        return 1
    print("Lore source synchronization contract passed.")
    return 0

if __name__=="__main__":
    sys.exit(main())
