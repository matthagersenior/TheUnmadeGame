#include "World/UnmadePrototypeHub.h"
#include "Combat/UnmadeRealmGuardian.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Player/UnmadeCharacter.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void AUnmadePrototypeHub::BuildRealmGuardians()
{
    if(!GetWorld())return;
    for(const auto& Spec:UnmadeCore::RealmGuardians)
    {
        const int idx=UnmadeCore::LaterIndex(Spec.realm);
        if(idx<0)continue;
        const auto& Place=UnmadeCore::LaterRealms[idx];
        // Physical north-terrace encounter near, but never on, the plinth.
        if(AUnmadeRealmGuardian* Guardian=GetWorld()->SpawnActor<AUnmadeRealmGuardian>(
            FVector(Place.centerX+1150,Place.centerY+2950,110),
            FRotator::ZeroRotator))
            Guardian->ConfigureGuardian(Spec.realm);
    }
}

void AUnmadePrototypeHub::RefreshRealmGuardians()
{
    if(!GetWorld())return;
    for(TActorIterator<AUnmadeRealmGuardian> It(GetWorld());It;++It)
    {
        const auto realm=It->GetRealm();
        const bool bReady=EchoQuest.Ending(realm)!=0;
        const bool bUnresolved=Guardians.Outcome(realm)==
            UnmadeCore::GuardianOutcome::Unresolved;
        const bool bActive=bReady && bUnresolved;
        It->SetActorHiddenInGame(!bActive);
        It->SetActorEnableCollision(bActive);
        It->SetActorTickEnabled(bActive);
    }
}

bool AUnmadePrototypeHub::ResolveRealmGuardian(
    UnmadeCore::Realm Realm,bool bMercy)
{
    if(bGuardianSaveRejected || bEchoSaveRejected || !GetWorld())
        return false;
    const auto Before=Guardians.Snapshot();
    const auto Result=Guardians.Resolve(
        Realm,EchoQuest.Ending(Realm)!=0,bMercy);
    if(Result!=UnmadeCore::GuardianResolution::Pacified &&
       Result!=UnmadeCore::GuardianResolution::Defeated)return false;
    if(!WriteWorldSnapshot())
    {
        Guardians.Restore(Before);
        return false;
    }
    RefreshRealmGuardians();
    const int idx=UnmadeCore::LaterIndex(Realm);
    if(idx>=0 && GEngine)
        GEngine->AddOnScreenDebugMessage(-1,12.f,FColor::Cyan,
            FString::Printf(TEXT("GUARDIAN %s: %s. The community remembers."),
                bMercy?TEXT("PACIFIED"):TEXT("DEFEATED"),
                UTF8_TO_TCHAR(UnmadeCore::RealmGuardians[idx].name)));
    return true;
}

bool AUnmadePrototypeHub::TryCalmNearbyGuardian(
    AUnmadeCharacter* Player,double CompetingNpcDistanceSq)
{
    if(!GetWorld() || !IsValid(Player))return false;
    const FVector Origin=Player->GetActorLocation();
    const double Limit=FMath::Min(FMath::Square(310.0),CompetingNpcDistanceSq);
    AUnmadeRealmGuardian* Target=nullptr;
    double Closest=Limit;
    for(TActorIterator<AUnmadeRealmGuardian> It(GetWorld());It;++It)
    {
        if(It->IsHidden() || It->GetCombat()->IsDefeated())continue;
        const double D=FVector::DistSquared(Origin,It->GetActorLocation());
        if(D>=Closest)continue;
        FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeGuardianMercy),false);
        Sight.AddIgnoredActor(Player);
        Sight.AddIgnoredActor(*It);
        if(GetWorld()->LineTraceTestByChannel(
            Origin+FVector(0,0,55),It->GetActorLocation()+FVector(0,0,55),
            ECC_Visibility,Sight))continue;
        Target=*It;Closest=D;
    }
    if(!Target)return false;
    const auto Realm=Target->GetRealm();
    const auto* Spec=UnmadeCore::LaterIndex(Realm)>=0
        ?&UnmadeCore::RealmGuardians[UnmadeCore::LaterIndex(Realm)]
        :nullptr;
    if(!Spec)return false;
    if(EchoQuest.Ending(Realm)==0)
    {
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Yellow,
            TEXT("The guardian cannot accept an empty promise. Finish this realm's witnessed Echo."));
        return true;
    }
    if(!ResolveRealmGuardian(Realm,true))
    {
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Red,
            TEXT("Guardian pact could not be saved; no resolution was recorded."));
        return true;
    }
    if(GEngine)
    {
        const int choice=EchoQuest.Ending(Realm);
        GEngine->AddOnScreenDebugMessage(-1,11.f,FColor::Green,
            FString::Printf(TEXT("%s: %s"),
                UTF8_TO_TCHAR(Spec->name),
                UTF8_TO_TCHAR(choice==1?Spec->careMemory:Spec->truthMemory)));
    }
    return true; // Direct nearby E; no hidden global mercy hotkey.
}
