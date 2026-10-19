#pragma once

#include "Items/UnmadeItemRules.h"
#include "Items/UnmadeItemRules.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fracture/UnmadeFractureRules.h"
#include "Player/UnmadeCharacterIdentityRules.h"
#include "Story/UnmadeConflictRules.h"
#include "Story/UnmadeCommitmentRules.h"
#include "World/UnmadeFactionChronicleRules.h"
#include "World/UnmadeTenfoldChronicle.h"
#include "World/UnmadeAbilityStudyRules.h"
class UUnmadePrototypeSave;
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
    bool HasRejectedSave()const{return bFractureSaveRejected||bConflictSaveRejected|| bCharacterIdentitySaveRejected||bStudySaveRejected;}
    void CaptureSession(UUnmadePrototypeSave* Save)const;
    bool SaveSessionNow();
    void CraftRecipeSelection(int Recipe);
    void TradeSelection(UnmadeCore::ItemId Item,bool Buy);
    AUnmadeCharacter();
    void PlayIdentityMotion(FName Cue);
    void RpgMenuAction(FName Action);
    void OpenRpgPage(int Page);
    void DodgeStep();
    void StartSneak(){Crouch();}
    void StopSneak(){UnCrouch();}
    void UseFieldTechnique();
    void UseFieldKit();
    void PersonalChoiceA();
    void PersonalChoiceB();
    void OpenWorldMap();
    bool IsRpgMenuOpen()const;
    UPROPERTY(Transient) TObjectPtr<class UUnmadeRpgWidget> RpgWidget;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    UUnmadeCombatComponent* GetCombat() const { return Combat; }
    UUnmadeEquipmentComponent* GetEquipment() const { return Equipment; }
    bool IsBorrowedLifeActive() const;
    int32 GetBorrowedCraftLevel() const;
    int32 GetBorrowedLifeRole() const;
    UnmadeCore::FractureSnapshot GetRealitySnapshot() const { return FractureModel.TakeSnapshot(); }
    double GetRealityStrain() const { return FractureModel.CurrentStrain(); }
    bool SpendRealityStrain(int32 Cost) { return FractureModel.SpendStrain(Cost); }
    void RefundRealityStrain(int32 Cost) {
        FractureModel.Recover(static_cast<double>(Cost) / UnmadeCore::FractureModel::RecoveryPerSecond);
    }
    bool HasNearbyHollowKeeper() const;
    void ApplyRiteAbility(UnmadeCore::RiteId Rite, const UnmadeCore::RiteEffect& Effect,
                          int32 SelectedLaw);
    void ReportRiteWitnessEvent();
    void ReportRealityConsequenceEvent(UnmadeCore::RiteId Id);
    void ReportRiteChoiceEvent(int32 Choice);
    void ReportBrokenOathEvent();
    void ReportRedeemedOathEvent();
    void ReconcileEarnedRewards();
    UFUNCTION(BlueprintCallable,Category="Unmade|Ability Study") void StudyObserve();
    UFUNCTION(BlueprintCallable,Category="Unmade|Ability Study") void StudyProgress();
    UFUNCTION(BlueprintCallable,Category="Unmade|Ability Study") void StudyUse();
    UFUNCTION(BlueprintCallable,Category="Unmade|Ability Study") void StudyNext();
    UFUNCTION(BlueprintPure,Category="Unmade|Ability Study") FString GetStudyJournal()const;

    /** Persistent identity choices used by the native Character page. */
    UFUNCTION(BlueprintCallable, Category="Unmade|Identity")
    bool SetCharacterFeature(int32 Aspect,int32 Choice);
    int GetCharacterFeature(int Aspect)const;

    UFUNCTION(BlueprintCallable, Category="Unmade|Identity")
    bool SetCharacterChosenName(const FString& Name);

    UFUNCTION(BlueprintPure, Category="Unmade|Identity")
    FString GetCharacterChosenName() const;

    void NotifyProjectileImpact(class AUnmadeEnemyCharacter* Target,float Before){HandleMeleeImpact(Target,Before);}
    void NotifyWeaponContact(){TickWeaponContact();}
    void ReportIntegrationEvent(FName Event,FName Subject){ReportLocalEvent(Event,Subject);}
    void RecoverIntegrationStrain(double Amount){FractureModel.Recover(Amount/UnmadeCore::FractureModel::RecoveryPerSecond);}
