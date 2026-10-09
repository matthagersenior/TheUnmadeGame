#include "Combat/UnmadeBossCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Player/UnmadeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AUnmadeBossCharacter::AUnmadeBossCharacter()
{
    PrimaryActorTick.TickInterval=.1f;
}
void AUnmadeBossCharacter::ConfigureBoss(UnmadeCore::BossId Boss)
{
    const auto* Profile=UnmadeCore::FindBoss(Boss);
    if(!Profile) return;
    BossId=Boss;
    Encounter=UnmadeCore::BossEncounter(Boss);
    ConfigureStyle(UnmadeCore::EnemyStyle::Stalker);
    GetCombat()->Configure(Profile->health,Profile->damage,0.35);
    Tags.AddUnique(FName(UTF8_TO_TCHAR(Profile->name)));
}
void AUnmadeBossCharacter::Tick(float DeltaSeconds)
{
    // Intentionally bypass generic Stalker attacks; boss actions must be telegraphed.
    ACharacter::Tick(DeltaSeconds);
    if(BossId==UnmadeCore::BossId::None || !GetWorld())return;
    if(GetCombat()->IsDefeated())
    {
        if(!bDefeatedHandled)
        {
            bDefeatedHandled=true;
            SetActorEnableCollision(false);
            SetActorHiddenInGame(true);
            SetActorTickEnabled(false);
        }
        return;
    }
    const AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(
        UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    if(!IsValid(Player) || !IsValid(Player->GetCombat()) || Player->GetCombat()->IsDefeated())
        return;
    FVector Direction=Player->GetActorLocation()-GetActorLocation();
    Direction.Z=0;
    const double Distance=Direction.Size();
    if(Distance>2600) return;
    const auto* Profile=UnmadeCore::FindBoss(BossId);
    if(!Profile)return;
    const double Now=GetWorld()->GetTimeSeconds();
    const auto Beat=Encounter.Advance(Now,
        GetCombat()->GetHealth()/GetCombat()->GetMaxHealth(),Distance,
        IsFractureExposed(Now));
    if(Beat.action==UnmadeCore::BossAction::Approach)
    {
        AddActorWorldOffset(Direction.GetSafeNormal()*FMath::Max(0.f,DeltaSeconds)*105.f,true);
        bTelegraphShown=false;
    }
    else if(Beat.action==UnmadeCore::BossAction::Telegraph && !bTelegraphShown)
    {
        bTelegraphShown=true;
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Orange,
            FString::Printf(TEXT("BOSS WINDUP: %s. Evade or Fold to interrupt!"),
                UTF8_TO_TCHAR(Profile->name)));
    }
    else if(Beat.action==UnmadeCore::BossAction::Stagger)
    {
        bTelegraphShown=false;
    }
    else if(Beat.action==UnmadeCore::BossAction::Strike)
    {
        bTelegraphShown=false;
        // Combat health/damage are still authoritative and armor still mitigates.
        FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeBossSight),false);
        Sight.AddIgnoredActor(this);
        Sight.AddIgnoredActor(Player);
        const bool bClear=!GetWorld()->LineTraceTestByChannel(
            GetActorLocation()+FVector(0,0,50),
            Player->GetActorLocation()+FVector(0,0,50),ECC_Visibility,Sight);
        if(bClear)
        {
            GetCombat()->SetGearBonuses(FMath::RoundToInt(Beat.damage-Profile->damage),0);
            GetCombat()->TryStrikeTarget(Player->GetCombat(),Now,true,false);
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,
                FString::Printf(TEXT("BOSS ATTACK: %s"),UTF8_TO_TCHAR(Profile->name)));
        }
    }
    if(Distance>50)SetActorRotation(Direction.Rotation());
}
