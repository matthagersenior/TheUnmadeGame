"""Extend both intact v20 masters, preserving every inherited paragraph/image."""
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
 fig=plt.figure(figsize=(8.6,5.6),facecolor='#11212a');fig.text(.045,.94,'THE UNMADE   PLAYER FACING RELIABILITY',fontsize=15,weight='bold',color='white');fig.text(.045,.88,'Six source fixes shared across the nine realm adventures',fontsize=10,color='#c2d4d9')
 cards=[('Recovery','Same profile and action twice','Never overwrite the target first'),('Controls','Original tokens interpreted once','Mapped labels remain intact'),('Trading','Alive, nearby, consenting, visible','Buying and selling share the gate'),('Character','Name and eight saved aspects','Final identity assets remain pending'),('Injury','Stagger outranks lower cues','Death takes priority'),('Audit','Production-rule failure cases','Adapters and callbacks checked')]
 for i,(title,first,last) in enumerate(cards):
  ax=fig.add_axes([.045+(i%3)*.32,.56-(i//3)*.29,.30,.22],facecolor='#1b343e');ax.set_xticks([]);ax.set_yticks([])
  for sp in ax.spines.values():sp.set_color('#57747a')
  ax.text(.06,.78,title,fontsize=12,color='#efc993',weight='bold',transform=ax.transAxes)
  ax.text(.06,.57,'\n'.join(textwrap.wrap(first,27)),fontsize=9,color='white',transform=ax.transAxes,va='top')
  ax.text(.06,.25,'\n'.join(textwrap.wrap(last,30)),fontsize=8,color='#c2d4d9',transform=ax.transAxes,va='top')
 fig.text(.045,.17,'Crossings  Saltwake  Cinderhold  Drevlach  Orravane',fontsize=10,color='white');fig.text(.045,.125,'Vathless  Eillun  Tharniv  Auvren',fontsize=10,color='white');fig.text(.045,.065,'Eight menu pages  /  Twenty-eight actions  /  Unreal acceptance deferred',fontsize=9,color='#c2d4d9')
 buf=io.BytesIO();fig.savefig(buf,format='png',dpi=180,facecolor=fig.get_facecolor(),bbox_inches='tight');plt.close(fig)
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
 art=a.out_dir/'THE_UNMADE_v20_Player_Facing_Reliability_Atlas.png';atlas(art);source=(ROOT/'docs/lore/CHAPTER_40_PLAYER_FACING_RELIABILITY.md').read_text().splitlines();inputs=[]
 for prefix in ['THE_UNMADE_Complete_Illustrated_Lore_Bible_Volume_X','THE_UNMADE_Volume_X_Illustrated']:
  previous=a.previous_books/(prefix+'_Complete_RPG_Loops_v19.docx');doc=Document(previous);old=len(doc.inline_shapes);doc.add_page_break();i=0
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
    if line.startswith('### 40.1'):
     para=doc.add_paragraph();para.alignment=WD_ALIGN_PARAGRAPH.CENTER;para.add_run().add_picture(str(art),width=Inches(6.3));doc.add_paragraph('Figure X 40 Shared safeguards apply across all nine realms. Native identity choices retain prototype presentation and require engine acceptance.',style='Caption')
   else:
    para=doc.add_paragraph(line);para.paragraph_format.line_spacing=1.06 if prefix.endswith("Illustrated") else 1.10;para.paragraph_format.space_after=Pt(2 if prefix.endswith("Illustrated") else 3.5);continue
   for run in h.runs:run.font.color.rgb=RGBColor(0,0,0)
  doc.core_properties.title='The Unmade Player Facing Reliability v20';target=a.out_dir/(prefix+'_Player_Facing_Reliability_v20.docx');buf=io.BytesIO();doc.save(buf)
  with target.open('wb') as f:f.write(buf.getvalue());f.flush();os.fsync(f.fileno())
  assert len(doc.inline_shapes)==old+1
  inputs.append({'name':previous.name,'sha256':hashlib.sha256(previous.read_bytes()).hexdigest(),'retained_images':old});print(target.name,len(doc.inline_shapes))
 (a.out_dir/'v20_build_inputs.json').write_text(json.dumps(inputs,indent=2)+'\n')
if __name__=='__main__':main()
