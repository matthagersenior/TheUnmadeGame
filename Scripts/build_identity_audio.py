#!/usr/bin/env python3
"""Original deterministic procedural sound sources. No recordings, VO or paid assets."""
from pathlib import Path
import argparse,hashlib,json,math,random,struct,wave
RATE=22050
EVENTS=['swing','impact','guard','injury','death','cast','inspect','repair','release','reward','warning','boss_crush','boss_sweep','boss_seal','yield','footstep_stone','footstep_wood','footstep_salt']
EVENTS += ['boss_signature_'+str(n) for n in range(9)]
REALMS=['crossings','saltwake','cinderhold','drevlach','orravane','vathless','eillun','tharniv','auvren']
REVIEW_CUES=['report_review_'+realm for realm in REALMS]
FRACTURE_CUES=['first_fracture_'+realm for realm in REALMS]
def build(out):
 out.mkdir(parents=True,exist_ok=True);rows=[]
 for n,name in enumerate(EVENTS+['ambience_'+x for x in REALMS]+REVIEW_CUES+FRACTURE_CUES):
  loop=name.startswith('ambience_');review=name.startswith('report_review_');fracture=name.startswith('first_fracture_')
  duration=1.6 if fracture else 1.25 if review else 8 if loop else 1.1 if name.startswith('boss_') else .55
  rng=random.Random(1500+n);data=[]
  for i in range(int(RATE*duration)):
   t=i/RATE
   if fracture:
    # Slightly detuned, uneven pulses: one observed contradiction per realm.
    # No speech, borrowed samples, narration or lore-answering melody.
    realm=n-45
    base=75+realm*21
    glide=base*(1+.21*t/duration)
    fade=math.sin(math.pi*t/duration)**2
    knock=sum((.7 if b==0 else .4)*math.exp(-((t-tick)/.045)**2)
       for b,tick in enumerate((.16,.47+(realm%3)*.055,.92+(realm//3)*.058)))
    v=fade*(.12*math.sin(2*math.pi*(base*t+.5*(glide-base)*t*t/duration))
       +.055*math.sin(2*math.pi*(base*1.517)*t)
       +.065*knock*(rng.random()*2-1))
   elif review:
    # Three paired resonances: pulse spacing, pitch and overtone change by realm.
    # The score is authored procedural Foley, never simulated speech or music.
    realm=n-36
    f=185+realm*31
    beats=(.09,.35+(realm%3)*.055,.75+(realm//3)*.047)
    pulse=sum((1 if j==0 else .67)*math.exp(-((t-beat)/.046)**2) for j,beat in enumerate(beats))
    fade=max(0,min(1,t/.018,(duration-t)/.055))
    v=pulse*fade*(.19*math.sin(2*math.pi*f*t)+.055*math.sin(2*math.pi*(f*1.73)*t)+.015*(rng.random()*2-1))
   else:
    env=math.sin(math.pi*t/duration)**2 if loop else min(1,t/.015)*math.exp(-t*7/duration)*min(1,(duration-t)/.025)
    f=48+n*9 if loop else 100+n*17
    v=env*(.12*math.sin(2*math.pi*f*t)+.045*math.sin(2*math.pi*(f*1.503)*t)+.04*(rng.random()*2-1))
   data.append(int(v*32767))
  path=out/('SFX_ID_'+name+'.wav')
  with wave.open(str(path),'wb') as w:w.setparams((1,2,RATE,0,'NONE','not compressed'));w.writeframes(struct.pack('<'+'h'*len(data),*data))
  rows.append({'cue':name,'file':path.name,'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'loop':loop,'duration':duration,'peak':max(abs(v) for v in data)/32767,'rms':math.sqrt(sum(v*v for v in data)/len(data))/32767,'unreal_asset':'/Game/UnmadeProduction/Audio/Identity/SFX_ID_'+name})
 m={'schema':1,'license':'Original project-authored procedural sources; same license as repository','production_status':'PROTOTYPE_SYNTHETIC_FOLEY_AND_AMBIENCE_NOT_FINAL_MIX_OR_VO','assets':rows}
 (out/'identity_audio_manifest.json').write_text(json.dumps(m,indent=2)+'\n');return m
def validate(out):
 m=json.loads((out/'identity_audio_manifest.json').read_text());assert len(m['assets'])==54
 for a in m['assets']:
  p=out/a['file'];assert hashlib.sha256(p.read_bytes()).hexdigest()==a['sha256']
  with wave.open(str(p)) as w:
   assert (w.getnchannels(),w.getsampwidth(),w.getframerate())==(1,2,RATE)
   raw=w.readframes(w.getnframes());v=struct.unpack('<'+'h'*(len(raw)//2),raw)
   assert 0<max(abs(x) for x in v)<32767 and abs(v[0])<50 and abs(v[-1])<50
 return m
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--out',type=Path,required=True);a=p.parse_args();build(a.out);validate(a.out);print('PASS: 54 original WAV sources (36 existing + 9 review + 9 first-fracture motifs)')
