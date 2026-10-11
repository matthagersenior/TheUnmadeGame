#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnmadeUnpaidShiftSubtitleWidget.generated.h"

class UBorder;
class UTextBlock;

/** Non-modal, safe-area cinematic captions. Always works without WAV/VO.
 * One beat at a time; subtitles never float over the image's center.
 * BP may subclass this for art, but source retains an accessible fallback.
 */
UCLASS(BlueprintType)
class THEUNMADEGAME_API UUnmadeUnpaidShiftSubtitleWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable,Category="Unmade|Unpaid Shift")
    void DisplayBeat(const FString& Speaker,const FString& Subtitle);
    UFUNCTION(BlueprintCallable,Category="Unmade|Unpaid Shift")
    void ClearBeat();

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> SpeakerText;
    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> SubtitleText;
    UPROPERTY(Transient)
    TObjectPtr<UBorder> Backdrop;
};
