"""Extend both intact v17 masters, preserving every inherited paragraph/image."""
from pathlib import Path
import argparse,hashlib,json,textwrap,io,os
from docx import Document
from docx.shared import Inches,Pt,RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_TABLE_ALIGNMENT,WD_CELL_VERTICAL_ALIGNMENT
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
ROOT=Path(__file__).resolve().parents[2]
def atlas(path):
 old=json.loads((ROOT/'Authoring/adventure_depth_v16.json').read_text())['realms'];new=json.loads((ROOT/'Authoring/player_experience_v17.json').read_text())['realms'];fig=plt.figure(figsize=(8.6,7.5),facecolor='#11212a')
 fig.text(.045,.96,'THE UNMADE   ROOMS BEYOND THE FAMILIAR ROAD',fontsize=15,weight='bold',color='white')
 fig.text(.045,.918,'Six retained chambers  /  Three optional wing rooms  /  Earned return shortcuts',fontsize=10,color='#c2d4d9')
 labels=['Guide at three bearings','Low water and dry ledge','Cooling relays in twenty seconds','Guarded oath and living yield','Elevated choir stations','Paired memory and return','Unobserved lamp readings','Earned Living Roads barrier','Moving lift and safe lower floor']
 for i,(c,n) in enumerate(zip(old,new)):
  x=.04+(i%3)*.32;y=.68-(i//3)*.285;ax=fig.add_axes([x,y,.30,.19],facecolor='#1b343e');ax.set_xticks([]);ax.set_yticks([])
  for sp in ax.spines.values():sp.set_color('#57747a')
  pts=c['room_grid'];wing=n['wing_grid'];allpts=pts+wing;xx=[p[0] for p in allpts];yy=[p[1] for p in allpts]
  ax.plot([p[0] for p in pts],[p[1] for p in pts],color='#93b5b5',linewidth=2,zorder=1)
  anchor=max(range(6),key=lambda j:pts[j][0]);branch=[pts[anchor]]+wing;ax.plot([p[0] for p in branch],[p[1] for p in branch],color='#e0c399',linewidth=2,zorder=1)
  ax.plot([wing[2][0],wing[0][0],wing[0][0]],[wing[2][1],wing[2][1],wing[0][1]],color='#b9cbb8',linestyle='--',linewidth=1,zorder=1)
  ax.scatter(xx,yy,s=220,marker='s',c=['#71898b']*5+['#af6c5a']+['#d0b487','#d0b487','#bca1c1'],zorder=2)
  for j,(u,v) in enumerate(allpts):ax.text(u,v,str(j+1) if j<6 else ['A','B','C'][j-6],ha='center',va='center',fontsize=8.5,color='#10212a',weight='bold')
  ax.set_xlim(min(xx)-.5,max(xx)+.5);ax.set_ylim(min(yy)-.6,max(yy)+.6);ax.set_title(c['city'],fontsize=12,color='#efc993',weight='bold',loc='left',pad=5);ax.text(0,-.12,labels[i],fontsize=8,color='white',transform=ax.transAxes,va='top')
 fig.text(.045,.028,'27 additional room areas  /  9 named wing guardians  /  36 conditional return scenes',fontsize=10,color='#dce6e8');buf=io.BytesIO();fig.savefig(buf,format='png',dpi=180,facecolor=fig.get_facecolor(),bbox_inches='tight');plt.close(fig)
 with path.open('wb') as f:f.write(buf.getvalue());f.flush();os.fsync(f.fileno())

def add_table(doc,lines):
 rows=[[x.strip() for x in l.strip('|').split('|')] for l in lines if not l.startswith('| ---')];t=doc.add_table(rows=0,cols=len(rows[0]));t.style='Table Grid';t.autofit=False;w=[1.05,2.35,2.8] if len(rows[0])==3 else [1.2,1.8,1.8,1.4]
 t.alignment=WD_TABLE_ALIGNMENT.CENTER
 for j,width in enumerate(w):t.columns[j].width=Inches(width)
 for ri,record in enumerate(rows):
  row=t.add_row();row._tr.get_or_add_trPr().append(OxmlElement("w:cantSplit"))
  for j,value in enumerate(record):
   cell=row.cells[j];cell.width=Inches(w[j]);cell.vertical_alignment=WD_CELL_VERTICAL_ALIGNMENT.CENTER;cell.text=value;pr=cell._tc.get_or_add_tcPr()
   margins=OxmlElement('w:tcMar')
   for side in ['top','bottom','left','right']:
    m=OxmlElement('w:'+side);m.set(qn('w:w'),'85');m.set(qn('w:type'),'dxa');margins.append(m)
   pr.append(margins);b=OxmlElement('w:tcBorders')
   for side in ['top','bottom','left','right']:
    x=OxmlElement('w:'+side);x.set(qn('w:val'),'single');x.set(qn('w:sz'),'4');x.set(qn('w:color'),'D9D9D9');b.append(x)
   pr.append(b)
   if ri==0:
    shade=OxmlElement('w:shd');shade.set(qn('w:fill'),'DFE8EC');pr.append(shade)
   for para in cell.paragraphs:
    para.paragraph_format.space_after=Pt(3);para.paragraph_format.line_spacing=1.1
    for run in para.runs:run.font.size=Pt(min(10,doc.styles['Normal'].font.size.pt));run.bold=ri==0
  if ri==0:row._tr.get_or_add_trPr().append(OxmlElement('w:tblHeader'))
def main():
 p=argparse.ArgumentParser();p.add_argument('--previous-books',type=Path,required=True);p.add_argument('--out-dir',type=Path,required=True);a=p.parse_args();a.out_dir.mkdir(parents=True,exist_ok=True)
 art=a.out_dir/'THE_UNMADE_v17_Player_Experience_Atlas.png';atlas(art);source=(ROOT/'docs/lore/CHAPTER_37_PLAYER_EXPERIENCE.md').read_text().splitlines();inputs=[]
 for prefix in ['THE_UNMADE_Complete_Illustrated_Lore_Bible_Volume_X','THE_UNMADE_Volume_X_Illustrated']:
  previous=a.previous_books/(prefix+'_Adventure_Depth_v16.docx');doc=Document(previous);old=len(doc.inline_shapes);doc.add_page_break();i=0
  while i<len(source):
   line=source[i];i+=1
   if not line.strip():continue
   if line.startswith('|'):
    lines=[line]
    while i<len(source) and source[i].startswith('|'):lines.append(source[i]);i+=1
    add_table(doc,lines);continue
   if line.startswith('## '):h=doc.add_heading(line[3:],1)
   elif line.startswith('### '):
    h=doc.add_heading(line[4:],2)
    if line.startswith('### 37.1'):
     para=doc.add_paragraph();para.alignment=WD_ALIGN_PARAGRAPH.CENTER;para.add_run().add_picture(str(art),width=Inches(6.3));doc.add_paragraph('Figure X 37 Nine optional wings extend the existing routes. All layouts shown here are production knowledge; the player map reveals earned discoveries. Engine testing remains deferred.',style='Caption')
   else:
    para=doc.add_paragraph(line);para.paragraph_format.line_spacing=1.10;para.paragraph_format.space_after=Pt(3.5);continue
   for run in h.runs:run.font.color.rgb=RGBColor(0,0,0)
  doc.core_properties.title='The Unmade Player Experience v17';target=a.out_dir/(prefix+'_Player_Experience_v17.docx');buf=io.BytesIO();doc.save(buf)
  with target.open('wb') as f:f.write(buf.getvalue());f.flush();os.fsync(f.fileno())
  assert len(doc.inline_shapes)==old+1
  inputs.append({'name':previous.name,'sha256':hashlib.sha256(previous.read_bytes()).hexdigest(),'retained_images':old});print(target.name,len(doc.inline_shapes))
 (a.out_dir/'v17_build_inputs.json').write_text(json.dumps(inputs,indent=2)+'\n')
if __name__=='__main__':main()
