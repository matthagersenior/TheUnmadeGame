#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnmadeBesQuestTrackerWidget.generated.h"

class UBorder;
class UTextBlock;

/**
 * Read-only, non-modal native UMG objective for the saved wage quest.
 * No optional Blueprint or imported assets required.
 */
UCLASS(BlueprintType)
class THEUNMADEGAME_API UUnmadeBesQuestTrackerWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable,Category="Unmade|Unpaid Shift")
    void ShowObjective(const FString& Goal,const FString& Facts);
    UFUNCTION(BlueprintCallable,Category="Unmade|Unpaid Shift")
    void HideObjective();

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(Transient)
    TObjectPtr<UBorder> Background;
    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> ObjectiveText;
    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> LedgerText;
};
