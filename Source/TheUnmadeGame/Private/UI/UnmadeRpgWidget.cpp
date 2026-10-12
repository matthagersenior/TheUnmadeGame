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
 auto* tabs=WidgetTree->ConstructWidget<UHorizontalBox>();panelStack->AddChildToVerticalBox(tabs);
 auto Button=[&](UHorizontalBox* row,const FString& title,auto callback){auto* button=WidgetTree->ConstructWidget<UButton>();button->SetContent(Text(title,17));row->AddChildToHorizontalBox(button);FScriptDelegate click;click.BindUFunction(this,FName(callback));button->OnClicked.Add(click);return button;};
 InitialFocus=Button(tabs,TEXT(" Journal [J] "),TEXT("Journal"));Button(tabs,TEXT(" Equipment [I] "),TEXT("Equipment"));Button(tabs,TEXT(" Map [Tab] "),TEXT("Map"));Button(tabs,TEXT(" Settings "),TEXT("Settings"));Button(tabs,TEXT(" Close [Esc] "),TEXT("Dismiss"));
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
 for(int group=0;group<4;++group){auto* actions=WidgetTree->ConstructWidget<UWrapBox>();ActionGroups.Add(actions);actions->SetInnerSlotPadding(FVector2D(6,6));panelStack->AddChildToVerticalBox(actions);auto Action=[&](const FString& title,const TCHAR* callback){auto* b=WidgetTree->ConstructWidget<UButton>();b->SetContent(Text(title,16));actions->AddChildToWrapBox(b);FScriptDelegate d;d.BindUFunction(this,FName(callback));b->OnClicked.Add(d);};
 if(group==0){Action(TEXT(" Journal category "),TEXT("JournalNext"));Action(TEXT(" Choice A "),TEXT("ChoiceA"));Action(TEXT(" Choice B "),TEXT("ChoiceB"));Action(TEXT(" Duelist "),TEXT("BuildDuelist"));Action(TEXT(" Warden "),TEXT("BuildWarden"));Action(TEXT(" Pathfinder "),TEXT("BuildPathfinder"));Action(TEXT(" Earn tool "),TEXT("EarnTool"));Action(TEXT(" Craft binding "),TEXT("CraftBinding"));Action(TEXT(" Privilege "),TEXT("EarnPrivilege"));Action(TEXT(" Repair "),TEXT("RepairTown"));Action(TEXT(" Mediate "),TEXT("Mediate"));Action(TEXT(" Specialty "),TEXT("Specialty"));Action(TEXT(" Rest six hours "),TEXT("Rest"));}
 if(group==1){Action(TEXT(" Weapon "),TEXT("Weapon"));Action(TEXT(" Armor "),TEXT("Armor"));Action(TEXT(" Charm "),TEXT("Charm"));Action(TEXT(" Salve "),TEXT("Heal"));Action(TEXT(" Select technique "),TEXT("SelectTechnique"));Action(TEXT(" Equip selected realm build "),TEXT("SelectBuild"));}
 if(group==2){Action(TEXT(" Track this realm "),TEXT("Track"));Action(TEXT(" Select technique "),TEXT("SelectTechnique"));Action(TEXT(" Equip selected realm build "),TEXT("SelectBuild"));}
 if(group==3){Action(TEXT(" Larger text "),TEXT("TextLarger"));Action(TEXT(" Smaller text "),TEXT("TextSmaller"));Action(TEXT(" Camera "),TEXT("CameraCycle"));Action(TEXT(" Sound "),TEXT("AudioCycle"));Action(TEXT(" Contrast "),TEXT("Contrast"));Action(TEXT(" Reduced flashes "),TEXT("Flashes"));Action(TEXT(" Prompts "),TEXT("PromptMode"));Action(TEXT(" Tutorials "),TEXT("Tutorials"));Action(TEXT(" Rebind attack "),TEXT("BindAttack"));Action(TEXT(" Rebind dodge "),TEXT("BindDodge"));Action(TEXT(" Rebind interact "),TEXT("BindInteract"));Action(TEXT(" Rebind guard "),TEXT("BindGuard"));Action(TEXT(" Rebind technique "),TEXT("BindTechnique"));}
 }
 return Super::RebuildWidget();
}
void UUnmadeRpgWidget::NativeConstruct(){Super::NativeConstruct();if(GetGameInstance())if(auto* dialogue=GetGameInstance()->GetSubsystem<UUnmadeLocalDialogueSubsystem>())dialogue->OnDialogueReady.AddUniqueDynamic(this,&UUnmadeRpgWidget::NpcLine);Refresh();}
void UUnmadeRpgWidget::NativeDestruct(){if(GetGameInstance())if(auto* dialogue=GetGameInstance()->GetSubsystem<UUnmadeLocalDialogueSubsystem>())dialogue->OnDialogueReady.RemoveDynamic(this,&UUnmadeRpgWidget::NpcLine);Close();Super::NativeDestruct();}
void UUnmadeRpgWidget::Open(int value){if(bOpen&&Page==value){Close();return;}if(auto* h=Hub(GetWorld()))if(h->IsBesDirectedScenePlaying())return;bool WasOpen=bOpen;if(!WasOpen)if(auto* p=Player(this)){p->GetCombat()->SetGuarding(false);p->GetCombat()->CancelWeaponStrike();}if(!WasOpen&&!UGameplayStatics::IsGamePaused(this))bPausedWorld=UGameplayStatics::SetGamePaused(this,true);Page=FMath::Clamp(value,0,3);bOpen=true;Panel->SetVisibility(ESlateVisibility::Visible);if(auto* pc=GetOwningPlayer()){pc->bShowMouseCursor=true;FInputModeUIOnly mode;mode.SetWidgetToFocus(TakeWidget());pc->SetInputMode(mode);if(!WasOpen){pc->SetIgnoreMoveInput(true);pc->SetIgnoreLookInput(true);}}if(InitialFocus)InitialFocus->SetKeyboardFocus();Refresh();}
void UUnmadeRpgWidget::Close(){if(!bOpen)return;bOpen=false;CaptureBinding=0;if(bPausedWorld){UGameplayStatics::SetGamePaused(this,false);bPausedWorld=false;}if(Panel)Panel->SetVisibility(ESlateVisibility::Collapsed);if(auto* pc=GetOwningPlayer()){pc->bShowMouseCursor=false;pc->SetIgnoreMoveInput(false);pc->SetIgnoreLookInput(false);pc->SetInputMode(FInputModeGameOnly());}}
bool UUnmadeRpgWidget::CaptureControl(FKey key){if(!CaptureBinding)return false;if(key==EKeys::Escape){CaptureBinding=0;Refresh();return true;}FString reason;if(GetMutableDefault<UUnmadePlayerPreferences>()->Bind(CaptureBinding,key,reason))CaptureBinding=0;if(auto* h=Hub(GetWorld()))h->NotifyRpg(reason);Refresh();return true;}
FReply UUnmadeRpgWidget::NativeOnMouseButtonDown(const FGeometry& g,const FPointerEvent& e){if(CaptureControl(e.GetEffectingButton()))return FReply::Handled();return Super::NativeOnMouseButtonDown(g,e);}
FReply UUnmadeRpgWidget::NativeOnKeyDown(const FGeometry& g,const FKeyEvent& e){auto k=e.GetKey();if(CaptureControl(k))return FReply::Handled();if(k==EKeys::Escape||k==EKeys::Gamepad_FaceButton_Right){Close();return FReply::Handled();}if(k==EKeys::J){Open(0);return FReply::Handled();}if(k==EKeys::I){Open(1);return FReply::Handled();}if(k==EKeys::Gamepad_LeftShoulder||k==EKeys::Gamepad_RightShoulder){Open((Page+(k==EKeys::Gamepad_RightShoulder?1:3))%4);return FReply::Handled();}if(k==EKeys::Six){ChoiceA();return FReply::Handled();}if(k==EKeys::Seven){ChoiceB();return FReply::Handled();}bool navigation=k==EKeys::Tab||k==EKeys::Up||k==EKeys::Down||k==EKeys::Left||k==EKeys::Right||k==EKeys::Enter||k==EKeys::SpaceBar||k==EKeys::Gamepad_DPad_Up||k==EKeys::Gamepad_DPad_Down||k==EKeys::Gamepad_DPad_Left||k==EKeys::Gamepad_DPad_Right||k==EKeys::Gamepad_FaceButton_Bottom;if(bOpen&&!navigation)return FReply::Handled();return Super::NativeOnKeyDown(g,e);}

