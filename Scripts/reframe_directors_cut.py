#!/usr/bin/env python3
"""Build safe, uncropped 56-shot Unmade animatics from the conversation director kit.

Requires Pillow for images and FFmpeg for MP4 exports. Source-only --check
uses just Python's standard library. Narration is never fabricated, and the
rejected synthetic voice is never selected without --allow-scratch-voice.
"""
from __future__ import annotations
import argparse
import json
import shutil
import subprocess
import tempfile
import zipfile
from contextlib import contextmanager
from pathlib import Path

W, H = 1280, 720
ART_BOX = (80, 92, 1200, 542)
TITLE_Y, SUBTITLE_Y, FOOTER_Y = 564, 625, 676
COLORS = {
    "THE IMPOSSIBLE PERSON": (175, 150, 225),
    "LIVING CIVILIZATIONS": (126, 195, 210),
    "THE PEOPLE WHO REMEMBER": (205, 169, 223),
    "PLAYER FREEDOM & CONSEQUENCE": (221, 182, 117),
    "QUESTS, RETURN & DISCOVERY": (163, 203, 178),
    "THE ANSWER THAT ATE ITS QUESTION": (232, 139, 144),
    "THE WORLDS AFTER VICTORY": (210, 200, 161),
}

def validate_scene_source(manifest: dict, root: Path) -> list[dict]:
    scenes = manifest["scenes"]
    if len(scenes) != 56:
        raise ValueError(f"Expected 56 scenes, found {len(scenes)}")
    if not (0 < ART_BOX[0] < ART_BOX[2] < W and
            0 < ART_BOX[1] < ART_BOX[3] < TITLE_Y < SUBTITLE_Y < FOOTER_Y < H):
        raise ValueError("Unsafe frame geometry")
    now = 0.0
    for index, scene in enumerate(scenes, 1):
        if scene["id"] != f"C{index:02d}":
            raise ValueError("Nonsequential shot ID")
        if scene["chapter"] not in COLORS:
            raise ValueError("Unknown cinematic chapter")
        if not scene.get("title") or not scene.get("subtitle"):
            raise ValueError("Missing visible title/subtitle")
        art = scene.get("art", "")
        if not art or art != Path(art).name:
            raise ValueError("Unsafe art reference")
        if not (root / "rebuild" / "art_atlas" / art).is_file():
            raise FileNotFoundError(art)
        if abs(float(scene["start"]) - now) > .02 or float(scene["duration"]) <= 0:
            raise ValueError("Scene duration gap/overlap: " + scene["id"])
        now = round(now + float(scene["duration"]), 2)
        if abs(float(scene["end"]) - now) > .02:
            raise ValueError("Scene end mismatch")
    if abs(now - float(manifest["total_seconds"])) > .05:
        raise ValueError("Film duration mismatch")
    return scenes

def fit_font(draw, text, start, width, bold=False):
    from PIL import ImageFont
    regular = Path("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")
    heavy = Path("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")
    if not regular.is_file():
        regular = Path("/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf")
        heavy = Path("/usr/share/fonts/truetype/liberation2/LiberationSans-Bold.ttf")
    for size in range(start, 11, -1):
        font = ImageFont.truetype(str(heavy if bold else regular), size)
        if draw.textbbox((0, 0), text, font=font)[2] <= width:
            return font
    raise ValueError("Text outside title safe area: " + text)