private:
    UnmadeCore::AbilityStudy AbilityStudy;
    bool bStudySaveRejected=false;
    int32 SelectedStudy=0;
    double NextStudyTrialAt=0;
    double NextStudyPowerAt=0;
    double StudyWardUntil=-1;
    bool PersistStudy();
    bool RecordStudyEvidence(UnmadeCore::StudyId Id,int Bit);
    class AUnmadeEnemyCharacter* FindStudyEnemy(int32 Subject,bool RequireWarning=false)const;
    bool NearEchoStudySite()const;
    void TryClaimAtlas();
    UnmadeCore::ItemId SelectedTrade=UnmadeCore::ItemId::Count;
    bool bFractureSaveRejected=false,bConflictSaveRejected=false;
    bool bSessionRestored=false;
    double NextSessionSave=0;
    void RestoreSessionWhenReady();
    UPROPERTY(VisibleAnywhere, Category="Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, Category="Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<UStaticMeshComponent> PlaceholderBody;

    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<UStaticMeshComponent> PlaceholderCrest;

    UPROPERTY(VisibleAnywhere, Category="Unmade|Combat")
    TObjectPtr<UUnmadeCombatComponent> Combat;

    UPROPERTY(VisibleAnywhere, Category="Unmade|Lexicon")
    TObjectPtr<UUnmadeLexiconComponent> Lexicon;

    UPROPERTY(VisibleAnywhere, Category="Unmade|Equipment")
    TObjectPtr<UUnmadeEquipmentComponent> Equipment;

    UPROPERTY(VisibleAnywhere, Category="Unmade|SignatureRites")
    TObjectPtr<UUnmadeTenfoldComponent> Tenfold;

    void ShowInventory();
    void ShowCharacterProfile();
    void ApplyIdentitySilhouette();
    bool SaveCharacterIdentity();
    UnmadeCore::CharacterIdentity Identity;
    bool bCharacterIdentitySaveRejected=false;
    bool bSkeletalRigLoaded=false;
    FName PlayerMotion=NAME_None;
    double PlayerAttackMotionUntil=-1.0;
    void AnimatePlayer(FName Cue);
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
    UnmadeCore::CommitmentGate CommitmentGate;
    void CrossFrontierGateway();

    void ChooseShelter();
    void ChooseResearch();
    void ChooseLocalConflict(UnmadeCore::ConflictChoice Choice);
    void ProgressSupplyActivity();
    void ShowStoryJournal();
    void ShowEvidenceNotebook();
    bool SaveLocalConflict();
    void ApplyConflictGates();
    UnmadeCore::ConflictModel LocalConflict;
    UnmadeCore::ConflictChoice PendingStoryChoice = UnmadeCore::ConflictChoice::None;
    double PendingStoryExpiresAt = -1.0;

    void AttemptMeleeAttack();
    void TickWeaponContact();
    void HandleMeleeImpact(class AUnmadeEnemyCharacter* Target,float Before);
    double PlayerBuildSpeed=1;
    bool bFocusProjectile=false;
    double WeaponStrikeStarted=-1,WeaponStrikeRhythm=.75,WeaponReach=240;
    FVector PreviousWeaponTip=FVector::ZeroVector;
    double NextFootstepAt=0;
    float LastPresentationHealth=-1;
    void StartGuard();
    void StopGuard();
    bool bPlayerDefeatHandled = false;
    double DefeatRecordedAt=-1.0;
    uint64 LastHazardWarningPulse[2]={0,0};
    uint64 LastHazardImpactPulse[2]={0,0};
    uint64 LastLaterHazardWarningPulse[6]={0,0,0,0,0,0};
    uint64 LastLaterHazardPulse[6]={0,0,0,0,0,0};
    void RecoverAtSafeCheckpoint();

    void TurnView(float Value);
    void LookView(float Value);
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Interact();
    void SkipDirectedScene();
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
