#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/UnmadeCombatRules.h"
#include "Combat/UnmadeBossRules.h"
#include "UnmadeEnemyCharacter.generated.h"

class UUnmadeCombatComponent;
class UStaticMeshComponent;
class AUnmadePrototypeHub;

/** Two minimal, locally simulated enemy styles. No language model involved. */
UCLASS()
class THEUNMADEGAME_API AUnmadeEnemyCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AUnmadeEnemyCharacter();
    virtual void Tick(float DeltaSeconds) override;
    virtual void BeginPlay() override;
    void ConfigureStyle(UnmadeCore::EnemyStyle NewStyle);
    void SetVisualScale(FVector Scale);
    void ExposeToFold(double Now, double Seconds = 4.0);
    bool IsFractureExposed(double Now) const;
    void ConfigureEncounter(FName Identity,const FString& Name,const FLinearColor& Accent);
    void NotifyPlayerHit(float Damage);

    UUnmadeCombatComponent* GetCombat() const { return Combat; }
    UnmadeCore::EnemyStyle GetEnemyStyle() const { return Style; }

private:
    UPROPERTY(VisibleAnywhere, Category="Unmade|Combat")
    TObjectPtr<UUnmadeCombatComponent> Combat;

    UPROPERTY(VisibleAnywhere, Category="Unmade|Visual")
    TObjectPtr<UStaticMeshComponent> TemporaryEnemyVisual;
    UPROPERTY(VisibleAnywhere,Category="Unmade|Visual")
    TObjectPtr<UStaticMeshComponent> EncounterCrest;
    UPROPERTY(VisibleAnywhere,Category="Unmade|Visual")
    TObjectPtr<class UTextRenderComponent> EncounterLabel;

    UPROPERTY(VisibleAnywhere,Category="Unmade|Combat Feedback")
    TObjectPtr<class UTextRenderComponent> CombatSignal;
    UPROPERTY(VisibleAnywhere,Category="Unmade|Combat Feedback")
    TObjectPtr<class UPointLightComponent> TelegraphLight;
    FName CurrentMotion = NAME_None;
    TWeakObjectPtr<class UAnimSequence> CurrentMotionSequence;
    bool bSkeletalRigLoaded=false;
    double NextPathRefreshAt=0.0;
    FVector NextNavWaypoint=FVector::ZeroVector;
    bool bHasNavWaypoint=false;
    double WindupEndsAt=-1.0;
    double NextWindupAllowedAt=0.0;
    double HitFlashEndsAt=-1.0;
    void TryLoadEnemyRig();
    void SetMotionCue(FName Motion);
    UnmadeCore::EnemyStyle Style = UnmadeCore::EnemyStyle::Stalker;
    double FoldExposedUntil = -1.0;
    bool bDeathHandled = false;
    TWeakObjectPtr<AUnmadePrototypeHub> CachedRealityHub;
};
