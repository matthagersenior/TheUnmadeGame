#!/usr/bin/env python3
"""Build spoiler-light public reader editions and the full-source encyclopedia."""
from pathlib import Path
import argparse,html,re,shutil
R=Path(__file__).resolve().parents[2];S=R/"site";O=S/"dist"
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
 for fn in ["index.html","main.js","styles.css","real-models.html","play-bellwold.html","field-codex.html","gameplay-guide.html","trailer-storyboard.html","downloads/the-unmade-field-codex.pdf","downloads/the-unmade-gameplay-guide.pdf"]:
  p=O/fn
  if not p.is_file() or p.stat().st_size<30:raise RuntimeError("Missing "+str(p))
 if not (O/"downloads/the-unmade-definitive-canon-encyclopedia.pdf").is_file():raise RuntimeError("Missing canonical full PDF")
 for pdf in (O/"downloads").glob("*.pdf"):
  if not pdf.read_bytes().startswith(b"%PDF"):raise RuntimeError("Invalid PDF "+str(pdf))
 print("PASS: HTML, accessible reader editions, 3 PDF downloads, no prototype trailer")
def main():
 p=argparse.ArgumentParser();p.add_argument("--check",action="store_true");a=p.parse_args()
 if a.check:verify();return
 if O.exists():shutil.rmtree(O)
 O.mkdir();shutil.copytree(S/"art",O/"art")
 for fn in ("index.html","styles.css","main.js","real-models.html","play-bellwold.html"):shutil.copyfile(S/fn,O/fn)
 (O/".nojekyll").write_text("")
 books();full_encyclopedia();verify()
if __name__=="__main__":main()
