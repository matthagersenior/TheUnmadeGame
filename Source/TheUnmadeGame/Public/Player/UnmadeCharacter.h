#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fracture/UnmadeFractureRules.h"
#include "UnmadeCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AUnmadeFractureAnchor;
class UStaticMeshComponent;
class UUnmadeCombatComponent;
class UUnmadeLexiconComponent;

/** Foundational third-person pawn. Visual mesh and Enhanced Input data assets are editor work. */
UCLASS()
class THEUNMADEGAME_API AUnmadeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AUnmadeCharacter();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    UUnmadeCombatComponent* GetCombat() const { return Combat; }

private:
    UPROPERTY(VisibleAnywhere, Category="Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, Category="Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<UStaticMeshComponent> PlaceholderBody;

    UPROPERTY(VisibleAnywhere, Category="Unmade|Combat")
    TObjectPtr<UUnmadeCombatComponent> Combat;

    UPROPERTY(VisibleAnywhere, Category="Unmade|Lexicon")
    TObjectPtr<UUnmadeLexiconComponent> Lexicon;

    void AttemptMeleeAttack();
    void StartGuard();
    void StopGuard();
    bool bPlayerDefeatHandled = false;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Interact();
    void OfferAid();
    void DemonstrateAnomaly();
    void ReportLocalEvent(FName EventKind, FName SubjectId = NAME_None);
    void SaveNearbyNpcMemories();
    void FoldReality();
    void RewriteOpen();
    void RewriteSealed();
    bool SaveFractureState();
    AUnmadeFractureAnchor* FindNearbyFractureAnchor() const;
    void ApplyFractureVisuals();
    void RewriteChoice(FName ChoiceId);

    UnmadeCore::FractureModel FractureModel{"region.prototype.hub", {"variant.open", "variant.sealed"}};
    FName PendingRewriteChoice = NAME_None;
    double PendingRewriteExpiresAt = -1.0;
};
