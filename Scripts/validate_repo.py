#!/usr/bin/env python3
"""Engine-independent repository checks. Does NOT compile or run Unreal."""
from __future__ import annotations

import json
import sys
from pathlib import Path

REQUIRED_FILES = (
    ".gitignore", ".gitattributes", "README.md", "TheUnmadeGame.uproject",
    "Config/DefaultEngine.ini", "Config/DefaultGame.ini", "Config/DefaultInput.ini",
    "Source/TheUnmadeGame.Target.cs", "Source/TheUnmadeGameEditor.Target.cs",
    "Source/TheUnmadeGame/TheUnmadeGame.Build.cs",
    "Source/TheUnmadeGame/Public/TheUnmadeGame.h",
    "Source/TheUnmadeGame/Private/TheUnmadeGame.cpp",
    "Source/TheUnmadeGame/Public/Player/UnmadeCharacter.h",
    "Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp",
    "Source/TheUnmadeGame/Public/Game/UnmadeGameMode.h",
    "Source/TheUnmadeGame/Private/Game/UnmadeGameMode.cpp",
    "Source/TheUnmadeGame/Private/Tests/BootstrapAutomationTest.cpp",
    "Scripts/run_ue_tests.ps1",
    "docs/specs/2026-10-08-the-unmade-foundations-design-v0.3.md",
    "docs/plans/2026-10-08-the-unmade-first-playable-slice.md",
)

def validate(root: Path) -> list[str]:
    issues = []
    for relative in REQUIRED_FILES:
        if not (root / relative).is_file():
            issues.append(f"missing required file: {relative}")

    descriptor = root / "TheUnmadeGame.uproject"
    if descriptor.is_file():
        try:
            data = json.loads(descriptor.read_text(encoding="utf-8"))
            modules = data.get("Modules", [])
            if not any(m.get("Name") == "TheUnmadeGame" and m.get("Type") == "Runtime" for m in modules):
                issues.append("uproject missing TheUnmadeGame runtime module")
            if not any(p.get("Name") == "EnhancedInput" and p.get("Enabled") for p in data.get("Plugins", [])):
                issues.append("uproject must enable EnhancedInput")
        except (json.JSONDecodeError, UnicodeError) as exc:
            issues.append(f"invalid uproject descriptor: {exc}")

    attributes = root / ".gitattributes"
    if attributes.is_file():
        text = attributes.read_text(encoding="utf-8")
        for extension in ("*.uasset", "*.umap"):
            if not any(line.startswith(extension + " ") and "filter=lfs" in line for line in text.splitlines()):
                issues.append(f"Git LFS missing for {extension}")

    ignore = root / ".gitignore"
    if ignore.is_file():
        lines = {line.strip() for line in ignore.read_text(encoding="utf-8").splitlines()}
        for generated in ("Binaries/", "Intermediate/", "Saved/", "DerivedDataCache/"):
            if generated not in lines:
                issues.append(f"generated directory not ignored: {generated}")

    engine = root / "Config/DefaultEngine.ini"
    if engine.is_file():
        text = engine.read_text(encoding="utf-8")
        if "GlobalDefaultGameMode=/Script/TheUnmadeGame.UnmadeGameMode" not in text:
            issues.append("default game mode not configured")

    return issues

def main() -> int:
    root = Path(__file__).resolve().parent.parent
    issues = validate(root)
    if issues:
        for issue in issues:
            print("FAIL:", issue, file=sys.stderr)
        return 1
    print(f"PASS: repository structure verified ({len(REQUIRED_FILES)} required files)")
    print("NOTE: Unreal compilation, asset cooking and runtime behavior were NOT tested.")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
