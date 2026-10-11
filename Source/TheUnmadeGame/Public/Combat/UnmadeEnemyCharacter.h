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
    void ConfigureStyle(UnmadeCore::EnemyStyle NewStyle);
    void SetVisualScale(FVector Scale);
    void ExposeToFold(double Now, double Seconds = 4.0);
    bool IsFractureExposed(double Now) const;
    void ConfigureEncounter(FName Identity,const FString& Name,const FLinearColor& Accent);

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

    UnmadeCore::EnemyStyle Style = UnmadeCore::EnemyStyle::Stalker;
    double FoldExposedUntil = -1.0;
    bool bDeathHandled = false;
    TWeakObjectPtr<AUnmadePrototypeHub> CachedRealityHub;
};
