"""Validate the 56-scene director animatic index and optionally verify delivered MP4 sound.

Offline index check is suitable for GitHub Actions. The optional --media check
requires ffmpeg/ffprobe and the actual rendered binary. A passing source index
does NOT prove an MP4 exists or Unreal animation was produced.
"""
from __future__ import annotations
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
INDEX = ROOT / "Authoring/cinematic_directors_cut_v2_index.json"
EXPECTED_CHAPTER_COUNTS = [5, 18, 4, 11, 6, 5, 7]
COVERAGE = (
    "THE THREEFOLD REACH",
    "SALTWAKE / THE WIDOWED RAIN",
    "CINDERHOLD / THE HEARTH BENEATH",
    "DREVLACH / THE SEA OF WRITTEN DEBTS",
    "ORRAVANE / THE UPSIDE-DOWN CHOIR",
    "VATHLESS / THE BONES OF YESTERDAYS",
    "EILLUN / THE HUNDRED UNLIVED",
    "THARNIV / THE ORCHARD OF KINGS",
    "AUVREN / THE PLACE BEFORE PLACE",
    "82 INDIVIDUAL RESIDENTS",
    "TEN SIGNATURE DISCIPLINES",
    "GLIMPSE",
    "FOLD",
    "REWRITE",
    "THE UNANSWERED ROAD",
    "THE FALSE VICTORY",
    "THE HELD MORNING",
    "THE MANY MORNINGS",
    "THE OPTIONAL ECHO",
)

def verify_index(data: dict) -> None:
    if data.get("version") != 2:
        raise ValueError("Director cut index must use version 2")
    if data.get("scene_count") != 56 or not 734.9 < data.get("duration_seconds", 0) < 735.2:
        raise ValueError("Wrong scene count/duration")
    chapters = data.get("chapters")
    if not isinstance(chapters, list) or [len(c.get("scenes", [])) for c in chapters] != EXPECTED_CHAPTER_COUNTS:
        raise ValueError("Seven complete chapters required")
    scenes = [s for ch in chapters for s in ch["scenes"]]
    titles = {x.get("title") for x in scenes}
    for entry in COVERAGE:
        if entry not in titles:
            raise ValueError(f"Missing core cinematic subject: {entry}")
    last = 0.
    for i, shot in enumerate(scenes, 1):
        if shot.get("id") != f"C{i:02d}":
            raise ValueError("Shot identifiers drift")
        start, end = shot.get("start_seconds"), shot.get("end_seconds")
        if not isinstance(start, (int, float)) or not isinstance(end, (int, float)):
            raise ValueError("Missing shot timing")
        if abs(start-last) > 0.012 or end <= start or end-start < 6:
            raise ValueError(f"Gap/overlap/malformed duration: {shot['id']}")
        last = end
    if abs(last-data["duration_seconds"]) > 0.015:
        raise ValueError("End frame is not timed to audio")
    policy = data.get("audio_acceptance", {})
    if policy.get("channels") != 2 or policy.get("sample_rate_hz") != 48000:
        raise ValueError("Missing stereo 48k sound acceptance")
    if not policy.get("music_only_not_substitute_for_voice") or not policy.get("voice_present"):
        raise ValueError("Music is not a substitute for voice")
    if "not unreal" not in str(data.get("editorial_status", "")).lower():
        raise ValueError("A concept animatic must not be represented as engine gameplay")
    if "not a spoiler-light public trailer" not in str(data.get("public_spoiler_policy", "")).lower():
        raise ValueError("Spoiler-inclusive director edition must stay private/internal")


def run_json(args: list[str]) -> dict:
    cp = subprocess.run(args, capture_output=True, text=True, check=True)
    return json.loads(cp.stdout)


def verify_media(video: Path, index: dict, subtitle: Path | None) -> dict:
    if not video.is_file():
        raise ValueError("Rendered media path does not exist")
    inspect = run_json([
        "ffprobe", "-v", "error", "-show_entries",
        "stream=codec_type,codec_name,duration,sample_rate,channels",
        "-show_entries", "format=duration", "-of", "json", str(video)
    ])
    v = [s for s in inspect["streams"] if s["codec_type"] == "video"]
    a = [s for s in inspect["streams"] if s["codec_type"] == "audio"]
    if len(v) != 1 or len(a) != 1:
        raise ValueError("Expected exactly one video and one audio stream")
    policy = index["audio_acceptance"]
    sound = a[0]
    if sound.get("codec_name") != policy["audio_codec"]:
        raise ValueError("Audio codec mismatch")
    if int(sound.get("channels", 0)) != policy["channels"] or int(sound.get("sample_rate", 0)) != policy["sample_rate_hz"]:
        raise ValueError("Wrong number of audio channels or sample rate")
    for stream in (v[0], sound):
        if abs(float(stream.get("duration", 0))-index["duration_seconds"]) > policy["max_abs_duration_difference_seconds"]:
            raise ValueError("Truncated audio or video detected")
    level = subprocess.run([
        "ffmpeg", "-hide_banner", "-nostats", "-i", str(video),
        "-map", "0:a:0", "-vn", "-af", "volumedetect", "-f", "null", "-"
    ], capture_output=True, text=True, check=True).stderr
    mean = re.search(r"mean_volume:\s*(-?[0-9.]+) dB", level)
    peak = re.search(r"max_volume:\s*(-?[0-9.]+) dB", level)
    if mean is None or peak is None:
        raise ValueError("Cannot measure audio loudness")
    mean_db, peak_db = float(mean.group(1)), float(peak.group(1))
    if mean_db < policy["mean_dbfs_min"] or peak_db > policy["peak_dbfs_max"]:
        raise ValueError(f"Audio too quiet/clipping: mean {mean_db}, peak {peak_db}")
    if subtitle is not None:
        if not subtitle.is_file():
            raise ValueError("Separate SRT file missing")
        srt = subtitle.read_text(encoding="utf-8-sig")
        if len(re.findall(r"\d\d:\d\d:\d\d,\d{3} --> ", srt)) != 56:
            raise ValueError("Subtitle cue count must match 56 shots")
        if not srt.strip():
            raise ValueError("Empty subtitles")
    return {"video_seconds":float(v[0]["duration"]),
            "audio_seconds":float(sound["duration"]),
            "channels":int(sound["channels"]),
            "mean_dbfs":mean_db,"peak_dbfs":peak_db,
            "subtitle_checked":subtitle is not None}


def main() -> int:
    p = argparse.ArgumentParser()
    p.add_argument("--manifest", type=Path, default=INDEX)
    p.add_argument("--media", type=Path)
    p.add_argument("--subtitles", type=Path)
    args = p.parse_args()
    if args.subtitles and not args.media:
        p.error("--subtitles requires --media")
    try:
        index = json.loads(args.manifest.read_text(encoding="utf-8"))
        verify_index(index)
        if args.media:
            status = verify_media(args.media, index, args.subtitles)
            print(f"PASS: duration and audible stereo media verified: {status}")
        else:
            print("PASS: 56-shot full-scope index only; audio/binary NOT checked")
        return 0
    except (OSError, ValueError, subprocess.CalledProcessError, KeyError, json.JSONDecodeError) as exc:
        print(f"FAIL: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
