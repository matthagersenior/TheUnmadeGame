"""UV/PBR residential finish and terrain relief for each culturally distinct settlement.

This visual-development module gives all modeled homes individual surface maps
and makes district foundations gently sculpted. It does not assert physical
pathfinding clearance, interior floorplans, final Unreal rendering, or canon
world coordinates. No external or third-party imagery is used.
"""
from __future__ import annotations
import math
import numpy as np
import trimesh
import importlib.util
from pathlib import Path
_spec=importlib.util.spec_from_file_location('district_realm_pbr',Path(__file__).with_name('realm_hero_pbr.py'))
pbr=importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(pbr)

# The existing models provide city-specific silhouette and functional features.
# These finishes are supplemental exterior skins fitted just outside the
# existing walls and sculpted roof, not a replacement for inhabited structures.
CITY_RHYTHM={
 "Crossings":(1.22,.9,.16),
 "Bellwold":(1.1,.8,.13),
 "Paperhaven":(1.65,.55,.11),
 "Saltwake":(1.35,1.27,.19),
 "Cinderhold":(.77,.87,.29),
 "Drevlach":(1.42,.56,.16),
 "Orravane":(.55,1.61,.25),
 "Vathless":(.93,1.28,.27),
 "Eillun":(1.20,.77,.15),
 "Tharniv":(.65,1.09,.24),
 "Auvren":(1.83,.62,.32),
}

def _sheet(k,city,name,vertices,kind="structure"):
    mesh=trimesh.Trimesh(vertices=vertices,faces=[[0,1,2],[0,2,3]],process=False)
    pbr._put(k,city,"district_pbr_"+name,mesh,kind)

def _pitch(k,city,idx,x,y,z,w,d,rise):
    a=w*.556;b=d*.567
    pts=[
     [x-a,y,z-b],[x,y+rise,z-b],[x+a,y,z-b],
     [x-a,y,z+b],[x,y+rise,z+b],[x+a,y,z+b]]
    faces=[[0,1,2],[3,5,4],[0,3,4],[0,4,1],
           [1,4,5],[1,5,2],[0,2,5],[0,5,3]]
    roof=trimesh.Trimesh(vertices=pts,faces=faces,process=False)
    pbr._put(k,city,"district_pbr_pitched_roof_"+str(idx),roof,"roof")

def _structure_beam(k,city,name,a,b,radius=.07,kind="fitting"):
    pbr.beam(k,city,"district_pbr_"+name,a,b,radius,kind)

def window_height(h):
    """Keep the two upper windows above the tallest door even in small homes."""
    return max(h*.75,1.9)

def apertured_facade_mesh(x,y,z,w,h,door_side):
    """Return actual face geometry surrounding three rectangular openings.

    A window or door location is excluded from the triangulated visible
    surface, not covered with a flat decorative texture. A backing dwelling
    shell still exists: these are deep visual reveals, not enterable interiors.
    """
    if door_side not in (-1,1):raise ValueError("Door side is +/-1")
    door=(x+door_side*.40-.31,x+door_side*.40+.31,
          y+.10,y+1.57)
    windows=[(x+side*w*.33-.17,x+side*w*.33+.17,
              y+window_height(h)-.20,y+window_height(h)+.20)
             for side in (-1,1)]
    openings=[door,*windows]
    xs=sorted(set([x-w*.5,x+w*.5]+[q for a,b,_,_ in openings for q in (a,b)]))
    ys=sorted(set([y+.08,y+h-.06]+[q for _,_,a,b in openings for q in (a,b)]))
    faces=[];vertices=[]
    for i in range(len(xs)-1):
        for j in range(len(ys)-1):
            x0,x1=xs[i:i+2];y0,y1=ys[j:j+2]
            xm=(x0+x1)*.5;ym=(y0+y1)*.5
            if x1-x0<1e-5 or y1-y0<1e-5:continue
            if any(a<xm<b and low<ym<high for a,b,low,high in openings):
                continue
            v=len(vertices)
            vertices.extend(((x0,y0,z),(x1,y0,z),(x1,y1,z),(x0,y1,z)))
            faces.extend(((v,v+1,v+2),(v,v+2,v+3)))
    return trimesh.Trimesh(vertices=np.asarray(vertices),faces=np.asarray(faces),process=False)

