#pragma once
// Volume X, Chapter 20: Hessa drafts a reply AFTER a real next morning.
// The player explicitly chooses its disclosure, takes it in person and
// returns it to Orrel. No automatic courier, remote awareness or quest skip.
#include "World/UnmadeWitnessDispatchRules.h"

namespace UnmadeCore {
enum class WitnessReturnStage : int { AwaitingMorning=0, ReplyPrepared=1, PlayerCarrying=2, OrrelReceived=3 };
enum class WitnessReturnRoute : int { Unchosen=0, PrivateCounsel=1, PublicHearing=2 };
enum class WitnessReturnEvent : int { NotReady=0, Chosen=1, PickedUp=2, Delivered=3, AlreadyDone=4, InvalidClock=5, InvalidChoice=6 };
struct WitnessReturnSnapshot {
    int stage=0;
    int route=0;
    int chosenDay=0;
    int collectedDay=0;
    int deliveredDay=0;
};
class WitnessReturn final {
public:
    WitnessReturnSnapshot Snapshot()const noexcept{return state_;}
    WitnessReturnStage Stage()const noexcept{
        return static_cast<WitnessReturnStage>(state_.stage);
    }
    WitnessReturnRoute Route()const noexcept{
        return static_cast<WitnessReturnRoute>(state_.route);
    }
    static bool CanChoose(const WitnessDispatch& first, WitnessBraidOutcome braid,int day)noexcept{
        const auto delivery=first.Snapshot();
        return braid!=WitnessBraidOutcome::Unresolved &&
               first.Stage()==WitnessDispatchStage::HandDelivered &&
               day>delivery.deliveredDay && day<=10000000;
    }
    bool Restore(WitnessReturnSnapshot saved,const WitnessDispatch& first,
                 WitnessBraidOutcome braid,int currentDay)noexcept{
        if(currentDay<1||currentDay>10000000 || saved.stage<0||saved.stage>3 ||
           saved.route<0||saved.route>2 || saved.chosenDay<0 ||
           saved.collectedDay<0||saved.deliveredDay<0||
           saved.chosenDay>currentDay||saved.collectedDay>currentDay||
           saved.deliveredDay>currentDay)return false;
        if(saved.stage==0) {
            if(saved.route!=0||saved.chosenDay!=0||
               saved.collectedDay!=0||saved.deliveredDay!=0)return false;
        } else {
            if(saved.route==0 || !CanChoose(first,braid,saved.chosenDay))return false;
            if(saved.stage==1 && (saved.collectedDay!=0||saved.deliveredDay!=0))return false;
            if(saved.stage>=2 && (saved.collectedDay<saved.chosenDay||
                                  saved.collectedDay==0))return false;
            if(saved.stage==2 && saved.deliveredDay!=0)return false;
            if(saved.stage==3 && (saved.deliveredDay<saved.collectedDay||
                                  saved.deliveredDay==0))return false;
        }
        state_=saved;
        return true;
    }
    WitnessReturnEvent Choose(int route,int day,const WitnessDispatch& first,
                              WitnessBraidOutcome braid)noexcept {
        if(route!=1&&route!=2)return WitnessReturnEvent::InvalidChoice;
        if(day<1||day>10000000)return WitnessReturnEvent::InvalidClock;
        if(state_.stage!=0)return WitnessReturnEvent::AlreadyDone;
        if(!CanChoose(first,braid,day))return WitnessReturnEvent::NotReady;
        state_={1,route,day,0,0};
        return WitnessReturnEvent::Chosen;
    }
    WitnessReturnEvent Take(int day)noexcept {
        if(day<1||day>10000000 || (state_.chosenDay && day<state_.chosenDay))
            return WitnessReturnEvent::InvalidClock;
        if(state_.stage==0)return WitnessReturnEvent::NotReady;
        if(state_.stage!=1)return WitnessReturnEvent::AlreadyDone;
        state_.stage=2;state_.collectedDay=day;
        return WitnessReturnEvent::PickedUp;
    }
    WitnessReturnEvent GiveToOrrel(int day)noexcept {
        if(day<1||day>10000000 || (state_.collectedDay && day<state_.collectedDay))
            return WitnessReturnEvent::InvalidClock;
        if(state_.stage<2)return WitnessReturnEvent::NotReady;
        if(state_.stage==3)return WitnessReturnEvent::AlreadyDone;
        state_.stage=3;state_.deliveredDay=day;
        return WitnessReturnEvent::Delivered;
    }
    static const char* Warning(int route)noexcept {
        if(route==1)return "PRIVATE COUNSEL: Hessa asks Orrel to protect unnamed travelers; public evidence will remain incomplete.";
        if(route==2)return "PUBLIC HEARING: Hessa challenges the contradiction in public while keeping every refuge guest unnamed; scrutiny may follow.";
        return "Select a valid course.";
    }
    const char* Journal(const WitnessDispatch& first,WitnessBraidOutcome braid,int day)const noexcept {
        if(first.Stage()!=WitnessDispatchStage::HandDelivered)return "Bring the original folded record to Hessa first.";
        if(state_.stage==0)
            return CanChoose(first,braid,day)
                ? "A full morning has passed. Stand beside Hessa; F7 private counsel or F8 redacted hearing, press twice."
                : "Hessa needs one complete in-world morning to consider the record.";
        if(state_.stage==1)return "Hessa has prepared her reply. Return to her and press E to take the folded answer.";
        if(state_.stage==2)return "You carry Hessa's answer. Return to Orrel at the Crossings and press E in person.";
        return "Orrel has personally received Hessa's answer; the third road remains disputed.";
    }
    const char* HessaAfterChoosing()const noexcept {
        if(state_.stage==0)return "";
        if(state_.route==1)
            return "I have written to Orrel in confidence. A refuge cannot survive if every life becomes a witness exhibit.";
        return "I have written a challenge Orrel may read aloud. No guest's name appears, though the city may still ask after them.";
    }
    const char* OrrelOnReceipt(WitnessBraidOutcome original)const noexcept {
        if(state_.stage!=3)return "";
        if(state_.route==1)
            return original==WitnessBraidOutcome::ShelteredThread
                ? "I have her counsel, carried here by you. I will keep the private crossing; the builders' wages remain unpaid."
                : "The public docket remains, but Hessa asks for a private passage. I must answer both obligations.";
        return original==WitnessBraidOutcome::ShelteredThread
            ? "We sheltered the travelers, then brought the contradiction to hearing. The road may protect and accuse us."
            : "The hearing is requested in Hessa's words. We have a public question, not a verdict or a missing name.";
    }
private:
    WitnessReturnSnapshot state_{};
};
} // namespace UnmadeCore
