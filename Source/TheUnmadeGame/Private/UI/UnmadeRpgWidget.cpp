#include "Save/UnmadeSaveSession.h"
#include "Items/UnmadeFieldCraftRules.h"
#include "UI/UnmadePlayerActionRules.h"
#include "UI/UnmadeRpgWidget.h"
#include "UI/UnmadePlayerPreferences.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeAdventureLayout.h"
#include "World/UnmadeAdventureDepthRegistry.h"
#include "World/UnmadeTenfoldComponent.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Dialogue/UnmadeLocalDialogueSubsystem.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/WrapBox.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
namespace {
constexpr UnmadeCore::ItemId TradeGoods[]={UnmadeCore::ItemId::IronScrap,UnmadeCore::ItemId::WildHerbs,UnmadeCore::ItemId::BlankParchment,UnmadeCore::ItemId::HearthSalve,UnmadeCore::ItemId::StrainVial};
AUnmadePrototypeHub* Hub(UWorld* w){if(!w)return nullptr;for(TActorIterator<AUnmadePrototypeHub> h(w);h;++h)return *h;return nullptr;}
AUnmadeCharacter* Player(UUnmadeRpgWidget* w){return Cast<AUnmadeCharacter>(w->GetOwningPlayerPawn());}
}
TSharedRef<SWidget> UUnmadeRpgWidget::RebuildWidget(){SetIsFocusable(true);if(!WidgetTree)WidgetTree=NewObject<UWidgetTree>(this);
 RealmButtons.Reset();ActionGroups.Reset();BaseFonts.Reset();AppliedTextScale=GetDefault<UUnmadePlayerPreferences>()->TextScale;
 auto* root=WidgetTree->ConstructWidget<UCanvasPanel>();WidgetTree->RootWidget=root;
 auto Text=[&](const FString& value,int size){auto* t=WidgetTree->ConstructWidget<UTextBlock>();t->SetText(FText::FromString(value));t->SetAutoWrapText(true);t->SetColorAndOpacity(FSlateColor(FLinearColor(.96,.96,.93)));auto f=t->GetFont();BaseFonts.Add(TWeakObjectPtr<UTextBlock>(t),size);f.Size=FMath::RoundToInt(size*AppliedTextScale);t->SetFont(f);return t;};
 auto Frame=[&](FAnchors anchors){auto* b=WidgetTree->ConstructWidget<UBorder>();b->SetBrushColor(FLinearColor(.025,.04,.055,.96));b->SetPadding(FMargin(14));auto* slot=root->AddChildToCanvas(b);slot->SetAnchors(anchors);slot->SetOffsets(FMargin(0));return b;};
 auto* status=Frame(FAnchors(.02,.025,.36,.17));status->SetVisibility(ESlateVisibility::HitTestInvisible);auto* stack=WidgetTree->ConstructWidget<UVerticalBox>();status->SetContent(stack);Stats=Text(TEXT("Health / Stamina / Strain"),17);stack->AddChildToVerticalBox(Stats);Health=WidgetTree->ConstructWidget<UProgressBar>();Health->SetFillColorAndOpacity(FLinearColor(.85,.25,.2));stack->AddChildToVerticalBox(Health);Stamina=WidgetTree->ConstructWidget<UProgressBar>();Stamina->SetFillColorAndOpacity(FLinearColor(.25,.75,.55));stack->AddChildToVerticalBox(Stamina);
 auto* enemyFrame=Frame(FAnchors(.4,.025,.98,.17));enemyFrame->SetVisibility(ESlateVisibility::HitTestInvisible);EnemyText=Text(TEXT(""),18);enemyFrame->SetContent(EnemyText);
 auto* objectiveFrame=Frame(FAnchors(.02,.175,.98,.27));objectiveFrame->SetVisibility(ESlateVisibility::HitTestInvisible);Objective=Text(TEXT("Tracked objective"),17);objectiveFrame->SetContent(Objective);
 auto* promptFrame=Frame(FAnchors(.13,.91,.87,.985));promptFrame->SetVisibility(ESlateVisibility::HitTestInvisible);Prompt=Text(TEXT("E interact / J journal / I equipment / Tab map"),18);promptFrame->SetContent(Prompt);
 NoticePanel=Frame(FAnchors(.12,.64,.88,.89));NoticePanel->SetVisibility(ESlateVisibility::Collapsed);auto* noticeScroll=WidgetTree->ConstructWidget<UScrollBox>();NoticePanel->SetContent(noticeScroll);Notice=Text(TEXT(""),19);noticeScroll->AddChild(Notice);
 Panel=Frame(FAnchors(.07,.28,.93,.87));Panel->SetVisibility(ESlateVisibility::Collapsed);auto* panelStack=WidgetTree->ConstructWidget<UVerticalBox>();Panel->SetContent(panelStack);
 auto* tabs=WidgetTree->ConstructWidget<UWrapBox>();panelStack->AddChildToVerticalBox(tabs);
 auto Button=[&](UWrapBox* row,const FString& title,auto callback){auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(title,17));row->AddChildToWrapBox(button);FScriptDelegate click;click.BindUFunction(this,FName(callback));button->OnClicked.Add(click);return button;};
 InitialFocus=Button(tabs,TEXT(" Journal "),TEXT("Journal"));Button(tabs,TEXT(" Equipment "),TEXT("Equipment"));Button(tabs,TEXT(" Map "),TEXT("Map"));Button(tabs,TEXT(" Settings "),TEXT("Settings"));Button(tabs,TEXT(" Actions "),TEXT("Actions"));Button(tabs,TEXT(" Crafting "),TEXT("Crafting"));Button(tabs,TEXT(" Session "),TEXT("Session"));Button(tabs,TEXT(" Character "),TEXT("Character"));Button(tabs,TEXT(" Close "),TEXT("Dismiss"));
 auto* mapBox=WidgetTree->ConstructWidget<USizeBox>();MapContainer=mapBox;mapBox->SetHeightOverride(210);panelStack->AddChildToVerticalBox(mapBox);MapCanvas=WidgetTree->ConstructWidget<UCanvasPanel>();mapBox->SetContent(MapCanvas);
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[0].city),16));RealmButtons.Add(button);auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[0].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[0].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map0);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[1].city),16));RealmButtons.Add(button);auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[1].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[1].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map1);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[2].city),16));RealmButtons.Add(button);auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[2].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[2].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map2);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[3].city),16));RealmButtons.Add(button);auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[3].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[3].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map3);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[4].city),16));RealmButtons.Add(button);auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[4].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[4].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map4);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[5].city),16));RealmButtons.Add(button);auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[5].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[5].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map5);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[6].city),16));RealmButtons.Add(button);auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[6].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[6].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map6);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[7].city),16));RealmButtons.Add(button);auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[7].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[7].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map7);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[8].city),16));RealmButtons.Add(button);auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[8].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[8].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map8);}
 auto* scroll=WidgetTree->ConstructWidget<UScrollBox>();auto* scrollSlot=panelStack->AddChildToVerticalBox(scroll);scrollSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));Body=Text(TEXT(""),19);scroll->AddChild(Body);
 auto* actionSize=WidgetTree->ConstructWidget<USizeBox>();actionSize->SetMaxDesiredHeight(180);panelStack->AddChildToVerticalBox(actionSize);auto* actionScroll=WidgetTree->ConstructWidget<UScrollBox>();actionSize->SetContent(actionScroll);
 for(int group=0;group<8;++group){auto* actions=WidgetTree->ConstructWidget<UWrapBox>();ActionGroups.Add(actions);actions->SetInnerSlotPadding(FVector2D(6,6));actionScroll->AddChild(actions);auto Action=[&](const FString& title,const TCHAR* callback){auto* b=WidgetTree->ConstructWidget<UButton>();b->SetContent(Text(title,16));actions->AddChildToWrapBox(b);FScriptDelegate d;d.BindUFunction(this,FName(callback));b->OnClicked.Add(d);};
 if(group==0){Action(TEXT(" Journal category "),TEXT("JournalNext"));Action(TEXT(" Choice A "),TEXT("ChoiceA"));Action(TEXT(" Choice B "),TEXT("ChoiceB"));Action(TEXT(" Duelist "),TEXT("BuildDuelist"));Action(TEXT(" Warden "),TEXT("BuildWarden"));Action(TEXT(" Pathfinder "),TEXT("BuildPathfinder"));Action(TEXT(" Earn tool "),TEXT("EarnTool"));Action(TEXT(" Craft binding "),TEXT("CraftBinding"));Action(TEXT(" Privilege "),TEXT("EarnPrivilege"));Action(TEXT(" Repair "),TEXT("RepairTown"));Action(TEXT(" Mediate "),TEXT("Mediate"));Action(TEXT(" Specialty "),TEXT("Specialty"));Action(TEXT(" Rest six hours "),TEXT("Rest"));}
 if(group==1){Action(TEXT(" Weapon "),TEXT("Weapon"));Action(TEXT(" Armor "),TEXT("Armor"));Action(TEXT(" Charm "),TEXT("Charm"));Action(TEXT(" Salve "),TEXT("Heal"));Action(TEXT(" Select technique "),TEXT("SelectTechnique"));Action(TEXT(" Equip selected realm build "),TEXT("SelectBuild"));}
 if(group==2){Action(TEXT(" Track this realm "),TEXT("Track"));Action(TEXT(" Select technique "),TEXT("SelectTechnique"));Action(TEXT(" Equip selected realm build "),TEXT("SelectBuild"));}
 if(group==3){Action(TEXT(" Larger text "),TEXT("TextLarger"));Action(TEXT(" Smaller text "),TEXT("TextSmaller"));Action(TEXT(" Camera "),TEXT("CameraCycle"));Action(TEXT(" Sound "),TEXT("AudioCycle"));Action(TEXT(" Contrast "),TEXT("Contrast"));Action(TEXT(" Reduced flashes "),TEXT("Flashes"));Action(TEXT(" Prompts "),TEXT("PromptMode"));Action(TEXT(" Tutorials "),TEXT("Tutorials"));Action(TEXT(" Rebind attack "),TEXT("BindAttack"));Action(TEXT(" Rebind dodge "),TEXT("BindDodge"));Action(TEXT(" Rebind interact "),TEXT("BindInteract"));Action(TEXT(" Rebind guard "),TEXT("BindGuard"));Action(TEXT(" Rebind technique "),TEXT("BindTechnique"));}
 if(group==4){Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[0].title),TEXT("Action0"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[1].title),TEXT("Action1"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[2].title),TEXT("Action2"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[3].title),TEXT("Action3"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[4].title),TEXT("Action4"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[5].title),TEXT("Action5"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[6].title),TEXT("Action6"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[7].title),TEXT("Action7"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[8].title),TEXT("Action8"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[9].title),TEXT("Action9"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[10].title),TEXT("Action10"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[11].title),TEXT("Action11"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[12].title),TEXT("Action12"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[13].title),TEXT("Action13"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[14].title),TEXT("Action14"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[15].title),TEXT("Action15"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[16].title),TEXT("Action16"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[17].title),TEXT("Action17"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[18].title),TEXT("Action18"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[19].title),TEXT("Action19"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[20].title),TEXT("Action20"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[21].title),TEXT("Action21"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[22].title),TEXT("Action22"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[23].title),TEXT("Action23"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[24].title),TEXT("Action24"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[25].title),TEXT("Action25"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[26].title),TEXT("Action26"));Action(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[27].title),TEXT("Action27"));}
 if(group==5){Action(TEXT("Previous recipe"),TEXT("PreviousRecipe"));Action(TEXT("Next recipe"),TEXT("NextRecipe"));Action(TEXT("Craft selected recipe"),TEXT("CraftSelected"));Action(TEXT("Next trade item"),TEXT("NextGoods"));Action(TEXT("Buy selected item"),TEXT("BuyGoods"));Action(TEXT("Sell selected item"),TEXT("SellGoods"));}
 if(group==7){Action(TEXT("Next feature"),TEXT("NextAspect"));Action(TEXT("Previous option"),TEXT("PreviousFeature"));Action(TEXT("Next option"),TEXT("NextFeature"));Action(TEXT("Save chosen name"),TEXT("SaveName"));}
 if(group==6){Action(TEXT("Continue current game"),TEXT("ContinueGame"));Action(TEXT("Save now"),TEXT("SaveNow"));Action(TEXT("Next profile"),TEXT("NextProfile"));Action(TEXT("Load selected profile"),TEXT("LoadProfile"));Action(TEXT("Recover selected backup"),TEXT("LoadBackup"));Action(TEXT("New game in unused profile"),TEXT("NewProfile"));}
 }
 NameInput=WidgetTree->ConstructWidget<UEditableTextBox>();NameInput->SetVisibility(ESlateVisibility::Collapsed);NameInput->SetHintText(FText::FromString(TEXT("Chosen name")));panelStack->AddChildToVerticalBox(NameInput);
 return Super::RebuildWidget();
}
void UUnmadeRpgWidget::NativeConstruct(){Super::NativeConstruct();if(GetGameInstance())if(auto* dialogue=GetGameInstance()->GetSubsystem<UUnmadeLocalDialogueSubsystem>())dialogue->OnDialogueReady.AddUniqueDynamic(this,&UUnmadeRpgWidget::NpcLine);Refresh();}
void UUnmadeRpgWidget::NativeDestruct(){if(GetGameInstance())if(auto* dialogue=GetGameInstance()->GetSubsystem<UUnmadeLocalDialogueSubsystem>())dialogue->OnDialogueReady.RemoveDynamic(this,&UUnmadeRpgWidget::NpcLine);Close();Super::NativeDestruct();}
void UUnmadeRpgWidget::Open(int value){if(bOpen&&Page==value){Close();return;}if(auto* h=Hub(GetWorld()))if(h->IsBesDirectedScenePlaying())return;bool WasOpen=bOpen;SessionConfirm.Clear();ActionFeedback.Reset();if(value==7&&NameInput)if(auto* p=Player(this))NameInput->SetText(FText::FromString(p->GetCharacterChosenName()));if(!WasOpen)if(auto* p=Player(this)){p->GetCombat()->SetGuarding(false);p->GetCombat()->CancelWeaponStrike();}if(!WasOpen&&!UGameplayStatics::IsGamePaused(this))bPausedWorld=UGameplayStatics::SetGamePaused(this,true);Page=FMath::Clamp(value,0,7);bOpen=true;Panel->SetVisibility(ESlateVisibility::Visible);if(auto* pc=GetOwningPlayer()){pc->bShowMouseCursor=true;FInputModeUIOnly mode;mode.SetWidgetToFocus(TakeWidget());pc->SetInputMode(mode);if(!WasOpen){pc->SetIgnoreMoveInput(true);pc->SetIgnoreLookInput(true);}}if(InitialFocus)InitialFocus->SetKeyboardFocus();Refresh();}
void UUnmadeRpgWidget::Close(){if(!bOpen)return;SessionConfirm.Clear();ActionFeedback.Reset();bOpen=false;CaptureBinding=0;if(bPausedWorld){UGameplayStatics::SetGamePaused(this,false);bPausedWorld=false;}if(Panel)Panel->SetVisibility(ESlateVisibility::Collapsed);if(auto* pc=GetOwningPlayer()){pc->bShowMouseCursor=false;pc->SetIgnoreMoveInput(false);pc->SetIgnoreLookInput(false);pc->SetInputMode(FInputModeGameOnly());}}
bool UUnmadeRpgWidget::CaptureControl(FKey key){if(!CaptureBinding)return false;if(key==EKeys::Escape){CaptureBinding=0;Refresh();return true;}FString reason;if(GetMutableDefault<UUnmadePlayerPreferences>()->Bind(CaptureBinding,key,reason))CaptureBinding=0;if(auto* h=Hub(GetWorld()))h->NotifyRpg(reason);Refresh();return true;}
FReply UUnmadeRpgWidget::NativeOnMouseButtonDown(const FGeometry& g,const FPointerEvent& e){if(CaptureControl(e.GetEffectingButton()))return FReply::Handled();return Super::NativeOnMouseButtonDown(g,e);}
FReply UUnmadeRpgWidget::NativeOnKeyDown(const FGeometry& g,const FKeyEvent& e){auto k=e.GetKey();if(CaptureControl(k))return FReply::Handled();if(k==EKeys::Escape||k==EKeys::Gamepad_FaceButton_Right){Close();return FReply::Handled();}if(k==EKeys::J){Open(0);return FReply::Handled();}if(k==EKeys::I){Open(1);return FReply::Handled();}if(k==EKeys::Gamepad_LeftShoulder||k==EKeys::Gamepad_RightShoulder){Open((Page+(k==EKeys::Gamepad_RightShoulder?1:7))%8);return FReply::Handled();}if(k==EKeys::Six){ChoiceA();return FReply::Handled();}if(k==EKeys::Seven){ChoiceB();return FReply::Handled();}bool navigation=k==EKeys::Tab||k==EKeys::Up||k==EKeys::Down||k==EKeys::Left||k==EKeys::Right||k==EKeys::Enter||k==EKeys::SpaceBar||k==EKeys::Gamepad_DPad_Up||k==EKeys::Gamepad_DPad_Down||k==EKeys::Gamepad_DPad_Left||k==EKeys::Gamepad_DPad_Right||k==EKeys::Gamepad_FaceButton_Bottom;if(bOpen&&!navigation)return FReply::Handled();return Super::NativeOnKeyDown(g,e);}

