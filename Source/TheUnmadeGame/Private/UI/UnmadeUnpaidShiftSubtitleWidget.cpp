#include "UI/UnmadeUnpaidShiftSubtitleWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "UI/UnmadePlayerPreferences.h"

void UUnmadeUnpaidShiftSubtitleWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // Build at runtime: no missing Blueprint or font asset can make subtitles
    // disappear. Canvas anchors give equal horizontal safe margins at 16:9,
    // 4:3, widescreen, and small windows instead of cropping the scene art.
    if(!WidgetTree)return;
    UCanvasPanel* Root=WidgetTree->ConstructWidget<UCanvasPanel>();
    WidgetTree->RootWidget=Root;

    CaptionWidthConstraint=WidgetTree->ConstructWidget<USizeBox>();
    if(UCanvasPanelSlot* Slot=Root->AddChildToCanvas(CaptionWidthConstraint))
    {
        Slot->SetAnchors(FAnchors(0.5f,0.97f));
        Slot->SetOffsets(FMargin(0.f));
        Slot->SetAlignment(FVector2D(0.5f,1.f));
        Slot->SetAutoSize(true);
    }
    RefreshCaptionWidth();
    Backdrop=WidgetTree->ConstructWidget<UBorder>();
    Backdrop->SetBrushColor(FLinearColor(0.012f,0.016f,0.027f,0.87f));
    CaptionWidthConstraint->SetContent(Backdrop);
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

    FoleyText=WidgetTree->ConstructWidget<UTextBlock>();
    FoleyText->SetAutoWrapText(true);
    FoleyText->SetColorAndOpacity(FSlateColor(FLinearColor(0.78f,0.82f,0.88f,1.f)));
    FSlateFontInfo FoleyFont=FoleyText->GetFont();
    FoleyFont.Size=17;
    FoleyText->SetFont(FoleyFont);
    if(UVerticalBoxSlot* S=Stack->AddChildToVerticalBox(FoleyText))
        S->SetPadding(FMargin(0.f,4.f,0.f,0.f));
    RefreshAccessibilitySettings();
    SetVisibility(ESlateVisibility::Collapsed);
}

void UUnmadeUnpaidShiftSubtitleWidget::NativeTick(
    const FGeometry& MyGeometry,float InDeltaTime)
{
    Super::NativeTick(MyGeometry,InDeltaTime);
    RefreshCaptionWidth();
    RefreshAccessibilitySettings();
}

void UUnmadeUnpaidShiftSubtitleWidget::RefreshCaptionWidth()
{
    if(!CaptionWidthConstraint)return;
    int32 ViewportWidth=1600,ViewportHeight=900;
    if(APlayerController* PC=GetOwningPlayer())
        PC->GetViewportSize(ViewportWidth,ViewportHeight);
    const float DesiredWidth=FMath::Max(280.f,ViewportWidth*.86f);
    if(FMath::Abs(DesiredWidth-AppliedCaptionWidth)>1.f)
    {
        CaptionWidthConstraint->SetWidthOverride(DesiredWidth);
        AppliedCaptionWidth=DesiredWidth;
    }
}

void UUnmadeUnpaidShiftSubtitleWidget::RefreshAccessibilitySettings()
{
    const UUnmadePlayerPreferences* Preferences=
        GetDefault<UUnmadePlayerPreferences>();
    if(!Preferences)return;
    if(FMath::Abs(Preferences->TextScale-AppliedTextScale)>.001f)
    {
        const float Scale=FMath::Clamp(Preferences->TextScale,.85f,1.6f);
        if(SpeakerText)
        {
            FSlateFontInfo Font=SpeakerText->GetFont();
            Font.Size=FMath::RoundToInt(20.f*Scale);
            SpeakerText->SetFont(Font);
        }
        if(SubtitleText)
        {
            FSlateFontInfo Font=SubtitleText->GetFont();
            Font.Size=FMath::RoundToInt(25.f*Scale);
            SubtitleText->SetFont(Font);
        }
        if(FoleyText)
        {
            FSlateFontInfo Font=FoleyText->GetFont();
            Font.Size=FMath::RoundToInt(17.f*Scale);
            FoleyText->SetFont(Font);
        }
        AppliedTextScale=Scale;
    }
    if(Backdrop&&bAppliedHighContrast!=Preferences->HighContrast)
    {
        Backdrop->SetBrushColor(Preferences->HighContrast
            ?FLinearColor::Black:FLinearColor(0.012f,0.016f,0.027f,0.87f));
        bAppliedHighContrast=Preferences->HighContrast;
    }
}

void UUnmadeUnpaidShiftSubtitleWidget::DisplayBeat(
    const FString& Speaker,const FString& Subtitle,const FString& FoleyCaption)
{
    if(!SpeakerText||!SubtitleText||!FoleyText)return;
    SpeakerText->SetText(FText::FromString(Speaker.IsEmpty()
        ?TEXT("THE UNMADE"):Speaker));
    SubtitleText->SetText(FText::FromString(Subtitle));
    FoleyText->SetText(FText::FromString(FoleyCaption));
    SetVisibility(ESlateVisibility::HitTestInvisible); // never eats E, mouse or gamepad
}

void UUnmadeUnpaidShiftSubtitleWidget::ClearBeat()
{
    if(SpeakerText)SpeakerText->SetText(FText::GetEmpty());
    if(SubtitleText)SubtitleText->SetText(FText::GetEmpty());
    if(FoleyText)FoleyText->SetText(FText::GetEmpty());
    SetVisibility(ESlateVisibility::Collapsed);
}
