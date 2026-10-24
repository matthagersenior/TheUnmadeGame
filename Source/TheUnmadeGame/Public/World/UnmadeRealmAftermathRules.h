#pragma once
// A nine-realm continuity layer: distinct authored aftermath investigations.
// Realm data do not claim unbuilt levels are playable. The realm atlas remains
// source of truth for names, geography, attunement and connections.
#include "World/UnmadeWorldAtlas.h"
#include <array>
#include <cstring>
namespace UnmadeCore {
enum class RealmAftermathResult {
    Locked, Started, EvidenceFound, Witnessed, Resolved,
    WrongSite, WrongWitness, WrongChoice, NoChange,
    NeedMechanism, WrongMechanism, Prepared
};
struct RealmAftermathSpec {
    Realm realm;
    const char* title;
    const char* initiatingWitness;
    const char* mechanismSite; // physical flood/vent control before either evidence route
    const char* evidenceForCare;
    const char* evidenceForTruth;
    const char* careWitness;
    const char* truthWitness;
    const char* careConsequence;
    const char* truthConsequence;
    const char* signatureAbility;
    const char* ethicalDilemma;
    const char* aftershock;
    bool initialLevelExists;
};
#include "World/UnmadeCodexAftermathGenerated.inc"
struct RealmAftermathSnapshot {
    std::array<int,9> stage{}; // 0 dormant, 1 evidence, 2 testimony, 3 choice, 4 resolved
    std::array<int,9> prepared{}; // 0 mechanism untouched, 1 route physically opened
    std::array<int,9> approach{}; // 1 care, 2 public truth
    std::array<int,9> ending{};
};
class RealmAftermathChronicle final {
public:
    const RealmAftermathSnapshot& Snapshot() const noexcept {return state_;}
    bool Restore(const RealmAftermathSnapshot& s) noexcept {
        for(int i=0;i<9;++i){
            if(s.stage[i]<0 || s.stage[i]>4 ||
               s.prepared[i]<0 || s.prepared[i]>1 ||
               s.approach[i]<0 || s.approach[i]>2 ||
               s.ending[i]<0 || s.ending[i]>2 ||
               (s.stage[i]==0 && s.prepared[i]!=0) ||
               (s.stage[i]>=2 && s.prepared[i]!=1) ||
               (s.stage[i]<=1 && (s.approach[i]!=0 || s.ending[i]!=0)) ||
               ((s.stage[i]==2 || s.stage[i]==3) &&
                 (s.approach[i]==0 || s.ending[i]!=0)) ||
               (s.stage[i]==4 &&
                 (s.approach[i]==0 || s.ending[i]!=s.approach[i])))
                return false;
        }
        state_=s;return true;
    }
    int Stage(Realm r) const noexcept {
        const int i=static_cast<int>(r);
        return i>=0 && i<9?state_.stage[i]:0;
    }
    bool IsPrepared(Realm r) const noexcept {
        const int i=static_cast<int>(r);
        return i>=0 && i<9 && state_.prepared[i]==1;
    }
    int Ending(Realm r) const noexcept {
        const int i=static_cast<int>(r);
        return i>=0 && i<9?state_.ending[i]:0;
    }
    RealmAftermathResult Begin(Realm r, bool firstArcResolved,
                              bool locationVisited, const char* witness,
                              const char* civicWitness=nullptr) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9)return RealmAftermathResult::NoChange;
        if(state_.stage[i]!=0)return RealmAftermathResult::NoChange;
        if(!firstArcResolved || !locationVisited)return RealmAftermathResult::Locked;
        if(!witness || (std::strcmp(witness,RealmAftermathSpecs[i].initiatingWitness)!=0 &&
            (!civicWitness || std::strcmp(witness,civicWitness)!=0)))
            return RealmAftermathResult::WrongWitness;
        state_.stage[i]=1;return RealmAftermathResult::Started;
    }
    RealmAftermathResult Prepare(Realm r,const char* mechanismTag) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9 || state_.stage[i]!=1 || state_.prepared[i])return RealmAftermathResult::NoChange;
        if(!mechanismTag || std::strcmp(mechanismTag,RealmAftermathSpecs[i].mechanismSite)!=0)
            return RealmAftermathResult::WrongMechanism;
        state_.prepared[i]=1;
        return RealmAftermathResult::Prepared;
    }
    RealmAftermathResult Inspect(Realm r, int approach,
                                const char* site) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9 || (state_.stage[i]!=1 && state_.stage[i]!=2))
            return RealmAftermathResult::NoChange;
        if(!state_.prepared[i])return RealmAftermathResult::NeedMechanism;
        if(approach!=1 && approach!=2)return RealmAftermathResult::WrongChoice;
        const auto& spec=RealmAftermathSpecs[i];
        const char* expected=approach==1?spec.evidenceForCare:spec.evidenceForTruth;
        if(!site || std::strcmp(expected,site)!=0)return RealmAftermathResult::WrongSite;
        state_.stage[i]=2;state_.approach[i]=approach;
        return RealmAftermathResult::EvidenceFound;
    }
    RealmAftermathResult Testify(Realm r,const char* witness,
                                 const char* civicWitness=nullptr) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9 || state_.stage[i]!=2)return RealmAftermathResult::NoChange;
        const auto& spec=RealmAftermathSpecs[i];
        const char* expected=state_.approach[i]==1?spec.careWitness:spec.truthWitness;
        if(!witness || (std::strcmp(witness,expected)!=0 &&
            (!civicWitness || std::strcmp(witness,civicWitness)!=0)))
            return RealmAftermathResult::WrongWitness;
        state_.stage[i]=3;
        return RealmAftermathResult::Witnessed;
    }
    RealmAftermathResult Decide(Realm r,int ending) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9 || state_.stage[i]!=3)return RealmAftermathResult::NoChange;
        if(ending!=state_.approach[i])return RealmAftermathResult::WrongChoice;
        state_.stage[i]=4;state_.ending[i]=ending;
        return RealmAftermathResult::Resolved;
    }
    const char* WorldConsequence(Realm r) const noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9)return "";
        return state_.ending[i]==1?RealmAftermathSpecs[i].careConsequence:
               state_.ending[i]==2?RealmAftermathSpecs[i].truthConsequence:"";
    }
private:
    RealmAftermathSnapshot state_{};
};
} // namespace UnmadeCore