void UUnmadeRpgWidget::NativeTick(const FGeometry& g,float dt){Super::NativeTick(g,dt);if(GetWorld()&&GetWorld()->GetTimeSeconds()>=NextRefresh){NextRefresh=GetWorld()->GetTimeSeconds()+.2;Refresh();}}
void UUnmadeRpgWidget::Refresh(){auto* p=Player(this);auto* h=Hub(GetWorld());if(!p||!h||!Body)return;auto* combat=p->GetCombat();if(combat){Health->SetPercent(combat->GetHealth()/FMath::Max(1.f,combat->GetMaxHealth()));Stamina->SetPercent(combat->GetStamina()/100);Stats->SetText(FText::FromString(FString::Printf(TEXT("Health %.0f / %.0f   Stamina %.0f   Strain %.0f\n%s"),combat->GetHealth(),combat->GetMaxHealth(),combat->GetStamina(),p->GetRealityStrain(),*combat->GetCombatFeedback())));}
 FString threat;double best=FMath::Square(1400.f);for(auto weak:h->CachedEnemies()){auto* e=weak.Get();if(!e)continue;double d=FVector::DistSquared2D(p->GetActorLocation(),e->GetActorLocation());if(e->CanBeTargeted()&&d<best){FVector toward=e->GetActorLocation()-p->GetActorLocation();if(FVector::DotProduct(p->GetActorForwardVector(),toward.GetSafeNormal2D())<0)continue;FCollisionQueryParams sight(SCENE_QUERY_STAT(HudThreatSight),false);sight.AddIgnoredActor(p);sight.AddIgnoredActor(e);if(GetWorld()->LineTraceTestByChannel(p->GetActorLocation()+FVector(0,0,55),e->GetActorLocation()+FVector(0,0,55),ECC_Visibility,sight))continue;best=d;threat=FString::Printf(TEXT("%s   %.0f / %.0f health\n%s"),*e->GetCombatName(),e->GetCombat()->GetHealth(),e->GetCombat()->GetMaxHealth(),*e->GetCombatWarning());}}EnemyText->SetText(FText::FromString(threat));
 auto* pref=GetDefault<UUnmadePlayerPreferences>();if(FMath::Abs(pref->TextScale-AppliedTextScale)>.001){for(const auto& entry:BaseFonts)if(auto* t=entry.Key.Get()){auto f=t->GetFont();f.Size=FMath::RoundToInt(entry.Value*pref->TextScale);t->SetFont(f);}AppliedTextScale=pref->TextScale;}WidgetTree->ForEachWidget([&](UWidget* w){if(auto* b=Cast<UBorder>(w))b->SetBrushColor(pref->HighContrast?FLinearColor::Black:FLinearColor(.025,.04,.055,.96));});
 Objective->SetVisibility(!bOpen&&!h->IsBesDirectedScenePlaying()?ESlateVisibility::HitTestInvisible:ESlateVisibility::Collapsed);Objective->SetText(FText::FromString(pref->FormatControls(h->GetTrackedObjective())));for(int i=0;i<RealmButtons.Num();++i)RealmButtons[i]->SetVisibility(h->IsRealmKnown(i)?ESlateVisibility::Visible:ESlateVisibility::Collapsed);for(int i=0;i<ActionGroups.Num();++i)ActionGroups[i]->SetVisibility(bOpen&&i==Page?ESlateVisibility::Visible:ESlateVisibility::Collapsed);
 FString contextual=h->GetDepthPrompt(p);if(contextual.StartsWith(TEXT("E ")))contextual=pref->InteractKey.GetDisplayName().ToString()+contextual.Mid(1);if(pref->ControllerPrompts&&contextual.StartsWith(pref->InteractKey.GetDisplayName().ToString()))contextual=pref->PadInteract.GetDisplayName().ToString()+contextual.Mid(pref->InteractKey.GetDisplayName().ToString().Len());Prompt->SetText(FText::FromString(pref->FormatControls(h->GetDepthPrompt(p))+TEXT(" / ")+pref->ControlPrompt()));MapContainer->SetVisibility(bOpen&&Page==2?ESlateVisibility::Visible:ESlateVisibility::Collapsed);
 if(h->RpgNoticeSerial!=LastNoticeSerial){LastNoticeSerial=h->RpgNoticeSerial;LastNotice=h->RpgNotice;NoticeUntil=GetWorld()->GetTimeSeconds()+18;Notice->SetText(FText::FromString(pref->FormatControls(LastNotice)));}if(GetWorld()->GetTimeSeconds()<h->RpgSceneUntil){Notice->SetText(FText::FromString(h->RpgSceneNotice));NoticeUntil=FMath::Max(NoticeUntil,h->RpgSceneUntil);}
 NoticePanel->SetVisibility(!bOpen&&!h->IsBesDirectedScenePlaying()&&GetWorld()->GetTimeSeconds()<NoticeUntil?ESlateVisibility::HitTestInvisible:ESlateVisibility::Collapsed);
 if(NameInput)NameInput->SetVisibility(bOpen&&Page==7?ESlateVisibility::Visible:ESlateVisibility::Collapsed);
 if(!bOpen)return;
 FString text;if(Page==0){const TCHAR* categories[]={TEXT("Objectives"),TEXT("People and return journeys"),TEXT("Evidence"),TEXT("Ten disciplines"),TEXT("Economy")};text=FString(categories[JournalCategory])+TEXT(" — use Journal category to switch\n\n");if(JournalCategory==0)text+=h->GetExperienceJournal()+h->DescribeUnansweredRoad(p)+h->GetExpeditionJournal()+h->GetDepthJournal();if(JournalCategory==1){text+=h->GetRoadJournal()+h->GetCinderholdJournal()+h->GetBesHouseholdJournal()+TEXT("\nRECENT CONVERSATIONS\n");for(const auto& line:h->RpgHistory)text+=line+TEXT("\n\n");}if(JournalCategory==2)text+=h->GetWitnessEchoJournal()+h->GetEvidenceNotebook();if(JournalCategory==3){if(auto* rites=p->FindComponentByClass<UUnmadeTenfoldComponent>())for(const auto& spec:UnmadeCore::RiteSpecs){int stage=rites->RiteStage(spec.id);text+=FString::Printf(TEXT("%s: chapter %d/5 — %s\n"),UTF8_TO_TCHAR(spec.name),FMath::Min(stage+1,5),stage>=5?TEXT("mastered"):UTF8_TO_TCHAR(spec.chapters[stage]));}text+=h->GetRealityConsequenceJournal();}if(JournalCategory==4)text+=h->GetLocalMaterialJournal()+h->GetLivingEconomyJournal();}
 if(Page==1){auto* equipment=p->GetEquipment();auto style=h->PlayerWeaponStyle(equipment?equipment->EquippedWeapon():UnmadeCore::ItemId::Count);text=(equipment?equipment->DescribeInventory():TEXT("Equipment unavailable"))+FString::Printf(TEXT("\nACTIVE FIGHTING STYLE\n%s: reach %.0f cm, rhythm %.2f seconds, stamina %.0f per strike.\nThree-hit finishing combo applies a status and stagger. Shift dodge; right mouse guard; time guard entry to parry.\nMastery %d/3 improves stamina recovery, parry and dodge. F11 uses the selected earned technique.\n"),UTF8_TO_TCHAR(style.name),style.reach,style.cooldown,style.cost,h->GetActiveMastery())+h->GetDepthJournal();}
 if(Page==2)text=h->GetExperienceMap(SelectedRealm,p);
 if(Page==3)text=pref->Describe()+(CaptureBinding?TEXT("\nWAITING FOR A CONTROL — Escape cancels."):TEXT(""));
 if(Page==4){text=TEXT("ACTIONS — available with mouse, keyboard or controller. Each button invokes the existing action beside its real target; it does not bypass earned powers, consent, evidence, time or save checks.\n\n");if(auto* rite=p->FindComponentByClass<UUnmadeTenfoldComponent>())text+=FString::Printf(TEXT("Selected discipline: %s\n"),UTF8_TO_TCHAR(UnmadeCore::RiteSpecs[rite->CurrentSelectedRite()].name));for(const auto& a:UnmadeCore::PlayerActions)text+=FString(UTF8_TO_TCHAR(a.title))+TEXT(": ")+UTF8_TO_TCHAR(a.requirement)+TEXT("\n");}
 if(Page==5){if(p->GetEquipment())text=p->GetEquipment()->DescribeRecipe(SelectedRecipe);const auto goods=TradeGoods[SelectedGoods];const auto village=UnmadeCore::SettlementAt(p->GetActorLocation().X,p->GetActorLocation().Y);UnmadeCore::RegionalEconomy pricing;text+=FString::Printf(TEXT("\nSelected trade item: %s\nBase buy price %d marks; local trust can discount. Sell price %d marks. Trading requires the real consenting merchant.\nLegacy crafting requires its workshop; a field recipe requires its earned realm workbench during open hours."),UTF8_TO_TCHAR(UnmadeCore::FindItem(goods)->name),pricing.Price(goods,village),pricing.Price(goods,village)/3);}
 if(Page==6)text=UnmadeSave::DescribeProfiles()+FString::Printf(TEXT("\nSelected profile: %d. Loading preserves the current record; New Game refuses to overwrite an occupied profile.\n"),SelectedProfile);
 if(!LastNotice.IsEmpty())text+=TEXT("\n\nLATEST ACTION\n")+LastNotice;
 if(Page==7){const TCHAR* aspects[]={TEXT("Body frame"),TEXT("Face"),TEXT("Hair"),TEXT("Voice"),TEXT("Skin palette"),TEXT("Reality mark"),TEXT("Gait"),TEXT("Calling")};text=FString::Printf(TEXT("CHARACTER\nChosen name: %s\nSelected feature: %s, option %d of %d\nEach successful change is saved; failed writes retain the previous identity. Origin remains The Impossible. Frame, gait and palette use the existing prototype silhouette. Face, hair, voice and marks are saved selections pending final assets and engine acceptance."),*p->GetCharacterChosenName(),aspects[SelectedAspect],p->GetCharacterFeature(SelectedAspect)+1,UnmadeCore::CharacterIdentity::ChoiceCounts[SelectedAspect]);}
 if((Page==6||Page==7)&&!ActionFeedback.IsEmpty())text+=TEXT("\n\n")+ActionFeedback;
 const FString shown=Page==7?text:pref->FormatControls(text);if(Body->GetText().ToString()!=shown)Body->SetText(FText::FromString(shown));
}
void UUnmadeRpgWidget::NpcLine(FName Id,const FString& Line,bool UsedModel){if(auto* h=Hub(GetWorld()))h->NotifyRpg(Line);}
void UUnmadeRpgWidget::Journal(){Open(0);}void UUnmadeRpgWidget::Equipment(){Open(1);}void UUnmadeRpgWidget::Map(){Open(2);}void UUnmadeRpgWidget::Dismiss(){Close();}
void UUnmadeRpgWidget::Weapon(){if(auto* p=Player(this))p->RpgMenuAction(FName(TEXT("Weapon")));Refresh();}
void UUnmadeRpgWidget::Armor(){if(auto* p=Player(this))p->RpgMenuAction(FName(TEXT("Armor")));Refresh();}
void UUnmadeRpgWidget::Charm(){if(auto* p=Player(this))p->RpgMenuAction(FName(TEXT("Charm")));Refresh();}
void UUnmadeRpgWidget::Heal(){if(auto* p=Player(this))p->RpgMenuAction(FName(TEXT("Heal")));Refresh();}
void UUnmadeRpgWidget::ChoiceA(){if(auto* p=Player(this))p->RpgMenuAction(FName(TEXT("ChoiceA")));Refresh();}
void UUnmadeRpgWidget::ChoiceB(){if(auto* p=Player(this))p->RpgMenuAction(FName(TEXT("ChoiceB")));Refresh();}
void UUnmadeRpgWidget::SelectTechnique(){if(auto* h=Hub(GetWorld()))h->SelectDepthTechnique(SelectedRealm,Player(this));Refresh();}
void UUnmadeRpgWidget::SelectRealm(int i){if(auto* h=Hub(GetWorld()))if(!h->IsRealmKnown(i))return;SelectedRealm=FMath::Clamp(i,0,8);Refresh();}
void UUnmadeRpgWidget::Map0(){SelectRealm(0);}
void UUnmadeRpgWidget::Map1(){SelectRealm(1);}
void UUnmadeRpgWidget::Map2(){SelectRealm(2);}
void UUnmadeRpgWidget::Map3(){SelectRealm(3);}
void UUnmadeRpgWidget::Map4(){SelectRealm(4);}
void UUnmadeRpgWidget::Map5(){SelectRealm(5);}
void UUnmadeRpgWidget::Map6(){SelectRealm(6);}
void UUnmadeRpgWidget::Map7(){SelectRealm(7);}
void UUnmadeRpgWidget::Map8(){SelectRealm(8);}

