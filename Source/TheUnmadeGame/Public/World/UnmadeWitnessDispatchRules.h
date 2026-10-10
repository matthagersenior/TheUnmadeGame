#pragma once
// Physical, save-backed cross-city document custody: neither a courier NPC
// nor a globally-informed recipient is invented by a quest flag.
#include "World/UnmadeWitnessBraidRules.h"

namespace UnmadeCore {
enum class WitnessDispatchStage:int { Waiting=0, PlayerCarrying=1, HandDelivered=2 };
enum class WitnessDispatchEvent:int { NotReady=0, Collected=1, Delivered=2, AlreadyDone=3, InvalidClock=4 };
struct WitnessDispatchSnapshot {
    int stage=0;
    int collectedDay=0;
    int deliveredDay=0;
};
class WitnessDispatch final {
public:
    WitnessDispatchSnapshot Snapshot()const noexcept{return state_;}
    WitnessDispatchStage Stage()const noexcept{
        return static_cast<WitnessDispatchStage>(state_.stage);
    }
    bool Restore(WitnessDispatchSnapshot data,WitnessBraidOutcome choice) noexcept {
        if(data.stage<0||data.stage>2||data.collectedDay<0||
           data.collectedDay>10000000||data.deliveredDay<0||
           data.deliveredDay>10000000)return false;
        if(data.stage==0 && (data.collectedDay!=0||data.deliveredDay!=0))return false;
        if(data.stage==1 && (data.collectedDay==0||data.deliveredDay!=0))return false;
        if(data.stage==2 && (data.collectedDay==0 ||
           data.deliveredDay<data.collectedDay))return false;
        if(data.stage!=0&&choice==WitnessBraidOutcome::Unresolved)return false;
        state_=data;return true;
    }
    WitnessDispatchEvent Collect(WitnessBraidOutcome choice,int day)noexcept {
        if(day<1||day>10000000)return WitnessDispatchEvent::InvalidClock;
        if(choice==WitnessBraidOutcome::Unresolved)return WitnessDispatchEvent::NotReady;
        if(state_.stage!=0)return WitnessDispatchEvent::AlreadyDone;
        state_={1,day,0};return WitnessDispatchEvent::Collected;
    }
    WitnessDispatchEvent HandToHessa(WitnessBraidOutcome choice,int day)noexcept {
        if(day<1||day>10000000 ||
           (state_.collectedDay!=0&&day<state_.collectedDay))
            return WitnessDispatchEvent::InvalidClock;
        if(choice==WitnessBraidOutcome::Unresolved ||
           state_.stage==0)return WitnessDispatchEvent::NotReady;
        if(state_.stage==2)return WitnessDispatchEvent::AlreadyDone;
        state_.stage=2;state_.deliveredDay=day;
        return WitnessDispatchEvent::Delivered;
    }
    const char* PlayerLine(WitnessBraidOutcome choice)const noexcept {
        if(choice==WitnessBraidOutcome::Unresolved)
            return "No record yet travels from Orrel's nail.";
        if(state_.stage==0)
            return "The folded, unnamed record waits beside the Crossings nail. E: take it to Hessa in Bellwold.";
        if(state_.stage==1)
            return "You carry an actual record from the Crossings. Bring it to Hessa and use E to hand it over.";
        return "Hessa personally received the folded record. Others must still learn through witnessed conversation.";
    }
    const char* HessaLine(WitnessBraidOutcome choice)const noexcept {
        if(state_.stage!=2)return "";
        return choice==WitnessBraidOutcome::ShelteredThread
            ? "You carried Orrel's refuge guide here yourself. I can protect these travelers; I still owe the unseen a hearing."
            : "I hold the redacted docket you brought. You kept their names off it; now I must decide what a fair hearing can safely ask.";
    }
private:
    WitnessDispatchSnapshot state_{};
};
} // namespace UnmadeCore
