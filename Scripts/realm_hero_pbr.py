"""Nine realm and ten additional settlement PBR hero landmarks.

Original textures and meshes: deterministic, embedded GLB, no external art.
These are separate lore-led static dioramas, not Unreal gameplay, navmeshes,
cinematic camera captures, licensed imagery, or finished production models.
Bellwold keeps its separate hand-authored hero layer.
"""
from functools import lru_cache
import math
import numpy as np
import trimesh
from PIL import Image
from trimesh.visual.material import PBRMaterial
from trimesh.visual.texture import TextureVisuals

SIZE=256
# Locally distinct building materials: [primary masonry/work fabric, metal or
# timber fittings]. Material identities are place-specific, not nine color swaps.
CULTURES={
 "Bellwold":((109,116,117),(105,74,56),"refuge limestone","salvaged roof timber",.02,.0),
 "Crossings":((122,113,92),(95,143,146),"surveyor limestone","river-patinated bridge iron",.11,.72),
 "Paperhaven":((187,183,158),(94,85,88),"chalk archive stone","ink-dark storage brass",.02,.63),
 "Saltwake":((73,110,123),(127,104,78),"salt-bitten hull timber","weathered dock bronze",.02,.65),
 "Cinderhold":((76,66,61),(176,97,53),"furnace-sintered basalt","oxidized thermal copper",.03,.79),
 "Drevlach":((85,92,106),(172,152,95),"wet sluice granite","ledger-clock brass",.06,.82),
 "Orravane":((104,96,128),(139,157,171),"suspension slate","resonant choir alloy",.04,.81),
 "Vathless":((110,98,102),(172,155,140),"stratified memorial stone","veined support iron",.02,.7),
 "Eillun":((140,156,151),(114,85,91),"reused threshold plaster","reversible timber screen",.0,.04),
 "Tharniv":((97,120,78),(156,122,79),"living root bark","petition-grown resin",.0,.14),
 "Auvren":((150,159,168),(115,171,189),"unfounded limestone","witness-frame alloy",.01,.73),
}
# Focus is an actual geometry coordinate in each exported preview, never a
# fictional quest marker. Bellwold remains protected by its existing module.
FOCUS={
 "Crossings":("Three-way survey court",(0,3.0,0)),
 "Bellwold":("Empty bell yoke",(4.5,7.4,-5)),
 "Paperhaven":("Archive terrace",(-4,5.2,6)),
 "Saltwake":("Upturned dry hull",(-8,3.0,-4)),
 "Cinderhold":("Shared thermal kiln",(0,3.3,0)),
 "Drevlach":("Tomorrow's tide balance",(0,5.1,0)),
 "Orravane":("Inverted choir span",(0,8.1,0)),
 "Vathless":("Bearing-stone switchback",(0,4.0,0)),
 "Eillun":("Voluntary thresholds",(-3,3.0,5)),
 "Tharniv":("Root-grown civic court",(0,5.0,0)),
 "Auvren":("Contradictory founding portals",(0,5.1,0)),
}

