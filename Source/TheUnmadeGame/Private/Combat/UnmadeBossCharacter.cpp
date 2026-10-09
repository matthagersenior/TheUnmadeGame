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
    // Readable, non-identical blockout shapes for the three threats.
    SetVisualScale(Boss==UnmadeCore::BossId::HollowBell
        ? FVector(2.1,2.1,2.0)
        : Boss==UnmadeCore::BossId::RedactedCurator
        ? FVector(.9,.9,2.65) : FVector(1.4,1.2,2.35));
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
    if(Distance>2600)
    {
        Encounter.Reset(); // Re-entering an arena can never trigger an unseen old strike.
        bTelegraphShown=false;
        return;
    }
    const auto* Profile=UnmadeCore::FindBoss(BossId);
    if(!Profile)return;
    const double Now=GetWorld()->GetTimeSeconds();
    const auto Beat=Encounter.Advance(Now,
        GetCombat()->GetHealth()/GetCombat()->GetMaxHealth(),Distance,
        IsFractureExposed(Now));
    if(Beat.action==UnmadeCore::BossAction::Approach ||
       Beat.action==UnmadeCore::BossAction::Retreat ||
       Beat.action==UnmadeCore::BossAction::Charge)
    {
        const float Speed=Beat.action==UnmadeCore::BossAction::Charge?230.f:105.f;
        const float DirectionSign=Beat.action==UnmadeCore::BossAction::Retreat?-1.f:1.f;
        AddActorWorldOffset(Direction.GetSafeNormal()*
            FMath::Clamp(DeltaSeconds,0.f,.25f)*Speed*DirectionSign,true);
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
    else if(Beat.action==UnmadeCore::BossAction::Strike ||
            Beat.action==UnmadeCore::BossAction::Shockwave)
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
                FString::Printf(TEXT("%s: %s"),
                    Beat.action==UnmadeCore::BossAction::Shockwave
                        ? TEXT("BELL SHOCKWAVE") :
                        BossId==UnmadeCore::BossId::RedactedCurator
                        ? TEXT("REDACTED VOLLEY") : TEXT("PILGRIM'S CHARGE"),
                    UTF8_TO_TCHAR(Profile->name)));
        }
    }
    if(Distance>50)SetActorRotation(Direction.Rotation());
}
