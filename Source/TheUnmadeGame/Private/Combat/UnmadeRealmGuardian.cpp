#include "Combat/UnmadeRealmGuardian.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadePrototypeHub.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"

AUnmadeRealmGuardian::AUnmadeRealmGuardian()
{
    PrimaryActorTick.TickInterval=.1f;
}
void AUnmadeRealmGuardian::ConfigureGuardian(UnmadeCore::Realm Id)
{
    const int idx=UnmadeCore::LaterIndex(Id);
    if(idx<0)return;
    HomeRealm=Id;
    Encounter=UnmadeCore::GuardianBeat(Id);
    const auto& Profile=UnmadeCore::RealmGuardians[idx];
    ConfigureStyle(idx%2?UnmadeCore::EnemyStyle::Watcher:
                         UnmadeCore::EnemyStyle::Stalker);
    GetCombat()->Configure(Profile.maxHealth,Profile.damage,.7);
    SetVisualScale(FVector(1.25+idx*.11,.85+idx*.1,1.35+idx*.09));
    Tags.AddUnique(FName(*FString::Printf(TEXT("Guardian.%d"),idx)));
}
void AUnmadeRealmGuardian::SetGuardianResolved()
{
    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}
void AUnmadeRealmGuardian::Tick(float DeltaSeconds)
{
    // No inherited instant attacks: this NPC uses the authored warning model.
    ACharacter::Tick(DeltaSeconds);
    UWorld* World=GetWorld();
    const int idx=UnmadeCore::LaterIndex(HomeRealm);
    if(!World || idx<0)return;
    AUnmadePrototypeHub* Hub=Cast<AUnmadePrototypeHub>(
        UGameplayStatics::GetActorOfClass(World,AUnmadePrototypeHub::StaticClass()));
    if(!IsValid(Hub))return;
    if(Hub->GetRealmGuardianOutcome(HomeRealm)!=UnmadeCore::GuardianOutcome::Unresolved)
    {
        SetGuardianResolved();
        return;
    }
    if(GetCombat()->IsDefeated())
    {
        // Keep defeated actor available for retry if persistence fails. Never
        // falsely report an ending or discard an old save on a failed write.
        Hub->ResolveRealmGuardian(HomeRealm,false);
        return;
    }
    AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(
        UGameplayStatics::GetPlayerCharacter(World,0));
    if(!IsValid(Player)||!IsValid(Player->GetCombat())||
       Player->GetCombat()->IsDefeated())return;
    FVector Delta=Player->GetActorLocation()-GetActorLocation();
    Delta.Z=0;
    const double Distance=Delta.Size();
    const double Now=World->GetTimeSeconds();
    const auto Action=Encounter.Advance(Now,Distance,IsFractureExposed(Now),
        Hub->GetEchoQuestEnding(HomeRealm)!=0);
    if(Action==UnmadeCore::GuardianAction::Approach && Distance>180)
    {
        AddActorWorldOffset(Delta.GetSafeNormal()*FMath::Clamp(DeltaSeconds,0.f,.2f)*105.f,true);
        bWarningShown=false;
    }
    else if(Action==UnmadeCore::GuardianAction::Telegraph)
    {
        if(!bWarningShown && GEngine)
        {
            bWarningShown=true;
            GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Yellow,
                FString::Printf(TEXT("%s: %s"),
                    UTF8_TO_TCHAR(UnmadeCore::RealmGuardians[idx].name),
                    UTF8_TO_TCHAR(UnmadeCore::RealmGuardians[idx].warning)));
        }
    }
    else if(Action==UnmadeCore::GuardianAction::Strike)
    {
        bWarningShown=false;
        FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeGuardianSight),false);
        Sight.AddIgnoredActor(this);
        Sight.AddIgnoredActor(Player);
        const bool Clear=!World->LineTraceTestByChannel(
            GetActorLocation()+FVector(0,0,55),
            Player->GetActorLocation()+FVector(0,0,55),ECC_Visibility,Sight);
        if(Clear)
            GetCombat()->TryStrikeTarget(Player->GetCombat(),Now,true,false);
    }
    else if(Action==UnmadeCore::GuardianAction::Stagger)
    {
        bWarningShown=false;
    }
    if(Distance>50)SetActorRotation(Delta.Rotation());
}
