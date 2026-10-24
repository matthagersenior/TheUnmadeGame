#pragma once

#include "CoreMinimal.h"

class UUnmadeMemoryComponent;

/**
 * Pure boundary helpers for a dialogue-only local model.
 * No world, inventory, quest or save-system mutation occurs here.
 */
class THEUNMADEGAME_API FUnmadeDialoguePolicy
{
public:
    static constexpr int32 MaxContextEvents = 8;
    static constexpr int32 MaxDialogueCharacters = 240;

    /** NPC-specific evidence, including where hearsay came from. */
    static FString BuildEvidence(const UUnmadeMemoryComponent* Memory);

    /** Grounded, deterministic response for offline or rejected model requests. */
    static FString BuildOfflineReply(const UUnmadeMemoryComponent* Memory,
                                     const FString& PlayerUtterance,
                                     const FString& CharacterVoice);

    /** Accept only a completed Ollama /api/chat response containing JSON {"line":"..."}. */
    static bool TryExtractLine(const FString& ResponseJson, FString& OutLine);
};