void UUnmadeRpgWidget::NativeTick(const FGeometry& g,float dt){Super::NativeTick(g,dt);if(GetWorld()&&GetWorld()->GetTimeSeconds()>=NextRefresh){NextRefresh=GetWorld()->GetTimeSeconds()+.2;Refresh();}}
void UUnmadeRpgWidget::Refresh(){auto* p=Player(this);auto* h=Hub(GetWorld());if(!p||!h||!Body)return;auto* combat=p->GetCombat();if(combat){Health->SetPercent(combat->GetHealth()/FMath::Max(1.f,combat->GetMaxHealth()));Stamina->SetPercent(combat->GetStamina()/100);Stats->SetText(FText::FromString(FString::Printf(TEXT("Health %.0f / %.0f   Stamina %.0f   Strain %.0f\n%s"),combat->GetHealth(),combat->GetMaxHealth(),combat->GetStamina(),p->GetRealityStrain(),*combat->GetCombatFeedback())));}
 FString threat;double best=FMath::Square(1400.f);for(TActorIterator<AUnmadeEnemyCharacter> e(GetWorld());e;++e){double d=FVector::DistSquared2D(p->GetActorLocation(),e->GetActorLocation());if(e->CanBeTargeted()&&d<best){FVector toward=e->GetActorLocation()-p->GetActorLocation();if(FVector::DotProduct(p->GetActorForwardVector(),toward.GetSafeNormal2D())<0)continue;FCollisionQueryParams sight(SCENE_QUERY_STAT(HudThreatSight),false);sight.AddIgnoredActor(p);sight.AddIgnoredActor(*e);if(GetWorld()->LineTraceTestByChannel(p->GetActorLocation()+FVector(0,0,55),e->GetActorLocation()+FVector(0,0,55),ECC_Visibility,sight))continue;best=d;threat=FString::Printf(TEXT("%s   %.0f / %.0f health\n%s"),*e->GetCombatName(),e->GetCombat()->GetHealth(),e->GetCombat()->GetMaxHealth(),*e->GetCombatWarning());}}EnemyText->SetText(FText::FromString(threat));
 auto* pref=GetDefault<UUnmadePlayerPreferences>();if(FMath::Abs(pref->TextScale-AppliedTextScale)>.001){for(const auto& entry:BaseFonts)if(auto* t=entry.Key.Get()){auto f=t->GetFont();f.Size=FMath::RoundToInt(entry.Value*pref->TextScale);t->SetFont(f);}AppliedTextScale=pref->TextScale;}WidgetTree->ForEachWidget([&](UWidget* w){if(auto* b=Cast<UBorder>(w))b->SetBrushColor(pref->HighContrast?FLinearColor::Black:FLinearColor(.025,.04,.055,.96));});
 Objective->SetVisibility(!bOpen&&!h->IsBesDirectedScenePlaying()?ESlateVisibility::HitTestInvisible:ESlateVisibility::Collapsed);Objective->SetText(FText::FromString(h->GetTrackedObjective()));for(int i=0;i<RealmButtons.Num();++i)RealmButtons[i]->SetVisibility(h->IsRealmKnown(i)?ESlateVisibility::Visible:ESlateVisibility::Collapsed);for(int i=0;i<ActionGroups.Num();++i)ActionGroups[i]->SetVisibility(bOpen&&i==Page?ESlateVisibility::Visible:ESlateVisibility::Collapsed);
 FString contextual=h->GetDepthPrompt(p);if(contextual.StartsWith(TEXT("E ")))contextual=pref->InteractKey.GetDisplayName().ToString()+contextual.Mid(1);if(pref->ControllerPrompts&&contextual.StartsWith(pref->InteractKey.GetDisplayName().ToString()))contextual=pref->PadInteract.GetDisplayName().ToString()+contextual.Mid(pref->InteractKey.GetDisplayName().ToString().Len());Prompt->SetText(FText::FromString(contextual+TEXT(" / ")+pref->ControlPrompt()));MapContainer->SetVisibility(bOpen&&Page==2?ESlateVisibility::Visible:ESlateVisibility::Collapsed);
 if(h->RpgNoticeSerial!=LastNoticeSerial){LastNoticeSerial=h->RpgNoticeSerial;LastNotice=h->RpgNotice;NoticeUntil=GetWorld()->GetTimeSeconds()+18;Notice->SetText(FText::FromString(LastNotice));}if(GetWorld()->GetTimeSeconds()<h->RpgSceneUntil){Notice->SetText(FText::FromString(h->RpgSceneNotice));NoticeUntil=FMath::Max(NoticeUntil,h->RpgSceneUntil);}
 NoticePanel->SetVisibility(!bOpen&&!h->IsBesDirectedScenePlaying()&&GetWorld()->GetTimeSeconds()<NoticeUntil?ESlateVisibility::HitTestInvisible:ESlateVisibility::Collapsed);
 if(!bOpen)return;
 FString text;if(Page==0){const TCHAR* categories[]={TEXT("Objectives"),TEXT("People and return journeys"),TEXT("Evidence"),TEXT("Ten disciplines"),TEXT("Economy")};text=FString(categories[JournalCategory])+TEXT(" — use Journal category to switch\n\n");if(JournalCategory==0)text+=h->GetExperienceJournal()+h->DescribeUnansweredRoad(p)+h->GetExpeditionJournal()+h->GetDepthJournal();if(JournalCategory==1){text+=h->GetRoadJournal()+h->GetCinderholdJournal()+h->GetBesHouseholdJournal()+TEXT("\nRECENT CONVERSATIONS\n");for(const auto& line:h->RpgHistory)text+=line+TEXT("\n\n");}if(JournalCategory==2)text+=h->GetWitnessEchoJournal()+h->GetEvidenceNotebook();if(JournalCategory==3){if(auto* rites=p->FindComponentByClass<UUnmadeTenfoldComponent>())for(const auto& spec:UnmadeCore::RiteSpecs){int stage=rites->RiteStage(spec.id);text+=FString::Printf(TEXT("%s: chapter %d/5 — %s\n"),UTF8_TO_TCHAR(spec.name),FMath::Min(stage+1,5),stage>=5?TEXT("mastered"):UTF8_TO_TCHAR(spec.chapters[stage]));}text+=h->GetRealityConsequenceJournal();}if(JournalCategory==4)text+=h->GetLocalMaterialJournal()+h->GetLivingEconomyJournal();}
 if(Page==1){auto* equipment=p->GetEquipment();auto style=h->PlayerWeaponStyle(equipment?equipment->EquippedWeapon():UnmadeCore::ItemId::Count);text=(equipment?equipment->DescribeInventory():TEXT("Equipment unavailable"))+FString::Printf(TEXT("\nACTIVE FIGHTING STYLE\n%s: reach %.0f cm, rhythm %.2f seconds, stamina %.0f per strike.\nThree-hit finishing combo applies a status and stagger. Shift dodge; right mouse guard; time guard entry to parry.\nMastery %d/3 improves stamina recovery, parry and dodge. F11 uses the selected earned technique.\n"),UTF8_TO_TCHAR(style.name),style.reach,style.cooldown,style.cost,h->GetActiveMastery())+h->GetDepthJournal();}
 if(Page==2)text=h->GetExperienceMap(SelectedRealm,p);
 if(Page==3)text=pref->Describe()+(CaptureBinding?TEXT("\nWAITING FOR A CONTROL — Escape cancels."):TEXT(""));
 if(!LastNotice.IsEmpty())text+=TEXT("\n\nLATEST ACTION\n")+LastNotice;
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
