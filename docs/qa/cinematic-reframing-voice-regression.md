# The Unmade cinematic: framing and voice regression contract (2026-10-10)

## User-reported incident

The director animatic's source renderer arranged a 662 px headline panel over an art plate beginning at x=495. Text appeared across the art; the art was fit by cropping; baked-in source compendium labels appeared beneath new overlay copy. An on-TV screenshot of C04 "ONE SOUND IS MISSING" shows the resulting overlap. This **exists in the encoded media**, not only in a television overscan setting.

The file \`THE_UNMADE_Directors_Cut_No_Robotic_Voice.mp4\` is **intentionally without spoken narration**. It contains an audible soundtrack but does not contain an approved voiceover. The earlier \`THE_UNMADE_Full_Universe_Directors_Cut_Audio.mp4\` contains stereo scratch synthetic narration, which the user previously rejected as robotic. Audio stream presence must never be reported as successful narration.

## Repeatable no-crop build

The full kit and MP4 binaries live in the user conversation/library, **not the Git repo**. Use the ZIP under this name:

\`THE_UNMADE_Full_Universe_Audio_Production_Kit.zip\`

On a PC with Pillow, FFmpeg, Python:

\`\`\`bash
python Scripts/reframe_directors_cut.py --kit path/to/THE_UNMADE_Full_Universe_Audio_Production_Kit.zip --check
python Scripts/reframe_directors_cut.py --kit path/to/THE_UNMADE_Full_Universe_Audio_Production_Kit.zip --audio path/to/approved_soundtrack_or_narration.mp4 --output THE_UNMADE_Reframed.mp4
\`\`\`

Use \`--allow-scratch-voice\` **only** for a comparison export with the rejected robotic speech. A missing audio source fails instead of quietly choosing that voice. \`--frames-only\` is useful for review of all 56 still compositions before encoding.

## Acceptance requirements

- All 56 original images use *contain*, preserving edges and subjects. Decorative blurred matte may crop, but never the actual foreground artwork.
- Artwork stays within the coordinates (80,92)–(1200,542) on a 1280×720 frame. Titles are at y564 and subtitles at y625; no text panel overlaps the image.
- Header, counters, footer and subtitles remain within television-safe margins. No original body paragraph is superimposed across the illustration. Narration and external SRT contain the scene body text.
- Exactly 56 source scene references exist; timing cannot drift; missing images and traversal references fail during preflight.
- The delivered version must contain audible speech **if it is labeled narrated**, and voice quality must pass a separate **75-second narrator audition** before applying it to the full 12m15s cut.
- Output metadata must confirm H.264/1280×720/16:9, AAC/stereo/48 kHz, correct total duration, and actual listening review at the beginning, middle and end. A measured audio codec/mean level does not prove intelligibility.
- On-screen concept artwork is still a storyboard. Many existing source images are low resolution or have text baked into their imagery. This compositor preserves the complete source but **cannot turn those references into original cinematic plates, Unreal Sequencer shots, animated actors or human voice acting**. Those remain outstanding production work.

CI covers source references, timeline, geometry and explicit audio policy without binary media dependencies. The release producer remains responsible for the full-output visual/audio review and real narrator approval.
