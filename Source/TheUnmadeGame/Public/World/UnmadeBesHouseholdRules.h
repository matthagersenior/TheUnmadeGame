#pragma once
// The Unmade v5: one canon person's enforceable life, not a reputation score.
// Bes is the stable named Cinderhold courier in v4. His separate dwelling
// has finite warmth, work absences, documented unpaid wages and truthful
// first-hand inquiry. No currency, coal or testimony is generated here.
#include "World/UnmadeLivingEconomyRules.h"
#include <algorithm>

namespace UnmadeCore {
enum class BesPhase : int {
    Unheard=0, WagesReported=1, Documented=2,
    ReliefGiven=3, Walkout=4, Settled=5, Underwritten=6
};
enum class BesOutcome : int {
    Blocked=0, Reported=1, HomeInspected=2, LedgerCorroborated=3,
    WarmthDelivered=4, WalkoutBegun=5, WagesSettled=6, NoChange=7,
    GuaranteePaid=8, ConsentRecorded=9
};
struct BesHouseholdSnapshot {
    int phase=0;
    int day=1;
    int heatBaskets=1; // one existing personal hearth reserve, not town stock
    int daysCold=0;
    int daysWalkedOut=0;
    int reliefPacks=0;
    int firstReportDay=0;
    int guaranteeOwed=0;
    bool meraConsented=false;
    bool meraGuaranteeUsed=false;
    bool homeVisited=false;
    bool wardenInterviewed=false;
};
class BesHousehold final {
public:
    using Snapshot=BesHouseholdSnapshot;
    Snapshot Save()const noexcept{return record_;}
    bool InitializeAtDay(int day)noexcept{
        if(day<1||day>833334)return false;
        record_.day=day;
        return true;
    }
    bool Restore(Snapshot s,int currentDay)noexcept{
        if(currentDay<1||currentDay>833334||
           s.phase<0||s.phase>6||s.day<1||s.day>currentDay||
           s.heatBaskets<0||s.heatBaskets>50||
           s.daysCold<0||s.daysCold>s.day||
           s.daysWalkedOut<0||s.daysWalkedOut>s.day||
           s.reliefPacks<0||s.reliefPacks>10||
           s.firstReportDay<0||s.firstReportDay>s.day ||
           s.guaranteeOwed<0||s.guaranteeOwed>6||
           (s.meraGuaranteeUsed&&!s.meraConsented)||
           (s.guaranteeOwed>0&&!s.meraGuaranteeUsed)||
           (s.phase==static_cast<int>(BesPhase::Underwritten)&&
            (!s.meraGuaranteeUsed||s.guaranteeOwed==0))||
           (s.phase==0&&(s.firstReportDay!=0||s.homeVisited||
                          s.wardenInterviewed||s.reliefPacks>0||
                          s.meraConsented||s.meraGuaranteeUsed||
                          s.daysWalkedOut>0))||
           (s.phase>0&&s.firstReportDay==0)||
           (s.wardenInterviewed&&!s.homeVisited)||
           (s.phase>=static_cast<int>(BesPhase::Documented)&&
            (!s.homeVisited||!s.wardenInterviewed))||
           (s.phase==static_cast<int>(BesPhase::ReliefGiven)&&s.reliefPacks==0)||
           (s.phase==static_cast<int>(BesPhase::Walkout)&&
            !s.wardenInterviewed))return false;
        record_=s;return true;
    }
    BesPhase Phase()const noexcept{return static_cast<BesPhase>(record_.phase);}
    bool OnWalkout()const noexcept{return Phase()==BesPhase::Walkout;}
    BesOutcome HearBes(const LivingEconomy& economy,int day)noexcept{
        const auto* w=economy.Worker(Realm::HearthBeneath);
        if(!w||w->owedMarks<2||day<record_.day)return BesOutcome::Blocked;
        if(Phase()!=BesPhase::Unheard)return BesOutcome::NoChange;
        record_.phase=static_cast<int>(BesPhase::WagesReported);
        record_.firstReportDay=day;
        return BesOutcome::Reported;
    }
    BesOutcome InspectHome(int day)noexcept{
        if(Phase()==BesPhase::Unheard||day<record_.day)return BesOutcome::Blocked;
        if(record_.homeVisited)return BesOutcome::NoChange;
        record_.homeVisited=true;
        return BesOutcome::HomeInspected;
    }
    BesOutcome HearRheva(int day)noexcept{
        if(Phase()==BesPhase::Unheard||!record_.homeVisited||
           day<record_.day)return BesOutcome::Blocked;
        if(record_.wardenInterviewed)return BesOutcome::NoChange;
        record_.wardenInterviewed=true;
        record_.phase=static_cast<int>(BesPhase::Documented);
        return BesOutcome::LedgerCorroborated;
    }
    BesOutcome DeliverHome(MaterialWorld& material,int day)noexcept{
        if(!record_.wardenInterviewed||Phase()==BesPhase::Settled||
           day<record_.day||record_.heatBaskets>45||
           record_.reliefPacks>=10)return BesOutcome::Blocked;
        // Must really carry Cinderhold's single v3 relief pack, from a
        // previously debited depot. Never reach into its warehouse remotely.
        if(!material.DeliverToNamedHome(Realm::HearthBeneath,day))
            return BesOutcome::Blocked;
        record_.heatBaskets+=MaterialWorld::PackSize;
        ++record_.reliefPacks;
        if(Phase()==BesPhase::Documented)
            record_.phase=static_cast<int>(BesPhase::ReliefGiven);
        return BesOutcome::WarmthDelivered;
    }
    BesOutcome BeginWalkout(const LivingEconomy& economy,int day)noexcept{
        const auto* w=economy.Worker(Realm::HearthBeneath);
        if(!record_.wardenInterviewed||Phase()==BesPhase::Unheard||
           Phase()==BesPhase::Settled||day<record_.day||
           !w||w->owedMarks<2)return BesOutcome::Blocked;
        if(OnWalkout())return BesOutcome::NoChange;
        record_.phase=static_cast<int>(BesPhase::Walkout);
        return BesOutcome::WalkoutBegun;
    }
    BesOutcome OfferMeraGuarantee(int day)noexcept{
        if(!record_.wardenInterviewed||day<record_.day||
           Phase()==BesPhase::Settled||record_.meraConsented)
            return BesOutcome::Blocked;
        record_.meraConsented=true;
        return BesOutcome::ConsentRecorded;
    }
    BesOutcome AcceptMeraGuarantee(LivingEconomy& economy,int day)noexcept{
        if(!record_.meraConsented||record_.meraGuaranteeUsed||
           day<record_.day||Phase()==BesPhase::Settled)return BesOutcome::Blocked;
        const auto* bes=economy.Worker(Realm::HearthBeneath);
        const auto* mera=economy.Worker(Realm::WidowedRain);
        if(!bes||!mera||bes->owedMarks<=0||mera->earnedMarks<=0)return BesOutcome::Blocked;
        const int amount=std::min({6,bes->owedMarks,mera->earnedMarks});
        if(!economy.UnderwriteBesFromMera(amount))return BesOutcome::Blocked;
        record_.guaranteeOwed+=amount;record_.meraGuaranteeUsed=true;
        if(economy.Worker(Realm::HearthBeneath)->owedMarks==0)
            record_.phase=static_cast<int>(BesPhase::Underwritten);
        return BesOutcome::GuaranteePaid;
    }
    BesOutcome SettleFromTown(LivingEconomy& economy,int day)noexcept{
        if(!record_.wardenInterviewed||Phase()==BesPhase::Settled||
           day<record_.day)return BesOutcome::Blocked;
        const auto* w=economy.Worker(Realm::HearthBeneath);
        if(!w)return BesOutcome::Blocked;
        // Require full restitution, not a staged "solved" label after a
        // partial transfer. If the treasury is empty, shipments must be
        // physically fulfilled to fund the bargain.
        if(w->owedMarks>0&&!economy.PayAllBesArrears())return BesOutcome::Blocked;
        if(record_.guaranteeOwed>0)
        {
            if(!economy.RepayMeraGuarantee(record_.guaranteeOwed))
                return BesOutcome::Blocked;
            record_.guaranteeOwed=0;
        }
        record_.phase=static_cast<int>(BesPhase::Settled);
        return BesOutcome::WagesSettled;
    }
    bool AdvanceOneDay(int newDay)noexcept{
        if(newDay!=record_.day+1||newDay>833334)return false;
        if(record_.heatBaskets>0)--record_.heatBaskets;
        else ++record_.daysCold;
        if(OnWalkout())++record_.daysWalkedOut;
        record_.day=newDay;
        return true;
    }
    // Future cast/civic support cannot silently erase unpaid, individual work.
    bool EndgameWitness()const noexcept{
        return (Phase()==BesPhase::Settled ||
                Phase()==BesPhase::Underwritten) &&
               record_.homeVisited&&record_.wardenInterviewed;
    }
private:
    Snapshot record_{};
};
inline const char* BesLine(BesPhase phase,int owed,int heat,int daysCold)noexcept{
    switch(phase){
    case BesPhase::Unheard:
        return "I work the ember route. If I miss a day's pay, come back and hear me before anyone writes my story.";
    case BesPhase::WagesReported:
        return "I told you my pay was missed. Visit my actual hearth, then ask Rheva to open the common ledger.";
    case BesPhase::Documented:
        return "Rheva saw the same unpaid marks. Bring a real five-basket pack home, join my stoppage, or fund full restitution.";
    case BesPhase::ReliefGiven:
        return "You brought warmth to my home. The shelter benefited, but my wages remain a separate debt.";
    case BesPhase::Walkout:
        return "I refuse another unpaid shift. Production has fallen by one worker; receipts can fund my restitution.";
    case BesPhase::Settled:
        return "My wage claim was resolved without printing marks. No one should mistake cold days for debts forgiven.";
    case BesPhase::Underwritten:
        return "Mera lent her OWN savings to pay me. The hearth is warmer, but the city still owes her. Please remember whose sacrifice this was.";
    }
    (void)owed;(void)heat;(void)daysCold;
    return "";
}
} // namespace UnmadeCore
