# Cinematic and Nine-Realm Production — QA / Acceptance

### Source-level requirements
- [ ] All nine `Realm` ids in playbook match current atlas enum order; nine distinct signature dilemmas, three action verbs, at least three local residents and four Unreal asset production IDs per realm
- [ ] Sixteen shot IDs are contiguous `S01` through `S16`; exact start, end and duration arithmetic produces 149.5 seconds with no overlap/gaps
- [ ] Each storyboard scene has actual narration text, direction, art-source ID, visual disclaimer and explicit spoiler policy; the public narration does not disclose the false victory
- [ ] Both generated CSV files regenerate byte-identically from checked-in JSON; `python Scripts/build_cinematic_handoff.py --check` succeeds on PR and `main`
- [ ] First-PC PowerShell script runs the cinematic preflight and refuses stale/missing exports before the Unreal compile

### Cinematic deliverable requirements
- [ ] Full H.264 960×540 24fps, playable audio and valid 149.5-second duration; no corrupt frames, black tail or misaligned scene transitions
- [ ] Every shot has its correct subtitle and duration; complete recording script and SRT are included in the artifact ZIP
- [ ] At least four widely spaced actual frames have been visually inspected; text contrast and readability remain safe and no frame falsely claims gameplay
- [ ] The synthetic voice is labeled scratch/temporary and the procedural audio soundtrack is distinctly labeled a development cue
- [ ] The art provenance and retained original assets are credited; concept stills are not repackaged as engine screenshots

### Windows Unreal handoff requirements (NOT YET VERIFIED)
- [ ] Editor builds successfully with the new two `FTableRowBase` structs and loads both generated `UDataTable` assets; row count 16+9
- [ ] Sequence S01 through S16 exists with distinct camera framing, cut timing and safe skip/pause controller controls
- [ ] Eighty percent of the final world-camera footage comes from *genuine authored world assets*, not upscaled concept thumbnails (production target, not current state)
- [ ] Nine region-specific world verbs and two choices have real collision, level state transitions and reloaded consequences
- [ ] All nine hazards telegraph before dealing damage; no forced fake peril or unearned boss gate
- [ ] NPC first/rumored/public records reflect only actual observed local evidence, not unearned global omniscience
- [ ] Both actual main-campaign paths work with real attunement and no compulsory side-quest checklist
- [ ] Nhal-Vey's false victory and alternate world's next chapter are tested with actual cutscenes, ability windups, objective UI and save persistence
- [ ] The delivered MP4 is replaced in marketing with actual gameplay captures *only after actual development*
