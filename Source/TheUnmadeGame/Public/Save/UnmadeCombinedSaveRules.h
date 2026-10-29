#pragma once
#include "NPC/UnmadeResidentContinuityRules.h"
#include "World/UnmadeLivingWorldRules.h"
#include "World/UnmadeWitnessEchoRules.h"
#include "World/UnmadeWitnessBraidRules.h"
#include "World/UnmadeWitnessDispatchRules.h"
#include "World/UnmadeWitnessReturnRules.h"
#include "World/UnmadeSettlementRegistry.h"
#include "World/UnmadeRememberedRoadRules.h"
#include "World/UnmadeRegionalTaskRules.h"
#include "World/UnmadeFactionChronicleRules.h"
#include "World/UnmadeFrontierRealmRules.h"
#include "World/UnmadeTenfoldChronicle.h"
#include "World/UnmadeRealityConsequenceRules.h"
#include "World/UnmadeRealmReactionRules.h"
#include "World/UnmadeMaterialConsequencesRules.h"
#include "World/UnmadeLivingEconomyRules.h"
#include "World/UnmadeBesHouseholdRules.h"
#include "World/UnmadeHearthPetitionRules.h"
#include "World/UnmadePetitionRelayRules.h"
#include "World/UnmadeCrossRealmCarryRules.h"
#include "World/UnmadeCinderholdAdventureRules.h"
#include "World/UnmadeAfterlightRules.h"
#include "World/UnmadeRealmAftermathRules.h"
#include "World/UnmadeDiscoveryCivicRules.h"
#include "World/UnmadeLaterRealmRules.h"
#include "World/UnmadeEchoQuestRules.h"
#include "World/UnmadeRealmResonanceRules.h"
#include "Combat/UnmadeRealmGuardianRules.h"
#include "Combat/UnmadeFinalEncounterRules.h"
#include "World/UnmadeExpeditionRules.h"
#include "World/UnmadeIdentityRules.h"
#include "World/UnmadeAdventureDepthRules.h"
#include "World/UnmadeAbilityStudyRules.h"
#include "Items/UnmadeItemRules.h"
#include "World/UnmadePlayerExperienceRules.h"
#include "Save/UnmadeSessionRules.h"
#include "Items/UnmadeUnifiedLoadoutRules.h"
#include "Items/UnmadeCraftEconomyRules.h"
#include "Player/UnmadeCharacterIdentityRules.h"
#include "Fracture/UnmadeFractureRules.h"
#include "Story/UnmadeConflictRules.h"
#include "Lexicon/UnmadeLexiconRules.h"
#include "World/UnmadeConfluenceRules.h"
#include <string>
#include <vector>
#include <cstdint>

