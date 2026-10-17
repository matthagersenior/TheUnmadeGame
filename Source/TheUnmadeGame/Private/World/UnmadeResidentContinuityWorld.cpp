#include "UI/UnmadeRpgFeedback.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadeTenfoldComponent.h"
#include "Kismet/GameplayStatics.h"
#include "World/UnmadePrototypeHub.h"
#include "NPC/UnmadeResidentContinuityRules.h"
#include "Authoring/UnmadeResidentReturnData.h"
#include "Engine/Engine.h"

bool AUnmadePrototypeHub::RecordResidentConversation(FName ResidentId)
{
    if(bResidentContinuityRejected)return false;
    const FTCHARToUTF8 ID(*ResidentId.ToString());
    if(UnmadeCore::ResidentContinuityIndex(ID.Get())<0)return false;
    const auto Old=ResidentRelationships.Snapshot();
    const auto Result=ResidentRelationships.Talk(ID.Get(),GetGameDay());
    if(Result==UnmadeCore::EncounterEvent::SameDay)
        return true; // Speech repeat is free, no visit or relationship farming.
    if(Result!=UnmadeCore::EncounterEvent::FirstHello &&
       Result!=UnmadeCore::EncounterEvent::ReturnHello)return false;
    if(!WriteWorldSnapshot())
    {
        ResidentRelationships.Restore(Old);
        if(GEngine)UnmadeRpgFeedback(-1,6,FColor::Red,
            TEXT("The encounter was not saved; relationship progress has not changed."));
        return false;
    }
    return true;
}

bool AUnmadePrototypeHub::RecordResidentAid(FName ResidentId,bool AlreadyWitnessed)
{
    if(bResidentContinuityRejected||ResidentId.IsNone()||!GetWorld())return false;
    const FTCHARToUTF8 ID(*ResidentId.ToString());
    const bool Known=UnmadeCore::ResidentContinuityIndex(ID.Get())>=0;
    const bool Aided=Known&&ResidentRelationships.Aided(ID.Get());
    const auto Old=ResidentRelationships.Snapshot();
    if(Known&&!Aided&&ResidentRelationships.Aid(ID.Get())!=UnmadeCore::EncounterEvent::Aided)return false;
    bool Saved=false;
    if(AlreadyWitnessed||Aided){
        auto* Player=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
        auto* Rites=IsValid(Player)?Player->FindComponentByClass<UUnmadeTenfoldComponent>():nullptr;
        const bool MissingDeed=Rites&&!Rites->HasLegacyDeed(UnmadeCore::Deed::Protected);
        // Repair older split commits from real direct testimony or an existing
        // aid receipt, without inventing another event or farming trust.
        Saved=(!Known||Aided)&&!MissingDeed;
        if(!Saved)Saved=Rites?Rites->CommitActionDeeds(1u<<static_cast<int>(UnmadeCore::Deed::Protected),
            [&]{return WriteWorldSnapshot();}):WriteWorldSnapshot();
        if(Saved)NotifyRpg(TEXT("This resident's existing aid is already recorded; no repeat reward or testimony was added."));
    }else Saved=CommitWorldEvent(FName("Player.Helped"),ResidentId);
    if(!Saved){
        ResidentRelationships.Restore(Old);
        NotifyRpg(TEXT("Aid was not saved; witness, deed and relationship progress were restored. You can retry."));
    }
    return Saved;
}

FString AUnmadePrototypeHub::GetResidentReturnLine(
    FName ResidentId,UnmadeCore::Realm Home)const
{
    const FTCHARToUTF8 ID(*ResidentId.ToString());
    const bool LocallyKnown=Home==UnmadeCore::Realm::ThreefoldReach
        ?GetUnansweredRoadEvidence().openingWitnessed
        :UnmadeCore::RoadHas(GetUnansweredRoadEvidence(),Home);
    const auto* Profile=UnmadeCore::FindReturnDialogue(ID.Get());
    const auto Level=ResidentRelationships.Level(ID.Get());
    const bool Aided=ResidentRelationships.Aided(ID.Get());
    const bool OtherMorning=FinalStory.World()!=UnmadeCore::Morning::Unchosen;
    // Only if the actor has *actually* met the player on a previous day.
    // The local public record is not represented as a personal eyewitness claim.
    if(Profile && Level>=UnmadeCore::Familiarity::Acquainted)
    {
        const char* Line=OtherMorning&&LocallyKnown?Profile->newMorning:
            Aided?Profile->aid:
            LocallyKnown?Profile->publicLine:Profile->recognize;
        return FString(UTF8_TO_TCHAR(Line));
    }
    const auto Text=UnmadeCore::ResidentContinuityLine(
        Level,Aided,LocallyKnown,OtherMorning);
    return FString(UTF8_TO_TCHAR(Text));
}
