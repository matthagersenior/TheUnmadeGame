#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnmadeFractureAnchor.generated.h"

class UStaticMeshComponent;

/** Placeholder geometry; Glimpse, Fold and Rewrite affect collision/visibility. */
UCLASS()
class THEUNMADEGAME_API AUnmadeFractureAnchor : public AActor
{
    GENERATED_BODY()
public:
    AUnmadeFractureAnchor();
    void ApplyFractureState(bool bGlimpsing, bool bFolded, FName PersistentVariant);
    bool IsBarrierBlocking() const;
    bool IsClueVisible() const;
private:
    UPROPERTY(VisibleAnywhere, Category="Fracture")
    TObjectPtr<UStaticMeshComponent> Barrier;
    UPROPERTY(VisibleAnywhere, Category="Fracture")
    TObjectPtr<UStaticMeshComponent> Clue;
};