def compose_frame(scene, ordinal, total, atlas):
    """Actual artwork is contained in the image window, never crop-filled."""
    from PIL import Image, ImageDraw, ImageFilter, ImageEnhance, ImageOps
    canvas = Image.new("RGB", (W, H), (8, 13, 25))
    draw = ImageDraw.Draw(canvas)
    for y in range(0, H, 2):
        v = int(10 + 7 * y / H)
        draw.rectangle((0, y, W, y + 1), fill=(v, v + 4, v + 13))
    tint = COLORS[scene["chapter"]]
    x0, y0, x1, y1 = ART_BOX
    width, height = x1-x0, y1-y0
    art = Image.open(atlas / scene["art"]).convert("RGB")
    matte = ImageOps.fit(art, (width, height), centering=(.5, .5))
    matte = ImageEnhance.Brightness(matte).enhance(.21).filter(ImageFilter.GaussianBlur(28))
    canvas.paste(matte, (x0, y0))
    ratio = min(width / art.width, height / art.height)
    fw, fh = round(art.width * ratio), round(art.height * ratio)
    whole_art = art.resize((fw, fh), Image.Resampling.LANCZOS)
    canvas.paste(whole_art, (x0 + (width-fw)//2, y0 + (height-fh)//2))
    draw = ImageDraw.Draw(canvas)
    draw.rounded_rectangle((x0-2, y0-2, x1+2, y1+2), radius=6, outline=tint, width=2)
    chapter = scene["chapter"]
    header = f'CHAPTER {list(COLORS).index(chapter)+1:02d}   /   {chapter.replace("&", "AND")}'
    draw.text((80, 44), header, font=fit_font(draw, header, 19, 930, bold=True), fill=tint)
    shot = f"{ordinal:02d} / {total:02d}"
    draw.text((1070, 44), shot, font=fit_font(draw, shot, 20, 125, bold=True), fill=(226,225,222))
    title = scene["title"]
    draw.text((81, TITLE_Y), title, font=fit_font(draw, title, 38, 1108, bold=True), fill=(249,246,239))
    subtitle = scene["subtitle"]
    draw.text((82, SUBTITLE_Y), subtitle, font=fit_font(draw, subtitle, 24, 1108), fill=tint)
    draw.line((80, 666, 1200, 666), fill=(70, 80, 99), width=1)
    footer = "CONCEPT ART  •  DIRECTOR STORYBOARD  •  NOT UNREAL GAMEPLAY"
    draw.text((82, FOOTER_Y), footer, font=fit_font(draw, footer, 13, 1100), fill=(178,187,197))
    draw.rectangle((80, 701, 80 + int(1120*ordinal/total), 704), fill=tint)
    return canvas

@contextmanager
def unpack_kit(path: Path):
    if path.is_dir():
        yield path
    elif path.is_file() and path.suffix.lower() == ".zip":
        with tempfile.TemporaryDirectory(prefix="unmade_director_") as temp:
            with zipfile.ZipFile(path) as archive:
                for item in archive.infolist():
                    if item.filename.startswith("/") or ".." in Path(item.filename).parts:
                        raise ValueError("Unsafe ZIP path")
                archive.extractall(temp)
            yield Path(temp)
    else:
        raise FileNotFoundError(str(path))

def build(kit: Path, output: Path, audio: Path|None, check=False, frames_only=False, use_scratch=False):
    with unpack_kit(kit) as root:
        manifest = json.loads((root / "THE_UNMADE_Full_Universe_Directors_Cut_manifest.json").read_text(encoding="utf-8"))
        scenes = validate_scene_source(manifest, root)
        if check:
            print(f"PASS: {len(scenes)} artwork refs, safe layout and {manifest['total_seconds']}s timeline")
            return
        if audio is None:
            if not use_scratch and not frames_only:
                raise ValueError("Supply --audio with approved narration or music; old robotic narration requires --allow-scratch-voice.")
            audio = root / "THE_UNMADE_Full_Universe_Directors_Cut_Audio.mp4"
        if not frames_only and not audio.is_file():
            raise FileNotFoundError(str(audio))
        output.parent.mkdir(parents=True, exist_ok=True)
        frames = output.parent / (output.stem + "_frames")
        frames.mkdir(exist_ok=True)
        for index, scene in enumerate(scenes, 1):
            compose_frame(scene, index, len(scenes), root / "rebuild" / "art_atlas").save(frames / (scene["id"] + ".png"), optimize=True)
        playlist = output.parent / (output.stem + "_concat.txt")
        with playlist.open("w") as file:
            for scene in scenes:
                file.write(f"file '{(frames / (scene['id'] + '.png')).as_posix()}'\n")
                file.write(f"duration {float(scene['duration']):.2f}\n")
            file.write(f"file '{(frames / (scenes[-1]['id'] + '.png')).as_posix()}'\n")
        if frames_only:
            print("PASS: 56 uncropped 16:9 frames at", frames)
            return
        if not shutil.which("ffmpeg"):
            raise EnvironmentError("FFmpeg required")
        subprocess.run([
            "ffmpeg", "-hide_banner", "-loglevel", "error", "-y",
            "-f", "concat", "-safe", "0", "-i", str(playlist),
            "-i", str(audio), "-map", "0:v:0", "-map", "1:a:0",
            "-vf", "fps=12,format=yuv420p", "-c:v", "libx264",
            "-preset", "veryfast", "-crf", "23",
            "-c:a", "aac", "-ar", "48000", "-ac", "2", "-b:a", "192k",
            "-movflags", "+faststart", "-t", str(manifest["total_seconds"]), str(output)
        ], check=True)
        print("PASS: render complete", output)

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--kit", required=True, type=Path, help="Extracted 56-shot kit folder or ZIP")
    parser.add_argument("--output", type=Path, default=Path("THE_UNMADE_Reframed.mp4"))
    parser.add_argument("--audio", type=Path, help="Approved narration and score, MP4/WAV/FLAC")
    parser.add_argument("--check", action="store_true")
    parser.add_argument("--frames-only", action="store_true")
    parser.add_argument("--allow-scratch-voice", action="store_true")
    args = parser.parse_args()
    build(args.kit, args.output, args.audio, args.check, args.frames_only, args.allow_scratch_voice)

if __name__ == "__main__":
    main()
