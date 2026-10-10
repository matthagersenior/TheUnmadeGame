#pragma once
// Deterministic, save-backed return encounters for every source-spawned
// Threefold (48), Frontier (16), and later-realm (18) resident.
// IDs are external stable source identities, never generated display names.
#include "World/UnmadeSettlementRegistry.h"
#include "World/UnmadeFrontierRealmRules.h"
#include "Authoring/UnmadeOuterPeopleData.h"
#include <array>
#include <cstring>
namespace UnmadeCore {
inline constexpr int ResidentContinuityCount=static_cast<int>(
    Residents.size()+FrontierResidents.size()+OuterPeople.size());
inline const char* ResidentContinuityId(int i) noexcept {
    if(i<0||i>=ResidentContinuityCount)return nullptr;
    if(i<static_cast<int>(Residents.size()))return Residents[i].id;
    i-=static_cast<int>(Residents.size());
    if(i<static_cast<int>(FrontierResidents.size()))return FrontierResidents[i].id;
    i-=static_cast<int>(FrontierResidents.size());
    return OuterPeople[i].id;
}
inline int ResidentContinuityIndex(const char* id) noexcept {
    if(!id)return -1;
    for(int i=0;i<ResidentContinuityCount;++i)
        if(std::strcmp(id,ResidentContinuityId(i))==0)return i;
    return -1;
}
enum class Familiarity:int { Stranger=0, Recognized=1, Acquainted=2,
    Confidant=3, Trusted=4, Enduring=5 };
enum class EncounterEvent:int { Unknown=0, FirstHello=1, ReturnHello=2,
    SameDay=3, Aided=4, AlreadyAided=5, InvalidClock=6 };
struct ResidentContinuitySnapshot {
    std::array<int,ResidentContinuityCount> visits{};
    std::array<int,ResidentContinuityCount> lastDay{};
    std::array<int,ResidentContinuityCount> aids{};
};
class ResidentContinuity final {
public:
    const ResidentContinuitySnapshot& Snapshot()const noexcept{return state_;}
    bool Restore(const ResidentContinuitySnapshot& s) noexcept {
        for(int i=0;i<ResidentContinuityCount;++i)
            if(s.visits[i]<0||s.visits[i]>5||s.lastDay[i]<0||
               s.lastDay[i]>10000000||s.aids[i]<0||s.aids[i]>1||
               (s.visits[i]==0&&s.lastDay[i]!=0)||
               (s.visits[i]>0&&s.lastDay[i]==0))return false;
        state_=s;return true;
    }
    Familiarity Level(const char* id)const noexcept {
        const int i=ResidentContinuityIndex(id);
        if(i<0)return Familiarity::Stranger;
        return static_cast<Familiarity>(state_.visits[i]);
    }
    int Visits(const char* id)const noexcept {
        const int i=ResidentContinuityIndex(id);
        return i<0?0:state_.visits[i];
    }
    bool Aided(const char* id)const noexcept {
        const int i=ResidentContinuityIndex(id);
        return i>=0&&state_.aids[i]==1;
    }
    EncounterEvent Talk(const char* id,int day)noexcept {
        const int i=ResidentContinuityIndex(id);
        if(i<0)return EncounterEvent::Unknown;
        if(day<1||day>10000000||
           (state_.lastDay[i]!=0&&day<state_.lastDay[i]))
            return EncounterEvent::InvalidClock;
        if(state_.lastDay[i]==day)return EncounterEvent::SameDay;
        const bool returning=state_.visits[i]>0;
        if(state_.visits[i]<5)++state_.visits[i];
        state_.lastDay[i]=day;
        return returning?EncounterEvent::ReturnHello:EncounterEvent::FirstHello;
    }
    EncounterEvent Aid(const char* id)noexcept {
        const int i=ResidentContinuityIndex(id);
        if(i<0)return EncounterEvent::Unknown;
        if(state_.aids[i]==1)return EncounterEvent::AlreadyAided;
        state_.aids[i]=1;return EncounterEvent::Aided;
    }
private:ResidentContinuitySnapshot state_{};
};
inline const char* ResidentContinuityLine(Familiarity level,bool aided,
                                           bool localStoryKnown,
                                           bool newMorning)noexcept {
    if(newMorning&&localStoryKnown)
        return "The sky has changed, and I remember the road you helped us keep. Ask me what is different now.";
    if(aided)return "You've come back. I remember the help you gave me; there is room for you beside our fire.";
    if(localStoryKnown&&level>=Familiarity::Acquainted)
        return "You were here when our own records changed. Do you still believe the same account?";
    switch(level){
    case Familiarity::Stranger:
        return "You look familiar to no one here yet. Speak honestly and we can begin.";
    case Familiarity::Recognized:
        return "I remember our first meeting. The road was different that day.";
    case Familiarity::Acquainted:
        return "You've returned more than once; I'll speak as a neighbor rather than a visitor.";
    case Familiarity::Confidant:
        return "I remember the conversations you kept coming back to finish.";
    case Familiarity::Trusted:
    case Familiarity::Enduring:
        return "We have a history now. It won't vanish because the city changes its map.";
    }
    return "";
}
} // namespace UnmadeCore