namespace UnmadeCore {
template<class Stages> bool ValidSurveyRecord(int version,const Stages& stages){
    if(version!=1||stages.Num()!=6)return false;
    for(int stage:stages)if(stage<0||stage>2)return false;
    return true;
}

// Read-only preflight over the serialized record. Restore only disposable models:
// no actor state, rewards, migrations or disk writes may precede this decision.
// Record arrays provide Num()/indexing; Text converts names/strings to UTF-8,
// treating NAME_None as empty. Keeping this template engine-free makes the exact
// production validation executable against portable serialized-record fixtures.
template<class Record, class Text>
bool ValidCombinedRecord(const Record& record, Text text)
{
    const auto* Save=&record;
    if(Save->SchemaVersion!=1)return false;
    if(Save->bHasPlayerResume&&!ValidResume(Save->ResumePosition.X,
        Save->ResumePosition.Y,Save->ResumePosition.Z,Save->ResumeHealth,
        Save->ResumeStamina))return false;
    if((Save->bHasCinderholdAdventure||Save->bHasExpeditions||
        Save->bHasWorldIdentity||Save->bHasAdventureDepth||
        Save->bHasPlayerExperience||Save->bHasRememberedRoads||
        Save->bHasFactionChronicle||Save->bHasFrontierSnapshot||
        Save->bHasAfterlightSnapshot||Save->bHasRealmAftermathSnapshot||
        Save->bHasLaterRealmSnapshot||Save->bHasEchoQuestSnapshot||
        Save->bHasRealmResonanceSnapshot||Save->bHasGuardianSnapshot||
        Save->bHasFinalEncounterSnapshot||Save->bHasResidentContinuitySnapshot||
        Save->bHasWitnessEchoSnapshot||Save->bHasWitnessBraidSnapshot||
        Save->bHasWitnessDispatchSnapshot||Save->bHasWitnessReturnSnapshot||
        Save->bHasRealityConsequenceSnapshot||Save->bHasOuterRealmReactions||
        Save->bHasMaterialWorld||Save->bHasLivingEconomy||Save->bHasBesHousehold||
        Save->bHasHearthPetition||Save->bHasPetitionRelay||
        Save->bHasCrossRealmCarry)&&
        !Save->bHasLivingWorldSnapshot)return false;
    if(Save->bHasFractureSnapshot){
        FractureModel model("region.prototype.hub",{"variant.open","variant.sealed"});
        if(!model.Restore({text(Save->WorldVariantId),Save->PlayerStrain}))return false;
    }
    if(Save->bHasConflictSnapshot){
        ConflictModel model;
        if(!model.Restore({Save->LocalConflictChoice,Save->SupplyActivityStage}))return false;
    }
    if(Save->bHasCharacterIdentity){
        if(Save->CharacterIdentityChoices.Num()!=8)return false;
        CharacterIdentitySnapshot state;
        state.chosenName=text(Save->CharacterChosenName);
        for(int i=0;i<8;++i)state.options[i]=Save->CharacterIdentityChoices[i];
        CharacterIdentity model;if(!model.Restore(state))return false;
    }
    if(Save->bHasInventorySnapshot){
        InventorySnapshot state;
        if(Save->ItemQuantities.Num()<28||
           Save->ItemQuantities.Num()>static_cast<int>(ItemId::Count)||
           Save->EquippedItems.Num()!=3||Save->AwardedMilestoneBits<0)return false;
        for(int i=0;i<Save->ItemQuantities.Num();++i)state.quantities[i]=Save->ItemQuantities[i];
        for(int i=0;i<3;++i)state.equipped[i]=Save->EquippedItems[i];
        state.milestones=static_cast<std::uint64_t>(Save->AwardedMilestoneBits);
        InventoryModel model;if(!model.Restore(state))return false;
    }
    if(Save->bHasEconomySnapshot){
        if(Save->ProfessionSkills.Num()!=3||Save->PaidContractMask<0)return false;
        EconomySnapshot state;
        state.marks=Save->TradeMarks;
        for(int i=0;i<3;++i)state.craftsmanship[i]=Save->ProfessionSkills[i];
        state.completedContracts=static_cast<std::uint32_t>(Save->PaidContractMask);
        RegionalEconomy model;if(!model.Restore(state))return false;
    }
    std::vector<std::string> evidence;
    for(const auto& entry:Save->LexiconEvidence)evidence.push_back(text(entry));
    LexiconModel lexicon;if(!lexicon.Restore(evidence))return false;
    for(int n=0;n<Save->NpcSnapshots.Num();++n){
        const auto& npc=Save->NpcSnapshots[n];
        if(text(npc.NpcId).empty()||npc.Observations.Num()>64)return false;
        for(int prior=0;prior<n;++prior)
            if(npc.NpcId==Save->NpcSnapshots[prior].NpcId)return false;
        for(int i=0;i<npc.Observations.Num();++i){
            const auto& event=npc.Observations[i];
            const int kind=static_cast<int>(event.Evidence);
            if(!event.EventId.IsValid()||text(event.EventKind).empty()||kind<0||kind>1||
                (kind==1&&text(event.SpeakerId).empty()))return false;
            for(int j=0;j<i;++j)if(event.EventId==npc.Observations[j].EventId)return false;
        }
    }
    UnmadeCore::TenfoldChronicle RestoredTenfold;
    if(Save->bHasTenfoldChronicle){
        if(Save->RiteStages.Num()!=10||Save->RiteChoices.Num()!=10||
           Save->RiteReadyAt.Num()!=10||Save->DisciplineMastery.Num()!=6||
           Save->RiteTrialBits<0||Save->RiteTrialBits>1023||
           Save->LegacyDeedBits<0||Save->LegacyDeedBits>255||
           Save->VerifiedRoadBits<0||Save->VerifiedRoadBits>255)return false;
        TenfoldSnapshot state;
        for(int i=0;i<10;++i){state.stage[i]=Save->RiteStages[i];
            state.choice[i]=Save->RiteChoices[i];state.readyAt[i]=Save->RiteReadyAt[i];}
        for(int i=0;i<6;++i)state.disciplineMastery[i]=Save->DisciplineMastery[i];
        state.trialUsed=static_cast<std::uint16_t>(Save->RiteTrialBits);
        state.deeds=static_cast<std::uint8_t>(Save->LegacyDeedBits);
        state.verifiedRoutes=static_cast<std::uint8_t>(Save->VerifiedRoadBits);
        state.debtDueDay=Save->TomorrowDebtDueDay;
        state.exhaustedUntilDay=Save->TomorrowDebtExhaustedUntil;
        state.brokenOaths=Save->BrokenOathCount;
        state.oathActive=Save->bOathActive;state.oathRedeemed=Save->bOathRedeemed;
        if(!RestoredTenfold.Restore(state))return false;
    }
    if(Save->bHasConfluenceSnapshot){
        if(!Save->bHasTenfoldChronicle)return false;
        if(Save->ConfluenceStages.Num()!=6||Save->ConfluenceDecisions.Num()!=6||
           Save->ConfluenceCastMasks.Num()!=6||Save->ConfluenceWindowEnds.Num()!=6)return false;
        ConfluenceSnapshot state;
        for(int i=0;i<6;++i){state.stage[i]=Save->ConfluenceStages[i];
            state.decisions[i]=Save->ConfluenceDecisions[i];
            state.casts[i]=Save->ConfluenceCastMasks[i];state.windowEnds[i]=Save->ConfluenceWindowEnds[i];}
        ConfluenceJourney model;if(!model.Restore(state))return false;
    }
    if(Save->bHasFieldSurveys){
        if(!ValidSurveyRecord(1,Save->FieldSurveyStages))return false;
    }
    UnmadeCore::CinderholdAdventure Cinderhold;
    UnmadeCore::Expeditions Expeditions;
    UnmadeCore::WorldIdentity Identity;
    UnmadeCore::AdventureDepth Depth;
    UnmadeCore::PlayerExperience Experience;
    UnmadeCore::RememberedRoads RememberedRoads;
    UnmadeCore::LivingWorldClock RestoredClock;
    UnmadeCore::DiscoveryLedger RestoredDiscoveries;
    UnmadeCore::SettlementVisits RestoredVillages;
    UnmadeCore::RegionalTaskModel RestoredTasks;
    UnmadeCore::FactionChronicle RestoredChronicle;
    UnmadeCore::FrontierJourney RestoredFrontier;
    UnmadeCore::BellwoldAfterlight RestoredAfterlight;
    UnmadeCore::RealmAftermathChronicle RestoredRealmAftermath;
    UnmadeCore::LaterRealmJourney RestoredLaterRealm;
    UnmadeCore::EchoChronicle RestoredEcho;
    UnmadeCore::RealmResonanceJourney RestoredResonance;
    UnmadeCore::GuardianChronicle RestoredGuardians;
    UnmadeCore::FinalJourney RestoredFinal;
    UnmadeCore::ResidentContinuity RestoredResidentContinuity;
    UnmadeCore::WitnessEchoLedger RestoredWitnessEcho;
    UnmadeCore::WitnessBraidChronicle RestoredWitnessBraid;
    UnmadeCore::WitnessDispatch RestoredDispatch;
    UnmadeCore::WitnessReturn RestoredReturn;
    UnmadeCore::RealityConsequenceLedger RestoredReality;
    UnmadeCore::OuterRealmReactions RestoredOuter;
    UnmadeCore::MaterialWorld RestoredMaterials;
    UnmadeCore::LivingEconomy RestoredLiving;
    UnmadeCore::BesHousehold RestoredBes;
    UnmadeCore::HearthPetition RestoredPetition;
    UnmadeCore::PetitionRelay RestoredRelay;
    if (!RestoredClock.Restore(Save->LivingWorldSeconds) ||
        !RestoredDiscoveries.Restore(Save->DiscoveredLoreMask) ||
        !RestoredVillages.Restore(Save->VisitedSettlementsMask) ||
        !RestoredTasks.Restore({Save->BellwoldTaskStage, Save->PaperhavenTaskStage}))
    { return false; }
    if (Save->bHasFactionChronicle)
    {
        UnmadeCore::FactionSnapshot State;
        if (Save->FactionStages.Num()!=3 || Save->FactionEndings.Num()!=3)
        { return false; }
        for(int i=0;i<3;++i)
        {
            State.stages[i]=Save->FactionStages[i];
            State.endings[i]=Save->FactionEndings[i];
        }
        if (!RestoredChronicle.Restore(State))
        { return false; }
    }
    if(Save->bHasFrontierSnapshot)
    {
        UnmadeCore::FrontierSnapshot State;
        if(Save->FrontierStages.Num()!=2 || Save->FrontierEndings.Num()!=2)
        { return false; }
        State.visits=Save->VisitedFrontierRealms;
        State.discoveries=Save->DiscoveredFrontierClues;
        for(int i=0;i<2;++i)
        {
            State.stages[i]=Save->FrontierStages[i];
            State.endings[i]=Save->FrontierEndings[i];
        }
        if(!RestoredFrontier.Restore(State))
        { return false; }
    }
    if(Save->bHasAfterlightSnapshot &&
       !RestoredAfterlight.Restore({Save->BellwoldAfterlightStage,
           Save->BellwoldAfterlightApproach,Save->BellwoldAfterlightOutcome}))
    { return false; }
    if(Save->bHasRealmAftermathSnapshot)
    {
        if(Save->RealmAftermathStages.Num()!=9 || Save->RealmAftermathPrepared.Num()!=9 ||
           Save->RealmAftermathApproaches.Num()!=9 || Save->RealmAftermathEndings.Num()!=9)
        { return false; }
        UnmadeCore::RealmAftermathSnapshot State;
        for(int i=0;i<9;++i)
        {
            State.stage[i]=Save->RealmAftermathStages[i];
            State.prepared[i]=Save->RealmAftermathPrepared[i];
            State.approach[i]=Save->RealmAftermathApproaches[i];
            State.ending[i]=Save->RealmAftermathEndings[i];
        }
        if(!RestoredRealmAftermath.Restore(State))
        { return false; }
    }
    if(Save->bHasLaterRealmSnapshot)
    {
        if(Save->LaterRealmStages.Num()!=6 || Save->LaterRealmChoices.Num()!=6)
        { return false; }
        UnmadeCore::LaterRealmSnapshot State;
        State.visits=Save->LaterVisitedMask;
        for(int i=0;i<6;++i)
        {
            State.stage[i]=Save->LaterRealmStages[i];
            State.choice[i]=Save->LaterRealmChoices[i];
        }
        if(!RestoredLaterRealm.Restore(State))
        { return false; }
    }
    if(Save->bHasEchoQuestSnapshot)
    {
        if(Save->EchoQuestStages.Num()!=6 || Save->EchoQuestEvidence.Num()!=6 ||
           Save->EchoQuestTestimonies.Num()!=6 || Save->EchoQuestEndings.Num()!=6)
        { return false; }
        UnmadeCore::EchoSnapshot S;
        for(int i=0;i<6;++i)
        {
            S.stage[i]=Save->EchoQuestStages[i];
            S.evidence[i]=Save->EchoQuestEvidence[i];
            S.testimony[i]=Save->EchoQuestTestimonies[i];
            S.ending[i]=Save->EchoQuestEndings[i];
        }
        if(!RestoredEcho.Restore(S))
        { return false; }
    }
    if(Save->bHasRealmResonanceSnapshot)
    {
        if(Save->RealmResonanceStages.Num()!=6 ||
           Save->RealmResonanceFirstCasts.Num()!=6 ||
           Save->RealmResonanceDeadlines.Num()!=6)
        { return false; }
        UnmadeCore::ResonanceSnapshot S;
        for(int i=0;i<6;++i)
        {
            S.stage[i]=Save->RealmResonanceStages[i];
            S.firstCast[i]=Save->RealmResonanceFirstCasts[i];
            S.deadline[i]=Save->RealmResonanceDeadlines[i];
        }
        if(!RestoredResonance.Restore(S))
        { return false; }
        // An expired window is valid and restartable. A window whose first
        // cast lies after its own persisted clock is a contradictory record.
        for(int i=0;i<6;++i)if(S.stage[i]==1 &&
            S.deadline[i]>Save->LivingWorldSeconds+UnmadeCore::RealmResonanceJourney::WindowSeconds)return false;
        if(Save->bHasTenfoldChronicle)for(int i=0;i<6;++i){
            if(S.stage[i]==0)continue;
            const auto& spec=UnmadeCore::RealmResonances[i];
            const auto first=S.firstCast[i]==1?spec.first:spec.second;
            const auto other=S.firstCast[i]==1?spec.second:spec.first;
            if(!RestoredTenfold.IsMastered(first) ||
               (S.stage[i]==2 && !RestoredTenfold.IsMastered(other)))return false;
        }
    }
    if(Save->bHasGuardianSnapshot)
    {
        if(Save->RealmGuardianOutcomes.Num()!=6)
        { return false; }
        UnmadeCore::GuardianSnapshot S;
        for(int i=0;i<6;++i)S.outcomes[i]=Save->RealmGuardianOutcomes[i];
        if(!RestoredGuardians.Restore(S))
        { return false; }
    }
    // Optional older records may omit a chapter. When both sections are
    // present, independently valid snapshots must agree on authored gates.
    for(const auto& chapter:UnmadeCore::LaterRealms){
        const auto realm=chapter.realm;
        if(Save->bHasLaterRealmSnapshot && !RestoredLaterRealm.IsComplete(realm) &&
           ((Save->bHasRealmAftermathSnapshot && RestoredRealmAftermath.Stage(realm)>0) ||
            (Save->bHasEchoQuestSnapshot && RestoredEcho.Stage(realm)>0)))return false;
        if(Save->bHasRealmAftermathSnapshot && RestoredRealmAftermath.Stage(realm)!=4 &&
           Save->bHasEchoQuestSnapshot && RestoredEcho.Stage(realm)>0)return false;
        if(Save->bHasEchoQuestSnapshot && RestoredEcho.Ending(realm)==0 &&
           ((Save->bHasGuardianSnapshot && RestoredGuardians.Outcome(realm)!=UnmadeCore::GuardianOutcome::Unresolved) ||
            (Save->bHasRealmResonanceSnapshot && RestoredResonance.Stage(realm)>0)))return false;
    }
    if(Save->bHasFinalEncounterSnapshot)
    {
        const UnmadeCore::FinalSnapshot State{
            Save->FinalEncounterStage,Save->FinalMorningChoice,Save->FinalMemorySeed
        };
        if(!RestoredFinal.Restore(State))
        { return false; }
        if(Save->bHasLaterRealmSnapshot && State.act>0 &&
           !RestoredLaterRealm.IsComplete(UnmadeCore::Realm::FirstAbsence))return false;
    }
    if(Save->bHasResidentContinuitySnapshot)
    {
        if(Save->ResidentEncounterVisits.Num()!=UnmadeCore::ResidentContinuityCount ||
           Save->ResidentEncounterLastDays.Num()!=UnmadeCore::ResidentContinuityCount ||
           Save->ResidentEncounterAidFlags.Num()!=UnmadeCore::ResidentContinuityCount)
        { return false; }
        UnmadeCore::ResidentContinuitySnapshot RS;
        for(int i=0;i<UnmadeCore::ResidentContinuityCount;++i)
        {
            RS.visits[i]=Save->ResidentEncounterVisits[i];
            RS.lastDay[i]=Save->ResidentEncounterLastDays[i];
            RS.aids[i]=Save->ResidentEncounterAidFlags[i];
        }
        if(!RestoredResidentContinuity.Restore(RS))
        { return false; }
    }
    if(Save->bHasWitnessEchoSnapshot)
    {
        if(!RestoredWitnessEcho.Restore({
            static_cast<std::uint32_t>(Save->WitnessEchoFirstMask),
            static_cast<std::uint32_t>(Save->WitnessEchoReturnMask)}))
        { return false; }
    }
    if(Save->bHasWitnessBraidSnapshot &&
       !RestoredWitnessBraid.Restore({Save->WitnessBraidOutcome},RestoredWitnessEcho))
    { return false; }
    if(Save->bHasWitnessDispatchSnapshot &&
       !RestoredDispatch.Restore({Save->WitnessDispatchStage,
            Save->WitnessDispatchCollectedDay,Save->WitnessDispatchDeliveredDay},
            RestoredWitnessBraid.Outcome()))
    { return false; }
    if(Save->bHasWitnessReturnSnapshot &&
       !RestoredReturn.Restore({Save->WitnessReturnStage,Save->WitnessReturnRoute,
           Save->WitnessReturnChosenDay,Save->WitnessReturnCollectedDay,
           Save->WitnessReturnDeliveredDay},
           RestoredDispatch,RestoredWitnessBraid.Outcome(),
           RestoredClock.DayIndex()+1))
    { return false; }
    if(Save->bHasRealityConsequenceSnapshot &&
       (Save->RealityCareWitnessMask<0 || Save->RealityCareWitnessMask>1023 ||
        Save->RealityInquiryWitnessMask<0 || Save->RealityInquiryWitnessMask>1023 ||
        !RestoredReality.Restore({
            static_cast<std::uint16_t>(Save->RealityCareWitnessMask),
            static_cast<std::uint16_t>(Save->RealityInquiryWitnessMask),
            Save->RealityLastRite,Save->RealityActiveUntil
        },RestoredClock.ElapsedSeconds())))
    { return false; }
    if(Save->bHasOuterRealmReactions)
    {
        bool bValid=Save->OuterCareMasks.Num()==8 &&
            Save->OuterInquiryMasks.Num()==8 &&
            Save->OuterActiveRites.Num()==8 &&
            Save->OuterActiveUntil.Num()==8;
        UnmadeCore::OuterRealmReactions::Snapshot Snapshot{};
        if(bValid)
        {
            for(int i=0;i<8;++i)
            {
                const int Care=Save->OuterCareMasks[i],Inquiry=Save->OuterInquiryMasks[i];
                if(Care<0||Care>1023||Inquiry<0||Inquiry>1023)
                { bValid=false;break; }
                Snapshot[i]={
                    static_cast<std::uint16_t>(Care),static_cast<std::uint16_t>(Inquiry),
                    Save->OuterActiveRites[i],Save->OuterActiveUntil[i]};
            }
        }
        if(!bValid || !RestoredOuter.Restore(Snapshot,RestoredClock.ElapsedSeconds()))
        { return false; }
    }
    if(Save->bHasMaterialWorld)
    {
        const decltype(Save->MaterialDays)* Columns[10]={
            &Save->MaterialDays,&Save->MaterialStores,&Save->MaterialHouseholds,
            &Save->MaterialIntegrity,&Save->MaterialProduced,&Save->MaterialConsumed,
            &Save->MaterialShortageDays,&Save->MaterialLaborHours,
            &Save->MaterialDeliveries,&Save->MaterialRepairs
        };
        bool bValid=true;
        for(const auto* Column:Columns)if(Column->Num()!=9)bValid=false;
        UnmadeCore::MaterialWorld::Towns Data{};
        if(bValid)
            for(int i=0;i<9;++i)
                Data[i]={(*Columns[0])[i],(*Columns[1])[i],(*Columns[2])[i],
                    (*Columns[3])[i],(*Columns[4])[i],(*Columns[5])[i],
                    (*Columns[6])[i],(*Columns[7])[i],(*Columns[8])[i],
                    (*Columns[9])[i]};
        if(!bValid || !RestoredMaterials.Restore(Data,{
                Save->MaterialCargoRealm,Save->MaterialCargoUnits,
                Save->MaterialCargoDay},RestoredClock.DayIndex()+1))
        { return false; }
    }
    else RestoredMaterials.InitializeAtDay(RestoredClock.DayIndex()+1);
    if(Save->bHasLivingEconomy)
    {
        const decltype(Save->LivingEmployerMarks)* Workers[6]={
            &Save->LivingEmployerMarks,&Save->LivingEarnedMarks,
            &Save->LivingOwedMarks,&Save->LivingPaidShifts,
            &Save->LivingPaidDays,&Save->LivingWorkerReceipts
        };
        const decltype(Save->LivingForeignStock)* Routes[4]={
            &Save->LivingForeignStock,&Save->LivingRouteReceipts,
            &Save->LivingTradePayments,&Save->LivingConsumedSupport
        };
        bool bValid=true;
        for(const auto* Col:Workers)if(Col->Num()!=9)bValid=false;
        for(const auto* Col:Routes)if(Col->Num()!=2)bValid=false;
        auto Saved=RestoredLiving.Save();
        if(bValid)
        {
            for(int i=0;i<9;++i)
                Saved.workers[i]={(*Workers[0])[i],(*Workers[1])[i],
                    (*Workers[2])[i],(*Workers[3])[i],
                    (*Workers[4])[i],(*Workers[5])[i]};
            for(int i=0;i<2;++i)
                Saved.routes[i]={(*Routes[0])[i],(*Routes[1])[i],
                    (*Routes[2])[i],(*Routes[3])[i]};
            Saved.cargo={Save->LivingTradeCargoRoute,Save->LivingTradeCargoDay};
        }
        if(!bValid||!RestoredLiving.Restore(
            Saved,RestoredMaterials,RestoredClock.DayIndex()+1))
        { return false; }
    }
    else RestoredLiving.InitializeAtDay(RestoredClock.DayIndex()+1);
    if(Save->bHasBesHousehold)
    {
        const UnmadeCore::BesHouseholdSnapshot State={
            Save->BesQuestPhase,Save->BesHouseholdDay,
            Save->BesHeatBaskets,Save->BesColdDays,
            Save->BesWalkoutDays,Save->BesReliefPacks,
            Save->BesFirstReportDay,Save->BesMeraGuaranteeOwed,
            Save->bBesMeraConsented,Save->bBesMeraGuaranteeUsed,
            Save->bBesHomeVisited,Save->bBesRhevaInterviewed};
        if(!RestoredBes.Restore(State,RestoredClock.DayIndex()+1))
        { return false; }
    }
    else RestoredBes.InitializeAtDay(RestoredClock.DayIndex()+1);
    if(Save->bHasHearthPetition)
    {
        const auto& Data=Save->HearthPetitionState;
        if(Data.Num()!=6 || !RestoredPetition.Restore(
             {Data[0],Data[1],Data[2],Data[3],Data[4],Data[5]},
             RestoredBes.Save(),RestoredClock.DayIndex()+1))return false;
    }
    if(Save->bHasPetitionRelay)
    {
        if(!Save->bHasHearthPetition)return false;
        const auto& Data=Save->PetitionRelayState;
        if(Data.Num()!=6 || !RestoredRelay.Restore(
             {Data[0],Data[1],Data[2],Data[3],Data[4],Data[5]},
             RestoredPetition,RestoredClock.DayIndex()+1))return false;
    }
    if(Save->bHasCinderholdAdventure)
    {
        const auto& S=Save->CinderholdAdventureState;
        if(S.Num()!=10||!Cinderhold.Restore({S[0],S[1],S[2],S[3],S[4],S[5],S[6],S[7],S[8],S[9]},RestoredClock.DayIndex()+1))
        { return false; }
    }
    if(Save->bHasExpeditions){std::vector<int> Data;for(int V:Save->ExpeditionState)Data.push_back(V);if(!Expeditions.Restore(Data)){ return false; }}
    if(Cinderhold.Save().route==4 && (!Save->bHasExpeditions||
        !(Expeditions.Arc(2)->rooms&1)))return false;
    if(Save->bHasRealmAftermathSnapshot &&
       !CivicEvidenceValidOnRestore(RestoredRealmAftermath.Snapshot(),
            Expeditions,Save->bHasExpeditions))return false;
    if(Save->bHasWorldIdentity){std::vector<int> Data;for(int V:Save->WorldIdentityState)Data.push_back(V);if(!Save->bHasLivingWorldSnapshot||!Identity.Restore(Data,Expeditions,RestoredClock.DayIndex()+1)){ return false; }}
    if(Save->bHasAdventureDepth){std::vector<int> Data;for(int V:Save->AdventureDepthState)Data.push_back(V);if(!Depth.Restore(Data,Expeditions,RestoredClock.DayIndex()+1)){ return false; }}
    if(Save->bHasPlayerExperience){std::vector<int> Data;for(int V:Save->PlayerExperienceState)Data.push_back(V);if(!Experience.Restore(Data,Expeditions,Depth,RestoredClock.DayIndex()+1)){ return false; }}
    if(Save->bHasAbilityStudy){
        if(Save->AbilityStudyStages.Num()!=StudyCount ||
           Save->AbilityStudyEvidence.Num()!=StudyCount ||
           Save->AbilityStudyTrials.Num()!=StudyCount ||
           Save->AbilityStudySelection<0 ||
           Save->AbilityStudySelection>=StudyCount)return false;
        StudySnapshot snapshot;
        for(int i=0;i<StudyCount;++i){
            snapshot.stages[i]=Save->AbilityStudyStages[i];
            snapshot.evidence[i]=Save->AbilityStudyEvidence[i];
            snapshot.trials[i]=Save->AbilityStudyTrials[i];
        }
        AbilityStudy research;
        if(!research.Restore(snapshot))return false;
    }
    if(Save->bHasCrossRealmCarry){
        if(!Save->bHasExpeditions||!Save->bHasWorldIdentity||
           !Save->bHasAdventureDepth)return false;
        std::vector<int> data;
        for(int value:Save->CrossRealmCarryState)data.push_back(value);
        std::array<bool,9> sourceReady{},destVisited{},localEvidence{};
        for(int a=0;a<9;++a){
            sourceReady[a]=Depth.Arc(a)->choice!=0&&
                Expeditions.Arc(a)->returned&&Identity.Arc(a)->repair==7;
            destVisited[a]=(Expeditions.Arc(a)->visited&1)!=0;
            localEvidence[a]=Depth.Arc(a)->evidence!=0;
        }
        UnmadeCore::CrossRealmCarry proof;
        if(!proof.Restore(data,RestoredClock.DayIndex()+1,
                          sourceReady,destVisited,localEvidence))return false;
    }

    if(Save->bHasRememberedRoads)
    {
        std::vector<int> Data;for(int V:Save->RememberedRoadState)Data.push_back(V);
        if(!RememberedRoads.Restore(Data,RestoredClock.DayIndex()+1)||
           (RememberedRoads.City(4)->briefDay&&!Cinderhold.Has(UnmadeCore::ReturnedHome))||
           (RememberedRoads.City(4)->route&&RememberedRoads.City(4)->route!=Cinderhold.Save().route))
        { return false; }
    }
    if(Save->bHasUnifiedLoadout){
        if(Save->UnifiedRealmLoadout.Num()!=3)return false;
        UnifiedLoadout loadout{Save->UnifiedRealmLoadout[0],Save->UnifiedRealmLoadout[1],Save->UnifiedRealmLoadout[2]};
        if(!loadout.Valid(Expeditions))return false;
    }
    return true;
}
// Validate both directions of backup rotation before the first writer runs.
// Recovery never rotates its source, even if a failed primary write left bytes.
template<class Record, class Text, class Writer>
bool CommitCombinedRecord(const Record& next, const Record* primary,
    const Record* backup, bool recovering, bool hasRecord, Text text, Writer write)
{
    if(!ValidCombinedRecord(next,text))return false;
    if(primary&&primary->SchemaVersion!=1)return false;
    const Record* previous=!recovering&&primary&&ValidCombinedRecord(*primary,text)?primary:nullptr;
    if(hasRecord&&!previous&&!(backup&&ValidCombinedRecord(*backup,text)))return false;
    return CommitWithPrevious(next,previous,write);
}
} // namespace UnmadeCore
