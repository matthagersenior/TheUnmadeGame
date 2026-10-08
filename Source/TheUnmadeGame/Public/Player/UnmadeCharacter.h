#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnmadeCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;

/** Foundational third-person pawn. Visual mesh and Enhanced Input data assets are editor work. */
UCLASS()
class THEUNMADEGAME_API AUnmadeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AUnmadeCharacter();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(VisibleAnywhere, Category="Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, Category="Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, Category="Prototype")
    TObjectPtr<UStaticMeshComponent> PlaceholderBody;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Interact();
    void OfferAid();
    void DemonstrateAnomaly();
    void ReportLocalEvent(FName EventKind);
    void SaveNearbyNpcMemories();
};