def _texture(city,kind):
    if kind not in ("structure","fitting","roof"):raise ValueError(kind)
    yy,xx=np.mgrid[:SIZE,:SIZE].astype(float)
    base=CULTURES[city][0 if kind=="structure" else 1]
    rng=np.random.default_rng(sum((i+1)*ord(ch) for i,ch in enumerate(city+kind))+211)
    grit=rng.standard_normal((SIZE,SIZE))
    cross=np.sin(xx*.27+yy*.052)*np.cos(yy*.2-xx*.041)
    weave=np.sin(xx*.11)*np.cos(yy*.52)
    # Culture determines physical surface logic: no common brick skin.
    if city=="Saltwake":
        deposits=12*np.cos(yy*.125+xx*.035)+7*weave
    elif city=="Paperhaven":
        band=np.mod(yy+7*np.floor(xx/45),29)
        deposits=-29*(band<2.5)+9*weave
    elif city=="Orravane":
        deposits=13*np.cos(xx*.075+yy*.11)+8*cross
    elif city=="Tharniv":
        deposits=16*np.sin(xx*.19+6*np.sin(yy*.033))+9*weave
    elif city=="Vathless":
        deposits=18*np.sin(yy*.066+xx*.025)-12*(np.mod(yy+xx*.11,35)<2)
    elif city=="Auvren":
        deposits=12*np.sin(yy*.14)-16*(np.mod(xx*.8+yy*.22,53)<1.4)
    elif city=="Drevlach":
        deposits=12*np.cos(xx*.23)+9*cross-15*(np.mod(yy,40)<2)
    elif city=="Eillun":
        deposits=16*np.sin(xx*.13+yy*.064)+7*weave
    elif city=="Cinderhold":
        deposits=15*np.cos(xx*.33-yy*.13)-18*(np.mod(xx+yy*.1,28)<3)
    else:
        deposits=13*cross+9*weave
    if kind=="roof":
        # Separate roof wear and courses for every city: seams, shingle wear, soot.
        seam=np.mod(yy+11*np.floor(xx/41),22)
        deposits=deposits*.63-24*(seam<2)+8*np.sin(xx*.29+yy*.08)
    erosion=grit*(9 if kind=="structure" else 15)+deposits
    mineral=(yy/SIZE)*(13 if kind=="structure" else -6)
    albedo=np.clip(np.asarray(base)[None,None,:]+erosion[:,:,None]+mineral[:,:,None],0,255).astype(np.uint8)
    height=deposits/32+grit*.15
    slope_y,slope_x=np.gradient(height)
    nx=-slope_x*.21;ny=-slope_y*.21;nz=np.ones_like(nx)
    length=np.sqrt(nx**2+ny**2+nz**2)
    normal=np.stack(((nx/length*.5+.5)*255,(ny/length*.5+.5)*255,(nz/length*.5+.5)*255),axis=2)
    return Image.fromarray(albedo,"RGB"),Image.fromarray(np.clip(normal,0,255).astype(np.uint8),"RGB")

@lru_cache(maxsize=40)
def material(city,kind):
    albedo,normal=_texture(city,kind)
    metal=CULTURES[city][4 if kind=="structure" else 5]
    name=f"{city} "+("locally weathered residential roofing" if kind=="roof" else CULTURES[city][2 if kind=="structure" else 3])
    return PBRMaterial(name=name,baseColorTexture=albedo,normalTexture=normal,
      metallicFactor=(.015 if kind=="roof" else metal),roughnessFactor=(.92 if kind in ("roof","structure") else .59),
      baseColorFactor=[255,255,255,255],doubleSided=(kind=="roof"))

def _put(k,city,feature,geo,kind):
    faces=np.asarray(geo.faces,dtype=np.int64)
    verts=np.asarray(geo.vertices,dtype=float)[faces].reshape(-1,3)
    normals=np.repeat(np.asarray(geo.face_normals),3,axis=0)
    major=np.argmax(np.abs(normals),axis=1)
    uv=np.zeros((len(verts),2),dtype=np.float32)
    for axis,(i,j) in ((0,(2,1)),(1,(0,2)),(2,(0,1))):
        mask=major==axis
        if mask.any():
            for coord,d in ((i,0),(j,1)):
                val=verts[mask,coord]
                uv[mask,d]=(val-val.min())/max(float(np.ptp(val)),1e-3)
    model=trimesh.Trimesh(vertices=verts,faces=np.arange(len(verts)).reshape(-1,3),process=False)
    model.visual=TextureVisuals(uv=np.clip(uv,0,1),material=material(city,kind))
    name=f"{k.count:04d}_hero_pbr_{city.lower()}_{feature}"
    k.count+=1
    k.scene.add_geometry(model,geom_name=name,node_name=name)

def box(k,city,name,xyz,scale,kind="structure"):
    geo=trimesh.creation.box(extents=scale);geo.apply_translation(xyz)
    _put(k,city,name,geo,kind)

def beam(k,city,name,a,b,r=.12,kind="fitting"):
    a=np.asarray(a,dtype=float);b=np.asarray(b,dtype=float)
    length=np.linalg.norm(a-b)
    if length<.02:raise ValueError("zero-length landmark beam")
    geo=trimesh.creation.cylinder(radius=r,height=length,sections=12)
    geo.apply_transform(trimesh.geometry.align_vectors([0,0,1],(b-a)/length))
    geo.apply_translation((a+b)*.5)
    _put(k,city,name,geo,kind)

def column(k,city,name,center,base_r,height,kind="structure",sections=16):
    m=trimesh.creation.cone(radius=base_r,height=height,sections=sections) if base_r<.48 else trimesh.creation.cylinder(radius=base_r,height=height,sections=sections)
    # Trimesh primitives use local +Z for height; rotate to glTF +Y.
    m.apply_transform(trimesh.transformations.rotation_matrix(-math.pi/2,[1,0,0]))
    m.apply_translation(center)
    _put(k,city,name,m,kind)

