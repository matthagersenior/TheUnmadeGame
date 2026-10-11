"""Extend both intact v16 masters, preserving every inherited paragraph/image."""
from pathlib import Path
import argparse,hashlib,json,textwrap
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
 d=json.loads((ROOT/'Authoring/adventure_depth_v16.json').read_text());fig=plt.figure(figsize=(8.2,7.2),facecolor='#11212a')
 fig.text(.045,.96,'THE UNMADE   NINE DISTINCT DUNGEON ROUTES',fontsize=15,weight='bold',color='white')
 fig.text(.045,.918,'Six chambers each  /  Boss entry and northern return  /  Optional side alcoves',fontsize=10,color='#c2d4d9')
 for i,c in enumerate(d['realms']):
  x=.04+(i%3)*.32;y=.68-(i//3)*.285
  ax=fig.add_axes([x,y,.30,.19],facecolor='#1b343e');ax.set_xticks([]);ax.set_yticks([])
  for sp in ax.spines.values():sp.set_color('#57747a')
  pts=c['room_grid'];xx=[p[0] for p in pts];yy=[p[1] for p in pts]
  ax.plot(xx,yy,color='#9bc8c7',linewidth=3,zorder=1)
  ax.scatter(xx,yy,s=320,marker='s',c=['#c1a47b']*5+['#ae6856'],zorder=2)
  for n,(u,v) in enumerate(pts):ax.text(u,v,str(n+1),ha='center',va='center',fontsize=10,color='#10212a',weight='bold')
  ax.set_xlim(min(xx)-.6,max(xx)+.6);ax.set_ylim(min(yy)-.6,max(yy)+.6)
  ax.set_title(c['city'],fontsize=12,color='#efc993',weight='bold',loc='left',pad=5)
  labels=['Bells 1 3 2','Outer pumps at low water','Three relays before 18 seconds','Weights total five','Upper walkway anchors','Shutters 1 2 3','Unobserved lamp readings','Roots 3 1 2','Boundary mirrors 2 and 3']
  ax.text(0,-.12,labels[i],fontsize=8.5,color='white',transform=ax.transAxes,va='top')
 fig.text(.045,.028,'29 settlements with services  /  9 personal accords  /  57 keyed prototype clips',fontsize=10,color='#dce6e8')
 fig.savefig(path,dpi=180,facecolor=fig.get_facecolor(),bbox_inches='tight');plt.close(fig)
def add_table(doc,lines):
 rows=[[x.strip() for x in l.strip('|').split('|')] for l in lines if not l.startswith('| ---')];t=doc.add_table(rows=0,cols=4);t.style='Table Grid';t.autofit=False;w=[1.1,1.8,.85,2.45] if rows[0][0]=="City" else [1.2,1.8,1.8,1.4]
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
 art=a.out_dir/'THE_UNMADE_v16_Adventure_Depth_Atlas.png';atlas(art);source=(ROOT/'docs/lore/CHAPTER_36_ADVENTURE_DEPTH.md').read_text().splitlines();inputs=[]
 for prefix in ['THE_UNMADE_Complete_Illustrated_Lore_Bible_Volume_X','THE_UNMADE_Volume_X_Illustrated']:
  previous=a.previous_books/(prefix+'_World_Identity_v15.docx');doc=Document(previous);old=len(doc.inline_shapes);doc.add_page_break();i=0
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
    if line.startswith('### 36.1'):
     para=doc.add_paragraph();para.alignment=WD_ALIGN_PARAGRAPH.CENTER;para.add_run().add_picture(str(art),width=Inches(6.3));doc.add_paragraph('Figure X 36 Nine distinct dungeon room layouts with optional exploration demands. Source atlas; engine testing deferred.',style='Caption')
   else:
    para=doc.add_paragraph(line);para.paragraph_format.line_spacing=1.10;para.paragraph_format.space_after=Pt(3.5);continue
   for run in h.runs:run.font.color.rgb=RGBColor(0,0,0)
  doc.core_properties.title='The Unmade Adventure Depth v16';target=a.out_dir/(prefix+'_Adventure_Depth_v16.docx');doc.save(target);assert len(doc.inline_shapes)==old+1
  inputs.append({'name':previous.name,'sha256':hashlib.sha256(previous.read_bytes()).hexdigest(),'retained_images':old});print(target.name,len(doc.inline_shapes))
 (a.out_dir/'v16_build_inputs.json').write_text(json.dumps(inputs,indent=2)+'\n')
if __name__=='__main__':main()