void UUnmadeRpgWidget::Settings(){Open(3);}void UUnmadeRpgWidget::Track(){if(auto* h=Hub(GetWorld()))h->TrackExperienceRealm(SelectedRealm,Player(this));Refresh();}
void UUnmadeRpgWidget::JournalNext(){JournalCategory=(JournalCategory+1)%5;Refresh();}
void UUnmadeRpgWidget::Upgrade(int option){if(auto* h=Hub(GetWorld()))h->ExperienceAction(option,Player(this));Refresh();}
void UUnmadeRpgWidget::BuildDuelist(){Upgrade(1);}
void UUnmadeRpgWidget::BuildWarden(){Upgrade(2);}
void UUnmadeRpgWidget::BuildPathfinder(){Upgrade(3);}
void UUnmadeRpgWidget::EarnTool(){Upgrade(4);}
void UUnmadeRpgWidget::CraftBinding(){Upgrade(5);}
void UUnmadeRpgWidget::EarnPrivilege(){Upgrade(6);}
void UUnmadeRpgWidget::RepairTown(){Upgrade(7);}
void UUnmadeRpgWidget::Mediate(){Upgrade(8);}
void UUnmadeRpgWidget::Specialty(){Upgrade(9);}
void UUnmadeRpgWidget::Rest(){Upgrade(11);}
void UUnmadeRpgWidget::TextLarger(){auto* p=GetMutableDefault<UUnmadePlayerPreferences>();p->TextScale=FMath::Min(1.6f,p->TextScale+.1f);p->Validate();p->SaveConfig();Refresh();}
void UUnmadeRpgWidget::TextSmaller(){auto* p=GetMutableDefault<UUnmadePlayerPreferences>();p->TextScale=FMath::Max(.85f,p->TextScale-.1f);p->Validate();p->SaveConfig();Refresh();}
void UUnmadeRpgWidget::CameraCycle(){auto* p=GetMutableDefault<UUnmadePlayerPreferences>();p->CameraSensitivity=p->CameraSensitivity>=2?.4f:p->CameraSensitivity+.2f;p->Validate();p->SaveConfig();Refresh();}
void UUnmadeRpgWidget::AudioCycle(){auto* p=GetMutableDefault<UUnmadePlayerPreferences>();p->AudioVolume=p->AudioVolume>=.99?0:p->AudioVolume+.2f;p->Validate();p->SaveConfig();Refresh();}
void UUnmadeRpgWidget::Contrast(){auto* p=GetMutableDefault<UUnmadePlayerPreferences>();p->HighContrast=!p->HighContrast;p->Validate();p->SaveConfig();Refresh();}
void UUnmadeRpgWidget::Flashes(){auto* p=GetMutableDefault<UUnmadePlayerPreferences>();p->ReducedFlashes=!p->ReducedFlashes;p->Validate();p->SaveConfig();Refresh();}
void UUnmadeRpgWidget::PromptMode(){auto* p=GetMutableDefault<UUnmadePlayerPreferences>();p->ControllerPrompts=!p->ControllerPrompts;p->Validate();p->SaveConfig();Refresh();}
void UUnmadeRpgWidget::Tutorials(){auto* p=GetMutableDefault<UUnmadePlayerPreferences>();p->ShowTutorials=!p->ShowTutorials;p->Validate();p->SaveConfig();Refresh();}
void UUnmadeRpgWidget::BindAttack(){CaptureBinding=1;SetKeyboardFocus();Refresh();}
void UUnmadeRpgWidget::BindDodge(){CaptureBinding=2;SetKeyboardFocus();Refresh();}
void UUnmadeRpgWidget::BindInteract(){CaptureBinding=3;SetKeyboardFocus();Refresh();}

