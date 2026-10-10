#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeRealmResonanceRules.h"
#include "World/UnmadeLaterRealmRules.h"
#include "Player/UnmadeCharacter.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Engine/World.h"
namespace {
FName ResonanceTag(int idx,const TCHAR* kind) {
    return FName(*FString::Printf(TEXT("Resonance.%d.%s"),idx,kind));
}
}

void AUnmadePrototypeHub::BuildRealmResonance()
{
    if(!GetWorld())return;
    for(int i=0;i<static_cast<int>(UnmadeCore::RealmResonances.size());++i)
    {
        const auto& Place=UnmadeCore::LaterRealms[i];
        const FVector Origin(Place.centerX,Place.centerY,0);
        // Reachable from the upper civic terrace only after prior choices.
        SpawnBlock(Origin+FVector(0,2830,105),FVector(.6,.8,2),
                   ResonanceTag(i,TEXT("Plinth")));
        // An honest extra crossing; absent until both mastered rites are
        // cast at the actual plinth, not a magical fast-travel reward.
        SpawnBlock(Origin+FVector(0,3850,20),FVector(8,7,.35),
                   ResonanceTag(i,TEXT("Bridge")));
        SpawnBlock(Origin+FVector(0,4300,-50),FVector(14,5,1),
                   ResonanceTag(i,TEXT("Island")));
        SpawnBlock(Origin+FVector(0,4380,105),FVector(.75,.7,2.3),
                   ResonanceTag(i,TEXT("Archive")));
    }
}

void AUnmadePrototypeHub::RefreshRealmResonanceWorld()
{
    for(int i=0;i<static_cast<int>(UnmadeCore::RealmResonances.size());++i)
    {
        const auto realm=UnmadeCore::RealmResonances[i].realm;
        const bool visible=EchoQuest.Ending(realm)!=0;
        const bool opened=Resonance.Stage(realm)==2;
        SetRiteWorldActorState(ResonanceTag(i,TEXT("Plinth")),visible);
        SetRiteWorldActorState(ResonanceTag(i,TEXT("Bridge")),opened);
        SetRiteWorldActorState(ResonanceTag(i,TEXT("Island")),opened);
        SetRiteWorldActorState(ResonanceTag(i,TEXT("Archive")),opened);
    }
}

bool AUnmadePrototypeHub::RecordNearbyRealmRite(
    AUnmadeCharacter* Player,UnmadeCore::RiteId Id)
{
    if(!IsValid(Player)||!GetWorld() || bResonanceSaveRejected ||
       bEchoSaveRejected || bLaterRealmSaveRejected || bRealmAftermathSaveRejected)
        return false;
    const FVector Position=Player->GetActorLocation();
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        if(It->IsHidden() ||
           FVector::DistSquared(Position,It->GetActorLocation())>
               FMath::Square(370.f))continue;
        for(int i=0;i<static_cast<int>(UnmadeCore::RealmResonances.size());++i)
        {
            if(!It->ActorHasTag(ResonanceTag(i,TEXT("Plinth"))))continue;
            FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeResonanceCast),false);
            Sight.AddIgnoredActor(Player);
            Sight.AddIgnoredActor(*It);
            if(GetWorld()->LineTraceTestByChannel(
                Position+FVector(0,0,50),
                It->GetActorLocation()+FVector(0,0,50),
                ECC_Visibility,Sight))continue;
            const auto& Spec=UnmadeCore::RealmResonances[i];
            const auto Before=Resonance.Snapshot();
            const auto Result=Resonance.Record(
                Spec.realm,Id,Clock.ElapsedSeconds(),
                EchoQuest.Ending(Spec.realm)!=0,true);
            if(Result==UnmadeCore::ResonanceResult::Started ||
               Result==UnmadeCore::ResonanceResult::ExpiredRestarted ||
               Result==UnmadeCore::ResonanceResult::Opened)
            {
                if(!WriteWorldSnapshot())
                {
                    Resonance.Restore(Before);
                    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,7.f,FColor::Red,
                        TEXT("Your rite succeeded, but its resonance did not save; try again."));
                    return true;
                }
                RefreshRealmResonanceWorld();
            }
            if(GEngine)
            {
                FString Notice;
                if(Result==UnmadeCore::ResonanceResult::Started ||
                   Result==UnmadeCore::ResonanceResult::ExpiredRestarted)
                    Notice=FString::Printf(TEXT("OBSERVATORY: %s. Cast the OTHER mastered rite here within two world-minutes."),
                                           UTF8_TO_TCHAR(Spec.question));
                else if(Result==UnmadeCore::ResonanceResult::Opened)
                    Notice=FString::Printf(TEXT("OBSERVATORY OPEN: %s. A bridge now reaches the archive."),
                                           UTF8_TO_TCHAR(Spec.title));
                else if(Result==UnmadeCore::ResonanceResult::WrongAbility)
                    Notice=TEXT("The plinth responds only to its two named disciplines.");
                else if(Result==UnmadeCore::ResonanceResult::AlreadyRecorded)
                    Notice=TEXT("That discipline has been witnessed. Cast the other named power.");
                else if(Result==UnmadeCore::ResonanceResult::AlreadyOpened)
                    Notice=TEXT("The observatory remains permanently open. Cross to inspect its records.");
                else Notice=TEXT("The observatory remains sealed until its earlier story is resolved.");
                GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Cyan,Notice);
            }
            return true;
        }
    }
    return false;
}

bool AUnmadePrototypeHub::InspectResonanceArchive(
    AUnmadeCharacter* Player,double CompetingNpcDistanceSq)
{
    if(!IsValid(Player) || !GetWorld())return false;
    const double limit=FMath::Min(FMath::Square(310.0),CompetingNpcDistanceSq);
    AStaticMeshActor* Best=nullptr;
    int index=-1;
    double closest=limit;
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        if(It->IsHidden())continue;
        const double d=FVector::DistSquared(Player->GetActorLocation(),
                                             It->GetActorLocation());
        if(d>=closest)continue;
        for(int i=0;i<static_cast<int>(UnmadeCore::RealmResonances.size());++i)
        {
            if(!It->ActorHasTag(ResonanceTag(i,TEXT("Archive"))) ||
               Resonance.Stage(UnmadeCore::RealmResonances[i].realm)!=2)continue;
            FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeResonanceArchive),false);
            Sight.AddIgnoredActor(Player);
            Sight.AddIgnoredActor(*It);
            if(GetWorld()->LineTraceTestByChannel(
                Player->GetActorLocation()+FVector(0,0,50),
                It->GetActorLocation()+FVector(0,0,50),
                ECC_Visibility,Sight))continue;
            Best=*It;index=i;closest=d;break;
        }
    }
    if(!Best || index<0)return false;
    const auto& Spec=UnmadeCore::RealmResonances[index];
    const int choice=EchoQuest.Ending(Spec.realm);
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Cyan,
        FString::Printf(TEXT("%s | %s | %s"),
            UTF8_TO_TCHAR(Spec.title),UTF8_TO_TCHAR(Spec.openedArchive),
            UTF8_TO_TCHAR(choice==1?Spec.responseCare:Spec.responseTruth)));
    return true; // One-time story state; reading does not farm items or marks.
}
