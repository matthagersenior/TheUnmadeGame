#pragma once
// The first *return visit* in THE UNMADE. Two evidence paths each grant
// a different civic benefit; the entire story works without any LLM.
#include <cstring>
namespace UnmadeCore {
enum class AfterlightChoice : int { None=0, Relief=1, Revelation=2 };
enum class AfterlightResult {
    NoChange, Locked, Started, WrongEvidence, EvidenceFound,
    WrongWitness, Witnessed, Resolved
};
struct AfterlightSnapshot { int stage=0, approach=0, outcome=0; };
struct AfterlightBenefit {
    const char* worldTag="";         // one of two persistent physical changes
    const char* description="The second night has not yet been survived.";
    int shelterCapacity=0;           // shelter-only, does not award trade currency
    int publishedWitnesses=0;        // public-record-only, no forged NPC testimony
};
class BellwoldAfterlight final {
public:
    int Stage() const noexcept {return state_.stage;}
    AfterlightChoice Outcome() const noexcept {
        return static_cast<AfterlightChoice>(state_.outcome);
    }
    AfterlightChoice Approach() const noexcept {
        return static_cast<AfterlightChoice>(state_.approach);
    }
    AfterlightSnapshot Snapshot() const noexcept {return state_;}
    bool Restore(const AfterlightSnapshot& s) noexcept {
        if(s.stage<0 || s.stage>4 || s.approach<0 || s.approach>2 ||
           s.outcome<0 || s.outcome>2 ||
           ((s.stage==0 || s.stage==1) && (s.approach!=0 || s.outcome!=0)) ||
           ((s.stage==2 || s.stage==3) && (s.approach==0 || s.outcome!=0)) ||
           (s.stage==4 && (s.approach==0 || s.outcome!=s.approach)))
            return false;
        state_=s;return true;
    }
    AfterlightResult Begin(int lanternTaskStage,int refugeFactionEnding,
                           const char* witness) noexcept {
        if(state_.stage!=0)return AfterlightResult::NoChange;
        if(lanternTaskStage!=2 || refugeFactionEnding<1 || refugeFactionEnding>2)
            return AfterlightResult::Locked;
        if(!witness || std::strcmp(witness,"npc.bellwold.matron.001")!=0)
            return AfterlightResult::NoChange;
        state_.stage=1;return AfterlightResult::Started;
    }
    AfterlightResult Inspect(AfterlightChoice path,const char* physicalTag) noexcept {
        if(state_.stage!=1 && state_.stage!=2)return AfterlightResult::NoChange;
        const char* expected=path==AfterlightChoice::Relief?
            "Bellwold.Afterlight.Relief" :path==AfterlightChoice::Revelation?
            "Bellwold.Afterlight.Census" :nullptr;
        if(!expected || !physicalTag || std::strcmp(expected,physicalTag)!=0)
            return AfterlightResult::WrongEvidence;
        state_.stage=2;
        state_.approach=static_cast<int>(path);
        return AfterlightResult::EvidenceFound;
    }
    AfterlightResult Converse(const char* id) noexcept {
        if(state_.stage!=2)return AfterlightResult::NoChange;
        const char* expected=state_.approach==1?
            "npc.bellwold.healer.001":"npc.bellwold.childtutor.001";
        if(!id || std::strcmp(id,expected)!=0)
            return AfterlightResult::WrongWitness;
        state_.stage=3;return AfterlightResult::Witnessed;
    }
    AfterlightResult Resolve(AfterlightChoice chosen) noexcept {
        if(state_.stage!=3)return AfterlightResult::NoChange;
        if(static_cast<int>(chosen)!=state_.approach)return AfterlightResult::WrongEvidence;
        state_.outcome=state_.approach;
        state_.stage=4;
        return AfterlightResult::Resolved;
    }
    AfterlightBenefit Effect() const noexcept {
        if(state_.stage!=4)return {};
        if(state_.outcome==1)
            return {"Bellwold.Afterlight.SafeWard",
                    "A safe ward and its lanterns shelter families no record ever named.",2,0};
        return {"Bellwold.Afterlight.OpenCensus",
                "The missing citizens' names are publicly engraved; no registrar can erase them.",0,2};
    }
private:
    AfterlightSnapshot state_{};
};
}
