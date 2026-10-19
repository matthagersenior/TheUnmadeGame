#pragma once
// Money, barter, recipes and professions: offline, deterministic, atomic C++17.
#include "Items/UnmadeItemRules.h"
#include "World/UnmadeSettlementRegistry.h"
#include <array>
#include <algorithm>
#include <cstdint>
namespace UnmadeCore {
enum class Profession { Smith=0, Apothecary=1, Scribe=2, Count=3 };
enum class RecipeId { TemperedEdge=0, HerbSalve=1, ScribedStillness=2,
                      LanternMail=3, InkboundCharm=4, LifeAsh=5, Count=6 };
enum class EconomyResult { Completed, Invalid, NotAvailable, NotEnoughMoney,
                           NotEnoughMaterials, NoCapacity, InsufficientSkill, AlreadyPaid };
struct RecipeDef {
    RecipeId id;
    const char* title;
    Profession profession;
    SettlementId workshop;
    int skill;
    ItemId inputA;int unitsA;
    ItemId inputB;int unitsB;
    ItemId output;int unitsOut;
};
#include "Items/UnmadeCodexCraftingGenerated.inc"
inline const RecipeDef* FindRecipe(RecipeId id) noexcept {
    const int i=static_cast<int>(id);
    return i>=0 && i<6?&Recipes[i]:nullptr;
}
struct EconomySnapshot {
    int marks=45;
    std::array<int,3> craftsmanship{};
    std::uint32_t completedContracts=0;
};
class RegionalEconomy final {
public:
    const EconomySnapshot& Snapshot() const noexcept {return state_;}
    bool Restore(const EconomySnapshot& s) noexcept {
        if(s.marks<0 || s.marks>100000 || (s.completedContracts&~std::uint32_t{7})!=0)
            return false;
        for(int skill:s.craftsmanship)if(skill<0 || skill>30)return false;
        state_=s;return true;
    }
    void Practice(Profession p){int i=static_cast<int>(p);if(i>=0&&i<3)state_.craftsmanship[i]=std::min(30,state_.craftsmanship[i]+1);}
    int Marks() const noexcept {return state_.marks;}
    int Skill(Profession p) const noexcept {
        const int i=static_cast<int>(p);return i>=0 && i<3?state_.craftsmanship[i]:0;
    }
    // Distinct local specializations: Bellwold metal/herbs, Paperhaven paper/vials.
    int Price(ItemId id,SettlementId village,int trust=0) const noexcept {
        int base=0;
        switch(id) {
        case ItemId::IronScrap: base=12;break;
        case ItemId::WildHerbs:base=9;break;
        case ItemId::BlankParchment:base=10;break;
        case ItemId::HearthSalve:base=20;break;
        case ItemId::StrainVial:base=25;break;
        default:return 0;
        }
        if(!FindSettlement(village))return 0;
        if(village==SettlementId::Bellwold &&
           (id==ItemId::IronScrap || id==ItemId::WildHerbs)) base-=3;
        if(village==SettlementId::Paperhaven &&
           (id==ItemId::BlankParchment || id==ItemId::StrainVial)) base-=3;
        // Trust discount is limited; cannot turn the market into negative prices.
        const int discount=std::clamp(trust,0,60)/20;
        return std::max(3,base-discount);
    }
    EconomyResult Buy(InventoryModel& inventory,ItemId id,SettlementId village,
                       int count,int trust=0) noexcept {
        if(count<=0 || count>20)return EconomyResult::Invalid;
        const int price=Price(id,village,trust);
        if(price==0)return EconomyResult::NotAvailable;
        if(static_cast<std::int64_t>(price)*count>state_.marks)
            return EconomyResult::NotEnoughMoney;
        InventoryModel next=inventory;
        if(!next.Add(id,count))return EconomyResult::NoCapacity;
        inventory=next;
        state_.marks-=price*count;
        return EconomyResult::Completed;
    }
    EconomyResult Sell(InventoryModel& inventory,ItemId id,SettlementId village,int count) noexcept {
        if(count<=0 || count>20)return EconomyResult::Invalid;
        const int price=Price(id,village);
        if(price==0)return EconomyResult::NotAvailable;
        const int revenue=(price/3)*count; // a clear loss on buy->sell loops
        if(state_.marks>100000-revenue)return EconomyResult::NoCapacity;
        InventoryModel next=inventory;
        if(!next.Take(id,count))return EconomyResult::NotEnoughMaterials;
        inventory=next;
        state_.marks+=revenue;
        return EconomyResult::Completed;
    }
    EconomyResult Craft(InventoryModel& inventory,RecipeId id,SettlementId at,int borrowedSkill=0) noexcept {
        if(borrowedSkill<0 || borrowedSkill>2)return EconomyResult::Invalid;
        const auto* recipe=FindRecipe(id);
        if(!recipe)return EconomyResult::Invalid;
        if(at!=recipe->workshop)return EconomyResult::NotAvailable;
        if(Skill(recipe->profession)+borrowedSkill<recipe->skill)
            return EconomyResult::InsufficientSkill;
        InventoryModel next=inventory;
        if(!next.Take(recipe->inputA,recipe->unitsA) ||
           !next.Take(recipe->inputB,recipe->unitsB))
            return EconomyResult::NotEnoughMaterials;
        if(!next.Add(recipe->output,recipe->unitsOut))
            return EconomyResult::NoCapacity;
        inventory=next;
        const int i=static_cast<int>(recipe->profession);
        state_.craftsmanship[i]=std::min(30,state_.craftsmanship[i]+1);
        return EconomyResult::Completed;
    }
    EconomyResult PayContract(SettlementId village,int stage) noexcept {
        // One-time civic contracts, not farmable repeated NPC conversations.
        const int i=static_cast<int>(village);
        if(i<0 || i>2 || stage!=2)return EconomyResult::Invalid;
        if((state_.completedContracts&(1u<<i))!=0)return EconomyResult::AlreadyPaid;
        const int pay=i==0?15:i==1?36:32;
        if(state_.marks>100000-pay)return EconomyResult::NoCapacity;
        state_.marks+=pay;
        state_.completedContracts|=1u<<i;
        return EconomyResult::Completed;
    }
private:
    EconomySnapshot state_{};
};
}