def _facade_recesses(k,city,idx,x,y,z,w,h,d):
    """Frame actual visual openings and create a distinct street-facing entry."""
    side=(-1 if idx%2 else 1)
    center=x+side*.40
    glass_y=y+window_height(h)
    # Surface at z+d*.5+.025. Frames project +.09 while pane/door panels sit
    # at +.007, in front of underlying colored wall but recessed into PBR stone.
    face_z=z+d*.5+.025
    pbr._put(k,city,"district_pbr_apertured_facade_"+str(idx),
             apertured_facade_mesh(x,y,face_z,w,h,side),"structure")
    panels=[]
    def pane(x0,x1,y0,y1,depth):
        v=len(panels)*4
        # Four separate triangles per panel are packed in one GLB geometry.
        return ([(x0,y0,depth),(x1,y0,depth),(x1,y1,depth),(x0,y1,depth)],
                ((0,1,2),(0,2,3)))
    vertices=[];faces=[]
    for a,b,low,high in [(center-.27,center+.27,y+.12,y+1.52)]+[
            (x+side*w*.33-.13,x+side*w*.33+.13,glass_y-.16,glass_y+.16)
            for side in (-1,1)]:
        v=len(vertices)
        vertices.extend(((a,low,face_z-.014),(b,low,face_z-.014),
                         (b,high,face_z-.014),(a,high,face_z-.014)))
        faces.extend(((v,v+1,v+2),(v,v+2,v+3)))
    backing=trimesh.Trimesh(vertices=vertices,faces=faces,process=False)
    pbr._put(k,city,"district_pbr_recessed_glazing_"+str(idx),backing,"fitting")
    # Load-bearing stone/timber framing around a visually recessed door and
    # windows. Merge per-house pieces to keep WebGL draw calls manageable.
    members=[]
    def slab(name,xyz,size):
        m=trimesh.creation.box(extents=size);m.apply_translation(xyz)
        members.append(m)
    slab("left_door", (center-.35,y+.84,face_z+.068),(.10,1.53,.12))
    slab("right_door",(center+.35,y+.84,face_z+.068),(.10,1.53,.12))
    slab("door_lintel",(center,y+1.62,face_z+.068),(.84,.13,.14))
    slab("door_sill",(center,y+.10,face_z+.068),(.83,.16,.17))
    for side in (-1,1):
        wc=x+side*w*.33
        slab("window_sill",(wc,glass_y-.24,face_z+.065),(.50,.10,.15))
        slab("window_lintel",(wc,glass_y+.24,face_z+.065),(.50,.10,.13))
        for edge in (-1,1):
            slab("window_reveal",(wc+edge*.22,glass_y,face_z+.065),(.09,.45,.12))
    frames=trimesh.util.concatenate(members)
    pbr._put(k,city,"district_pbr_entrance_casing_"+str(idx),frames,"structure")
    # City-specific entry apron with two real height changes. These are
    # illustrative access steps, not verified walkable navigation/collision.
    step_mats=[]
    for n in range(3):
        step=trimesh.creation.box(extents=(.90,.12,1.14-n*.23))
        step.apply_translation((center,y+.09+n*.11,face_z+.88-n*.17))
        step_mats.append(step)
    threshold=trimesh.util.concatenate(step_mats)
    pbr._put(k,city,"district_pbr_public_threshold_"+str(idx),threshold,"fitting")

