#include "Items/UnmadeItemRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main(){
    assert(ItemCatalog.size()>=25);
    for(int i=0;i<static_cast<int>(ItemCatalog.size());++i){
        const auto& a=ItemCatalog[i]; assert(static_cast<int>(a.id)==i);
        assert(std::strlen(a.name)>4 && std::strlen(a.story)>8);
        assert(FindItem(a.code)==&a);
        assert(a.maxStack>0 && a.maxStack<=20);
        for(int j=0;j<i;++j)assert(std::strcmp(a.code,ItemCatalog[j].code)!=0);
    }
    assert(FindItem("blade.unknown")==nullptr && FindItem(nullptr)==nullptr);
    InventoryModel i;
    assert(i.Quantity(ItemId::HearthSalve)==0);
    assert(i.Claim(Achievement::Starter)==RewardResult::Awarded);
    assert(i.Claim(Achievement::Starter)==RewardResult::AlreadyAwarded);
    assert(i.Quantity(ItemId::HearthSalve)==3 && i.Quantity(ItemId::SalvagerBlade)==1);
    assert(i.Equip(ItemId::SalvagerBlade) && i.Equip(ItemId::TravelerMantle));
    assert(i.AttackBonus()==3 && i.ArmorBonus()==2);
    assert(i.GlimpseStrainDiscount()==0 && i.FoldDurationBonus()==0);
    assert(i.RecoveryMultiplier()==1.0);
    assert(!i.Equip(ItemId::HearthSalve) && !i.Equip(ItemId::Waybreaker));
    int health=99,strain=99;
    assert(i.Consume(ItemId::HearthSalve,0,0,health,strain)==ConsumeResult::NoBenefit);
    assert(i.Quantity(ItemId::HearthSalve)==3);
    assert(i.Consume(ItemId::HearthSalve,16,0,health,strain)==ConsumeResult::Used);
    assert(health==16 && strain==0 && i.Quantity(ItemId::HearthSalve)==2);
    assert(i.Consume(ItemId::StrainVial,0,10,health,strain)==ConsumeResult::Used);
    assert(health==0 && strain==10 && i.Quantity(ItemId::StrainVial)==1);
    assert(i.Claim(Achievement::BellwoldLanterns)==RewardResult::Awarded);
    assert(i.Claim(Achievement::PaperhavenTestimony)==RewardResult::Awarded);
    assert(i.Claim(Achievement::ThreeVillages)==RewardResult::Awarded);
    assert(i.Quantity(ItemId::BellMetal)==1 && i.Quantity(ItemId::ArchiveInk)==1);
    assert(!i.ForgeWaybreaker());
    assert(i.Claim(Achievement::EchoWell)==RewardResult::Awarded);
    assert(i.Equip(ItemId::ReturnedVoice));
    assert(i.GlimpseStrainDiscount()==3);
    assert(i.Equip(ItemId::PaperhavenLens));
    assert(i.GlimpseStrainDiscount()==2);
    assert(i.Claim(Achievement::FirstStalker)==RewardResult::Awarded);
    assert(i.Quantity(ItemId::EchoGlass)==2);
    assert(i.ForgeWaybreaker() && i.Quantity(ItemId::Waybreaker)==1);
    assert(!i.ForgeWaybreaker());
    assert(i.Quantity(ItemId::EchoGlass)==0 && i.Quantity(ItemId::BellMetal)==0);
    // Paperhaven Lens remains equipped (+4), so Waybreaker (+32) stacks to 36.
    assert(i.Equip(ItemId::Waybreaker) && i.AttackBonus()==36);
    assert(i.FoldDurationBonus()==3);
    assert(i.Claim(Achievement::AllLandmarks)==RewardResult::Awarded);
    assert(i.Equip(ItemId::UnwrittenCrown));
    assert(i.GlimpseStrainDiscount()==0 && i.RecoveryMultiplier()==1.5);
    assert(i.AttackBonus()==44 && i.ArmorBonus()==12);
    auto snapshot=i.Snapshot();
    InventoryModel restored;
    assert(restored.Restore(snapshot));
    assert(restored.AttackBonus()==44 && restored.HasClaimed(Achievement::AllLandmarks));
    auto bad=snapshot;
    bad.quantities[static_cast<int>(ItemId::HearthSalve)]=400;
    assert(!restored.Restore(bad) && restored.AttackBonus()==44);
    bad=snapshot;
    bad.equipped[0]=static_cast<int>(ItemId::UnwrittenCrown);
    assert(!restored.Restore(bad));
    bad=snapshot;
    bad.milestones=~std::uint64_t{0};
    assert(!restored.Restore(bad));
    InventoryModel firstBoss;
    assert(firstBoss.Claim(Achievement::HollowBell)==RewardResult::Awarded);
    assert(firstBoss.Claim(Achievement::HollowBell)==RewardResult::AlreadyAwarded);
    assert(firstBoss.Quantity(ItemId::BellheartAegis)==1);
    assert(firstBoss.Claim(Achievement::RedactedCurator)==RewardResult::Awarded);
    assert(firstBoss.Quantity(ItemId::RedactedScepter)==1);
    assert(firstBoss.Claim(Achievement::UnfinishedPilgrim)==RewardResult::Awarded);
    assert(firstBoss.Quantity(ItemId::PilgrimsSignet)==1);
    const auto savedBoss=firstBoss.Snapshot();
    InventoryModel reloadedBoss;
    assert(reloadedBoss.Restore(savedBoss));
    assert(reloadedBoss.HasClaimed(Achievement::HollowBell));
    InventoryModel noRewards;
    assert(!noRewards.ForgeWaybreaker());
    assert(noRewards.Claim(static_cast<Achievement>(999))==RewardResult::Invalid);
    assert(noRewards.Consume(ItemId::Waybreaker,10,0,health,strain)==ConsumeResult::NotConsumable);
    std::cout<<"PASS: unique loot catalog, once-only rewards, epic forging, equipment, consumables, atomic snapshots\n";
}
