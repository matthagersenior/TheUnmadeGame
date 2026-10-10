#pragma once
// THE UNMADE / Volume X — opt-in witness echoes.
// Portable C++17 domain: records physical observations, never converts an
// interpretation into the objective truth of an OPEN mystery.
#include <array>
#include <cstdint>
#include <cstring>

namespace UnmadeCore {

enum class WitnessEchoEvent : int {
    NotAnEcho=0, FirstReading=1, LaterMeaning=2, AlreadyRecorded=3
};
struct WitnessEchoSpec {
    const char* id;
    const char* physicalSite;
    const char* title;
    const char* firstObservation;
    const char* careInterpretation;
    const char* truthInterpretation;
    const char* openQuestion;
};

// CALL ids match Authoring/transmedia_storyworld_v1.json and stay stable.
// Only three are physically spawned in existing graybox settlements.
// Reserve bits for all thirteen Volume X callback IDs without implying
// those later-world scenes are implemented.
inline constexpr int WitnessEchoRegistrySize=13;
inline constexpr std::array<WitnessEchoSpec,3> WitnessEchoSites = {{
    {"CALL.01","site.callback.uncounted_cup","Bellwold / the uncounted cup",
     "A chipped white cup waits by the refuge door. Hessa does not enter it in the tally.",
     "After the relief, the cup waits beside a genuinely available shelter bed. No guest is identified.",
     "After the public testimony, the cup remains unnumbered; the refuge now keeps an open annotation beside the protected name.",
     "Was its fine crack made before the bell that was never cast?"},
    {"CALL.03","site.callback.two_faced_press","Paperhaven / the unmarked seal",
     "A seal press bears the Index Hall mark on only one face. Sevrin has left the other unengraved.",
     "The public index still lists an unclaimed bed without the resident's name. The reverse seal remains blank.",
     "The public record gains a witnessed annotation, not the name of whoever slept behind the closed door.",
     "Who pressed the date before the register was begun?"},
    {"CALL.04","site.callback.inkless_nail","The Crossings / the bridge nail",
     "Orrel's bent nail has no ink upon it, yet three disagreeing surveyors drew the same road from its mark.",
     "The shelter passage is open; the nail marks the route people may take without declaring where they came from.",
     "The archive passage is open; its displayed map admits the crossings were once taxed without consent.",
     "Why does the nail vibrate beside roads whose builders never met?"}
}};

struct WitnessEchoSnapshot {
    uint32_t first=0;
    uint32_t returnRead=0;
};

inline constexpr uint32_t WitnessEchoValidMask =
    (uint32_t{1}<<WitnessEchoRegistrySize)-1;

inline const WitnessEchoSpec* WitnessEchoFromSite(const char* site) noexcept {
    if(!site)return nullptr;
    for(const auto& s:WitnessEchoSites)
        if(std::strcmp(site,s.physicalSite)==0)return &s;
    return nullptr;
}
inline int WitnessEchoIndex(const char* id) noexcept {
    if(!id)return -1;
    for(int i=0;i<static_cast<int>(WitnessEchoSites.size());++i)
        if(std::strcmp(id,WitnessEchoSites[i].id)==0)return i;
    return -1;
}

class WitnessEchoLedger final {
public:
    WitnessEchoSnapshot Snapshot()const noexcept{return state_;}
    bool Restore(WitnessEchoSnapshot s) noexcept {
        if((s.first&~WitnessEchoValidMask)!=0 ||
           (s.returnRead&~WitnessEchoValidMask)!=0 ||
           (s.returnRead&~s.first)!=0)return false;
        state_=s;
        return true;
    }
    WitnessEchoEvent Read(const char* physicalSite,int localOutcome) noexcept {
        const auto* spec=WitnessEchoFromSite(physicalSite);
        if(!spec)return WitnessEchoEvent::NotAnEcho;
        const int i=WitnessEchoIndex(spec->id);
        if(i<0)return WitnessEchoEvent::NotAnEcho;
        const uint32_t bit=uint32_t{1}<<i;
        if(!(state_.first&bit)){
            state_.first|=bit;
            return WitnessEchoEvent::FirstReading;
        }
        if((localOutcome==1 || localOutcome==2) && !(state_.returnRead&bit)){
            state_.returnRead|=bit;
            return WitnessEchoEvent::LaterMeaning;
        }
        return WitnessEchoEvent::AlreadyRecorded;
    }
    bool HasFirst(const char* id)const noexcept {
        const int i=WitnessEchoIndex(id);
        return i>=0 && (state_.first&(uint32_t{1}<<i))!=0;
    }
    bool HasReturn(const char* id)const noexcept {
        const int i=WitnessEchoIndex(id);
        return i>=0 && (state_.returnRead&(uint32_t{1}<<i))!=0;
    }
    int FirstCount()const noexcept {
        int total=0;
        for(const auto& s:WitnessEchoSites)if(HasFirst(s.id))++total;
        return total;
    }
    int ReturnCount()const noexcept {
        int total=0;
        for(const auto& s:WitnessEchoSites)if(HasReturn(s.id))++total;
        return total;
    }
private:
    WitnessEchoSnapshot state_{};
};

inline const char* WitnessEchoText(const WitnessEchoSpec& spec,
                                   WitnessEchoEvent event,int localOutcome) noexcept {
    if(event==WitnessEchoEvent::FirstReading ||
       (event==WitnessEchoEvent::AlreadyRecorded && localOutcome==0))
        return spec.firstObservation;
    if(event==WitnessEchoEvent::LaterMeaning ||
       (event==WitnessEchoEvent::AlreadyRecorded && (localOutcome==1||localOutcome==2)))
        return localOutcome==1?spec.careInterpretation:spec.truthInterpretation;
    return spec.firstObservation;
}

} // namespace UnmadeCore
