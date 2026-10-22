"""Terrain-grounded civic construction for The Unmade's eleven unique settlements.

Generate local structural footings and graded residential approaches, taking
the exact height sampler used to sculpt the terrain. These are UV/PBR art
surfaces: not Unreal collision, navmesh, accessibility, water simulation or
canonical traversal promises.
"""
from __future__ import annotations
import importlib.util
from pathlib import Path
import math
import numpy as np
import trimesh

spec=importlib.util.spec_from_file_location(
    "terrain_joinery_realm_pbr",Path(__file__).with_name("realm_hero_pbr.py"))
pbr=importlib.util.module_from_spec(spec)
spec.loader.exec_module(pbr)

# Building methods differ by culture: larger timber walkboards at Saltwake,
# high suspended platforms in Orravane, bearing cut-stone in Vathless, roots
# in Tharniv, and narrower threshold pathways elsewhere.
FORM={
 "Crossings":(.94,.20),"Bellwold":(1.12,.16),"Paperhaven":(.84,.10),
 "Saltwake":(1.23,.24),"Cinderhold":(.90,.11),"Drevlach":(.95,.19),
 "Orravane":(.82,.31),"Vathless":(1.04,.09),"Eillun":(1.14,.13),
 "Tharniv":(1.02,.27),"Auvren":(.85,.12),
}

def _box(at,size):
    geo=trimesh.creation.box(extents=np.asarray(size,dtype=float))
    geo.apply_translation(at)
    return geo

def join_house_to_relief(k,city,idx,house_x,house_y,house_z,w,d,
                         district,cx,cz,floor_y,sample_height):
    """Add two genuinely physical, textured source geometry objects per home.

    Four individual footing locations use the same sampled relief beneath
    them. The graded walkway blends the local civic ground to the residential
    doorway without claiming a player-walkable route.
    """
    if city not in FORM:raise ValueError("Unknown settlement: "+str(city))
    if not callable(sample_height):raise TypeError("Terrain sampler required")
    supports=[]
    for corner,(side_x,side_z) in enumerate(((-1,-1),(-1,1),(1,-1),(1,1))):
        xx=house_x+side_x*w*.41;zz=house_z+side_z*d*.39
        ground=sample_height(city,district,xx,zz,cx,cz,floor_y)
        house_bottom=house_y-.28
        lowest=min(ground-.16,house_bottom)
        highest=max(ground+.11,house_bottom+.19)
        supports.append(_box((xx,(lowest+highest)*.5,zz),
                             (.32,highest-lowest,.34)))
        # The feet are real sizes and grounded; not decorative bars floating
        # at one universal elevation above every separate region's topography.
        supports.append(_box((xx,ground+.035,zz),(.60,.17,.55)))
    if city in ("Saltwake","Orravane","Tharniv"):
        # Distinctive braces convey the load path of locally raised houses.
        for side in (-1,1):
            bx=house_x+side*w*.40
            a=np.array((bx,house_y-.2,house_z-.38))
            b=np.array((house_x+side*(w*.55+.45),house_y+.5,house_z+.2))
            delta=b-a
            m=trimesh.creation.cylinder(radius=.09,height=np.linalg.norm(delta),sections=10)
            m.apply_transform(trimesh.geometry.align_vectors([0,0,1],delta/np.linalg.norm(delta)))
            m.apply_translation((a+b)*.5)
            supports.append(m)
    pbr._put(k,city,f"terrain_joinery_support_{idx}",
             trimesh.util.concatenate(supports),"structure")

    entrance_x=house_x+(-.4 if idx%2 else .4)
    destination=np.array((entrance_x,house_z+d*.5+.68),dtype=float)
    origin=np.array((cx,cz),dtype=float)
    vector=destination-origin
    length=np.linalg.norm(vector)
    if length<.08:
        # Exceptional location where the door happens to coincide with the
        # district civic center; still form a meaningful arrival line.
        origin=destination+np.array((-1.4,-2.5));vector=destination-origin
        length=np.linalg.norm(vector)
    tang=vector/length;normal=np.array((-tang[1],tang[0]))
    width,bend=FORM[city]
    vertices=[];faces=[]
    for segment in range(9):
        t=segment/8
        # Curves differ in amplitude and sign, but still attach exactly at
        # the district node and at the selected front-door apron.
        transverse=math.sin(math.pi*t)*(bend*(1 if idx%2 else -1))
        point=origin+vector*t+normal*transverse
        earth=sample_height(city,district,point[0],point[1],cx,cz,floor_y)
        goal=house_y+.12
        top=(earth+.14)*(1-t*t)+goal*t*t
        if city=="Vathless":
            # Quarry walkways are finely tiered, never globally flat.
            top=math.floor(top/.10)*.10+.045
        left=point-normal*(width*.5);right=point+normal*(width*.5)
        vertices.extend(((left[0],top,left[1]),(right[0],top,right[1]),
                         (left[0],top-.14,left[1]),(right[0],top-.14,right[1])))
    for segment in range(8):
        a=segment*4;b=(segment+1)*4
        # Walk surface, underside, two curb faces: an actual thin solid mesh.
        faces.extend(((a,b,a+1),(a+1,b,b+1),
                      (a+2,a+3,b+2),(a+3,b+3,b+2),
                      (a,a+2,b),(a+2,b+2,b),
                      (a+1,b+1,a+3),(a+3,b+1,b+3)))
    road=trimesh.Trimesh(vertices=np.asarray(vertices),faces=np.asarray(faces),process=False)
    pbr._put(k,city,f"terrain_joinery_walkway_{idx}",road,"fitting")
