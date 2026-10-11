#include "UI/UnmadeBesQuestTrackerWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"

void UUnmadeBesQuestTrackerWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if(!WidgetTree)return;
    UCanvasPanel* Canvas=WidgetTree->ConstructWidget<UCanvasPanel>();
    WidgetTree->RootWidget=Canvas;
    Background=WidgetTree->ConstructWidget<UBorder>();
    Background->SetBrushColor(FLinearColor(.035f,.033f,.045f,.83f));
    Background->SetPadding(FMargin(13.f,9.f,13.f,9.f));
    if(UCanvasPanelSlot* Slot=Canvas->AddChildToCanvas(Background))
    {
        Slot->SetAnchors(FAnchors(.53f,.055f,.965f,.215f));
        Slot->SetOffsets(FMargin(0.f));
        Slot->SetAlignment(FVector2D(0,0));
        Slot->SetAutoSize(false);
    }
    UVerticalBox* Stack=WidgetTree->ConstructWidget<UVerticalBox>();
    Background->SetContent(Stack);
    ObjectiveText=WidgetTree->ConstructWidget<UTextBlock>();
    ObjectiveText->SetAutoWrapText(true);
    ObjectiveText->SetColorAndOpacity(FSlateColor(FLinearColor(1.f,.84f,.58f,1.f)));
    FSlateFontInfo GoalFont=ObjectiveText->GetFont();
    GoalFont.Size=18;
    ObjectiveText->SetFont(GoalFont);
    Stack->AddChildToVerticalBox(ObjectiveText);
    LedgerText=WidgetTree->ConstructWidget<UTextBlock>();
    LedgerText->SetAutoWrapText(true);
    LedgerText->SetColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)));
    FSlateFontInfo LedgerFont=LedgerText->GetFont();
    LedgerFont.Size=14;
    LedgerText->SetFont(LedgerFont);
    if(UVerticalBoxSlot* S=Stack->AddChildToVerticalBox(LedgerText))
        S->SetPadding(FMargin(0.f,5.f,0.f,0.f));
    SetVisibility(ESlateVisibility::Collapsed);
}

void UUnmadeBesQuestTrackerWidget::ShowObjective(
    const FString& Goal,const FString& Facts)
{
    if(!ObjectiveText||!LedgerText)return;
    ObjectiveText->SetText(FText::FromString(TEXT("THE UNPAID SHIFT  |  ")+Goal));
    LedgerText->SetText(FText::FromString(Facts));
    SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUnmadeBesQuestTrackerWidget::HideObjective()
{
    SetVisibility(ESlateVisibility::Collapsed);
}
