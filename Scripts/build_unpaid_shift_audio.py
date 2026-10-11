#!/usr/bin/env python3
"""Generate REAL, deterministic, original placeholder Foley WAVs for the 39
Unpaid Shift beats. No network, synthesized voice, external samples or music.
These are functional prototype effects, NOT final human Foley/VO performances.
Usage: python Scripts/build_unpaid_shift_audio.py --out-dir <directory>
"""
import argparse
import hashlib
import json
import math
import random
import re
import struct
import wave
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
SOURCE=ROOT/"Source/TheUnmadeGame/Public/World/UnmadeUnpaidShiftSceneRules.h"
RATE=32000
SAMPLES=2  # stereo
CUE_RE=re.compile(r'\{UnpaidScene::\w+,\s*\d+,\s*"[^"]+",\s*"[^"]+",\s*"[^"]+",\s*"[^"]+",\s*"([a-z_]+)"')
AMBIENT=("wind","room_tone","boss_ambience","dock_water","distant_kiln","rain","vent_low","kiln_missing")
METAL=("coin","iron_mark","six_mark","bell","ledger_stamp")
PAPER=("paper","ledger","pen")
EMBER=("ember","fire","ash","coal")
WOOD=("basket","cart","door")

def cues_from_source():
    found=CUE_RE.findall(SOURCE.read_text(encoding="utf-8"))
    if len(found)!=39:
        raise ValueError(f"Expected 39 source beats, parsed {len(found)}")
    if any(not re.fullmatch("[a-z_]+",x) for x in found):
        raise ValueError("Invalid cue name")
    return sorted(set(found)),found

def parameters(name):
    if any(x in name for x in AMBIENT): return "ambient",2.35,0.21
    if any(x in name for x in METAL): return "metal",1.35,0.34
    if any(x in name for x in PAPER): return "paper",0.90,0.28
    if any(x in name for x in EMBER): return "ember",1.55,0.23
    if any(x in name for x in WOOD): return "wood",1.10,0.33
    return "textural",1.10,0.22

def make_wave(name):
    kind,seconds,loudness=parameters(name)
    seed=int.from_bytes(hashlib.sha256(name.encode()).digest()[:8],"big")
    rng=random.Random(seed)
    count=int(seconds*RATE)
    base=105 + seed%135
    carrier=0.0
    low=0.0
    diff=0.0
    frames=bytearray()
    maxpeak=0.0
    sumsq=0.0
    # Distinct stereo signals; no phase-inverted mono or random digital hiss.
    for n in range(count):
        t=n/RATE
        progress=n/count
        noise=rng.uniform(-1.0,1.0)
        low+=(noise-low)*(0.009 if kind=="ambient" else 0.065)
        diff+=(noise-diff)*0.11
        carrier=math.sin(2*math.pi*(base*t+0.6*t*t))
        if kind=="ambient":
            env=min(1.0,t/.18)*min(1.0,(seconds-t)/.45)
            sig=(low*1.25+0.17*math.sin(2*math.pi*(base/3)*t)*math.sin(2*math.pi*.38*t))
        elif kind=="metal":
            env=math.exp(-5.2*t)*(1-math.exp(-t*170))
            sig=.60*carrier+.23*math.sin(2*math.pi*base*2.73*t)+.17*diff
        elif kind=="paper":
            env=math.exp(-3.5*t)*(1-math.exp(-t*105))*(.72+.28*math.sin(2*math.pi*8*t))
            sig=.40*noise+.45*diff+.15*low
        elif kind=="ember":
            env=math.exp(-2*t)*(1-math.exp(-t*30))
            crackle=1.0 if rng.random()<.0007 else 0.0
            sig=low*1.05+.21*noise+.9*crackle
        elif kind=="wood":
            env=math.exp(-5.0*t)*(1-math.exp(-t*200))
            sig=.7*low+.19*noise+.15*carrier
        else:
            env=math.exp(-2.7*t)*(1-math.exp(-t*90))
            sig=.65*low+.35*carrier
        # Fade both ends to prevent clicks and keep prototype output safe.
        fade=min(1,t/.012,(seconds-t)/.040)
        v=max(-.82,min(.82,sig*env*loudness*max(0,fade)))
        l=v*(.95+.05*math.sin(2*math.pi*.8*t))
        r=v*(.91+.09*math.sin(2*math.pi*.8*t+1.2))
        maxpeak=max(maxpeak,abs(l),abs(r))
        sumsq+=(l*l+r*r)/2
        frames.extend(struct.pack("<hh",int(l*32767),int(r*32767)))
    return bytes(frames),round(seconds,3),round(maxpeak,5),round(math.sqrt(sumsq/count),5),kind

def build(out_dir):
    out_dir=Path(out_dir)
    out_dir.mkdir(parents=True,exist_ok=True)
    cues,beat_cues=cues_from_source()
    records=[]
    for cue in cues:
        pcm,duration,peak,rms,kind=make_wave(cue)
        if not (0<peak<0.85 and rms>0.00005):
            raise RuntimeError(f"Invalid audible output for {cue}: peak={peak}, rms={rms}")
        name="SFX_US_"+cue+".wav"
        path=out_dir/name
        with wave.open(str(path),"wb") as wf:
            wf.setnchannels(2);wf.setsampwidth(2);wf.setframerate(RATE)
            wf.writeframes(pcm)
        records.append({"cue":cue,"asset":name,"unreal_asset":"/Game/UnmadeProduction/Audio/UnpaidShift/SFX_US_"+cue,
                        "duration_seconds":duration,"sample_rate":RATE,"channels":2,
                        "peak":peak,"rms":rms,"kind":kind,
                        "sha256":hashlib.sha256(path.read_bytes()).hexdigest()})
    manifest={"schema":1,"production_status":"ORIGINAL_SYNTHETIC_PLACEHOLDER_FOLEY_NOT_FINAL_VO",
              "beat_count":len(beat_cues),"unique_cues":len(cues),
              "source":"Source/TheUnmadeGame/Public/World/UnmadeUnpaidShiftSceneRules.h",
              "assets":records}
    (out_dir/"unpaid_shift_audio_manifest.json").write_text(
        json.dumps(manifest,indent=2,sort_keys=True)+"\n",encoding="utf-8")
    return manifest

def main():
    p=argparse.ArgumentParser()
    p.add_argument("--out-dir",default="Build/UnpaidShiftAudio")
    p.add_argument("--check",action="store_true")
    args=p.parse_args()
    manifest=build(args.out_dir)
    if args.check:
        with open(Path(args.out_dir)/"unpaid_shift_audio_manifest.json",encoding="utf-8") as f:
            m=json.load(f)
        assert m["beat_count"]==39 and m["unique_cues"]==35
        for rec in m["assets"]:
            f=Path(args.out_dir)/rec["asset"]
            assert hashlib.sha256(f.read_bytes()).hexdigest()==rec["sha256"]
            with wave.open(str(f),"rb") as wf:
                assert wf.getnchannels()==2 and wf.getsampwidth()==2 and wf.getframerate()==RATE
    print(f"PASS: {manifest['beat_count']} beats, {manifest['unique_cues']} actual stereo prototype sounds, no human narration claimed")
if __name__=="__main__":
    main()
