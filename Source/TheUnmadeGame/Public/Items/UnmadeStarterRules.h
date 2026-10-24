#pragma once
#include "Items/UnmadeItemRules.h"
namespace UnmadeCore {
// Stage the whole kit before saving; never replace an older chosen loadout.
template<class Writer> bool CommitStarterKit(InventoryModel& inventory,bool ready,Writer write){
 if(!ready)return false;
 if(inventory.HasClaimed(Achievement::Starter))return true;
 const auto before=inventory.Snapshot();
 if(inventory.Claim(Achievement::Starter)!=RewardResult::Awarded)return false;
 if(!FindItem(inventory.Equipped(GearSlot::Weapon)))inventory.Equip(ItemId::SalvagerBlade);
 if(!FindItem(inventory.Equipped(GearSlot::Armor)))inventory.Equip(ItemId::TravelerMantle);
 if(write())return true;
 inventory.Restore(before);return false;
}
}