void UUnmadeRpgWidget::BindGuard(){CaptureBinding=4;SetKeyboardFocus();Refresh();}
void UUnmadeRpgWidget::BindTechnique(){CaptureBinding=5;SetKeyboardFocus();Refresh();}

void UUnmadeRpgWidget::SelectBuild(){if(auto* h=Hub(GetWorld()))h->SelectExperienceBuild(SelectedRealm,Player(this));Refresh();}

void UUnmadeRpgWidget::Actions(){Open(4);}void UUnmadeRpgWidget::Crafting(){Open(5);}void UUnmadeRpgWidget::Session(){Open(6);}
void UUnmadeRpgWidget::PerformAction(int n){if(n<0||n>=28)return;auto* p=Player(this);if(!p)return;Close();p->RpgMenuAction(FName(UTF8_TO_TCHAR(UnmadeCore::PlayerActions[n].action)));}
void UUnmadeRpgWidget::CycleRecipe(int direction){auto* h=Hub(GetWorld());for(int n=0;n<15;++n){SelectedRecipe=(SelectedRecipe+direction+15)%15;if(SelectedRecipe<6||(h&&h->KnowsFieldRecipe(SelectedRecipe-6)))break;}Refresh();}
void UUnmadeRpgWidget::PreviousRecipe(){CycleRecipe(-1);}void UUnmadeRpgWidget::NextRecipe(){CycleRecipe(1);}
void UUnmadeRpgWidget::CraftSelected(){auto* p=Player(this);if(!p)return;Close();p->CraftRecipeSelection(SelectedRecipe);}
void UUnmadeRpgWidget::NextGoods(){SelectedGoods=(SelectedGoods+1)%5;Refresh();}
void UUnmadeRpgWidget::BuyGoods(){if(auto* p=Player(this)){Close();p->TradeSelection(TradeGoods[SelectedGoods],true);}}
void UUnmadeRpgWidget::SellGoods(){if(auto* p=Player(this)){Close();p->TradeSelection(TradeGoods[SelectedGoods],false);}}
void UUnmadeRpgWidget::NextProfile(){SelectedProfile=(SelectedProfile+1)%4;SessionConfirm.Clear();ActionFeedback.Reset();Refresh();}
void UUnmadeRpgWidget::ContinueGame(){Close();}
void UUnmadeRpgWidget::SaveNow(){SessionConfirm.Clear();auto* p=Player(this);bool saved=p&&p->SaveSessionNow();ActionFeedback=saved?TEXT("Game saved with a safe resume checkpoint and current health/stamina."):TEXT("Save failed. Recent progress may be missing; inspect Save Profiles for recovery.");Refresh();}
void UUnmadeRpgWidget::LoadProfile(){auto* p=Player(this);if(!p)return;if(!SessionConfirm.Request(UnmadeCore::SessionIntent::Load,SelectedProfile,p->SaveSessionNow())){ActionFeedback=TEXT("Current state could not save. Press Load again for this profile to leave unsaved changes; existing records remain intact.");Refresh();return;}Close();if(!UnmadeSave::SelectProfile(this,SelectedProfile))if(auto* h=Hub(GetWorld()))h->NotifyRpg(TEXT("Selected profile is empty or unreadable. Its record was not replaced."));}
void UUnmadeRpgWidget::LoadBackup(){if(!SessionConfirm.Request(UnmadeCore::SessionIntent::Recover,SelectedProfile,false)){ActionFeedback=TEXT("Recovery leaves unsaved session changes and loads the selected prior backup. Press Recover again for this profile to confirm. The backup will not be overwritten by saving this session.");Refresh();return;}Close();if(!UnmadeSave::SelectProfile(this,SelectedProfile,true))if(auto* h=Hub(GetWorld()))h->NotifyRpg(TEXT("Selected backup cannot load. Check its availability and whether the primary record is incompatible."));}
void UUnmadeRpgWidget::NewProfile(){auto* p=Player(this);if(!p)return;if(!SessionConfirm.Request(UnmadeCore::SessionIntent::New,SelectedProfile,p->SaveSessionNow())){ActionFeedback=TEXT("Current state could not save. Press New Game again to leave unsaved changes and create an unused profile. Existing records remain intact.");Refresh();return;}Close();if(!UnmadeSave::CreateUnusedProfile(this))if(auto* h=Hub(GetWorld()))h->NotifyRpg(TEXT("No unused profile is available, or saving failed. Existing records remain intact."));}
void UUnmadeRpgWidget::Character(){Open(7);}
void UUnmadeRpgWidget::NextAspect(){SelectedAspect=(SelectedAspect+1)%8;Refresh();}
void UUnmadeRpgWidget::ChangeFeature(int direction){if(auto* p=Player(this)){int count=UnmadeCore::CharacterIdentity::ChoiceCounts[SelectedAspect];int next=(p->GetCharacterFeature(SelectedAspect)+count+direction)%count;bool saved=p->SetCharacterFeature(SelectedAspect,next);ActionFeedback=saved?TEXT("Chosen feature saved."):TEXT("Feature could not save; previous identity retained.");Refresh();}}
void UUnmadeRpgWidget::PreviousFeature(){ChangeFeature(-1);}
void UUnmadeRpgWidget::NextFeature(){ChangeFeature(1);}
void UUnmadeRpgWidget::SaveName(){auto* p=Player(this);if(!p||!NameInput)return;bool saved=p->SetCharacterChosenName(NameInput->GetText().ToString().TrimStartAndEnd());ActionFeedback=saved?TEXT("Chosen name saved."):TEXT("Name could not save. Use a meaningful name within 48 UTF-8 bytes; previous identity retained.");Refresh();}

