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

# Each society has a different load-bearing language, including
# irregular dry-set stone, tapered salvaged ship ribs, hanging choir supports,
# and fluted grafted roots. These parameters shape actual mesh vertices.
PIER_CHARACTER={
 "Crossings":(.27,.19,.14,4,.08),
 "Bellwold":(.31,.20,.09,6,.05),
 "Paperhaven":(.30,.22,.04,5,.03),
 "Saltwake":(.19,.27,.23,3,.11),
 "Cinderhold":(.33,.23,.05,8,.07),
 "Drevlach":(.28,.16,.19,7,.06),
 "Orravane":(.17,.11,.26,4,.14),
 "Vathless":(.37,.26,.07,5,.03),
 "Eillun":(.29,.16,.10,6,.08),
 "Tharniv":(.33,.09,.31,3,.21),
 "Auvren":(.25,.30,.12,7,.04),
}

def _sculpted_pier(city,x,z,y0,y1,idx,corner):
    """Real 3D multi-ring tapered pier with local cultural fluting/sway."""
    r0,r1,twist,lobes,sway=PIER_CHARACTER[city]
    n=12
    verts=[]
    for ring in range(4):
        t=ring/3
        y=y0+(y1-y0)*t
        center_x=x+sway*math.sin(math.pi*t)*math.cos(idx*.49+corner)
        center_z=z+sway*math.sin(math.pi*t)*math.sin(idx*.37+corner)
        base_r=(1-t)*r0+t*r1
        for q in range(n):
            angle=2*math.pi*q/n+twist*t
            flute=1+.105*math.sin(lobes*angle+idx*.31+corner*.17)
            radial=base_r*flute
            verts.append((center_x+radial*math.cos(angle),y,
                          center_z+radial*math.sin(angle)))
    faces=[]
    for ring in range(3):
        for q in range(n):
            a=ring*n+q;b=ring*n+(q+1)%n
            c=(ring+1)*n+(q+1)%n;d=(ring+1)*n+q
            faces.extend(((a,b,c),(a,c,d)))
    for j in range(1,n-1):
        faces.append((0,j+1,j))
        faces.append((3*n,3*n+j,3*n+j+1))
    return trimesh.Trimesh(vertices=np.asarray(verts),faces=np.asarray(faces),process=False)

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
        supports.append(_sculpted_pier(city,xx,zz,lowest,highest,idx,corner))
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
