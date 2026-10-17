#include "UI/UnmadeRpgFeedback.h"
#include "Combat/UnmadeEncounterInterruptionRules.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Combat/UnmadeBossCharacter.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "Save/UnmadeSaveSession.h"
#include "World/UnmadePrototypeHub.h"
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
    DefeatSave.Reset();bDefeatedHandled=false;bMercyPending=false;bTelegraphShown=false;
    SetActorTickEnabled(true);SetActorHiddenInGame(false);SetActorEnableCollision(true);
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
    if(GetCombat()->IsDefeated()||bMercyPending)
    {
        CommitDefeatReceipt();
        return;
    }
    const AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(
        UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    if(!UnmadeCore::RequireEncounterPlayer(IsValid(Player)&&IsValid(Player->GetCombat())&&
       !Player->GetCombat()->IsDefeated()&&Player->GetWorld()==GetWorld()&&UnmadeSave::IsSessionWritable(Player),[&]{
        UnmadeCore::InterruptEncounter(Encounter);bTelegraphShown=false;
        GetCharacterMovement()->StopMovementImmediately();
    }))return;
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
        if(GEngine)UnmadeRpgFeedback(-1,3.f,FColor::Orange,
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
            if(GEngine)UnmadeRpgFeedback(-1,3.f,FColor::Red,
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

bool AUnmadeBossCharacter::ResolveByMercy()
{
    if(BossId!=UnmadeCore::BossId::HollowBell||bDefeatedHandled||!IsValid(GetCombat())||GetCombat()->IsDefeated())return false;
    bMercyPending=true;
    CommitDefeatReceipt();
    return true;
}

void AUnmadeBossCharacter::CommitDefeatReceipt()
{
    if(bDefeatedHandled||!GetWorld()||!IsValid(GetCombat())||(!bMercyPending&&!GetCombat()->IsDefeated()))return;
    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
    // Keep ticking a settled combat encounter until its existing milestone receipt saves.
    if(!UnmadeSave::IsWorldWritable(GetWorld()))return;
    auto* Player=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    if(!IsValid(Player)||!IsValid(Player->GetEquipment()))return;
    auto* Equipment=Player->GetEquipment();
    const auto Result=DefeatSave.Attempt(static_cast<int>(BossId),GetWorld()->GetTimeSeconds(),[&]{
        return UnmadeCore::CommitNamedBossReward(BossId,
            [&](auto reward){return Equipment->HasClaimed(reward);},
            [&](auto reward){return Equipment->Claim(reward);});
    });
    if(Result==UnmadeCore::DefeatSaveResult::FirstFailure)
        UnmadeRpgFeedback(-1,10.f,FColor::Orange,TEXT("BOSS VICTORY NOT SAVED: retrying every five seconds. Make inventory room if needed; reloading returns to the previous save."));
    if(Result==UnmadeCore::DefeatSaveResult::Saved){
        bDefeatedHandled=true;
        SetActorTickEnabled(false);
        if(bMercyPending)UnmadeRpgFeedback(-1,9.f,FColor::Green,
            TEXT("HOLLOW KEEPER SPARED: testimony ended the battle without a killing blow."));
    }
}

bool AUnmadeBossCharacter::HasPendingDefeatSave(const AUnmadePrototypeHub& Hub)const
{
    if(Hub.GetWorld()!=GetWorld()||!IsValid(GetCombat()))return false;
    const auto* Player=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    const auto* Equipment=IsValid(Player)?Player->GetEquipment():nullptr;
    const bool Claimed=IsValid(Equipment)&&Equipment->HasClaimed(UnmadeCore::NamedBossReward(BossId));
    return UnmadeCore::NamedBossPending(BossId,GetCombat()->IsDefeated(),bMercyPending,Claimed);
}
