#!/usr/bin/env python3
"""Build original Unmade cinematic concept film and spoiler-light reader editions."""
from pathlib import Path
import argparse,html,math,random,re,shutil,struct,subprocess,wave
R=Path(__file__).resolve().parents[2];S=R/"site";O=S/"dist"
SHOTS=[
("THE UNMADE","SOME WORLDS ARE BROKEN","THIS ONE REMEMBERS BEING WHOLE","void"),
("THE THREEFOLD REACH","A BELL WITH NO LAST NOTE","TWO ROADS. TWO WITNESSES.","bell"),
("THE WIDOWED RAIN","A SHORE WITHOUT WATER","THE RAIN REMEMBERS THE SEA","rain"),
("THE HEARTH BENEATH","A FIRE WITHOUT A SUN","WHO KEEPS THE HOMES WARM?","fire"),
("THE INVERTED CHOIR","A CITY HELD BY A SONG","THE MISSING NOTE HAS WEIGHT","choir"),
("THE YESTERDAY QUARRY","A HISTORY BURIED IN STONE","SOME NAMES BECOME FOUNDATIONS","stone"),
("THE UNCLAIMED CITY","NOT ALL LIVES HAVE A RECORD","EVERYONE DESERVES A DOOR","door"),
("THE UNTITLED ORCHARD","A LAW WITHOUT A RULER","WHO AGREED TO THE EMPTY THRONE?","tree"),
("THE WORLD REMEMBERS","DISCOVER. CHOOSE. RETURN.","REGISTER BETA INTEREST · NOT GAMEPLAY","void")]
D=6;TOTAL=D*len(SHOTS)
def books():
 import markdown
 from reportlab.platypus import SimpleDocTemplate,Paragraph,Spacer
 from reportlab.lib.styles import getSampleStyleSheet,ParagraphStyle
 from reportlab.lib import colors
 from reportlab.lib.units import inch
 st=getSampleStyleSheet()
 for name,parent,size,lead,space in [("UT","Title",27,32,20),("UH","Heading2",16,21,13),("US","Heading3",12,16,9),("UB","BodyText",10,15,6)]:
  st.add(ParagraphStyle(name=name,parent=st[parent],fontSize=size,leading=lead,spaceAfter=space,textColor=colors.HexColor("#403342")))
 (O/"downloads").mkdir(exist_ok=True)
 for fn,pdf,label in [("field-codex.md","the-unmade-field-codex.pdf","Field Codex"),("gameplay-guide.md","the-unmade-gameplay-guide.pdf","Gameplay Guide"),("trailer-storyboard.md",None,"Trailer Storyboard")]:
  source=(S/fn).read_text()
  frame="<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>"+html.escape(label)+" · THE UNMADE</title><link rel='stylesheet' href='styles.css'></head><body><header class='topbar'><a class='brand' href='index.html'>THE UNMADE</a><nav><a href='index.html'>← Back to site</a></nav></header><main class='reader'><p class='eyebrow'>PUBLIC FIELD LIBRARY</p>"+markdown.markdown(source,extensions=["tables"])+"</main><footer><p>© 2026 Matthew Hager. All rights reserved. Source reference, not playable footage.</p></footer></body></html>"
  (O/fn.replace(".md",".html")).write_text(frame,encoding="utf-8")
  if not pdf:continue
  content=[]
  for raw in source.splitlines():
   v=raw.strip()
   if not v or v=="---" or v.startswith("|"):
    if not v:content.append(Spacer(1,5))
    continue
   style="UB"
   for prefix,typ in [("# ","UT"),("## ","UH"),("### ","US"),("> ","UB")]:
    if v.startswith(prefix):v=v[len(prefix):];style=typ;break
   if v.startswith("- "):v="• "+v[2:]
   v=html.escape(v)
   v=re.sub(r"\*\*(.*?)\*\*",r"<b>\1</b>",v)
   v=re.sub(r"\*(.*?)\*",r"<i>\1</i>",v)
   content.append(Paragraph(v,st[style]))
  def footer(c,d):
   c.saveState();c.setFont("Helvetica",8);c.drawString(50,28,"THE UNMADE · PUBLIC CONCEPT EDITION · NOT A PLAYABLE RELEASE");c.drawRightString(550,28,str(d.page));c.restoreState()
  SimpleDocTemplate(str(O/"downloads"/pdf),pagesize=(8.5*inch,11*inch),rightMargin=50,leftMargin=50,topMargin=54,bottomMargin=54,title="THE UNMADE "+label,author="Matthew Hager").build(content,onFirstPage=footer,onLaterPages=footer)
