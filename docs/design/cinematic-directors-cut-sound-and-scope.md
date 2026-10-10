# The Unmade — Full-Universe Director Cinematic, Audio Repair and Scope Contract

**10 October 2026 · Director edition v2 · 56 scenes / 12:15.03 / seven chapters**

## Incident and verified diagnosis

The Volume IX public 16-shot animatic lasted 2:29.5 and could not represent a nine-realm RPG's depth. The older production kit supplied two MP4s: `THE_UNMADE_Cinematic_World_Explainer_TempVO.mp4` (an AAC mono track with synthetic narration, mean measured about −22.0 dBFS) and `THE_UNMADE_Cinematic_World_Explainer_Captions_Only.mp4` (music only, mean about −25.0 dBFS). Both have streams: they are not file-level bitstream-silent, but the low-level music-only cut and incompatible expectations can reasonably produce the reported feeling of no audible film. Do not try to solve it by adding an empty audio stream or merely renaming the existing MP4.

A newly rendered **internal** full-universe reference animatic has 56 scenes and an audible stereo AAC soundtrack for its entire runtime. Its 48 kHz two-channel soundtrack was inspected using `ffprobe`; decoded mean volume was approximately −14.9 dBFS, peak −0.4 dBFS. The full output video lasts 735.00 seconds and audio 735.03 seconds. Its soundtrack is **temporary offline synthetic narration, scratch score and atmospheric cues**, not recorded character actors. It is still a series of concept/storyboard compositions, not true UE cinematography. The video and source audio/subtitles/complete narrated JSON have been delivered in a downloadable conversation production kit, not committed as GitHub binaries.

## The version 2 source index

`Authoring/cinematic_directors_cut_v2_index.json` freezes chapter boundaries, exact 56 stable shot IDs, scene titles, timing and scope requirements. It does not replace the 16-shot spoiler-light public `Authoring/cinematic_explainer_v1.json`; preserve both as separate editorial products. The full narration, footage, source art and mix stems live in the director kit. `Scripts/validate_directors_cut.py` checks exact scope/timeline from JSON in normal CI; when passed real media and SRT it also rejects short/truncated audio, wrong sample rate/channels, overly quiet mixes and missing subtitle cues. The source index alone does not prove a video exists.

## Seven narrative movements

1. **00:00–01:04 — The Impossible Person.** Immersive third-person identity without a chosen-one bloodline, a mysterious shared origin, ordinary working people, the impossible bell and uncomfortable choices between public truth and protected lives.
2. **01:04–04:49 — Living Civilizations.** All nine original realms, two authored scenes per realm: named settlements and daily labor, an environmental contradiction, a small humane civic action, observable stakes and separate public endings. This is not a slide listing nine names without character or physical verbs.
3. **04:49–05:46 — People Who Remember.** The 82 resident identities, firsthand/rumor provenance, repeat visits, their own private lives, languages and discovered cultural rules. Never imply all 82 are professionally performed or currently walkable actors.
4. **05:46–08:16 — Freedom and Consequence.** Earned travel, physical world changes, Glimpse, Fold and safeguarded Rewrite, all ten signature disciplines across two technique chapters, fair combat, optional guardian mercy and finite item/crafting systems. Do not reduce reality manipulation to montage magic without a cost.
5. **08:16–09:33 — Quests and Return.** The nonlinear Unanswered Road, real post-story physical changes, optional Echo chapters, paired-rite observatories, clues connecting cultures, learnable difficult encounters and safe recovery from mistakes. No content-completion gate for the final fight.
6. **09:33–10:39 — Nhal-Vey, the Answer That Ate Its Question.** A SPOILER-INCLUSIVE progression from first fair boss phase through false victory and unmasked confrontation to deliberate committed decision. This does NOT belong in spoiler-light marketing.
7. **10:39–12:15 — The Worlds After Victory.** Held Morning, Many Mornings, physically returning to changed regions, optional boss echo, residents living after the antagonist and an explicit disclaimer distinguishing this source animatic from finished Unreal game scenes.

## Minimum sound production contract

Audio is not an optional decoration. Every film export requires a **real spoken narration stem** when narration is advertised; separate music and effects stems; dialogue-first ducking; stereo 48 kHz AAC in the MP4; smooth episode transitions; non-clipping peaks; a measured average that remains audible on a phone speaker; and complete 56-cue external SRT in stable shot order. Audio must run to the same end as video, not stop at the nine-minute mark because an encoder was interrupted. The distributor checks with:

```bash
python Scripts/validate_directors_cut.py
python Scripts/validate_directors_cut.py --media path/to/directors_cut.mp4 --subtitles path/to/directors_cut.srt
```

Media validation uses ffprobe and ffmpeg with the actual binary available. The CI source check never claims real video validation without a rendered MP4.

## Final cinematic production requires actual content

Before this becomes immersive cinematic work, every shot needs production-ready visual sources—not low-resolution slides. The required upgrade is a three-dimensional environment plate, scene-ready original material/lighting design, actor rig and blocking, animated camera transforms, character performance, emotional voice takes with punctuation-specific timing, new scene-based composed music and ambient SFX, captioned English and localization keys, shot-safe-area and no-flash reviews, script-to-actual-source quest state verification, and strict skip/resume/save semantics. The 56-slide animatic is a **complete topic outline**, not certification that those Unreal requirements are already done.

**Public spoiler policy:** retain the v1 spoiler-light cut as a separate branch of the edit. The director edition reveals both final forms and the two postgame worlds. Never attach that cut as an unlabeled promotional trailer.

**Scope gap still open:** natural professional VO, realistic camera motion, animation, actors, every scene's unique location, scores and true UE Sequencer need further authoring and testing. The goal is to produce these materials as far as legitimately possible before the first-PC handoff, without calling static images finished animation.
