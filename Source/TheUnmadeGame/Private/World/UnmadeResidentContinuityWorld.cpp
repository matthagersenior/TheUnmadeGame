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
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6,FColor::Red,
            TEXT("The encounter was not saved; relationship progress has not changed."));
        return false;
    }
    return true;
}

bool AUnmadePrototypeHub::RecordResidentAid(FName ResidentId)
{
    if(bResidentContinuityRejected)return false;
    const FTCHARToUTF8 ID(*ResidentId.ToString());
    if(UnmadeCore::ResidentContinuityIndex(ID.Get())<0)return false;
    const auto Old=ResidentRelationships.Snapshot();
    if(ResidentRelationships.Aid(ID.Get())!=UnmadeCore::EncounterEvent::Aided)
        return false;
    if(!WriteWorldSnapshot())
    {
        ResidentRelationships.Restore(Old);
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6,FColor::Red,
            TEXT("Aid recognition was not saved; the previous history is unchanged."));
        return false;
    }
    return true;
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