def ring(k,city,name,center,radius,kind="fitting"):
    m=trimesh.creation.annulus(r_min=radius*.83,r_max=radius,height=.16,sections=32)
    m.apply_transform(trimesh.transformations.rotation_matrix(-math.pi/2,[1,0,0]))
    m.apply_translation(center);_put(k,city,name,m,kind)

def arch(k,city,name,x,z,h,w,kind="structure"):
    for side in (-1,1):
        box(k,city,name+f"_pier_{side}",(x+side*w*.5,h*.50,z),(.4,h,.5),kind)
        beam(k,city,name+f"_brace_{side}",(x+side*w*.5,h-1.0,z),(x+side*(w*.27),h+.12,z),.15,kind)
    beam(k,city,name+"_lintel",(x-w*.5,h,z),(x+w*.5,h,z),.22,"fitting")

def add_unique_hero(k,city):
    """Construct distinctive public geometry. Names are visual identity, not canon IDs."""
    if city not in CULTURES:raise ValueError(city)
    if city=="Crossings":
        for n,(x,z) in enumerate(((-6,3),(3,-4),(7,6))):
            arch(k,city,f"survey_fork_charter_{n}",x,z,4+n*.8,3.0)
            for j in range(3):
                beam(k,city,f"survey_fork_measure_{n}_{j}",(x,z*.0+1.2,z),
                     (x+2*math.cos(j*2*math.pi/3),1.25,z+2*math.sin(j*2*math.pi/3)),.07)
        box(k,city,"survey_fork_unsettled_map_table",(0,.8,0),(4,.3,2),"fitting")
    elif city=="Paperhaven":
        for n,(x,z,h) in enumerate(((-7,-5,4.8),(-4,6,6.2),(7,5,8.2))):
            for side in (-1,1):
                column(k,city,f"archive_terrace_buttress_{n}_{side}",(x+side*1.9,h*.5,z+1.3),.37,h)
            for j in range(4):
                box(k,city,f"archive_terrace_folio_slot_{n}_{j}",
                  (x-1.4+j*.89,h*.65,z+1.84),(.12,.73,.32),"fitting")
            box(k,city,f"archive_terrace_balcony_{n}",(x,h*.65,z+2.25),(4.8,.22,1.4))
        arch(k,city,"archive_terrace_civic_entry",3,-5,5.8,3.4)
    elif city=="Saltwake":
        for n,(x,z) in enumerate(((-8,-4),(-2,5),(7,1))):
            # Hull strakes and ribs rise as an *inverted ship* instead of a house roof.
            for j in range(6):
                offset=-1.5+j*.6
                beam(k,city,f"dry_hull_keel_rib_{n}_{j}",
                     (x+offset,.6,z-1.4),(x+.23*math.sin(j),3.7,z+.8),.13)
                beam(k,city,f"dry_hull_keel_second_strake_{n}_{j}",
                     (x+.23*math.sin(j),3.7,z+.8),(x+offset*.83,.7,z+2.6),.11)
            box(k,city,f"dry_hull_stern_deck_{n}",(x,3.53,z+.6),(3.2,.24,2.15))
            box(k,city,f"dry_hull_rain_collector_{n}",(x+.2,1.1,z+2.15),(1.8,.36,.84),"fitting")
        arch(k,city,"dry_hull_public_sluice",8,-7,4.2,4.2)
    elif city=="Cinderhold":
        for n,(x,z) in enumerate(((-8,4),(1,-6),(9,3))):
            column(k,city,f"thermal_kiln_vertical_chimney_{n}",(x,3.25,z),.87,6.5)
            for h in (1.1,2.3,3.9):
                ring(k,city,f"thermal_kiln_banded_collar_{n}_{h}",(x,h,z),1.05)
            for j in range(3):
                angle=j*2*math.pi/3
                beam(k,city,f"thermal_kiln_feed_pipe_{n}_{j}",
                    (x+.9*math.cos(angle),1,z+.9*math.sin(angle)),
                    (x+2.35*math.cos(angle),.5,z+2.35*math.sin(angle)),.19)
        box(k,city,"thermal_kiln_shared_heat_canal",(2,.21,0),(1.55,.38,12),"fitting")
    elif city=="Drevlach":
        arch(k,city,"tide_balance_clock_arch",0,0,8.5,5.1)
        for j in range(10):
            th=2*math.pi*j/10
            beam(k,city,f"tide_balance_gauge_ray_{j}",(0,6.5,0),
                (2.1*math.cos(th),6.5+2.1*math.sin(th),0),.08)
        ring(k,city,"tide_balance_civic_clock",(0,6.5,0),2.4)
        for n,(x,z) in enumerate(((-9,-6),(8,5))):
            box(k,city,f"tide_balance_counterweight_{n}",(x,1.4,z),(.76,2.3,1.1),"fitting")
            beam(k,city,f"tide_balance_lock_boom_{n}",(0,7.6,0),(x,2.8,z),.13)
            box(k,city,f"tide_balance_ledger_rail_{n}",(x,.5,z),(4.5,.3,.5))
    elif city=="Orravane":
        for n,(x,z,h) in enumerate(((-8,2,6.1),(2,5,9.0),(8,-7,7.7))):
            box(k,city,f"choir_span_suspended_floor_{n}",(x,h,z),(5.1,.34,4.0))
            for side in (-1,1):
                beam(k,city,f"choir_span_cable_{n}_{side}",(x+side*2.2,h,z),
                     (x+side*1.2,h+8.3,z-1.3),.085,"fitting")
                beam(k,city,f"choir_span_railing_{n}_{side}",(x+side*2.2,h+.7,z-1.7),
                     (x+side*2.2,h+.7,z+1.7),.09)
            ring(k,city,f"choir_span_acoustic_weight_{n}",(x,h-1.2,z),.58)
        beam(k,city,"choir_span_public_bridge",(-8,6.1,2),(2,9.0,5),.19)
    elif city=="Vathless":
        for n,(x,z) in enumerate(((-8,-5),(-4,7),(5,-6))):
            for h in range(5):
                box(k,city,f"quarry_cascade_bearing_course_{n}_{h}",
                    (x+.5*h, .48+.55*h,z+.3*h),(4.7-.31*h,.55,3.4-.23*h))
                box(k,city,f"quarry_cascade_inscribed_line_{n}_{h}",
                    (x+.5*h,.68+.55*h,z+.3*h+1.4),(.95,.12,.21),"fitting")
            beam(k,city,f"quarry_cascade_safety_stanchion_{n}",
                 (x-2,0,z-1),(x-2,4.3,z+1),.14,"fitting")
    elif city=="Eillun":
        for n,(x,z) in enumerate(((-9,-6),(-3,5),(7,-3),(10,7))):
            arch(k,city,f"voluntary_threshold_door_{n}",x,z,3.0+n*.2,2.4,"structure")
            # Reversible privacy screens, explicitly not identity checkpoints.
            for side in (-1,1):
                box(k,city,f"voluntary_threshold_folded_screen_{n}_{side}",
                    (x+side*1.1,1.5,z+.75),(.22,2.2,1.05),"fitting")
            box(k,city,f"voluntary_threshold_welcome_sill_{n}",(x,.24,z+1.1),
                (3.15,.32,.84))
    elif city=="Tharniv":
        for n,(x,z) in enumerate(((-8,8),(8,-7),(-8,-8),(7,8))):
            column(k,city,f"grafted_court_trunk_{n}",(x,3.0,z),.57,6.0)
            for j in range(5):
                th=j*2*math.pi/5
                end=(x+3.1*math.cos(th),7.2,z+3.1*math.sin(th))
                beam(k,city,f"grafted_court_live_branch_{n}_{j}",
                     (x,4.7,z),end,.17)
            ring(k,city,f"grafted_court_petition_root_ring_{n}",(x,.42,z),2.1)
        box(k,city,"grafted_court_uncrowned_civic_plinth",(0,.9,0),(3.9,1.7,3.5))
    elif city=="Auvren":
        for n,(x,z,h) in enumerate(((-7,5,7.3),(7,-4,6.1))):
            # Physically different gateways do not meet or resolve their origin.
            arch(k,city,f"founding_portal_conflicting_gate_{n}",x,z,h,3.8)
            box(k,city,f"founding_portal_unclaimed_sill_{n}",(x,.34,z+.7),(4.7,.27,1.12),"fitting")
            for side in (-1,1):
                beam(k,city,f"founding_portal_split_diagonal_{n}_{side}",
                    (x+side*1.5,2.1,z),(x+side*.9,h-.1,z),.15)
        for n,z in enumerate((-8,-3,3,8)):
            box(k,city,f"founding_portal_evidence_steps_{n}",(0,.6+n*.36,z),(2.8,.35,1.7))
