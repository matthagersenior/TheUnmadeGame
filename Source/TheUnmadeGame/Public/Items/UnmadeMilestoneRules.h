#pragma once
#include "Items/UnmadeItemRules.h"
#include "World/UnmadeSettlementRegistry.h"
namespace UnmadeCore {
// Enumerate authored rewards from an already validated committed record only.
// The caller owns inventory persistence/rollback; these predicates grant nothing.
template<class Record,class Evidence,class Claim>
void ForEachEarnedMilestone(const Record& record,Evidence hasEvidence,Claim claim){
    const auto* Save=&record;
    const int Sites=Save->DiscoveredLoreMask;
    struct Entry { UnmadeCore::Achievement Id; bool Earned; };
    const Entry Earned[]={
        {UnmadeCore::Achievement::BellwoldLanterns,Save->BellwoldTaskStage==2},
        {UnmadeCore::Achievement::PaperhavenTestimony,Save->PaperhavenTaskStage==2},
        {UnmadeCore::Achievement::ThreeVillages,(Save->VisitedSettlementsMask&7)==7},
        {UnmadeCore::Achievement::EchoWell,(Sites&1)!=0},
        {UnmadeCore::Achievement::PaperOrchard,(Sites&2)!=0},
        {UnmadeCore::Achievement::SilentMile,(Sites&4)!=0},
        {UnmadeCore::Achievement::BellGrave,(Sites&8)!=0},
        {UnmadeCore::Achievement::DebtMarket,(Sites&16)!=0},
        {UnmadeCore::Achievement::AllLandmarks,(Sites&63)==63},
        {UnmadeCore::Achievement::ShelterSupplies,Save->bHasConflictSnapshot&&Save->SupplyActivityStage==2},
        {UnmadeCore::Achievement::ShelterChoice,Save->bHasConflictSnapshot&&Save->LocalConflictChoice==1},
        {UnmadeCore::Achievement::ResearchChoice,Save->bHasConflictSnapshot&&Save->LocalConflictChoice==2},
        {UnmadeCore::Achievement::LanguageVeyl,
            hasEvidence("evidence.glimpse") &&
            hasEvidence("evidence.archivist")},
        {UnmadeCore::Achievement::SaltwakeStory,
            Save->bHasFrontierSnapshot && Save->FrontierEndings.Num()==2 &&
            Save->FrontierEndings[0]!=0},
        {UnmadeCore::Achievement::CinderholdStory,
            Save->bHasFrontierSnapshot && Save->FrontierEndings.Num()==2 &&
            Save->FrontierEndings[1]!=0},
        {UnmadeCore::Achievement::RefugeCovenant,
            Save->bHasFactionChronicle && Save->FactionEndings.Num()==3 &&
            Save->FactionEndings[0]!=0},
        {UnmadeCore::Achievement::ArchiveCovenant,
            Save->bHasFactionChronicle && Save->FactionEndings.Num()==3 &&
            Save->FactionEndings[1]!=0},
        {UnmadeCore::Achievement::RoadboundCovenant,
            Save->bHasFactionChronicle && Save->FactionEndings.Num()==3 &&
            Save->FactionEndings[2]!=0},
        {UnmadeCore::Achievement::RiteUnwriteLaw,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[0]==5},
        {UnmadeCore::Achievement::RiteWitnesscraft,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[1]==5},
        {UnmadeCore::Achievement::RiteBorrowedLives,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[2]==5},
        {UnmadeCore::Achievement::RiteLegacyForging,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[3]==5},
        {UnmadeCore::Achievement::RiteLivingRoads,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[4]==5},
        {UnmadeCore::Achievement::RiteTomorrowDebt,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[5]==5},
        {UnmadeCore::Achievement::RiteUnderstandingBosses,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[6]==5},
        {UnmadeCore::Achievement::RiteParadoxConvergence,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[7]==5},
        {UnmadeCore::Achievement::RiteOathbinding,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[8]==5},
        {UnmadeCore::Achievement::RiteCartography,
            Save->bHasTenfoldChronicle && Save->RiteStages.Num()==10 && Save->RiteStages[9]==5},
        {UnmadeCore::Achievement::ConfluenceSilentAlarm,
            Save->bHasConfluenceSnapshot && Save->ConfluenceStages.Num()==6 && Save->ConfluenceStages[0]==2},
        {UnmadeCore::Achievement::ConfluenceTwoNames,
            Save->bHasConfluenceSnapshot && Save->ConfluenceStages.Num()==6 && Save->ConfluenceStages[1]==2},
        {UnmadeCore::Achievement::ConfluenceUnmappedWay,
            Save->bHasConfluenceSnapshot && Save->ConfluenceStages.Num()==6 && Save->ConfluenceStages[2]==2},
        {UnmadeCore::Achievement::ConfluenceTomorrowPromise,
            Save->bHasConfluenceSnapshot && Save->ConfluenceStages.Num()==6 && Save->ConfluenceStages[3]==2},
        {UnmadeCore::Achievement::ConfluenceTwoKeepers,
            Save->bHasConfluenceSnapshot && Save->ConfluenceStages.Num()==6 && Save->ConfluenceStages[4]==2},
        {UnmadeCore::Achievement::ConfluenceFirstAbsence,
            Save->bHasConfluenceSnapshot && Save->ConfluenceStages.Num()==6 && Save->ConfluenceStages[5]==2},
        {UnmadeCore::Achievement::AfterlightShelter,
            Save->bHasAfterlightSnapshot && Save->BellwoldAfterlightStage==4 &&
            Save->BellwoldAfterlightOutcome==1},
        {UnmadeCore::Achievement::AfterlightNames,
            Save->bHasAfterlightSnapshot && Save->BellwoldAfterlightStage==4 &&
            Save->BellwoldAfterlightOutcome==2}
    };
    for(const Entry& Reward: Earned)
        if(Reward.Earned) claim(Reward.Id);
}
// These are the original three finite player contracts, distinct from named wages.
template<class Record,class Pay>
void ForEachCivicContract(const Record& record,Pay pay){
    const auto* Save=&record;
    pay(SettlementId::Crossings,Save->bHasConflictSnapshot&&Save->LocalConflictChoice!=0?2:0);
    pay(SettlementId::Bellwold,Save->BellwoldTaskStage);
    pay(SettlementId::Paperhaven,Save->PaperhavenTaskStage);
}
}
