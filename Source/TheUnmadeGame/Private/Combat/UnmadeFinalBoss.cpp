#include "Combat/UnmadeFinalBoss.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadePrototypeHub.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AUnmadeFinalBoss::AUnmadeFinalBoss()
{
    PrimaryActorTick.TickInterval=.1f;
}
void AUnmadeFinalBoss::ConfigureAct(UnmadeCore::FinalAct Act)
{
    Rhythm.Reset();
    ConfiguredAct=static_cast<int32>(Act);
    bWarningShown=false;
    ConfigureStyle(UnmadeCore::EnemyStyle::Watcher);
    const bool bFirst=Act==UnmadeCore::FinalAct::Veil;
    const bool bEcho=Act==UnmadeCore::FinalAct::EchoAwake;
    GetCombat()->Configure(bFirst?330:bEcho?240:290,
                           bFirst?17:bEcho?24:21,.4);
    SetVisualScale(bFirst?FVector(2.1,2.1,2.75):
                   bEcho?FVector(1.8,2.8,2.3):FVector(3.1,1.5,3.1));
}
void AUnmadeFinalBoss::Tick(float DeltaSeconds)
{
    // No inherited untelegraphed Watcher attack, even after the reveal.
    ACharacter::Tick(DeltaSeconds);
    UWorld* World=GetWorld();
    if(!World)return;
    AUnmadePrototypeHub* Hub=Cast<AUnmadePrototypeHub>(
        UGameplayStatics::GetActorOfClass(World,AUnmadePrototypeHub::StaticClass()));
    if(!IsValid(Hub))return;
    const auto Act=Hub->GetFinalAct();
    const bool bFight=(Act==UnmadeCore::FinalAct::Veil &&
                       Hub->GetLaterRealmStage(UnmadeCore::Realm::FirstAbsence)==3) ||
                      Act==UnmadeCore::FinalAct::Unmasked ||
                      Act==UnmadeCore::FinalAct::EchoAwake;
    SetActorHiddenInGame(!bFight);
    SetActorEnableCollision(bFight);
    if(!bFight)return;
    if(ConfiguredAct!=static_cast<int>(Act))ConfigureAct(Act);
    if(GetCombat()->IsDefeated())
    {
        if(Act==UnmadeCore::FinalAct::Veil)
            Hub->BreakFinalMask(); // False victory: the second bar is ACTUALLY different.
        else if(Act==UnmadeCore::FinalAct::Unmasked)
            Hub->BreakFinalCore();
        else if(Act==UnmadeCore::FinalAct::EchoAwake)
            Hub->SettleFinalEcho();
        return; // failed persistence never invents a victory or instant heal
    }
    AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(
        UGameplayStatics::GetPlayerCharacter(World,0));
    if(!IsValid(Player)||!IsValid(Player->GetCombat())||
       Player->GetCombat()->IsDefeated())return;
    FVector Direction=Player->GetActorLocation()-GetActorLocation();
    Direction.Z=0;
    const double Dist=Direction.Size();
    const double Now=World->GetTimeSeconds();
    const auto Beat=Rhythm.Advance(Now,Dist,IsFractureExposed(Now),Act,
                                   Hub->GetFinalMemorySeed(),Hub->GetNewMorning());
    if(Beat.move==UnmadeCore::FinalMove::Disengage)
    {
        bWarningShown=false;
        return;
    }
    if(Beat.move==UnmadeCore::FinalMove::Approach)
    {
        AddActorWorldOffset(Direction.GetSafeNormal()*
            FMath::Clamp(DeltaSeconds,0.f,.25f)*110.f,true);
        bWarningShown=false;
    }
    else if(Beat.move==UnmadeCore::FinalMove::Telegraph)
    {
        if(!bWarningShown && GEngine)
            GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Orange,
                FString::Printf(TEXT("NHAL-VEY: %s (%.1f s warning)"),
                    UTF8_TO_TCHAR(Beat.warning),Beat.warningSeconds));
        bWarningShown=true;
    }
    else if(Beat.move==UnmadeCore::FinalMove::Stagger)
    {
        if(bWarningShown && GEngine)
            GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Green,
                TEXT("The last law breaks for a heartbeat. Your Fold interrupted the strike."));
        bWarningShown=false;
    }
    else if(Beat.move==UnmadeCore::FinalMove::Strike)
    {
        bWarningShown=false;
        FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeFinalBossSight),false);
        Sight.AddIgnoredActor(this);
        Sight.AddIgnoredActor(Player);
        const FVector Offset=Player->GetActorLocation()-GetActorLocation();
        const double Ahead=FVector::DotProduct(Offset,GetActorForwardVector());
        const double Side=FVector::DotProduct(Offset,GetActorRightVector());
        if(UnmadeCore::FinalStrikeInFootprint(Beat.attack,Ahead,Side) &&
           !World->LineTraceTestByChannel(
                GetActorLocation()+FVector(0,0,65),
                Player->GetActorLocation()+FVector(0,0,65),
                ECC_Visibility,Sight))
        {
            // Apply damage only after the previous warning, through the same
            // authoritative guard/armor/dedup combat component as all foes.
            const float Before=Player->GetCombat()->GetHealth();
            GetCombat()->SetGearBonuses(
                FMath::Max(0,FMath::RoundToInt(Beat.damage-17)),0);
            GetCombat()->TryStrikeTarget(Player->GetCombat(),Now,true,false);
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,
                FString::Printf(TEXT("A remembered attack lands: %.0f injury."),
                    Before-Player->GetCombat()->GetHealth()));
        }
    }
    // Never auto-track the player through a displayed warning: real
    // sidestepping must avoid the previously telegraphed hit footprint.
    if(Dist>50 && (Beat.move==UnmadeCore::FinalMove::Approach ||
                   Beat.move==UnmadeCore::FinalMove::Still))
        SetActorRotation(Direction.Rotation());
}