def finish_dwelling(k,city,idx,x,y,z,w,h,d):
    """Layer UV-resolved material across each individual inhabited exterior."""
    if city not in CITY_RHYTHM:raise ValueError(city)
    front=z+d*.5+.025
    back=z-d*.5-.025
    left=x-w*.5-.024
    top=y+h-.06
    # A textured ashlar/timber/industrial outer finish; use actual planar
    # geometry with stable UVs, not one material token in the manifest.
    # Apertured façade replaces the old opaque PBR sheet, which concealed
    # the windows and the door panels behind it.
    _facade_recesses(k,city,idx,x,y,z,w,h,d)
    # Independent side sheathing gives the camera both front and oblique detail.
    # Keep normal outward (-X) for correct one-sided glTF rendering.
    _sheet(k,city,"weather_side_"+str(idx),[
      [left,y+.08,z-d*.49],[left,y+.08,z+d*.49],
      [left,top,z+d*.49],[left,top,z-d*.49]])
    # Existing architecture has a sloped gable at y+h+.13. A subtly raised
    # UV-mapped physical canopy conforms to its same profile, not a roof decal.
    rise=.72+.19*(idx%4)
    if city in ("Saltwake","Tharniv","Orravane"):rise=1.05+.11*(idx%5)
    if city in ("Paperhaven","Drevlach","Vathless"):rise=.48+.17*(idx%3)
    _pitch(k,city,idx,x,y+h+.18,z,w,d,rise+.055)
    # Exposed exterior repairs/utility structures differ by culture.
    # Repeatability is per material culture; roof profile and placement vary.
    _structure_beam(k,city,"eave_edge_"+str(idx),
         (x-w*.53,y+h+.15,z+d*.57),
         (x+w*.53,y+h+.15,z+d*.57),.08)
    if city in ("Bellwold","Crossings","Eillun"):
        for side in (-1,1):
            _structure_beam(k,city,f"public_porch_post_{idx}_{side}",
                (x+side*w*.41,y+.16,front+.52),
                (x+side*w*.41,y+h*.84,front+.52),.085,"structure")
        _structure_beam(k,city,"guest_awning_beam_"+str(idx),
            (x-w*.46,y+h*.84,front+.52),
            (x+w*.46,y+h*.84,front+.52),.08)
    elif city=="Paperhaven":
        for j in range(3):
            pbr.box(k,city,f"district_pbr_indexed_folio_sill_{idx}_{j}",
                (x-w*.32+j*w*.32,y+1.18,front+.085),(.37,.11,.18),"fitting")
    elif city=="Saltwake":
        for side in (-1,1):
            _structure_beam(k,city,f"inverted_boat_rib_{idx}_{side}",
                (x+side*w*.49,y+h+.14,z-.65*d),
                (x, y+h+rise+.42,z),.085,"fitting")
        pbr.box(k,city,"district_pbr_rain_trough_"+str(idx),
                (x,y+h-.2,front+.22),(w*.85,.16,.38))
    elif city=="Cinderhold":
        pbr.box(k,city,"district_pbr_ash_drain_"+str(idx),
            (x,y+.14,front+.57),(w*.7,.13,.9),"fitting")
        _structure_beam(k,city,"chimney_thermal_strap_"+str(idx),
            (x+w*.28,y+h+.24,z),
            (x+w*.28,y+h+rise+1.21,z),.10,"fitting")
    elif city=="Drevlach":
        for side in (-1,1):
            _structure_beam(k,city,f"canal_lock_pier_{idx}_{side}",
                (x+side*w*.46,y+.21,front+.40),
                (x+side*w*.46,y+h*.83,front+.40),.09)
        pbr.box(k,city,"district_pbr_date_ledger_"+str(idx),
                (x,y+1.47,front+.12),(w*.42,.61,.15))
    elif city=="Orravane":
        for side in (-1,1):
            _structure_beam(k,city,f"cable_hang_{idx}_{side}",
                (x+side*w*.52,y+h+.16,z),
                (x+side*w*.73,y+h+5.2,z-.35),.085,"fitting")
        pbr.box(k,city,"district_pbr_acoustic_balcony_"+str(idx),
                (x,y+.74,front+.55),(w+.3,.16,1.1))
    elif city=="Vathless":
        for j in range(3):
            pbr.box(k,city,f"district_pbr_quarry_retaining_step_{idx}_{j}",
                (x+w*.57+j*.32,y+.21+j*.29,front+.4+j*.29),
                (1.15,.29,.79))
    elif city=="Tharniv":
        for side in (-1,1):
            _structure_beam(k,city,f"root_borne_support_{idx}_{side}",
                (x+side*w*.49,y+.15,z),
                (x+side*w*.2,y+h+rise+.28,z),.17,"structure")
    elif city=="Auvren":
        pbr.box(k,city,"district_pbr_witness_frame_"+str(idx),
            (x,y+h*.78,front+.06),(w*.65,.16,.25),"fitting")
        for side in (-1,1):
            _structure_beam(k,city,f"second_stone_threshold_{idx}_{side}",
                (x+side*w*.55,y+.22,front+.30),
                (x+side*w*.55,y+h*.81,front+.30),.11)
    # Every roof has distinct surface wear, not just a uniform PBR material:
    # vary the seams and edges by building and culture.
    pitch=math.atan2(rise,max(w*.55,.01))
    width=w*.43
    for j in range(2):
        offset=(-.18 if j==0 else .18)*w
        _structure_beam(k,city,f"weathered_roof_seam_{idx}_{j}",
            (x+offset-width*.11,y+h+.25,z-d*.55),
            (x+offset,y+h+.25+math.tan(pitch)*width*.11,z+d*.56),
            .028,"roof")

