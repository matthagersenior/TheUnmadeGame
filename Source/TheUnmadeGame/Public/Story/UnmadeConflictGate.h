#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnmadeConflictGate.generated.h"

class UStaticMeshComponent;

/** Visible graybox route gate; independent of dialogue and optional local AI. */
UCLASS()
class THEUNMADEGAME_API AUnmadeConflictGate : public AActor
{
    GENERATED_BODY()

public:
    AUnmadeConflictGate();

    void Configure(FName StableGateId);
    void SetAccess(bool bAllowPassage);

    UFUNCTION(BlueprintPure, Category="Unmade|Story")
    bool IsPassable() const { return bPassable; }

    FName GetGateId() const { return GateId; }

private:
    UPROPERTY(VisibleAnywhere, Category="Unmade|Story")
    TObjectPtr<UStaticMeshComponent> GateMesh;

    UPROPERTY()
    FName GateId = NAME_None;

    bool bPassable = false;
};
