#include "UI/UnmadeRpgWidget.h"
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
AUnmadePrototypeHub* Hub(UWorld* w){if(!w)return nullptr;for(TActorIterator<AUnmadePrototypeHub> h(w);h;++h)return *h;return nullptr;}
AUnmadeCharacter* Player(UUnmadeRpgWidget* w){return Cast<AUnmadeCharacter>(w->GetOwningPlayerPawn());}
}
TSharedRef<SWidget> UUnmadeRpgWidget::RebuildWidget(){SetIsFocusable(true);if(!WidgetTree)WidgetTree=NewObject<UWidgetTree>(this);
 auto* root=WidgetTree->ConstructWidget<UCanvasPanel>();WidgetTree->RootWidget=root;
 auto Text=[&](const FString& value,int size){auto* t=WidgetTree->ConstructWidget<UTextBlock>();t->SetText(FText::FromString(value));t->SetAutoWrapText(true);t->SetColorAndOpacity(FSlateColor(FLinearColor(.96,.96,.93)));auto f=t->GetFont();f.Size=size;t->SetFont(f);return t;};
 auto Frame=[&](FAnchors anchors){auto* b=WidgetTree->ConstructWidget<UBorder>();b->SetBrushColor(FLinearColor(.025,.04,.055,.96));b->SetPadding(FMargin(14));auto* slot=root->AddChildToCanvas(b);slot->SetAnchors(anchors);slot->SetOffsets(FMargin(0));return b;};
 auto* status=Frame(FAnchors(.02,.025,.36,.17));status->SetVisibility(ESlateVisibility::HitTestInvisible);auto* stack=WidgetTree->ConstructWidget<UVerticalBox>();status->SetContent(stack);Stats=Text(TEXT("Health / Stamina / Strain"),17);stack->AddChildToVerticalBox(Stats);Health=WidgetTree->ConstructWidget<UProgressBar>();Health->SetFillColorAndOpacity(FLinearColor(.85,.25,.2));stack->AddChildToVerticalBox(Health);Stamina=WidgetTree->ConstructWidget<UProgressBar>();Stamina->SetFillColorAndOpacity(FLinearColor(.25,.75,.55));stack->AddChildToVerticalBox(Stamina);
 auto* enemyFrame=Frame(FAnchors(.4,.025,.98,.17));enemyFrame->SetVisibility(ESlateVisibility::HitTestInvisible);EnemyText=Text(TEXT(""),18);enemyFrame->SetContent(EnemyText);
 auto* promptFrame=Frame(FAnchors(.13,.91,.87,.985));promptFrame->SetVisibility(ESlateVisibility::HitTestInvisible);Prompt=Text(TEXT("E interact / J journal / I equipment / Tab map"),18);promptFrame->SetContent(Prompt);
 NoticePanel=Frame(FAnchors(.12,.64,.88,.89));NoticePanel->SetVisibility(ESlateVisibility::Collapsed);auto* noticeScroll=WidgetTree->ConstructWidget<UScrollBox>();NoticePanel->SetContent(noticeScroll);Notice=Text(TEXT(""),19);noticeScroll->AddChild(Notice);
 Panel=Frame(FAnchors(.07,.19,.93,.87));Panel->SetVisibility(ESlateVisibility::Collapsed);auto* panelStack=WidgetTree->ConstructWidget<UVerticalBox>();Panel->SetContent(panelStack);
 auto* tabs=WidgetTree->ConstructWidget<UHorizontalBox>();panelStack->AddChildToVerticalBox(tabs);
 auto Button=[&](UHorizontalBox* row,const FString& title,auto callback){auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(title,17));row->AddChildToHorizontalBox(button);FScriptDelegate click;click.BindUFunction(this,FName(callback));button->OnClicked.Add(click);return button;};
 InitialFocus=Button(tabs,TEXT(" Journal [J] "),TEXT("Journal"));Button(tabs,TEXT(" Equipment [I] "),TEXT("Equipment"));Button(tabs,TEXT(" Map [Tab] "),TEXT("Map"));Button(tabs,TEXT(" Close [Esc] "),TEXT("Dismiss"));
 auto* mapBox=WidgetTree->ConstructWidget<USizeBox>();MapContainer=mapBox;mapBox->SetHeightOverride(210);panelStack->AddChildToVerticalBox(mapBox);MapCanvas=WidgetTree->ConstructWidget<UCanvasPanel>();mapBox->SetContent(MapCanvas);
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[0].city),16));auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[0].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[0].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map0);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[1].city),16));auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[1].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[1].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map1);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[2].city),16));auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[2].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[2].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map2);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[3].city),16));auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[3].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[3].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map3);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[4].city),16));auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[4].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[4].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map4);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[5].city),16));auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[5].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[5].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map5);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[6].city),16));auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[6].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[6].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map6);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[7].city),16));auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[7].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[7].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map7);}
 {auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[8].city),16));auto* slot=MapCanvas->AddChildToCanvas(button);double x=.08+.84*(UnmadeCore::ExpeditionSpecs[8].x+200000)/400000.;double y=.18+.64*(UnmadeCore::ExpeditionSpecs[8].y+50000)/100000.;slot->SetAnchors(FAnchors(x,y));slot->SetAlignment(FVector2D(.5,.5));slot->SetOffsets(FMargin(0,0,108,45));button->OnClicked.AddDynamic(this,&UUnmadeRpgWidget::Map8);}
 auto* scroll=WidgetTree->ConstructWidget<UScrollBox>();auto* scrollSlot=panelStack->AddChildToVerticalBox(scroll);scrollSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));Body=Text(TEXT(""),19);scroll->AddChild(Body);
 auto* actions=WidgetTree->ConstructWidget<UWrapBox>();actions->SetInnerSlotPadding(FVector2D(6,6));panelStack->AddChildToVerticalBox(actions);auto Action=[&](const FString& title,const TCHAR* callback){auto* b=WidgetTree->ConstructWidget<UButton>();b->SetContent(Text(title,17));actions->AddChildToWrapBox(b);FScriptDelegate d;d.BindUFunction(this,FName(callback));b->OnClicked.Add(d);};Action(TEXT(" Weapon "),TEXT("Weapon"));Action(TEXT(" Armor "),TEXT("Armor"));Action(TEXT(" Charm "),TEXT("Charm"));Action(TEXT(" Salve "),TEXT("Heal"));Action(TEXT(" Choice A [6] "),TEXT("ChoiceA"));Action(TEXT(" Choice B [7] "),TEXT("ChoiceB"));Action(TEXT(" Select technique "),TEXT("SelectTechnique"));
 return Super::RebuildWidget();
}
void UUnmadeRpgWidget::NativeConstruct(){Super::NativeConstruct();if(GetGameInstance())if(auto* dialogue=GetGameInstance()->GetSubsystem<UUnmadeLocalDialogueSubsystem>())dialogue->OnDialogueReady.AddUniqueDynamic(this,&UUnmadeRpgWidget::NpcLine);Refresh();}
void UUnmadeRpgWidget::NativeDestruct(){if(GetGameInstance())if(auto* dialogue=GetGameInstance()->GetSubsystem<UUnmadeLocalDialogueSubsystem>())dialogue->OnDialogueReady.RemoveDynamic(this,&UUnmadeRpgWidget::NpcLine);Close();Super::NativeDestruct();}
void UUnmadeRpgWidget::Open(int value){if(bOpen&&Page==value){Close();return;}if(auto* h=Hub(GetWorld()))if(h->IsBesDirectedScenePlaying())return;bool WasOpen=bOpen;if(!WasOpen)if(auto* p=Player(this))p->GetCombat()->SetGuarding(false);if(!WasOpen&&!UGameplayStatics::IsGamePaused(this))bPausedWorld=UGameplayStatics::SetGamePaused(this,true);Page=FMath::Clamp(value,0,2);bOpen=true;Panel->SetVisibility(ESlateVisibility::Visible);if(auto* pc=GetOwningPlayer()){pc->bShowMouseCursor=true;FInputModeGameAndUI mode;mode.SetWidgetToFocus(TakeWidget());mode.SetHideCursorDuringCapture(false);pc->SetInputMode(mode);if(!WasOpen){pc->SetIgnoreMoveInput(true);pc->SetIgnoreLookInput(true);}}if(InitialFocus)InitialFocus->SetKeyboardFocus();Refresh();}
void UUnmadeRpgWidget::Close(){if(!bOpen)return;bOpen=false;if(bPausedWorld){UGameplayStatics::SetGamePaused(this,false);bPausedWorld=false;}if(Panel)Panel->SetVisibility(ESlateVisibility::Collapsed);if(auto* pc=GetOwningPlayer()){pc->bShowMouseCursor=false;pc->SetIgnoreMoveInput(false);pc->SetIgnoreLookInput(false);pc->SetInputMode(FInputModeGameOnly());}}
FReply UUnmadeRpgWidget::NativeOnKeyDown(const FGeometry& g,const FKeyEvent& e){auto k=e.GetKey();if(k==EKeys::Escape){Close();return FReply::Handled();}if(k==EKeys::J){Open(0);return FReply::Handled();}if(k==EKeys::I){Open(1);return FReply::Handled();}if(k==EKeys::Tab)return Super::NativeOnKeyDown(g,e);if(k==EKeys::Six){ChoiceA();return FReply::Handled();}if(k==EKeys::Seven){ChoiceB();return FReply::Handled();}if(bOpen&&k!=EKeys::Up&&k!=EKeys::Down&&k!=EKeys::Left&&k!=EKeys::Right&&k!=EKeys::Enter&&k!=EKeys::SpaceBar)return FReply::Handled();return Super::NativeOnKeyDown(g,e);}
void UUnmadeRpgWidget::NativeTick(const FGeometry& g,float dt){Super::NativeTick(g,dt);if(GetWorld()&&GetWorld()->GetTimeSeconds()>=NextRefresh){NextRefresh=GetWorld()->GetTimeSeconds()+.2;Refresh();}}
void UUnmadeRpgWidget::Refresh(){auto* p=Player(this);auto* h=Hub(GetWorld());if(!p||!h||!Body)return;auto* combat=p->GetCombat();if(combat){Health->SetPercent(combat->GetHealth()/FMath::Max(1.f,combat->GetMaxHealth()));Stamina->SetPercent(combat->GetStamina()/100);Stats->SetText(FText::FromString(FString::Printf(TEXT("Health %.0f / %.0f   Stamina %.0f   Strain %.0f\n%s"),combat->GetHealth(),combat->GetMaxHealth(),combat->GetStamina(),p->GetRealityStrain(),*combat->GetCombatFeedback())));}
 FString threat;double best=FMath::Square(1400.f);for(TActorIterator<AUnmadeEnemyCharacter> e(GetWorld());e;++e){double d=FVector::DistSquared2D(p->GetActorLocation(),e->GetActorLocation());if(e->CanBeTargeted()&&d<best){FVector toward=e->GetActorLocation()-p->GetActorLocation();if(FVector::DotProduct(p->GetActorForwardVector(),toward.GetSafeNormal2D())<0)continue;FCollisionQueryParams sight(SCENE_QUERY_STAT(HudThreatSight),false);sight.AddIgnoredActor(p);sight.AddIgnoredActor(*e);if(GetWorld()->LineTraceTestByChannel(p->GetActorLocation()+FVector(0,0,55),e->GetActorLocation()+FVector(0,0,55),ECC_Visibility,sight))continue;best=d;threat=FString::Printf(TEXT("%s   %.0f / %.0f health\n%s"),*e->GetCombatName(),e->GetCombat()->GetHealth(),e->GetCombat()->GetMaxHealth(),*e->GetCombatWarning());}}EnemyText->SetText(FText::FromString(threat));
 Prompt->SetText(FText::FromString(h->GetDepthPrompt(p)));MapContainer->SetVisibility(bOpen&&Page==2?ESlateVisibility::Visible:ESlateVisibility::Collapsed);
 if(h->RpgNoticeSerial!=LastNoticeSerial){LastNoticeSerial=h->RpgNoticeSerial;LastNotice=h->RpgNotice;NoticeUntil=GetWorld()->GetTimeSeconds()+18;Notice->SetText(FText::FromString(LastNotice));}NoticePanel->SetVisibility(!bOpen&&!h->IsBesDirectedScenePlaying()&&GetWorld()->GetTimeSeconds()<NoticeUntil?ESlateVisibility::HitTestInvisible:ESlateVisibility::Collapsed);
 if(!bOpen)return;
 FString text;if(Page==0)text=h->DescribeUnansweredRoad(p)+TEXT("\n\n")+h->GetDepthJournal()+h->GetExpeditionJournal()+h->GetRoadJournal()+h->GetCinderholdJournal()+h->GetBesHouseholdJournal()+h->GetWitnessEchoJournal()+h->GetEvidenceNotebook()+h->GetRealityConsequenceJournal()+h->GetLocalMaterialJournal()+h->GetLivingEconomyJournal()+TEXT("\nRECENT CONVERSATION\n")+LastNotice;
 if(Page==0){if(auto* rites=p->FindComponentByClass<UUnmadeTenfoldComponent>()){text+=TEXT("\nTEN SIGNATURE DISCIPLINES\n");for(const auto& spec:UnmadeCore::RiteSpecs){int stage=rites->RiteStage(spec.id);text+=FString::Printf(TEXT("%s: chapter %d/5 — %s\n"),UTF8_TO_TCHAR(spec.name),FMath::Min(stage+1,5),stage>=5?TEXT("mastered"):UTF8_TO_TCHAR(spec.chapters[stage]));}}text+=TEXT("\nRECENT WORLD CONVERSATIONS AND EVENTS\n");for(const auto& line:h->RpgHistory)text+=line+TEXT("\n\n");}
 if(Page==1){auto* equipment=p->GetEquipment();auto style=h->PlayerWeaponStyle(equipment?equipment->EquippedWeapon():UnmadeCore::ItemId::Count);text=(equipment?equipment->DescribeInventory():TEXT("Equipment unavailable"))+FString::Printf(TEXT("\nACTIVE FIGHTING STYLE\n%s: reach %.0f cm, rhythm %.2f seconds, stamina %.0f per strike.\nThree-hit finishing combo applies a status and stagger. Shift dodge; right mouse guard; time guard entry to parry.\nMastery %d/3 improves stamina recovery, parry and dodge. F11 uses the selected earned technique.\n"),UTF8_TO_TCHAR(style.name),style.reach,style.cooldown,style.cost,h->GetActiveMastery())+h->GetDepthJournal();}
 if(Page==2){auto& s=UnmadeCore::ExpeditionSpecs[SelectedRealm];auto* d=h->Depth.Arc(SelectedRealm);FVector delta=FVector(s.x,s.y,95)-p->GetActorLocation();text=FString::Printf(TEXT("%s\nFrontier: %s / refuge: %s\nDungeon: %s\nCity bearing from you: %.0f m east, %.0f m north. Distance %.0f m.\nRoute %s / cache %s / local favor %d / mastery %d.\nMap selection sets a destination and never teleports or unlocks travel.\n"),UTF8_TO_TCHAR(s.city),UTF8_TO_TCHAR(s.frontier),UTF8_TO_TCHAR(s.refuge),UTF8_TO_TCHAR(s.dungeon),delta.X/100,delta.Y/100,delta.Size2D()/100,d->route?TEXT("open"):TEXT("closed"),d->secret?TEXT("found"):TEXT("hidden"),d->favor,d->training);for(int r=0;r<6;++r){auto c=UnmadeCore::DungeonRoom(SelectedRealm,r);text+=FString::Printf(TEXT("Chamber %d: %.0f m east, %.0f m north from the city.\n"),r+1,c.x/100.,(15000+c.y)/100.);}if(SelectedRealm==0)text+=TEXT("The Reach also includes Bellwold and Paperhaven.\n");text+=TEXT("Selected personal story: ")+FString(UTF8_TO_TCHAR(UnmadeCore::DepthSpecs[SelectedRealm].quest))+TEXT("\nA: ")+UTF8_TO_TCHAR(UnmadeCore::DepthSpecs[SelectedRealm].option_a)+TEXT("\nB: ")+UTF8_TO_TCHAR(UnmadeCore::DepthSpecs[SelectedRealm].option_b);}
 if(Body->GetText().ToString()!=text)Body->SetText(FText::FromString(text));
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
void UUnmadeRpgWidget::SelectRealm(int i){SelectedRealm=FMath::Clamp(i,0,8);Refresh();}
void UUnmadeRpgWidget::Map0(){SelectRealm(0);}
void UUnmadeRpgWidget::Map1(){SelectRealm(1);}
void UUnmadeRpgWidget::Map2(){SelectRealm(2);}
void UUnmadeRpgWidget::Map3(){SelectRealm(3);}
void UUnmadeRpgWidget::Map4(){SelectRealm(4);}
void UUnmadeRpgWidget::Map5(){SelectRealm(5);}
void UUnmadeRpgWidget::Map6(){SelectRealm(6);}
void UUnmadeRpgWidget::Map7(){SelectRealm(7);}
void UUnmadeRpgWidget::Map8(){SelectRealm(8);}
