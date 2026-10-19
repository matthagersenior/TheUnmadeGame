#pragma once
// Noncombat visual definitions. Never change NPC identity, collision capsule,
// capabilities, species combat profile, save record, mastery or AI decisions.
#include "NPC/UnmadeNpcDecisionRules.h"
#include "Combat/UnmadeCreatureRules.h"
#include <array>
namespace UnmadeCore {
struct ResidentGrayboxVisual {
 NpcRole role; const char* asset; const char* roleObject;
 const char* bodyShape; const char* accessoryShape;
 float sx,sy,sz,accessoryScale,accessoryHeight;
 const char* silhouetteDescription;
};
inline constexpr std::array<ResidentGrayboxVisual,5> ResidentGrayboxVisuals={{
 {NpcRole::Merchant,"SK_Resident_Merchant","Two-sided hanging weighing scale",
  "Cylinder","Sphere",.66f,.68f,1.45f,.24f,78.f,"Rolled goods and a low asymmetric shoulder balance"},
 {NpcRole::Guard,"SK_Resident_Watch","Open-top lantern visor",
  "Cylinder","Cone",.62f,.62f,1.70f,.34f,102.f,"Tall protective head, broad straight stance and watch badge"},
 {NpcRole::Scholar,"SK_Resident_Scholar","Folded public ledger and blind witness seal",
  "Cylinder","Cube",.57f,.55f,1.60f,.36f,77.f,"Narrow upright scholar with angular archival satchel"},
 {NpcRole::Courier,"SK_Resident_Courier","Paired sealed message parcels",
  "Cylinder","Sphere",.57f,.59f,1.53f,.28f,64.f,"Lower asymmetric cross-body pouch, not royal heraldry"},
 {NpcRole::Wanderer,"SK_Resident_Wanderer","Mended travel hood",
  "Cylinder","Cone",.60f,.60f,1.58f,.42f,103.f,"Asymmetrical patched hood and unarmed traveling silhouette"}
}};
inline const ResidentGrayboxVisual& ResidentGraybox(NpcRole role)noexcept{
 for(const auto& v:ResidentGrayboxVisuals)if(v.role==role)return v;
 return ResidentGrayboxVisuals.back();
}
struct CreatureGrayboxVisual {
 CreatureKind species; const char* asset;const char* signature;
 const char* bodyShape;const char* crestShape;
 float sx,sy,sz,crestScale,crestHeight;
};
inline constexpr std::array<CreatureGrayboxVisual,12> CreatureGrayboxVisuals={{
 {CreatureKind::Cinderjaw,"SK_Cinderjaw_MoltenBite","Twin charcoal jaws and forward hooked ash crest","Cube","Cone",.83f,.59f,1.1f,.35f,58.f},
 {CreatureKind::PaleStrider,"SK_PaleStrider_AbsentHoof","Tall unpaired legs, rear-aligned antler spike","Cylinder","Cone",.55f,.57f,1.60f,.32f,103.f},
 {CreatureKind::Veilwatch,"SK_Veilwatch_ShroudedLens","Wide watcher shroud and slit-shaped floating eye","Cylinder","Sphere",1.1f,1.0f,.83f,.36f,105.f},
 {CreatureKind::GleamMoth,"SK_GleamMoth_GlassWing","Flattened lantern shell with light-shedding crown","Sphere","Cone",.88f,.88f,.61f,.42f,84.f},
 {CreatureKind::HollowHound,"SK_HollowHound_EmptyChest","Hunched feral forebody and hollow shoulder gap","Cube","Cone",1.1f,.55f,.83f,.38f,61.f},
 {CreatureKind::GriefLantern,"SK_GriefLantern_UnringingBell","Tall mourning lantern with suspended bell crown","Cylinder","Sphere",.61f,.61f,1.47f,.42f,109.f},
 {CreatureKind::Inkmaw,"SK_Inkmaw_DoubleJaw","Horizontal manuscript maw with broken spine fins","Cube","Cylinder",1.25f,.65f,1.05f,.42f,88.f},
 {CreatureKind::RedactionEye,"SK_RedactionEye_BlindIndex","Offset archival eye with a blank paper eclipse","Sphere","Cylinder",.96f,.96f,.89f,.46f,97.f},
 {CreatureKind::SaltglassCrawler,"SK_SaltglassCrawler_TideClaw","Broad salt-calcified shell and double pincer crest","Cube","Cone",1.26f,.86f,.70f,.43f,60.f},
 {CreatureKind::DrownedWatch,"SK_DrownedWatch_StillCurrent","Weighted upright drowned sentinel with water halo","Cylinder","Sphere",.70f,.78f,1.49f,.42f,113.f},
 {CreatureKind::Ashmaw,"SK_Ashmaw_BrokenFurnace","Bulky baffle-lined torso with split forge chimney","Cube","Cone",1.25f,1.05f,1.48f,.43f,117.f},
 {CreatureKind::KilnWisp,"SK_KilnWisp_SparedEmber","Small levitating ember with exposed chimney flame","Sphere","Cone",.57f,.57f,.72f,.33f,81.f}
}};
inline const CreatureGrayboxVisual& CreatureGraybox(CreatureKind species)noexcept{
 for(const auto& v:CreatureGrayboxVisuals)if(v.species==species)return v;
 return CreatureGrayboxVisuals.front();
}
} // namespace UnmadeCore
