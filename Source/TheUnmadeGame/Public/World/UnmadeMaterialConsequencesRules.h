#pragma once
// V3: finite, saveable supply/labor/repair accounting for all nine realm depots.
// Units are realm-specific; a Saltwake water cask is NOT Cinderhold ember fuel.
// No network, random drops, free stock, or settlement-teleporting inventory.
#include "World/UnmadeRealmReactionRules.h"
#include "World/UnmadeLivingWorldRules.h"
#include <array>
#include <algorithm>
#include <cstdint>

namespace UnmadeCore {
struct MaterialPolicy {
    Realm realm;
    const char* unit;
    const char* workers;
    const char* household;
    int initialStore, initialHouseholds, workersPerDay, outputPerWorker;
    int dailyNeed, automaticDelivery, repairCost;
};
inline constexpr std::array<MaterialPolicy,9> MaterialPolicies = {{
    {Realm::ThreefoldReach,"bracing fittings","bridge carpenters","safe-passage homes",22,12,3,2,6,6,8},
    {Realm::WidowedRain,"sealed rainwater casks","rainward counters","storm shelters",24,12,3,2,7,6,8},
    {Realm::HearthBeneath,"common ember baskets","kiln tenders","heated dwellings",18,9,3,2,7,5,8},
    {Realm::TidalLedger,"ferry maintenance iron","ledger ferrymen","dock homes",24,10,3,2,6,6,9},
    {Realm::SkyBelow,"resonant support pins","choir stewards","suspended homes",20,9,3,2,7,5,9},
    {Realm::CinderSpine,"quarry bracing stone","seam workers","quarry shelters",26,10,4,2,7,6,9},
    {Realm::HundredUnlived,"consent-safe shelter kits","refuge builders","unregistered homes",19,9,3,2,7,5,8},
    {Realm::OrchardOfKings,"communal root grafts","root tenders","unclaimed dwellings",24,10,3,2,6,6,8},
    {Realm::FirstAbsence,"memory anchor stakes","threshold keepers","anchored homes",20,10,3,2,7,5,9}
}};
struct MaterialTown {
    int day=1;
    int store=0, households=0, integrity=80;
    int produced=0, consumed=0, shortageDays=0, laborHours=0;
    int deliveries=0, repairs=0;
};
struct MaterialCargo {
    int realm=-1, units=0, collectedDay=0; // one physically-carried pack
};
enum class MaterialAction { Blocked, Collected, Delivered, Repaired, NoWork };
enum class MaterialDuty { Regular, Haul, Repair, Shelter };
class MaterialWorld final {
public:
    using Towns=std::array<MaterialTown,9>;
    MaterialWorld()noexcept{
        for(int i=0;i<9;++i){
            towns_[i].store=MaterialPolicies[i].initialStore;
            towns_[i].households=MaterialPolicies[i].initialHouseholds;
        }
    }
    const MaterialTown* Town(Realm realm)const noexcept{
        const int i=static_cast<int>(realm);
        return i>=0&&i<9?&towns_[i]:nullptr;
    }
    const MaterialCargo& Cargo()const noexcept{return cargo_;}
    Towns Snapshot()const noexcept{return towns_;}
    static constexpr int MaxAmount=1000000;
    static constexpr int PackSize=5;
    bool Restore(const Towns& next,MaterialCargo cargo,int currentDay)noexcept{
        if(currentDay<1||currentDay>833334)return false;
        for(int i=0;i<9;++i){
            const auto& t=next[i];
            if(t.day<1||t.day>currentDay || t.store<0||t.store>120 ||
               t.households<0||t.households>100 ||t.integrity<0||t.integrity>100 ||
               t.produced<0||t.produced>MaxAmount || t.consumed<0||t.consumed>MaxAmount ||
               t.laborHours<0||t.laborHours>MaxAmount ||
               t.shortageDays<0||t.shortageDays>MaxAmount||
               t.deliveries<0||t.deliveries>MaxAmount||
               t.repairs<0||t.repairs>MaxAmount ||
               t.shortageDays>t.day || t.deliveries>t.produced+t.store+120)
                return false;
        }
        if(cargo.realm==-1){
            if(cargo.units!=0||cargo.collectedDay!=0)return false;
        }else{
            if(cargo.realm<0||cargo.realm>=9||cargo.units!=PackSize||
               cargo.collectedDay<1||cargo.collectedDay>currentDay||
               cargo.collectedDay>next[cargo.realm].day)return false;
        }
        towns_=next;cargo_=cargo;return true;
    }
    static int Workers(Realm realm,int integrity,RealityPublicResponse civic)noexcept{
        const int i=static_cast<int>(realm);
        if(i<0||i>=9)return 0;
        return std::max(1,MaterialPolicies[i].workersPerDay -
                        (integrity<45?1:0) +
                        ((civic==RealityPublicResponse::SharedPassage||
                          civic==RealityPublicResponse::ContestedAccord)?1:0));
    }
    bool AdvanceOneDay(Realm realm,RealityPublicResponse civic)noexcept{
        const int i=static_cast<int>(realm);
        if(i<0||i>=9)return false;
        auto& t=towns_[i];
        if(t.day>=833334)return false;
        const auto& p=MaterialPolicies[i];
        const int workers=Workers(realm,t.integrity,civic);
        // Labor is finite per in-world day. Output is warehouse material, not
        // directly credited to household stock or player inventory.
        const int output=workers*p.outputPerWorker;
        t.produced=std::min(MaxAmount,t.produced+output);
        t.laborHours=std::min(MaxAmount,t.laborHours+workers*8);
        t.store=std::min(120,t.store+output);
        const int brought=std::min({t.store,p.automaticDelivery,100-t.households});
        t.store-=brought;t.households+=brought;
        const int used=std::min(p.dailyNeed,t.households);
        t.households-=used;
        t.consumed=std::min(MaxAmount,t.consumed+used);
        if(used<p.dailyNeed){
            ++t.shortageDays;
            t.integrity=std::max(0,t.integrity-(p.dailyNeed-used)*2);
        }
        ++t.day;
        return true;
    }
    // Bound CPU per tick: one in-world day of each town, never simulate 800k days
    // synchronously after a corrupt or arbitrarily advanced saved clock.
    bool AdvanceAllOneDay(const std::array<RealityPublicResponse,9>& civic)noexcept{
        for(int i=0;i<9;++i)if(towns_[i].day>=833334)return false;
        for(int i=0;i<9;++i)AdvanceOneDay(static_cast<Realm>(i),civic[i]);
        return true;
    }
    MaterialAction Collect(Realm realm,int day)noexcept{
        const int i=static_cast<int>(realm);
        if(i<0||i>=9||cargo_.realm!=-1||day<1||day<towns_[i].day||
           towns_[i].store<PackSize)return MaterialAction::Blocked;
        towns_[i].store-=PackSize;
        cargo_={i,PackSize,day};
        return MaterialAction::Collected;
    }
    MaterialAction Deliver(Realm realm,int day)noexcept{
        const int i=static_cast<int>(realm);
        if(i<0||i>=9||cargo_.realm!=i||cargo_.units!=PackSize||
           day<cargo_.collectedDay||day<towns_[i].day||
           towns_[i].households>100-PackSize)return MaterialAction::Blocked;
        towns_[i].households+=PackSize;
        towns_[i].deliveries=std::min(MaxAmount,towns_[i].deliveries+1);
        cargo_={};
        return MaterialAction::Delivered;
    }
    MaterialAction Repair(Realm realm,int day)noexcept{
        const int i=static_cast<int>(realm);
        if(i<0||i>=9||day<towns_[i].day)return MaterialAction::Blocked;
        auto& t=towns_[i];const auto& p=MaterialPolicies[i];
        if(t.integrity>=90)return MaterialAction::NoWork;
        if(t.store<p.repairCost)return MaterialAction::Blocked;
        t.store-=p.repairCost;
        t.integrity=std::min(100,t.integrity+18);
        t.repairs=std::min(MaxAmount,t.repairs+1);
        return MaterialAction::Repaired;
    }
    static MaterialDuty Duty(Realm realm,NpcRole role,const MaterialTown& town,
                             DayPhase phase)noexcept {
        if(!PolicyFor(realm)||phase==DayPhase::Night)return MaterialDuty::Regular;
        if(town.households<MaterialPolicies[static_cast<int>(realm)].dailyNeed)
            return role==NpcRole::Courier || role==NpcRole::Merchant
                ?MaterialDuty::Haul:MaterialDuty::Shelter;
        if(town.integrity<65 && (role==NpcRole::Guard||role==NpcRole::Wanderer))
            return MaterialDuty::Repair;
        return MaterialDuty::Regular;
    }
private:
    Towns towns_{};
    MaterialCargo cargo_{};
};
} // namespace UnmadeCore
