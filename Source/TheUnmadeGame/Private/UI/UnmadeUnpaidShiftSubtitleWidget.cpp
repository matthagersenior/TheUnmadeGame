#include "UI/UnmadeUnpaidShiftSubtitleWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"

void UUnmadeUnpaidShiftSubtitleWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // Build at runtime: no missing Blueprint or font asset can make subtitles
    // disappear. Canvas anchors give equal horizontal safe margins at 16:9,
    // 4:3, widescreen, and small windows instead of cropping the scene art.
    if(!WidgetTree)return;
    UCanvasPanel* Root=WidgetTree->ConstructWidget<UCanvasPanel>();
    WidgetTree->RootWidget=Root;

    Backdrop=WidgetTree->ConstructWidget<UBorder>();
    Backdrop->SetBrushColor(FLinearColor(0.012f,0.016f,0.027f,0.87f));
    if(UCanvasPanelSlot* Slot=Root->AddChildToCanvas(Backdrop))
    {
        Slot->SetAnchors(FAnchors(0.07f,0.76f,0.93f,0.955f));
        Slot->SetOffsets(FMargin(0.f));
        Slot->SetAlignment(FVector2D(0.f,0.f));
        Slot->SetAutoSize(false);
    }
    UVerticalBox* Stack=WidgetTree->ConstructWidget<UVerticalBox>();
    Backdrop->SetContent(Stack);
    Backdrop->SetPadding(FMargin(16.f,10.f,16.f,10.f));

    SpeakerText=WidgetTree->ConstructWidget<UTextBlock>();
    SpeakerText->SetAutoWrapText(true);
    SpeakerText->SetColorAndOpacity(FSlateColor(FLinearColor(0.97f,0.82f,0.57f,1.f)));
    FSlateFontInfo SpeakerFont=SpeakerText->GetFont();
    SpeakerFont.Size=20;
    SpeakerText->SetFont(SpeakerFont);
    if(UVerticalBoxSlot* S=Stack->AddChildToVerticalBox(SpeakerText))
        S->SetPadding(FMargin(0.f,0.f,0.f,5.f));

    SubtitleText=WidgetTree->ConstructWidget<UTextBlock>();
    SubtitleText->SetAutoWrapText(true);
    SubtitleText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    FSlateFontInfo CaptionFont=SubtitleText->GetFont();
    CaptionFont.Size=25;
    SubtitleText->SetFont(CaptionFont);
    Stack->AddChildToVerticalBox(SubtitleText);
    SetVisibility(ESlateVisibility::Collapsed);
}

void UUnmadeUnpaidShiftSubtitleWidget::DisplayBeat(
    const FString& Speaker,const FString& Subtitle)
{
    if(!SpeakerText||!SubtitleText)return;
    SpeakerText->SetText(FText::FromString(Speaker.IsEmpty()
        ?TEXT("THE UNMADE"):Speaker));
    SubtitleText->SetText(FText::FromString(Subtitle));
    SetVisibility(ESlateVisibility::HitTestInvisible); // never eats E, mouse or gamepad
}

void UUnmadeUnpaidShiftSubtitleWidget::ClearBeat()
{
    if(SpeakerText)SpeakerText->SetText(FText::GetEmpty());
    if(SubtitleText)SubtitleText->SetText(FText::GetEmpty());
    SetVisibility(ESlateVisibility::Collapsed);
}