def terrain_surface_height(city,district,x,z,cx,cz,floor_y):
    """Exact source function shared by sculpted relief and foundation supports.

    Coordinates are 3D-preview Y-up metres, not collision-ready landscape data.
    """
    if city not in CITY_RHYTHM:raise ValueError(city)
    freq_a,freq_b,amplitude=CITY_RHYTHM[city]
    x=float(x)-float(cx);z=float(z)-float(cz)
    if city=="Vathless":
        elevation=.12*np.floor((x+7)/2.5)+amplitude*np.sin(z*.44)
    elif city=="Drevlach":
        elevation=amplitude*(.5-abs(np.sin(x*.56)))*1.6+.06*np.cos(z*.5)
    elif city=="Saltwake":
        elevation=amplitude*(np.sin(x*.5+z*.26)**2)-.07*np.cos(z*.7)
    elif city=="Orravane":
        elevation=amplitude*(np.sin(x*.4)+np.cos(z*.47))+.14*np.sin(x*.18*z*.1)
    elif city=="Tharniv":
        elevation=amplitude*(np.sin(x*.38)**2+np.cos(z*.42)**2)*.64
    elif city=="Auvren":
        elevation=amplitude*(np.sign(np.sin(x*.47+z*.33))*.40+
            np.sin(z*.4))
    else:
        elevation=amplitude*(np.sin(x*freq_a*.37+district*.37)
           *np.cos(z*freq_b*.41)+.33*np.sin(x*.13*z*.12))
    elevation+=.014*district*(x/6.9)+.018*np.sin(x*.7+z*.31+district)
    return float(floor_y+.2+elevation)

def finish_district_terrain(k,city,district,cx,cz,floor_y):
    """Unique physical relief using the EXACT elevations of house supports."""
    if city not in CITY_RHYTHM:raise ValueError(city)
    count=15
    xs=np.linspace(-6.9,6.9,count)
    zs=np.linspace(-6.9,6.9,count)
    vertices=[]
    for z in zs:
        for x in xs:
            ax=float(cx+x);az=float(cz+z)
            vertices.append((ax,terrain_surface_height(city,district,ax,az,cx,cz,floor_y),az))
    faces=[]
    for iz in range(count-1):
        for ix in range(count-1):
            a=iz*count+ix;b=a+1;c=a+count;d=c+1
            faces.extend(([a,b,d],[a,d,c]))
    mesh=trimesh.Trimesh(vertices=vertices,faces=faces,process=False)
    pbr._put(k,city,f"district_terrain_relief_{district}",mesh,"structure")
