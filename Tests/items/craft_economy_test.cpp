#include "Items/UnmadeCraftEconomyRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main(){
    assert(Recipes.size()==6);
    InventoryModel bag;RegionalEconomy economy;
    assert(economy.Marks()==45);
    assert(economy.Price(ItemId::WildHerbs,SettlementId::Bellwold) <
           economy.Price(ItemId::WildHerbs,SettlementId::Paperhaven));
    assert(economy.Price(ItemId::BlankParchment,SettlementId::Paperhaven) <
           economy.Price(ItemId::BlankParchment,SettlementId::Bellwold));
    assert(economy.Buy(bag,ItemId::IronScrap,SettlementId::Bellwold,2)==EconomyResult::Completed);
    const int afterBuy=economy.Marks();
    assert(economy.Sell(bag,ItemId::IronScrap,SettlementId::Bellwold,2)==EconomyResult::Completed);
    assert(economy.Marks()<45 && economy.Marks()>afterBuy);
    assert(economy.PayContract(SettlementId::Bellwold,0)==EconomyResult::Invalid);
    assert(economy.PayContract(SettlementId::Bellwold,2)==EconomyResult::Completed);
    assert(economy.PayContract(SettlementId::Bellwold,2)==EconomyResult::AlreadyPaid);
    assert(economy.PayContract(SettlementId::Paperhaven,2)==EconomyResult::Completed);
    assert(economy.Buy(bag,ItemId::WildHerbs,SettlementId::Bellwold,2)==EconomyResult::Completed);
    assert(economy.Buy(bag,ItemId::IronScrap,SettlementId::Bellwold,1)==EconomyResult::Completed);
    const auto beforeWrong=bag.Snapshot();
    const auto moneyBefore=economy.Marks();
    assert(economy.Craft(bag,RecipeId::HerbSalve,SettlementId::Paperhaven)==EconomyResult::NotAvailable);
    assert(economy.Marks()==moneyBefore && bag.Snapshot().quantities==beforeWrong.quantities);
    assert(economy.Craft(bag,RecipeId::HerbSalve,SettlementId::Bellwold)==EconomyResult::Completed);
    assert(bag.Quantity(ItemId::HearthSalve)==2);
    assert(economy.Skill(Profession::Apothecary)==1);
    assert(economy.Craft(bag,RecipeId::LifeAsh,SettlementId::Bellwold)==EconomyResult::InsufficientSkill);
    assert(economy.Craft(bag,RecipeId::HerbSalve,SettlementId::Bellwold)==EconomyResult::NotEnoughMaterials);
    InventoryModel lifeBag;
    assert(lifeBag.Add(ItemId::IronScrap,6));
    assert(lifeBag.Add(ItemId::BellMetal,1));
    assert(economy.Craft(lifeBag,RecipeId::LanternMail,SettlementId::Bellwold)
           ==EconomyResult::InsufficientSkill);
    assert(economy.Craft(lifeBag,RecipeId::LanternMail,SettlementId::Bellwold,2)
           ==EconomyResult::Completed);
    assert(lifeBag.Quantity(ItemId::LanternMail)==1);
    assert(economy.Craft(lifeBag,RecipeId::LanternMail,SettlementId::Bellwold,3)
           ==EconomyResult::Invalid);
    const auto state=economy.Snapshot();
    RegionalEconomy restored;assert(restored.Restore(state));
    assert(restored.PayContract(SettlementId::Paperhaven,2)==EconomyResult::AlreadyPaid);
    auto bad=state;bad.marks=-1;
    assert(!restored.Restore(bad) && restored.Marks()==state.marks);
    bad=state;bad.craftsmanship[0]=99;
    assert(!restored.Restore(bad));
    assert(economy.Buy(bag,ItemId::Waybreaker,SettlementId::Bellwold,1)==EconomyResult::NotAvailable);
    assert(economy.Buy(bag,ItemId::WildHerbs,SettlementId::Bellwold,0)==EconomyResult::Invalid);
    assert(economy.Sell(bag,ItemId::BlankParchment,SettlementId::Paperhaven,1)==EconomyResult::NotEnoughMaterials);
    std::cout<<"PASS: village economy, professions, finite buying/selling, crafting gates, no-arbitrage, snapshots\n";
}
