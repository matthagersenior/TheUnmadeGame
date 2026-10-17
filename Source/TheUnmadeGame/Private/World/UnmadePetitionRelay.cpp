#include "World/UnmadePrototypeHub.h"
#include "Player/UnmadeCharacter.h"
#include "Save/UnmadeSaveSession.h"
#include "UI/UnmadeRpgFeedback.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "EngineUtils.h"

void AUnmadePrototypeHub::RefreshPetitionRelayWorld()
{
    // Distinct graybox evidence in all three realms; not a remote conversation.
    const int Stage=PetitionRelay.Stage();
    const int Route=PetitionRelay.Route();
    SetRiteWorldActorState(FName(TEXT("Relay.CinderholdCopy")),
        HearthPetition.Stage()==4&&Stage==0);
    SetRiteWorldActorState(FName(TEXT("Relay.SaltwakePrivate")),Stage>=2&&Route==1);
    SetRiteWorldActorState(FName(TEXT("Relay.SaltwakePublic")),Stage>=2&&Route==2);
    SetRiteWorldActorState(FName(TEXT("Relay.CrossingsPrivate")),Stage==4&&Route==1);
    SetRiteWorldActorState(FName(TEXT("Relay.CrossingsPublic")),Stage==4&&Route==2);
}

FString AUnmadePrototypeHub::GetPetitionRelayJournal()const
{
    // This is the player's carried-document record, not a declaration of
    // what unvisited residents in another city somehow already know.
    return FString(TEXT("\nTHE THREE-REALM PETITION RELAY\n"))+
        UTF8_TO_TCHAR(PetitionRelay.Journal())+TEXT("\n");
}

FString AUnmadePrototypeHub::GetPetitionRelayResidentLine(FName StableId)const
{
    if(StableId==FName(TEXT("npc.saltwake.courier.001"))&&PetitionRelay.Stage()>=2)
        return FString(UTF8_TO_TCHAR(PetitionRelay.MeraLine()));
    if(StableId==FName(TEXT("npc.bridgekeeper.001"))&&PetitionRelay.Stage()==4)
        return FString(UTF8_TO_TCHAR(PetitionRelay.OrrelLine()));
    return FString();
}

bool AUnmadePrototypeHub::RecordPetitionRelayConversation(FName StableId)
{
    if(!bWorldReady||bPetitionRelayRejected||bHearthPetitionRejected||
       bBesHouseholdRejected||!UnmadeSave::IsWorldWritable(GetWorld()))
        return false;
    const bool Mera=StableId==FName(TEXT("npc.saltwake.courier.001"));
    const bool Orrel=StableId==FName(TEXT("npc.bridgekeeper.001"));
    if(!Mera&&!Orrel)return false;
    const auto Event=UnmadeCore::CommitPetitionRelay(PetitionRelay,[&]{
        if(Mera&&PetitionRelay.Stage()==1)return PetitionRelay.GiveToMera(GetGameDay());
        if(Mera&&PetitionRelay.Stage()==2)return PetitionRelay.TakeMeraReply(GetGameDay());
        if(Orrel&&PetitionRelay.Stage()==3)return PetitionRelay.GiveToOrrel(GetGameDay());
        return UnmadeCore::RelayEvent::NotReady;
    },[&]{return WriteWorldSnapshot();});
    if(Event==UnmadeCore::RelayCommit::Unavailable)return false;
    if(Event==UnmadeCore::RelayCommit::SaveFailed)
    {
        NotifyRpg(TEXT("The conversation did not save. Document custody and NPC awareness were rolled back; retry."));
        return true;
    }
    RefreshPetitionRelayWorld();
    NotifyRpg(GetPetitionRelayJournal()+
        (Mera?GetPetitionRelayResidentLine(StableId):GetPetitionRelayResidentLine(StableId)));
    return true;
}

bool AUnmadePrototypeHub::TryPetitionRelayInteraction(
    AUnmadeCharacter* Player,double CompetingNpcDistanceSq)
{
    if(!IsValid(Player)||!GetWorld()||!bWorldReady||
       bPetitionRelayRejected||bHearthPetitionRejected||bBesHouseholdRejected||
       !UnmadeSave::IsSessionWritable(Player))return false;
    const FVector At=Player->GetActorLocation();
    const auto* Region=UnmadeCore::RealmReactionAt(At.X,At.Y);
    if(!Region)return false;
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        const bool Copy=It->ActorHasTag(FName(TEXT("Relay.CinderholdCopy")));
        const bool SaltPrivate=It->ActorHasTag(FName(TEXT("Relay.SaltwakePrivate")));
        const bool SaltPublic=It->ActorHasTag(FName(TEXT("Relay.SaltwakePublic")));
        const bool CrossPrivate=It->ActorHasTag(FName(TEXT("Relay.CrossingsPrivate")));
        const bool CrossPublic=It->ActorHasTag(FName(TEXT("Relay.CrossingsPublic")));
        if(!Copy&&!SaltPrivate&&!SaltPublic&&!CrossPrivate&&!CrossPublic)continue;
        if(Copy&&(Region->realm!=UnmadeCore::Realm::HearthBeneath||
                  HearthPetition.Stage()!=4||PetitionRelay.Stage()!=0))continue;
        if((SaltPrivate||SaltPublic)&&
          (Region->realm!=UnmadeCore::Realm::WidowedRain||
           PetitionRelay.Stage()<2||
           PetitionRelay.Route()!=(SaltPrivate?1:2)))continue;
        if((CrossPrivate||CrossPublic)&&
          (Region->realm!=UnmadeCore::Realm::TheReach|| // Crossings city
           PetitionRelay.Stage()!=4||
           PetitionRelay.Route()!=(CrossPrivate?1:2)))continue;
        const double Distance=FVector::DistSquared(At,It->GetActorLocation());
        if(Distance>FMath::Square(245.f)||Distance>=CompetingNpcDistanceSq)continue;
        FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadePetitionRelay),false);
        Sight.AddIgnoredActor(Player);Sight.AddIgnoredActor(*It);
        if(GetWorld()->LineTraceTestByChannel(
             At+FVector(0,0,70),It->GetActorLocation()+FVector(0,0,65),
             ECC_Visibility,Sight))continue;
        if(!Copy)
        {
            NotifyRpg((SaltPrivate||SaltPublic)
                ?(SaltPrivate
                   ?TEXT("SALTWAKE: Mera's sealed objection remains here. She received the private petition from your hands; it does not repay wages.")
                   :TEXT("SALTWAKE: Mera's redacted response asks whether public pressure might harm people with no power to answer."))
                :(CrossPrivate
                   ?TEXT("CROSSINGS: Orrel archives the sealed wage concern without exposing a refuge guest.")
                   :TEXT("CROSSINGS: Orrel's redacted docket requests an accounting, not a verdict.")));
            return true;
        }
        const auto Result=UnmadeCore::CommitPetitionRelay(PetitionRelay,[&]{
            return PetitionRelay.Collect(GetGameDay(),HearthPetition);
        },[&]{return WriteWorldSnapshot();});
        if(Result==UnmadeCore::RelayCommit::SaveFailed)
        {
            NotifyRpg(TEXT("Could not save the document pickup. No copy was carried."));
            return true;
        }
        if(Result==UnmadeCore::RelayCommit::Committed)
        {
            RefreshPetitionRelayWorld();
            NotifyRpg(GetPetitionRelayJournal());
            return true;
        }
    }
    return false;
}
