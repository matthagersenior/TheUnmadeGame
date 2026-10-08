#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UnmadeLocalDialogueSubsystem.generated.h"

class AUnmadeNpcCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FUnmadeDialogueReady, FName, NpcId, const FString&, Line, bool, bUsedLocalModel);

/**
 * Dialogue-only client for optional localhost Ollama.
 * NPC/world decisions remain Unreal-authoritative; no cloud fallback or tools.
 */
UCLASS(Config=Game)
class THEUNMADEGAME_API UUnmadeLocalDialogueSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    /** Disabled by default: all dialogue works with deterministic fallback offline. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="Unmade|Local AI")
    bool bEnableLocalModel = false;

    /** Must exist locally in Ollama; never an external endpoint. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="Unmade|Local AI")
    FString LocalModelName = TEXT("qwen3:4b-instruct");

    UPROPERTY(BlueprintAssignable, Category="Unmade|Local AI")
    FUnmadeDialogueReady OnDialogueReady;

    /** Asynchronous if enabled; immediate deterministic line if unavailable or busy. */
    UFUNCTION(BlueprintCallable, Category="Unmade|Local AI")
    void RequestDialogue(AUnmadeNpcCharacter* Npc, const FString& PlayerUtterance);

private:
    bool bRequestInFlight = false;
    void Present(FName NpcId, const FString& Line, bool bUsedModel);
};