void UUnmadeRpgWidget::Action0(){PerformAction(0);}
void UUnmadeRpgWidget::Action1(){PerformAction(1);}
void UUnmadeRpgWidget::Action2(){PerformAction(2);}
void UUnmadeRpgWidget::Action3(){PerformAction(3);}
void UUnmadeRpgWidget::Action4(){PerformAction(4);}
void UUnmadeRpgWidget::Action5(){PerformAction(5);}
void UUnmadeRpgWidget::Action6(){PerformAction(6);}
void UUnmadeRpgWidget::Action7(){PerformAction(7);}
void UUnmadeRpgWidget::Action8(){PerformAction(8);}
void UUnmadeRpgWidget::Action9(){PerformAction(9);}
void UUnmadeRpgWidget::Action10(){PerformAction(10);}
void UUnmadeRpgWidget::Action11(){PerformAction(11);}
void UUnmadeRpgWidget::Action12(){PerformAction(12);}
void UUnmadeRpgWidget::Action13(){PerformAction(13);}
void UUnmadeRpgWidget::Action14(){PerformAction(14);}
void UUnmadeRpgWidget::Action15(){PerformAction(15);}
void UUnmadeRpgWidget::Action16(){PerformAction(16);}
void UUnmadeRpgWidget::Action17(){PerformAction(17);}
void UUnmadeRpgWidget::Action18(){PerformAction(18);}
void UUnmadeRpgWidget::Action19(){PerformAction(19);}
void UUnmadeRpgWidget::Action20(){PerformAction(20);}
void UUnmadeRpgWidget::Action21(){PerformAction(21);}
void UUnmadeRpgWidget::Action22(){PerformAction(22);}
void UUnmadeRpgWidget::Action23(){PerformAction(23);}
void UUnmadeRpgWidget::Action24(){PerformAction(24);}
void UUnmadeRpgWidget::Action25(){PerformAction(25);}
void UUnmadeRpgWidget::Action26(){PerformAction(26);}
void UUnmadeRpgWidget::Action27(){PerformAction(27);}
