#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fracture/UnmadeFractureRules.h"
#include "Story/UnmadeConflictRules.h"
#include "World/UnmadeFactionChronicleRules.h"
#include "World/UnmadeTenfoldChronicle.h"
#include "UnmadeCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AUnmadeFractureAnchor;
class UStaticMeshComponent;
class UUnmadeCombatComponent;
class UUnmadeLexiconComponent;
class UUnmadeEquipmentComponent;
class UUnmadeTenfoldComponent;

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
    UUnmadeEquipmentComponent* GetEquipment() const { return Equipment; }
    bool IsBorrowedLifeActive() const;
    int32 GetBorrowedCraftLevel() const;
    int32 GetBorrowedLifeRole() const;
    double GetRealityStrain() const { return FractureModel.CurrentStrain(); }
    bool SpendRealityStrain(int32 Cost) { return FractureModel.SpendStrain(Cost); }
    void RefundRealityStrain(int32 Cost) {
        FractureModel.Recover(static_cast<double>(Cost) / UnmadeCore::FractureModel::RecoveryPerSecond);
    }
    bool HasNearbyHollowKeeper() const;
    void ApplyRiteAbility(UnmadeCore::RiteId Rite, const UnmadeCore::RiteEffect& Effect,
                          int32 SelectedLaw);
    void ReportRiteWitnessEvent();
    void ReportBrokenOathEvent();
    void ReportRedeemedOathEvent();
    void ReconcileEarnedRewards();

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

    UPROPERTY(VisibleAnywhere, Category="Unmade|Equipment")
    TObjectPtr<UUnmadeEquipmentComponent> Equipment;

    UPROPERTY(VisibleAnywhere, Category="Unmade|SignatureRites")
    TObjectPtr<UUnmadeTenfoldComponent> Tenfold;

    void ShowInventory();
    void EquipNextWeapon();
    void EquipNextArmor();
    void EquipNextCharm();
    void UseHealthPotion();
    void UseStrainPotion();
    void ForgeMythicGear();
    void BuyMarketSupplies();
    void SellMarketSupplies();
    void CraftLocalRecipe();
    void CommitSolidarity();
    void CommitTruth();
    void CommitFaction(UnmadeCore::FactionEnding Ending);
    void CrossFrontierGateway();

    void ChooseShelter();
    void ChooseResearch();
    void ChooseLocalConflict(UnmadeCore::ConflictChoice Choice);
    void ProgressSupplyActivity();
    void ShowStoryJournal();
    bool SaveLocalConflict();
    void ApplyConflictGates();
    UnmadeCore::ConflictModel LocalConflict;
    UnmadeCore::ConflictChoice PendingStoryChoice = UnmadeCore::ConflictChoice::None;
    double PendingStoryExpiresAt = -1.0;

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
