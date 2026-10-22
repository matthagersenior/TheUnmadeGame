"""Bellwold hero-layer geometry with original, embedded, UV-mapped glTF PBR.

Visual development only; independent of canonical quests, gameplay, collision,
and final Unreal materials. All textures are generated locally, no assets
downloaded and no third-party imagery or AI images embedded.
"""
from __future__ import annotations
from functools import lru_cache
import math
import numpy as np
import trimesh
from PIL import Image
from trimesh.visual.material import PBRMaterial
from trimesh.visual.texture import TextureVisuals

SIZE=192

def _textures(kind: str):
    """Make seeded, deterministic weathered albedo and tangent-space normal."""
    yy,xx=np.mgrid[:SIZE,:SIZE].astype(np.float32)
    seed={"stone":1311,"timber":3119,"copper":7283}[kind]
    rng=np.random.default_rng(seed)
    grains=rng.standard_normal((SIZE,SIZE)).astype(np.float32)
    if kind=="stone":
        # Irregular ashlar blocks, staggered mortar, rain and soot deposits.
        columns=5; rows=8
        xphase=np.mod(xx+(np.floor(yy/(SIZE/rows))%2)*SIZE/(columns*2),SIZE/columns)
        yphase=np.mod(yy,SIZE/rows)
        mortar=(xphase<2.0)|(yphase<2.4)
        chips=np.sin(xx*.42+yy*.13)*np.cos(yy*.26-xx*.04)
        wear=(grains*12+chips*13-yy/SIZE*29).astype(np.float32)
        color=np.stack((110+wear,116+wear*.98,120+wear*1.08),axis=2)
        color[mortar]*=.46
        height=.6*chips+grains*.17-mortar.astype(np.float32)*3.2
        metallic=0.02;roughness=.93
    elif kind=="timber":
        # Reused dark salvaged timber: lengthwise fiber, knots, end checking.
        knots=np.sin((xx-(SIZE*.5+9*np.sin(yy*.063)))*.12+yy*.61)
        thread=np.sin(xx*.52+np.sin(yy*.034)*2.2)
        shear=np.sin(yy*.18-xx*.06)
        wear=11*knots+13*thread+4*shear+8*grains
        color=np.stack((95+wear,69+wear*.73,49+wear*.48),axis=2)
        height=knots*.64+thread*.3+grains*.12
        metallic=0;roughness=.86
    elif kind=="copper":
        # Green-oxidized copper, with surviving hammered ochre edges.
        patina=np.sin(xx*.061)*np.cos(yy*.117)+.45*np.sin(xx*.19+yy*.21)
        bright=(patina> .64)
        wear=grains*17+patina*23
        color=np.stack((66+wear,119+wear*1.06,111+wear*.87),axis=2)
        color[bright]=np.stack((139+wear[bright],108+wear[bright]*.8,62+wear[bright]*.4),axis=1)
        height=np.sin(xx*.32)*np.sin(yy*.27)*.3+patina*.56+grains*.20
        metallic=.74;roughness=.54
    else:raise ValueError(kind)
    rgb=np.uint8(np.clip(color,0,255))
    gx=np.gradient(height,axis=1);gy=np.gradient(height,axis=0)
    nx=-gx*.22;ny=-gy*.22;nz=np.ones_like(nx)
    norm=np.sqrt(nx*nx+ny*ny+nz*nz)
    normal=np.uint8(np.clip(np.stack(((nx/norm*.5+.5)*255,(ny/norm*.5+.5)*255,(nz/norm*.5+.5)*255),axis=2),0,255))
    return Image.fromarray(rgb,"RGB"),Image.fromarray(normal,"RGB"),metallic,roughness

@lru_cache(maxsize=3)
def _material(kind: str):
    albedo,normal,metallic,roughness=_textures(kind)
    return PBRMaterial(name={
        "stone":"Bellwold Weathered Stone",
        "timber":"Bellwold Salvaged Timber",
        "copper":"Bellwold Verdigris Copper"}[kind],
        baseColorTexture=albedo,normalTexture=normal,
        metallicFactor=metallic,roughnessFactor=roughness,
        baseColorFactor=[255,255,255,255],doubleSided=False)

def _attach(k,name,mesh,kind):
    """UV unwrap per triangle to remove ambiguous shared-corner seams."""
    mesh=mesh.copy()
    vertices=np.asarray(mesh.vertices)[np.asarray(mesh.faces)].reshape(-1,3)
    faces=np.arange(len(vertices)).reshape(-1,3)
    normals=np.repeat(np.asarray(mesh.face_normals),3,axis=0)
    maxdim=np.argmax(np.abs(normals),axis=1)
    uv=np.empty((len(vertices),2),dtype=np.float32)
    # Stable tri-planar projections assigned per face, packed into 0..1.
    for normal_axis,(i,j) in ((0,(2,1)),(1,(0,2)),(2,(0,1))):
        indices=maxdim==normal_axis
        if indices.any():
            for axis,col in ((i,0),(j,1)):
                vals=vertices[indices,axis]
                extent=max(float(np.ptp(vals)),.001)
                uv[indices,col]=(vals-float(vals.min()))/extent
    unwrapped=trimesh.Trimesh(vertices=vertices,faces=faces,process=False)
    unwrapped.visual=TextureVisuals(uv=np.clip(uv,0,1),material=_material(kind))
    key=f"{k.count:04d}_hero_pbr_{name}"
    k.count+=1
    k.scene.add_geometry(unwrapped,geom_name=key,node_name=key)

