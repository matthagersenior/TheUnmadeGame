#pragma once
// Authored, offline, deterministic first-visit progression for the six later
// realms. Runtime actors and UE rendering are separate from this rule contract.
#include "World/UnmadeRealmAftermathRules.h"
#include <array>
#include <cstring>
namespace UnmadeCore {
struct LaterRealmSpec {
    Realm realm;
    const char* name;
    double centerX, centerY;
    const char* initiator;
    const char* trialEvidenceA;
    const char* trialEvidenceB;
    const char* mechanism;
    const char* trialChoiceA;
    const char* trialChoiceB;
    const char* arrival;
};
inline constexpr std::array<LaterRealmSpec,6> LaterRealms={{
    {Realm::TidalLedger,"Drevlach, the Owed Estuary",100000,-50000,
     "npc.tidal.ledgerkeeper.001","Tidal.Trial.FerryReceipt","Tidal.Trial.CollateralTide",
     "Tidal.Trial.DebtLatch","Shelter ferry opens without binding future passengers.",
     "Hidden collateral archive opens; lenders cannot erase their receipts.",
     "The tide counts the days you have not yet lived."},
    {Realm::SkyBelow,"Orravane, the Inverted Choir",-100000,-50000,
     "npc.sky.choirmaster.001","Sky.Trial.Counterweight","Sky.Trial.ForgottenChorus",
     "Sky.Trial.TuningFork","A counterweighted walkway holds while the choir rests.",
     "A silent chorus passage opens to preserve the erased verse.",
     "Footsteps pass above the roof; every bell rings below."},
    {Realm::CinderSpine,"Vathless, the Yesterday Quarry",100000,50000,
     "npc.bones.seamwarden.001","Bones.Trial.SafeWedge","Bones.Trial.LineageStone",
     "Bones.Trial.SeamClamp","Quarry residents gain an anchored safe walkway.",
     "The quarry records the seam that consumed a family line.",
     "Each new stone has a name older than the mountains."},
    {Realm::HundredUnlived,"Eillun, the Unclaimed City",-100000,50000,
     "npc.unlived.censuskeeper.001","Unlived.Trial.HiddenShelter",
     "Unlived.Trial.ConsentBoard","Unlived.Trial.NameLatch",
     "Unregistered guests gain a private refuge route.",
     "A consent-only public square opens; identities are not silently published.",
     "Doors recognize footsteps before they recognize names."},
    {Realm::OrchardOfKings,"Tharniv, the Untitled Orchard",200000,0,
     "npc.orchard.untitled.001","Orchard.Trial.SharedGraft",
     "Orchard.Trial.EmptyCoronation","Orchard.Trial.RootChannel",
     "A common rootway grows where a village road had failed.",
     "An unoccupied court path exposes the law nobody enacted.",
     "The trees bloom with laws that no ruler dared sign."},
    {Realm::FirstAbsence,"Auvren, the Before-Place",-200000,0,
     "npc.absence.firstwitness.001","Absence.Trial.StableMemory",
     "Absence.Trial.ContradictoryMap","Absence.Trial.OriginAnchor",
     "Travelers gain one reliable threshold without erasing older roads.",
     "Two contradictory origin paths survive without declaring a final victor.",
     "An unfinished horizon remembers all your previous answers."}
}};
inline int LaterIndex(Realm r) noexcept {
    const int i=static_cast<int>(r)-3;
    return i>=0 && i<6?i:-1;
}
inline const LaterRealmSpec* FindLaterRealm(Realm r) noexcept {
    const int i=LaterIndex(r);
    return i>=0?&LaterRealms[i]:nullptr;
}
enum class LaterResult { Locked, Visited, Started, EvidenceFound,
    Completed, WrongSite, WrongWitness, WrongMechanism, NeedEvidence, NoChange };
struct LaterRealmSnapshot {
    int visits=0;
    std::array<int,6> stage{}; // 0 dormant, 1 investigate, 2 approach selected, 3 opened
    std::array<int,6> choice{}; // 0 none, 1 common care, 2 preserved truth
};
class LaterRealmJourney final {
public:
    const LaterRealmSnapshot& Snapshot() const noexcept {return state_;}
    bool Restore(const LaterRealmSnapshot& s) noexcept {
        if(s.visits<0 || (s.visits&~63)!=0)return false;
        for(int i=0;i<6;++i) {
            const int stage=s.stage[i],choice=s.choice[i];
            if(stage<0 || stage>3 || choice<0 || choice>2 ||
               (stage>0 && !(s.visits&(1<<i))) ||
               (stage<=1 && choice!=0) ||
               (stage>=2 && choice==0))return false;
        }
        state_=s;
        return true;
    }
    bool Visited(Realm r) const noexcept {
        const int i=LaterIndex(r);
        return i>=0 && (state_.visits&(1<<i))!=0;
    }
    int Stage(Realm r) const noexcept {
        const int i=LaterIndex(r);
        return i<0?0:state_.stage[i];
    }
    int Outcome(Realm r) const noexcept {
        const int i=LaterIndex(r);
        return i<0?0:(state_.stage[i]==3?state_.choice[i]:0);
    }
    bool IsComplete(Realm r) const noexcept{return Stage(r)==3;}
    LaterResult Visit(Realm r) noexcept {
        const int i=LaterIndex(r);
        if(i<0 || Visited(r))return LaterResult::NoChange;
        state_.visits|=(1<<i);
        return LaterResult::Visited;
    }
    LaterResult Begin(Realm r,const char* witness) noexcept {
        const int i=LaterIndex(r);
        if(i<0 || state_.stage[i]!=0)return LaterResult::NoChange;
        if(!Visited(r))return LaterResult::Locked;
        if(!witness || std::strcmp(witness,LaterRealms[i].initiator)!=0)
            return LaterResult::WrongWitness;
        state_.stage[i]=1;
        return LaterResult::Started;
    }
    LaterResult Study(Realm r,int choice,const char* clue) noexcept {
        const int i=LaterIndex(r);
        if(i<0 || (state_.stage[i]!=1 && state_.stage[i]!=2))
            return LaterResult::NoChange;
        if(choice!=1 && choice!=2)return LaterResult::WrongSite;
        const char* expected=choice==1?LaterRealms[i].trialEvidenceA:
                                    LaterRealms[i].trialEvidenceB;
        if(!clue || std::strcmp(clue,expected)!=0)return LaterResult::WrongSite;
        state_.stage[i]=2;state_.choice[i]=choice;
        return LaterResult::EvidenceFound;
    }
    LaterResult Operate(Realm r,const char* site) noexcept {
        const int i=LaterIndex(r);
        if(i<0 || state_.stage[i]==0 || state_.stage[i]==3)
            return LaterResult::NoChange;
        if(state_.stage[i]!=2)return LaterResult::NeedEvidence;
        if(!site || std::strcmp(site,LaterRealms[i].mechanism)!=0)
            return LaterResult::WrongMechanism;
        state_.stage[i]=3;
        return LaterResult::Completed;
    }
private:
    LaterRealmSnapshot state_{};
};
} // namespace UnmadeCore
