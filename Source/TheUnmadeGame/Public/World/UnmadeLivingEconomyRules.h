#pragma once
// V4 living economy: named earners, finite municipal treasuries and carried
// cross-realm cargo. Does not mint resources, simulate unobserved deliveries,
// publish protected identities or replace the v3 material/household model.
#include "World/UnmadeMaterialConsequencesRules.h"
#include <array>
#include <algorithm>
#include <cstring>
#include <cstdint>

namespace UnmadeCore {
struct NamedTradeWorker {
    Realm home;
    const char* stableId;
    const char* profession;
    const char* household;
};
inline constexpr std::array<NamedTradeWorker,9> NamedTradeWorkers={{
    {Realm::ThreefoldReach,"npc.marketporter.001","market porter","Kesta's rented room"},
    {Realm::WidowedRain,"npc.saltwake.courier.001","rain courier","Mera's tide-side home"},
    {Realm::HearthBeneath,"npc.cinderhold.courier.001","ember runner","Bes's shared hearth"},
    {Realm::TidalLedger,"npc.tidal.ferryman.001","ferryman","Orshel's dock shelter"},
    {Realm::SkyBelow,"npc.sky.structuralist.001","support steward","Vhaurn's suspended home"},
    {Realm::CinderSpine,"npc.bones.mason.001","seam mason","Torrun's quarry dwelling"},
    {Realm::HundredUnlived,"npc.unlived.caretaker.001","refuge caretaker","Auvet's unnamed refuge"},
    {Realm::OrchardOfKings,"npc.orchard.gardener.001","root gardener","Imro's common plot"},
    {Realm::FirstAbsence,"npc.absence.caretaker.001","threshold keeper","Thae's doorway"}
}};
enum class TradeRoute : int { SaltToHearth=0, HearthToSalt=1 };
struct TradeContract {
    TradeRoute route;
    Realm origin,destination;
    const char* cargo;
    const char* receivingPurpose;
    int units, agreedMarks;
};
inline constexpr std::array<TradeContract,2> TradeContracts={{
    {TradeRoute::SaltToHearth,Realm::WidowedRain,Realm::HearthBeneath,
     "sealed rainwater casks","kiln cooling and safe drinking",5,6},
    {TradeRoute::HearthToSalt,Realm::HearthBeneath,Realm::WidowedRain,
     "common ember baskets","drying storm-soaked shelter beds",5,6}
}};
struct WorkerAccount {
    int employerMarks=32; // employer's finite treasury, NOT player money
    int earnedMarks=0; // named worker's owned wages
    int owedMarks=0; // unpaid obligation to named worker, never free currency
    int shiftsPaid=0;
    int lastPaidDay=1;
    int deliveriesWitnessed=0; // only when their own destination receives cargo
};
struct TradeCargo {
    int route=-1;
    int collectedDay=0;
};
struct TradeLedger {
    int foreignStock=0; // stored by destination, provenance preserved by route
    int deliveries=0;
    int paidMarks=0;
    int consumedSupport=0;
};
struct LivingEconomySnapshot {
    std::array<WorkerAccount,9> workers{};
    std::array<TradeLedger,2> routes{};
    TradeCargo cargo{};
};
enum class TradeResult { Blocked, Collected, Delivered };
class LivingEconomy final {
public:
    static constexpr int WagePerShift=2;
    using Snapshot=LivingEconomySnapshot;
    Snapshot Save()const noexcept {return data_;}
    const WorkerAccount* Worker(Realm realm)const noexcept {
        int i=static_cast<int>(realm);
        return i>=0&&i<9?&data_.workers[i]:nullptr;
    }
    const TradeLedger* Route(TradeRoute route)const noexcept{
        int i=static_cast<int>(route);
        return i>=0&&i<2?&data_.routes[i]:nullptr;
    }
    const TradeCargo& Cargo()const noexcept{return data_.cargo;}
    int WorkerIndex(const char* id)const noexcept {
        if(!id)return -1;
        for(int i=0;i<9;++i)
            if(std::strcmp(NamedTradeWorkers[i].stableId,id)==0)return i;
        return -1;
    }
    bool InitializeAtDay(int day)noexcept{
        if(day<1||day>833334)return false;
        for(auto& w:data_.workers)w.lastPaidDay=day;
        return true;
    }
    bool Restore(const Snapshot& in,const MaterialWorld& material,int currentDay)noexcept{
        if(currentDay<1||currentDay>833334)return false;
        for(int i=0;i<9;++i){
            const auto& w=in.workers[i];
            if(w.employerMarks<0||w.employerMarks>100000 ||
               w.earnedMarks<0||w.earnedMarks>100000 ||
               w.owedMarks<0||w.owedMarks>100000||
               w.shiftsPaid<0||w.shiftsPaid>833334 ||
               w.lastPaidDay<1||w.lastPaidDay>currentDay||
               w.deliveriesWitnessed<0||w.deliveriesWitnessed>100000 ||
               w.deliveriesWitnessed>in.routes[0].deliveries+in.routes[1].deliveries)
                return false;
        }
        for(const auto& r:in.routes)
            if(r.foreignStock<0||r.foreignStock>100||
               r.deliveries<0||r.deliveries>100000||
               r.paidMarks<0||r.paidMarks>100000||
               r.consumedSupport<0||r.consumedSupport>100000||
               r.paidMarks!=r.deliveries*6||
               r.consumedSupport+r.foreignStock!=r.deliveries*5)return false;
        if(in.cargo.route==-1){
            if(in.cargo.collectedDay!=0)return false;
        } else {
            if(in.cargo.route<0||in.cargo.route>=2||
               in.cargo.collectedDay<1||in.cargo.collectedDay>currentDay||
               material.Cargo().realm!=-1)return false;
            const auto& contract=TradeContracts[in.cargo.route];
            if(!material.Town(contract.origin))return false;
        }
        data_=in;return true;
    }
    // Wages are strictly a transfer from the civic employer to named worker.
    // If the municipality lacks funds the named wage becomes an obligation.
    // Payroll marks never enter player inventory or the material warehouse.
    bool AdvanceOneDay(int day)noexcept{
        if(day<1||day>833334)return false;
        for(const auto& w:data_.workers)
            if(w.lastPaidDay>=day)return false;
        for(auto& w:data_.workers){
            if(w.employerMarks>=WagePerShift && w.earnedMarks<=100000-WagePerShift){
                w.employerMarks-=WagePerShift;
                w.earnedMarks+=WagePerShift;
                ++w.shiftsPaid;
            } else if(w.owedMarks<=100000-WagePerShift){
                w.owedMarks+=WagePerShift;
            }
            w.lastPaidDay=day;
        }
        return true;
    }
    // Worker can reclaim arrears when an actual trade receipt replenishes
    // the municipal treasury, not merely because the player returns.
    bool PayArrears(Realm realm)noexcept{
        const int i=static_cast<int>(realm);
        if(i<0||i>=9)return false;
        auto& w=data_.workers[i];
        const int pay=std::min({w.employerMarks,w.owedMarks,100000-w.earnedMarks});
        if(pay<=0)return false;
        w.employerMarks-=pay;w.owedMarks-=pay;w.earnedMarks+=pay;
        return true;
    }
    TradeResult Collect(TradeRoute route,int day,MaterialWorld& world)noexcept{
        const int i=static_cast<int>(route);
        if(i<0||i>=2||data_.cargo.route!=-1||day<1||
           world.Cargo().realm!=-1)return TradeResult::Blocked;
        const auto& c=TradeContracts[i];
        if(!world.ExportForContract(c.origin,c.units,day))return TradeResult::Blocked;
        data_.cargo={i,day};
        return TradeResult::Collected;
    }
    TradeResult Deliver(TradeRoute route,int day,const MaterialWorld& world)noexcept{
        const int i=static_cast<int>(route);
        if(i<0||i>=2||data_.cargo.route!=i||day<data_.cargo.collectedDay||
           world.Cargo().realm!=-1)return TradeResult::Blocked;
        const auto& c=TradeContracts[i];
        const auto* destination=world.Town(c.destination);
        auto& buyer=data_.workers[static_cast<int>(c.destination)];
        auto& seller=data_.workers[static_cast<int>(c.origin)];
        auto& record=data_.routes[i];
        if(!destination||day<destination->day||buyer.employerMarks<c.agreedMarks||
           seller.employerMarks>100000-c.agreedMarks||
           record.foreignStock>100-c.units||
           record.deliveries>=100000||
           buyer.deliveriesWitnessed>=100000)
            return TradeResult::Blocked;
        buyer.employerMarks-=c.agreedMarks;
        seller.employerMarks+=c.agreedMarks;
        ++buyer.deliveriesWitnessed;
        record.foreignStock+=c.units;
        ++record.deliveries;
        record.paidMarks+=c.agreedMarks;
        data_.cargo={};
        return TradeResult::Delivered;
    }
    // Imported baskets/casks remain foreign goods. A real local shortage
    // may spend one imported unit supporting safety, NEVER mint local stock.
    int ApplyShelterSupport(Realm destination,const MaterialTown& current)noexcept{
        int supported=0;
        for(int i=0;i<2;++i){
            const auto& c=TradeContracts[i];
            if(c.destination!=destination||data_.routes[i].foreignStock<1)continue;
            if(current.households>=MaterialPolicies[static_cast<int>(destination)].dailyNeed)
                continue;
            --data_.routes[i].foreignStock;
            ++data_.routes[i].consumedSupport;
            ++supported;
        }
        return supported;
    }
    bool HasEarnedMutualAid()const noexcept{
        return data_.routes[0].deliveries>0 && data_.routes[1].deliveries>0;
    }
private:
    Snapshot data_{};
};
} // namespace UnmadeCore