def art(path,n):
 from PIL import Image,ImageDraw,ImageFont
 title,hook,sub,kind=SHOTS[n];W=1600;H=900
 palettes={"void":((10,10,26),(77,49,72)),"bell":((19,19,36),(91,68,78)),"rain":((10,26,40),(59,83,94)),"fire":((37,18,27),(129,63,49)),"choir":((18,24,46),(83,76,111)),"stone":((29,30,46),(99,81,93)),"door":((17,30,42),(78,86,104)),"tree":((17,36,31),(79,100,71))}
 a,b=palettes[kind];im=Image.new("RGB",(W,H));p=ImageDraw.Draw(im)
 for y in range(H):
  t=(y/H)**1.2;p.line((0,y,W,y),fill=tuple(int(x*(1-t)+z*t) for x,z in zip(a,b)))
 rng=random.Random(n+115)
 for j in range(130):
  x=rng.randrange(W);y=rng.randrange(H);p.point((x,y),fill=(164,149,165))
 bright=(211,167,149);black=(12,13,24)
 if kind=="void":
  for r in (230,300,380):p.ellipse((1130-r,420-r,1130+r,420+r),outline=bright,width=2)
  p.polygon([(1030,795),(1100,95),(1154,95),(1250,795)],fill=black);p.line((1126,100,1126,785),fill=bright,width=5)
 elif kind=="bell":
  p.polygon([(830,795),(950,135),(1140,135),(1280,795)],fill=black);p.rectangle((993,210,1092,330),outline=bright,width=8)
  for x in (720,1370):p.polygon([(x-140,790),(x,440),(x+140,790)],fill=black)
 elif kind=="rain":
  for x in range(650,1600,120):
   p.polygon([(x,795),(x+35,465),(x+58,795)],fill=black)
  for j in range(210):
   x=rng.randrange(660,1600);y=rng.randrange(70,795);p.line((x,y,x-20,y+65),fill=(153,177,186),width=2)
 elif kind=="fire":
  p.polygon([(580,790),(950,320),(1050,290),(1580,790)],fill=black)
  for x in (850,1090,1310):
   p.ellipse((x-50,360,x+60,650),fill=(191,107,62));p.polygon([(x-40,640),(x,330),(x+40,640)],fill=black)
 elif kind=="choir":
  for x in (680,900,1110,1360,1530):
   y=rng.randrange(160,300);p.line((x,0,x,y),fill=bright,width=4)
   p.polygon([(x-95,y),(x+85,y),(x+69,y+172),(x-85,y+172)],fill=black)
 elif kind=="stone":
  for x in range(650,1590,166):
   p.polygon([(x,360),(x+140,315),(x+154,795),(x,795)],fill=black);p.line((x+25,360,x+109,732),fill=bright,width=3)
 elif kind=="door":
  for x in (680,920,1190,1450):
   p.rounded_rectangle((x,305,x+150,795),radius=65,outline=bright,width=6)
   p.rectangle((x+20,425,x+130,790),fill=black)
 elif kind=="tree":
  for x in (730,1010,1310,1540):
   p.line((x,795,x,240),fill=(44,64,40),width=45)
   p.arc((x-180,110,x+180,560),180,365,fill=(140,138,99),width=24)
 p.polygon([(0,825),(320,770),(620,846),(1100,783),(1600,820),(1600,900),(0,900)],fill=(8,10,17))
 p.rectangle((0,0,W,80),fill=(7,8,16));p.rectangle((0,820,W,900),fill=(7,8,16))
 try:
  font=lambda f,s:ImageFont.truetype("/usr/share/fonts/truetype/dejavu/"+f,s)
  big=font("DejaVuSerif.ttf",49 if len(title)>22 else 64);med=font("DejaVuSans.ttf",29);small=font("DejaVuSans.ttf",18)
 except OSError:big=med=small=ImageFont.load_default()
 p.text((100,290),title,font=big,fill=(248,231,218))
 p.line((104,402,300,402),fill=bright,width=4)
 p.text((105,449),hook,font=med,fill=(233,199,173))
 p.text((105,505),sub,font=small,fill=(234,222,215))
 p.text((105,849),"THE UNMADE / ORIGINAL CINEMATIC CONCEPT / NOT GAMEPLAY",font=small,fill=(183,173,183))
 im.save(path)
