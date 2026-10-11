#!/usr/bin/env python3
"""Original offline prototype Foley; no voice, samples or network.
Generate a recognizable short-short-pause-low cadence and vent/elder textures.
"""
import argparse,hashlib,json,math,struct,wave
from pathlib import Path
try:
    from Scripts.build_unpaid_shift_audio import make_wave,RATE
except ModuleNotFoundError:
    from build_unpaid_shift_audio import make_wave,RATE
ROOT=Path(__file__).resolve().parents[1]
DEST='/Game/UnmadeProduction/Audio/Cinderhold'
def cadence():
    pcm=bytearray();length=int(2.8*RATE)
    for n in range(length):
        t=n/RATE;value=0.0
        for onset,freq,duration in ((.18,330,.13),(.52,330,.13),(1.85,135,.4)):
            dt=t-onset
            if 0<=dt<duration:
                envelope=math.sin(math.pi*dt/duration)*math.exp(-10*dt)
                value+=.56*envelope*(math.sin(2*math.pi*freq*dt)+.12*math.sin(2*math.pi*freq*2.7*dt))
        sample=int(max(-.8,min(.8,value))*32767)
        pcm.extend(struct.pack('<hh',sample,sample))
    return bytes(pcm)
def build(folder):
    folder=Path(folder);folder.mkdir(parents=True,exist_ok=True)
    sounds={'cadence':cadence(),'vent_cool':make_wave('vent_low')[0],'elder_ground':make_wave('iron_mark')[0]}
    rows=[]
    for cue,pcm in sounds.items():
        path=folder/f'SFX_CH_{cue}.wav'
        with wave.open(str(path),'wb') as w:
            w.setnchannels(2);w.setsampwidth(2);w.setframerate(RATE);w.writeframes(pcm)
        rows.append({'cue':cue,'asset':path.name,'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),
                     'unreal_asset':DEST+'/'+path.stem})
    manifest={'schema':1,'production_status':'ORIGINAL_PROTOTYPE_FOLEY_NOT_VO','assets':rows}
    (folder/'cinderhold_audio_manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    return manifest
def validate(folder):
    folder=Path(folder);m=json.loads((folder/'cinderhold_audio_manifest.json').read_text())
    if m['schema']!=1 or m['production_status']!='ORIGINAL_PROTOTYPE_FOLEY_NOT_VO' or len(m['assets'])!=3:raise ValueError('Bad Cinderhold audio manifest')
    if {a['cue'] for a in m['assets']}!={'cadence','vent_cool','elder_ground'}:raise ValueError('Bad cue set')
    for a in m['assets']:
        if a['asset']!='SFX_CH_'+a['cue']+'.wav' or a['unreal_asset']!=DEST+'/SFX_CH_'+a['cue']:raise ValueError('Bad cue path')
        p=folder/a['asset']
        if hashlib.sha256(p.read_bytes()).hexdigest()!=a['sha256']:raise ValueError('Bad digest')
        with wave.open(str(p),'rb') as w:
            if (w.getnchannels(),w.getframerate(),w.getsampwidth())!=(2,RATE,2):raise ValueError('Bad audio format')
            pcm=w.readframes(w.getnframes())
        samples=struct.unpack('<'+'h'*(len(pcm)//2),pcm)
        if not 0<max(abs(v) for v in samples)<.85*32767:raise ValueError('Silent or clipping audio')
    return m
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--out-dir',default=str(ROOT/'Build/CinderholdAudio'));p.add_argument('--check',action='store_true');a=p.parse_args()
    build(a.out_dir)
    if a.check:validate(a.out_dir)
    print('PASS: 3 original Cinderhold prototype Foley WAVs; no voice or imported Unreal assets claimed')
