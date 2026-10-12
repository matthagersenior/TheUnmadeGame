"""Build the canonical repository Markdown manuscript without external books."""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
def build(root: Path) -> tuple[str, dict]:
    config = json.loads((root / "docs/lore/bible_sources.json").read_text(encoding="utf-8"))
    if config["schema"] != 1 or not config["sources"]:
        raise ValueError("Unsupported or empty manuscript manifest")
    chunks = ["# " + config["title"], config["scope"]]
    records = []
    seen = set()
    for name in config["sources"]:
        source = (root / name).resolve()
        if not source.is_relative_to((root / "docs/lore").resolve()) or source.suffix != ".md" or name in seen:
            raise ValueError("Invalid or duplicate canonical source: " + name)
        seen.add(name)
        data = source.read_bytes()
        body = data.decode("utf-8").strip()
        if not body:
            raise ValueError("Empty canonical source: " + name)
        chunks.extend(["---", "<!-- Source: " + name + " -->", body])
        records.append({"path": name, "sha256": hashlib.sha256(data).hexdigest()})
    text = "\n\n".join(chunks) + "\n"
    return text, {"schema": 1, "sources": records, "manuscript_sha256": hashlib.sha256(text.encode()).hexdigest()}
def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="validate inputs and deterministic reconstruction without writing")
    parser.add_argument("--out-dir", type=Path, default=ROOT / "PublicationBuild")
    args = parser.parse_args()
    text, manifest = build(ROOT)
    if args.check:
        if (text, manifest) != build(ROOT):
            raise ValueError("Non-deterministic manuscript build")
        print(f"PASS: {len(manifest['sources'])} canonical sources; manuscript {manifest['manuscript_sha256']}")
    else:
        args.out_dir.mkdir(parents=True, exist_ok=True)
        (args.out_dir / "THE_UNMADE_CANON.md").write_text(text, encoding="utf-8")
        (args.out_dir / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
        print("Built canonical manuscript and input/output hashes:", args.out_dir)
if __name__ == "__main__":
    main()