def score(path):
 rate=22050;notes=[110,130.81,146.83,98,164.81,123.47,110,146.83,98]
 with wave.open(str(path),"wb") as w:
  w.setnchannels(1);w.setsampwidth(2);w.setframerate(rate);data=bytearray()
  for i in range(rate*TOTAL):
   t=i/rate;section=min(len(notes)-1,int(t/D));phase=t%D;f=notes[section]
   envelope=min(1,phase/.7,max(0,(D-phase)/.65),t/1.5,(TOTAL-t)/1.5)
   s=(math.sin(2*math.pi*f*t)+.45*math.sin(3*math.pi*f*t)+.22*math.sin(4*math.pi*f*t))*.17
   s+=math.sin(2*math.pi*659.25*phase)*math.exp(-3*phase)*.09
   data.extend(struct.pack("<h",int(max(-1,min(1,s*envelope))*27000)))
   if len(data)>32768:w.writeframes(data);data.clear()
  if data:w.writeframes(data)
def film():
 if not shutil.which("ffmpeg"):raise RuntimeError("FFmpeg missing; cannot make playable trailer")
 temp=O/"_film";temp.mkdir();media=O/"media";media.mkdir()
 clips=[]
 for i in range(len(SHOTS)):
  still=temp/("scene%02d.png"%i);clip=temp/("clip%02d.mp4"%i)
  art(still,i)
  zoom="zoompan=z='min(zoom+0.00035,1.08)':x='iw/2-iw/zoom/2':y='ih/2-ih/zoom/2':d=144:s=1280x720:fps=24,fade=t=in:st=0:d=0.4,fade=t=out:st=5.6:d=0.4,format=yuv420p"
  subprocess.run(["ffmpeg","-loglevel","error","-y","-loop","1","-framerate","24","-i",str(still),"-vf",zoom,"-frames:v","144","-an","-c:v","libx264","-preset","veryfast","-crf","27","-pix_fmt","yuv420p",str(clip)],check=True)
  clips.append(clip)
 listing=temp/"playlist.txt";listing.write_text("\n".join("file '"+str(p.resolve())+"'" for p in clips)+"\n")
 soundtrack=temp/"music.wav";score(soundtrack)
 output=media/"the-unmade-concept-trailer.mp4"
 subprocess.run(["ffmpeg","-loglevel","error","-y","-f","concat","-safe","0","-i",str(listing),"-i",str(soundtrack),"-c:v","copy","-c:a","aac","-b:a","128k","-movflags","+faststart","-shortest",str(output)],check=True)
 def t(s):return "%02d:%02d:%06.3f"%(s//3600,(s//60)%60,s%60)
 captions=["WEBVTT",""]
 for i,(title,line,sub,kind) in enumerate(SHOTS):captions.extend([t(i*D)+" --> "+t((i+1)*D),title+". "+line+". "+sub+".",""])
 (media/"trailer-captions.vtt").write_text("\n".join(captions))
 shutil.rmtree(temp)
 print("Built original film",output,output.stat().st_size,"bytes")

def full_encyclopedia():
 """Print all eight canonical source volumes verbatim as an optional spoiler-heavy PDF."""
 import hashlib,textwrap
 from reportlab.pdfgen import canvas
 from reportlab.lib.pagesizes import letter
 from reportlab.pdfbase import pdfmetrics
 from reportlab.pdfbase.ttfonts import TTFont
 canonical=R/"docs"/"canon"/"encyclopedia"
 volumes=sorted(canonical.glob("0?_*.md"))
 if len(volumes)!=8:raise RuntimeError("Expected eight canonical source volumes; got "+str(len(volumes)))
 output=O/"downloads"/"the-unmade-definitive-canon-encyclopedia.pdf"
 pdf=canvas.Canvas(str(output),pagesize=letter,pageCompression=1)
 pdf.setTitle("THE UNMADE — Definitive Canon Encyclopedia, Eight Volumes")
 pdf.setAuthor("Matthew Hager")
 mono="/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
 font="Courier"
 if Path(mono).exists():
  pdfmetrics.registerFont(TTFont("CanonMono",mono));font="CanonMono"
 def new_page():
  pdf.showPage();pdf.setFont(font,8)
  pdf.setFillColorRGB(.44,.39,.45)
  pdf.drawString(43,25,"THE UNMADE · COMPLETE SOURCE ENCYCLOPEDIA · FULL SPOILERS · NOT GAMEPLAY")
  pdf.setFillColorRGB(.14,.12,.18)
  return 745
 pdf.setFont("Helvetica-Bold",25)
 pdf.drawString(52,660,"THE UNMADE")
 pdf.setFont("Helvetica",16)
 pdf.drawString(52,624,"Definitive Canon Encyclopedia")
 pdf.setFont("Helvetica",11)
 pdf.drawString(52,596,"Eight source volumes · 20 October 2026")
 pdf.setFillColorRGB(.54,.23,.25)
 pdf.drawString(52,546,"FULL SPOILERS — includes unfinished story design and unresolved mysteries")
 pdf.setFillColorRGB(.15,.15,.17)
 pdf.setFont("Helvetica",10)
 pdf.drawString(52,512,"Faithful text reproduction of repository source files in their canonical order.")
 pdf.drawString(52,492,"OPEN, DISPUTED and SEALED mysteries remain unresolved in this edition.")
 pdf.drawString(52,472,"Unreal game completion and beta availability are not implied.")
 pdf.drawString(52,412,"Source: docs/canon/encyclopedia/01–08 on GitHub main.")
 y=new_page()
 for volume in volumes:
  content=volume.read_text(encoding="utf-8")
  digest=hashlib.sha256(content.encode("utf-8")).hexdigest()
  if y<690:y=new_page()
  pdf.setFont("Helvetica-Bold",11)
  pdf.drawString(43,y,"SOURCE VOLUME: "+volume.name)
  y-=18;pdf.setFont("Helvetica",8)
  pdf.drawString(43,y,"SHA-256: "+digest);y-=20
  pdf.setFont(font,8)
  for original in content.splitlines():
   # Do not skip tables, stable IDs, disclaimers or source annotations.
   row=original.expandtabs(2)
   wrapped=textwrap.wrap(row,width=106,break_long_words=True,break_on_hyphens=False,drop_whitespace=False,replace_whitespace=False) or [""]
   for line in wrapped:
    if y<42:y=new_page()
    pdf.drawString(43,y,line)
    y-=10.7
  y-=22
 pdf.save()
 print("Published complete canonical encyclopedia",output.stat().st_size,"bytes across eight source volumes")

def verify():
 for fn in ["index.html","main.js","styles.css","field-codex.html","gameplay-guide.html","trailer-storyboard.html","downloads/the-unmade-field-codex.pdf","downloads/the-unmade-gameplay-guide.pdf","media/the-unmade-concept-trailer.mp4","media/trailer-captions.vtt"]:
  p=O/fn
  if not p.is_file() or p.stat().st_size<30:raise RuntimeError("Missing "+str(p))
 if not (O/"downloads/the-unmade-definitive-canon-encyclopedia.pdf").is_file():raise RuntimeError("Missing canonical full PDF")
 for pdf in (O/"downloads").glob("*.pdf"):
  if not pdf.read_bytes().startswith(b"%PDF"):raise RuntimeError("Invalid PDF "+str(pdf))
 if b"ftyp" not in (O/"media/the-unmade-concept-trailer.mp4").read_bytes()[:64]:raise RuntimeError("Invalid MP4")
 print("PASS: HTML, 2 PDF downloads, 54-second MP4 and VTT")
def main():
 p=argparse.ArgumentParser();p.add_argument("--check",action="store_true");a=p.parse_args()
 if a.check:verify();return
 if O.exists():shutil.rmtree(O)
 O.mkdir();shutil.copytree(S/"art",O/"art")
 for fn in ("index.html","styles.css","main.js"):shutil.copyfile(S/fn,O/fn)
 (O/".nojekyll").write_text("")
 books();full_encyclopedia();film();verify()
if __name__=="__main__":main()
