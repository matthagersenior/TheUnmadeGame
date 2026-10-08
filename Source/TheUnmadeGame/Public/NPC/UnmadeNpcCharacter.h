#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnmadeNpcCharacter.generated.h"

class UStaticMeshComponent;
class UUnmadeMemoryComponent;

UCLASS()
class THEUNMADEGAME_API AUnmadeNpcCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AUnmadeNpcCharacter();
    void ConfigureIdentity(FName StableId, const FString& DisplayLabel);
    FString GetReactionText() const;
    FName GetStableId() const { return NpcId; }
    const FString& GetDisplayLabel() const { return NpcDisplayLabel; }
    UUnmadeMemoryComponent* GetMemory() const { return Memory; }

private:
    UPROPERTY(VisibleAnywhere, Category="Unmade|Memory")
    TObjectPtr<UUnmadeMemoryComponent> Memory;

    UPROPERTY(VisibleAnywhere, Category="Unmade|Visual")
    TObjectPtr<UStaticMeshComponent> PlaceholderVisual;

    UPROPERTY()
    FName NpcId = NAME_None;

    UPROPERTY()
    FString NpcDisplayLabel;
};