def _box(k,name,x,y,z,w,h,d,kind):
    geo=trimesh.creation.box(extents=[w,h,d])
    geo.apply_translation([x,y,z])
    _attach(k,name,geo,kind)

def _beam(k,name,a,b,radius,kind,sections=14):
    a=np.asarray(a,dtype=float);b=np.asarray(b,dtype=float)
    delta=b-a;length=np.linalg.norm(delta)
    if length<.02:raise ValueError("zero length hero beam")
    mesh=trimesh.creation.cylinder(radius=radius,height=length,sections=sections)
    mesh.apply_transform(trimesh.geometry.align_vectors([0,0,1],delta/length))
    mesh.apply_translation((a+b)/2)
    _attach(k,name,mesh,kind)

def _roof(k,name,x,z,y,w,d,rise,kind):
    # Sloped, two-pitch closed canopy; no flat box substituting for a roof.
    a=w*.5;b=d*.5
    vertices=np.array([
        [x-a,y,z-b],[x,y+rise,z-b],[x+a,y,z-b],
        [x-a,y,z+b],[x,y+rise,z+b],[x+a,y,z+b]],dtype=float)
    faces=np.array([[0,1,2],[3,5,4],[0,3,4],[0,4,1],
                    [1,4,5],[1,5,2],[0,2,5],[0,5,3]])
    _attach(k,name,trimesh.Trimesh(vertices=vertices,faces=faces,process=False),kind)

def add_bellwold_hero(k):
    """Layer a place-specific authored facade onto the canonical refuge/tower.

    The yoke must stay EMPTY. This code never instantiates a hanging bell, and
    its signifying absence is more important than any invented sculpture.
    """
    # Gray-stone refuge: chipped courses and an asymmetric public arch frame.
    # Position aligns with the existing refuge at x=-4, z=-3, forward +Z.
    for side,x in enumerate((-5.55,-2.45)):
        _box(k,f"refuge_stone_pier_{side}",x,1.40,-1.19,.54,2.8,.60,"stone")
        _box(k,f"refuge_stone_foot_{side}",x,.21,-1.08,.88,.42,.84,"stone")
    _box(k,"refuge_stone_arch_lintel",-4,2.88,-1.17,3.80,.30,.75,"stone")
    for row,y in enumerate((.60,1.12,1.65,2.17)):
        for i in range(3):
            x=-5.28+i*.43+(row%2)*.12
            _box(k,f"refuge_stone_course_west_{row}_{i}",x,y,-.855,.35,.29,.20,"stone")
            x=-2.75+i*.24-(row%2)*.07
            _box(k,f"refuge_stone_course_east_{row}_{i}",x,y,-.855,.18,.27,.17,"stone")
    # Curved-looking arched voussoirs are radial wedges, not a texture of an arch.
    for i in range(9):
        angle=math.pi-i*(math.pi/8)
        px=-4+1.15*math.cos(angle);py=1.82+.88*math.sin(angle)
        _beam(k,f"arch_stone_voussoir_{i}",(px,py,-.83),
              (px+(.18 if i<4 else -.18),py+.22,-.83),.18,"stone",10)
    for i,x in enumerate((-5.55,-2.45)):
        _beam(k,f"timber_public_stall_post_{i}",(x,.1,.39),(x,2.40,.39),.14,"timber")
        _beam(k,f"timber_diagonal_knee_brace_{i}",(x,1.9,.40),(-4.6 if i==0 else -3.4,2.46,.12),.10,"timber")
    _beam(k,"timber_refuge_porch_lintel",(-5.85,2.50,.34),(-2.14,2.50,.34),.18,"timber")
    for i in range(5):
        x=-5.58+i*.79
        _beam(k,f"timber_refuge_ridge_rafter_{i}",(x,2.48,.35),(x,3.59,-1.22),.12,"timber")
    _roof(k,"timber_asymmetrical_refuge_roof",-4,-2.35,3.55,4.48,3.45,.87,"timber")
    # Salvaged timber lathing and repaired shutters, not uniform flat bands.
    for i in range(4):
        _box(k,f"timber_refuge_lath_{i}",-4.96+i*.61,2.04,.40,.13,1.2,.14,"timber")
    _box(k,"refuge_stone_public_landing",-4,.19,.63,3.4,.32,1.1,"stone")
    _box(k,"refuge_stone_wash_basin",-5.05,.44,.71,.95,.27,.53,"stone")
    # Patinated surviving copper. Visual absence of a hanging bell remains.
    for i,x in enumerate((3.1,5.9)):
        _beam(k,f"copper_tower_reinforcing_strap_{i}",(x,5.65,-5),(x,8.35,-5),.16,"copper")
        _box(k,f"copper_tower_heel_clasp_{i}",x,5.58,-4.98,.88,.38,.79,"copper")
        _box(k,f"copper_tower_moss_streak_{i}",x,7.00,-4.58,.24,1.1,.18,"copper")
    _beam(k,"copper_empty_yoke_crosspiece",(3.1,8.31,-4.96),(5.9,8.31,-4.96),.20,"copper")
    for i,x in enumerate((3.85,5.15)):
        _box(k,f"copper_empty_yoke_rivet_{i}",x,8.39,-4.72,.18,.20,.12,"copper")
    _box(k,"copper_empty_yoke_absent_hanger_plate",4.5,8.01,-4.91,.46,.13,.24,"copper")
    # No cast bell, clapper, pseudo-ghost bell or implied lore resolution.
