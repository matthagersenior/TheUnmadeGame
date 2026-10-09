#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Lexicon/UnmadeLexiconRules.h"
#include "UnmadeLexiconComponent.generated.h"

/** The first authored two-clue language chain; independent of generative AI. */
UCLASS(ClassGroup=(Unmade), meta=(BlueprintSpawnableComponent))
class THEUNMADEGAME_API UUnmadeLexiconComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UUnmadeLexiconComponent();
    virtual void BeginPlay() override;

    /** Records a real clue once and persists it without modifying unrelated save data. */
    bool RecordEvidence(FName EvidenceId);

    UFUNCTION(BlueprintPure, Category="Unmade|Lexicon")
    bool UnderstandsVeyl() const;

    UFUNCTION(BlueprintPure, Category="Unmade|Lexicon")
    int32 GetClueCount() const;

private:
    UnmadeCore::LexiconModel Lexicon;
};
