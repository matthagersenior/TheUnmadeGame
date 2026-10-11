"""Render original glTF skinned mesh poses for offline visual QA. Requires numpy/matplotlib."""
from pathlib import Path
import argparse,base64,json,struct
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from PIL import Image,ImageDraw

def read_accessor(d,blob,i):
 a=d['accessors'][i];v=d['bufferViews'][a['bufferView']];dim={'SCALAR':1,'VEC3':3,'VEC4':4,'MAT4':16}[a['type']];dtype={5126:'<f4',5123:'<u2',5125:'<u4'}[a['componentType']];return np.frombuffer(blob,dtype=dtype,count=a['count']*dim,offset=v.get('byteOffset',0)+a.get('byteOffset',0)).reshape(-1,dim)
def preview(source,out):
 out.mkdir(parents=True,exist_ok=True)
 for kind in ['Resident','Beast','Wraith']:
  d=json.loads((source/('SK_Unmade'+kind+'.gltf')).read_text());blob=base64.b64decode(d['buffers'][0]['uri'].split(',')[1]);skin=d['skins'][0];prim=d['meshes'][0]['primitives'][0];attrs=prim['attributes'];pos=read_accessor(d,blob,attrs['POSITION']);joints=read_accessor(d,blob,attrs['JOINTS_0']).astype(int);weights=read_accessor(d,blob,attrs['WEIGHTS_0']);faces=read_accessor(d,blob,prim['indices']).reshape(-1,3);inv=read_accessor(d,blob,skin['inverseBindMatrices']).reshape(-1,4,4).transpose(0,2,1);parents={c:i for i,n in enumerate(d['nodes']) for c in n.get('children',[])}
  sheet=Image.new('RGB',(1550,len(d['animations'])*175),'white');draw=ImageDraw.Draw(sheet)
  for row,anim in enumerate(d['animations']):
   draw.text((10,row*175+5),anim['name'],fill='black')
   for column,key in enumerate([0,1,2,4]):
    rotation={ch['target']['node']:read_accessor(d,blob,anim['samplers'][ch['sampler']]['output'])[key] for ch in anim['channels']};globals={}
    def transform(i):
     if i in globals:return globals[i]
     n=d['nodes'][i];m=np.eye(4);x,y,z,w=rotation.get(i,[0,0,0,1]);m[:3,:3]=[[1-2*(y*y+z*z),2*(x*y-z*w),2*(x*z+y*w)],[2*(x*y+z*w),1-2*(x*x+z*z),2*(y*z-x*w)],[2*(x*z-y*w),2*(y*z+x*w),1-2*(x*x+y*y)]];m[:3,3]=n.get('translation',[0,0,0]);globals[i]=(transform(parents[i])@m) if i in parents else m;return globals[i]
    matrices=np.array([transform(i)@inv[k] for k,i in enumerate(skin['joints'])]);v=np.c_[pos,np.ones(len(pos))];skinned=sum(weights[:,k,None]*(matrices[joints[:,k]]@v[:,:,None])[:,:,0] for k in range(4))[:,:3];vertices=skinned[:,[0,2,1]]
    fig=plt.figure(figsize=(3.5,1.5),dpi=100);ax=fig.add_subplot(projection='3d');ax.add_collection3d(Poly3DCollection(vertices[faces],facecolors='#6a9da9',edgecolors='#314b55',linewidths=.15));ax.set(xlim=(-1.3,1.3),ylim=(-1.3,1.3),zlim=(-.4,2.6));ax.set_axis_off();ax.view_init(elev=15,azim=-65);fig.subplots_adjust(left=0,right=1,bottom=0,top=1);file=out/f'{kind}_{row}_{key}.png';fig.savefig(file);plt.close(fig)
    with Image.open(file) as im:sheet.paste(im,(120+column*350,row*175+20))
    file.unlink()
  sheet.save(out/('SK_Unmade'+kind+'_Pose_Contact_Sheet.png'))
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--source',type=Path,required=True);p.add_argument('--out',type=Path,required=True);a=p.parse_args();preview(a.source,a.out)
