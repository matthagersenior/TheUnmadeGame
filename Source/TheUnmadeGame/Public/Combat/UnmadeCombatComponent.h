#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/UnmadeCombatRules.h"
#include "Combat/UnmadeCombatRecoveryRules.h"
#include "Combat/UnmadeCombatStyleRules.h"
#include "Combat/UnmadeMasteryRules.h"
#include "Combat/UnmadeContactRules.h"
#include "Combat/UnmadeStatusDamageRules.h"
#include "UnmadeCombatComponent.generated.h"

// The weak original status owner is runtime-only, alongside the portable model
// copy. Profile load deliberately uses RestoreSession instead of this rollback.
struct FUnmadeCombatRecoveryState {
    UnmadeCore::CombatRecoveryState Combat;
    TWeakObjectPtr<AActor> StatusSource;
};

/** Authoritative native combat state. No LLM or paid service involved. */
UCLASS(ClassGroup=(Unmade), meta=(BlueprintSpawnableComponent))
class THEUNMADEGAME_API UUnmadeCombatComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UUnmadeCombatComponent();
    virtual void TickComponent(float Delta,ELevelTick Tick,FActorComponentTickFunction* Function)override;
    void EnablePlayerTempo(){bPlayerTempo=true;SetComponentTickEnabled(true);}
    void SetWeaponStyle(UnmadeCore::WeaponStyle Value,int Training){Style=Value;Mastery=FMath::Clamp(Training,0,3);State.SetCooldown(Style.cooldown);}
    float GetStamina()const{return bPlayerTempo?Tempo.Stamina():100;}
    bool Dodge(double Now){if(IsDefeated()||!bPlayerTempo||!Tempo.Dodge(Now,Mastery))return false;State.SetGuarding(false);return true;}
    void GrantEvasion(double Now,double Seconds){Tempo.GrantEvasion(Now,Seconds);}
    void GrantPressure(double Now){PressureUntil=Now+60;}
    void GrantQuiet(double Now){QuietUntil=Now+60;}
    bool HasQuietLens(double Now)const{return Now<QuietUntil;}
    bool HasTemporaryStatus()const;
    void ClearTemporaryStatus(){Status=UnmadeCore::CombatStatus::None;StatusUntil=0;StatusSource.Reset();}
    void GrantWard(double Now){WardUntil=Now+6;}
    bool UsePower(double Now){return UnmadeCore::TryMasteryPower(Tempo,Now,!IsDefeated(),bPlayerTempo,IsStaggered(Now));}
    void ApplyStatus(UnmadeCore::CombatStatus Type,double Seconds,AActor* Source=nullptr);
    bool IsStaggered(double Now)const{return Now<StaggerUntil;}
    float MoveMultiplier(double Now)const{return Now<StatusUntil&&Status==UnmadeCore::CombatStatus::Slow?.55f:1.f;}
    void Stagger(double Now,double Seconds){if(Resistance.Take(Now,Seconds,bHeavyResistance)){StaggerUntil=FMath::Max(StaggerUntil,Now+Seconds);Contact.Cancel();}}
    void SetHeavyResistance(bool Heavy){bHeavyResistance=Heavy;}
    bool BeginWeaponStrike(double Now);
    bool ContactActive(double Now)const{return Contact.Active(Now);}
    bool ResolveWeaponContact(UUnmadeCombatComponent* Target,double Now,bool Exposed);
    bool ReservedPlayerShot(double Now,uint64& Id,double& Damage,UnmadeCore::CombatStatus& Status);
    bool ReserveProjectile(double Now,uint64& Id,double& Damage);
    bool ReceiveTravelingHit(AActor* Source,uint64 Id,double Damage,double Now);
    void CancelWeaponStrike(){Contact.Cancel();}
    FString GetCombatFeedback()const{return Feedback;}


    void Configure(double MaxHealth, double Damage, double Cooldown);
    bool TryStrikeTarget(UUnmadeCombatComponent* Target, double Now, bool bValidHit, bool bFractureExposed);
    void SetGuarding(bool bEnabled);
    bool SetAttackDamage(double Damage){return State.SetDamage(Damage);}
    void SetGearBonuses(int32 Attack, int32 Armor);
    bool Heal(float Amount);
    /** A single identified storm/heat pulse, deduplicated by the combat domain. */
    bool ReceiveHazardPulse(uint64 SourceId,uint64 PulseId,double Damage);
    bool ReviveAtCheckpoint();
    FUnmadeCombatRecoveryState CaptureRecoveryState()const{return {{State,Tempo,Contact,Status,StaggerUntil,StatusUntil},StatusSource};}
    void RestoreRecoveryState(const FUnmadeCombatRecoveryState& Before){State=Before.Combat.body;Tempo=Before.Combat.tempo;Contact=Before.Combat.contact;Status=Before.Combat.status;StaggerUntil=Before.Combat.staggerUntil;StatusUntil=Before.Combat.statusUntil;StatusSource=Before.StatusSource;}
    bool RestoreSession(float Health,float Stamina){if(!FMath::IsFinite(Health)||!FMath::IsFinite(Stamina)||Health<0||Health>GetMaxHealth()||Stamina<0||Stamina>100)return false;return State.RestoreHealth(Health)&&Tempo.RestoreEnergy(Stamina); }
    float GetMissingHealth() const { return GetMaxHealth() - GetHealth(); }
    int32 GetGearAttackBonus() const { return GearAttack; }
    int32 GetGearArmorBonus() const { return GearArmor; }

    UFUNCTION(BlueprintPure, Category="Unmade|Combat")
    bool IsDefeated() const { return !State.IsAlive(); }

    UFUNCTION(BlueprintPure, Category="Unmade|Combat")
    float GetHealth() const { return static_cast<float>(State.Health()); }

    UFUNCTION(BlueprintPure, Category="Unmade|Combat")
    float GetMaxHealth() const { return static_cast<float>(State.MaximumHealth()); }

    UFUNCTION(BlueprintPure, Category="Unmade|Combat")
    bool IsGuarding() const { return State.IsGuarding(); }

private:
    UnmadeCore::ContactWindow Contact;
    UnmadeCore::StaggerResistance Resistance;
    bool bHeavyResistance=false;
    uint64 ReservedSwing=0;double ReservedDamage=0;UnmadeCore::StyleSwing ReservedStyle;
    bool ApplyReservedHit(UUnmadeCombatComponent* Target,double Now,uint64 Swing,double Damage,bool Exposed,const UnmadeCore::StyleSwing& Styled);
    bool bPlayerTempo=false;
    UnmadeCore::CombatTempo Tempo;
    UnmadeCore::WeaponStyle Style;
    UnmadeCore::CombatStatus Status=UnmadeCore::CombatStatus::None;
    TWeakObjectPtr<AActor> StatusSource;
    double PressureUntil=0,QuietUntil=0;
    double StatusUntil=0,NextBurn=0,StaggerUntil=0,WardUntil=0;
    uint64 BurnPulse=0;
    int Mastery=0;
    FString Feedback;
    UnmadeCore::Combatant State;
    int32 GearAttack = 0;
    int32 GearArmor = 0;
};
