#include "Combat/UnmadeCombatComponent.h"
#include "Combat/UnmadePresentation.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

UUnmadeCombatComponent::UUnmadeCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UUnmadeCombatComponent::Configure(double MaxHealth, double Damage, double Cooldown)
{
    State = UnmadeCore::Combatant(MaxHealth, Damage, Cooldown);
}

void UUnmadeCombatComponent::SetGearBonuses(int32 Attack, int32 Armor)
{
    GearAttack = FMath::Clamp(Attack, 0, 200);
    GearArmor = FMath::Clamp(Armor, 0, 200);
}

bool UUnmadeCombatComponent::Heal(float Amount)
{
    return State.Heal(static_cast<double>(Amount));
}

void UUnmadeCombatComponent::SetGuarding(bool bEnabled)
{
    if(bEnabled&&!State.IsGuarding())UnmadePresentation::Sound(GetOwner(),TEXT("guard"));
    if(bPlayerTempo&&bEnabled&&!State.IsGuarding()&&(!GetWorld()||!Tempo.Guard(GetWorld()->GetTimeSeconds(),Mastery)))return;
    State.SetGuarding(bEnabled);
}

bool UUnmadeCombatComponent::TryStrikeTarget(UUnmadeCombatComponent* Target, double Now,
                                              bool bValidHit, bool bFractureExposed)
{
    if (!IsValid(Target) || !bValidHit || Target == this || Target->IsDefeated())
    {
        return false;
    }
    AActor* OwnerActor = GetOwner();
    if (!IsValid(OwnerActor) || !IsValid(Target->GetOwner()))
    {
        return false;
    }
    if(IsStaggered(Now))return false;
    const auto BeforeTempo=Tempo;UnmadeCore::StyleSwing Styled;
    if(bPlayerTempo){Styled=Tempo.Attack(Now,Style,State.IsGuarding());if(!Styled.started){Feedback=TEXT("Recover stamina, release guard, or wait for your next strike.");return false;}}
    const UnmadeCore::AttackAttempt Swing = State.TryAttack(Now);
    if (Swing.outcome != UnmadeCore::AttackOutcome::Started)
    {
        Tempo=BeforeTempo;return false;
    }
    if(Target->State.DuplicateHit(OwnerActor->GetUniqueID(),Swing.swingId))return false;
    int Defense=Target->bPlayerTempo?Target->Tempo.Defense(Now,Target->State.IsGuarding()):0;
    if(Defense){Target->State.AcknowledgeHit(OwnerActor->GetUniqueID(),Swing.swingId);Target->Feedback=Defense==2?TEXT("PARRY - ENEMY STAGGERED"):TEXT("DODGE - STRIKE EVADED");if(Defense==2){Stagger(Now,1.4);if(auto* Pawn=Cast<ACharacter>(Target->GetOwner()))UnmadePresentation::Motion(Pawn->GetMesh(),TEXT("Resident"),TEXT("Parry"));}UnmadePresentation::Sound(Target->GetOwner(),TEXT("guard"));return false;}
    double Modifier=(bPlayerTempo?Styled.multiplier:1.)*(Now<StatusUntil&&Status==UnmadeCore::CombatStatus::Weaken?.7:1.);
    const auto Hit = Target->State.ReceiveHit(
        static_cast<std::uint64_t>(OwnerActor->GetUniqueID()),
        Swing.swingId, (Swing.baseDamage + GearAttack)*Modifier*(Now<Target->WardUntil?.5:1.), bFractureExposed||(Now<Target->StatusUntil&&Target->Status==UnmadeCore::CombatStatus::Expose), Target->GearArmor);
    if(Hit==UnmadeCore::HitOutcome::Applied){if(bPlayerTempo){Feedback=FString::Printf(TEXT("COMBO %d / %s"),Styled.combo,UTF8_TO_TCHAR(Style.name));if(Styled.combo==3){Target->Stagger(Now,.65);Target->ApplyStatus(Style.status,4);}}UnmadePresentation::Sound(Target->GetOwner(),Target->IsDefeated()?TEXT("death"):Target->State.IsGuarding()?TEXT("guard"):TEXT("impact"));}
    return Hit == UnmadeCore::HitOutcome::Applied;
}

bool UUnmadeCombatComponent::ReceiveHazardPulse(
    uint64 SourceId,uint64 PulseId,double Damage)
{
    if(State.DuplicateHit(SourceId,PulseId))return false;
    if(bPlayerTempo&&GetWorld()&&Tempo.Defense(GetWorld()->GetTimeSeconds(),false)){State.AcknowledgeHit(SourceId,PulseId);Feedback=TEXT("DODGED THE HAZARD PULSE");return false;}
    return State.ReceiveHit(static_cast<std::uint64_t>(SourceId),
                            static_cast<std::uint64_t>(PulseId),
                            Damage*(GetWorld()&&GetWorld()->GetTimeSeconds()<WardUntil?.5:1.),false,0.0)==UnmadeCore::HitOutcome::Applied;
}

bool UUnmadeCombatComponent::ReviveAtCheckpoint()
{
    if(!State.ReviveAtCheckpoint(0.75))return false;Tempo.Reset();Status=UnmadeCore::CombatStatus::None;StaggerUntil=0;return true;
}

void UUnmadeCombatComponent::ApplyStatus(UnmadeCore::CombatStatus Type,double Seconds){if(!GetWorld()||IsDefeated()||Type==UnmadeCore::CombatStatus::None||!FMath::IsFinite(Seconds)||Seconds<=0)return;Status=Type;StatusUntil=GetWorld()->GetTimeSeconds()+FMath::Min(Seconds,8.);NextBurn=GetWorld()->GetTimeSeconds()+1;}
void UUnmadeCombatComponent::TickComponent(float Delta,ELevelTick Tick,FActorComponentTickFunction* Function){Super::TickComponent(Delta,Tick,Function);if(!GetWorld()||IsDefeated())return;double Now=GetWorld()->GetTimeSeconds();if(bPlayerTempo){Tempo.Tick(Now,State.IsGuarding(),Mastery);if(Tempo.Stamina()<=0)State.SetGuarding(false);}if(Now>=StatusUntil){Status=UnmadeCore::CombatStatus::None;return;}if(Status==UnmadeCore::CombatStatus::Burn&&Now>=NextBurn){NextBurn=Now+1;State.ReceiveHit(static_cast<uint64>(GetUniqueID())+0x100000000ULL,++BurnPulse,4,false);UnmadePresentation::Sound(GetOwner(),TEXT("injury"));}}
