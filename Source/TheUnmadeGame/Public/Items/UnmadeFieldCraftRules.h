#pragma once
#include "Items/UnmadeCraftEconomyRules.h"
namespace UnmadeCore {
struct FieldRecipeDef {const char* name;Profession profession;ItemId a;int na;ItemId b;int nb;ItemId output;int count;};
inline constexpr std::array<FieldRecipeDef,9> FieldRecipes={{
 {"Surveyor's Stillness",Profession::Scribe,ItemId::BlankParchment,2,ItemId::WildHerbs,1,ItemId::StrainVial,1},
 {"Drywake Dressing",Profession::Apothecary,ItemId::WildHerbs,2,ItemId::BlankParchment,1,ItemId::HearthSalve,2},
 {"Cooling Forge Edge",Profession::Smith,ItemId::IronScrap,3,ItemId::BellMetal,1,ItemId::TemperedEdge,1},
 {"Unpledged Ink",Profession::Scribe,ItemId::BlankParchment,2,ItemId::ArchiveInk,1,ItemId::StrainVial,2},
 {"Choir Rest Tonic",Profession::Apothecary,ItemId::WildHerbs,3,ItemId::BlankParchment,1,ItemId::NightwatchTonic,1},
 {"Seamkeeper Mail",Profession::Smith,ItemId::IronScrap,6,ItemId::BellMetal,1,ItemId::LanternMail,1},
 {"Quiet Witness Seal",Profession::Scribe,ItemId::BlankParchment,5,ItemId::ArchiveInk,1,ItemId::InkboundCharm,1},
 {"Nursery Poultice",Profession::Apothecary,ItemId::WildHerbs,3,ItemId::RiftSeed,1,ItemId::RootboundPoultice,1},
 {"Threshold Ash",Profession::Apothecary,ItemId::RiftSeed,1,ItemId::RootboundPoultice,1,ItemId::AshOfPossibleLives,1}
}};
inline EconomyResult CraftField(InventoryModel& inventory,int realm,bool learned){if(realm<0||realm>=9)return EconomyResult::Invalid;if(!learned)return EconomyResult::InsufficientSkill;const auto& r=FieldRecipes[realm];auto next=inventory;if(!next.Take(r.a,r.na)||!next.Take(r.b,r.nb))return EconomyResult::NotEnoughMaterials;if(!next.Add(r.output,r.count))return EconomyResult::NoCapacity;inventory=next;return EconomyResult::Completed;}
}
