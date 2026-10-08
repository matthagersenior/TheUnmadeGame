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

    /** Accept only a completed Ollama /api/chat response containing JSON {"line":"..."}. */
    static bool TryExtractLine(const FString& ResponseJson, FString& OutLine);
};
