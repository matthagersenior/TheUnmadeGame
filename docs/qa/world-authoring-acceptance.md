# Nine-Realm Authoring Pack — Acceptance Checklist

On Linux / GitHub CI:
- `python Scripts/build_world_content.py --check` matches all committed generated outputs byte-for-byte.
- `python -m unittest discover -s Scripts/tests -v` rejects duplicate stable IDs, invalid masters, missing dialogue, unknown rites and drift.
- `Tests/authoring/outer_people_test.cpp` checks 18 stable distinct identities, their local Echo witnesses, and distinct day/night dialogue.
- All *nine* `Realm` enum members and *ten* `RiteId` identifiers agree with the authoring pack; no numeric save enums were renumbered.

On first Windows UE workstation:
- Compile the game Editor target and UHT row declarations. **Not yet attempted.**
- Import all 3 CSV tables into DataTable assets with the documented row structs. Check the 9/18/10 imported rows with a Blueprint/editor script. **Not yet attempted.**
- Verify each outer-realm NPC actor displays their authored name and day/night dialogue and their existing stable ID is unchanged after reload.
- Verify early settlements remain unaffected; a player can play all six first/after/third chapters without accidentally migrating these authoring DataTables into authoritative save files.
- Verify daylight, nighttime and rumor lines remain accessible and ethical; knowledge boundaries matter. Record voice performances and assign unique visual silhouettes afterward.
- Validate the actual Unreal asset pipeline: import CSV, reimport after one canonical JSON change, and prove existing DataTable row handles still resolve.
