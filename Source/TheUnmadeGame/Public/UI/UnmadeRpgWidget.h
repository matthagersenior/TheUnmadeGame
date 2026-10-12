#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "UnmadeRpgWidget.generated.h"
class UWrapBox;class UButton;class USizeBox;class UTextBlock;class UBorder;class UProgressBar;class UCanvasPanel;
UCLASS()
class THEUNMADEGAME_API UUnmadeRpgWidget : public UUserWidget {
 GENERATED_BODY()
public:
 void Open(int Page);
 void Close();
 bool IsOpen()const{return bOpen;}
protected:
 virtual TSharedRef<SWidget> RebuildWidget()override;
 virtual void NativeConstruct()override;
 virtual void NativeDestruct()override;
 virtual void NativeTick(const FGeometry& Geometry,float Delta)override;
 virtual FReply NativeOnMouseButtonDown(const FGeometry& Geometry,const FPointerEvent& Event)override;
 virtual FReply NativeOnKeyDown(const FGeometry& Geometry,const FKeyEvent& Event)override;
private:
 int Page=0,SelectedRealm=0,JournalCategory=0,CaptureBinding=0;float AppliedTextScale=1;bool bOpen=false,bPausedWorld=false;double NextRefresh=0,NoticeUntil=0;
 TMap<TWeakObjectPtr<UTextBlock>,int32> BaseFonts;
 FString LastNotice;uint64 LastNoticeSerial=0;
 UPROPERTY() TArray<TObjectPtr<UButton>> RealmButtons;
 UPROPERTY() TArray<TObjectPtr<UWrapBox>> ActionGroups;
 UPROPERTY() TObjectPtr<UTextBlock> Objective;
 UPROPERTY() TObjectPtr<UButton> InitialFocus;
 UPROPERTY() TObjectPtr<UTextBlock> EnemyText;
 UPROPERTY() TObjectPtr<UBorder> Panel;
 UPROPERTY() TObjectPtr<UBorder> NoticePanel;
 UPROPERTY() TObjectPtr<UTextBlock> Body;
 UPROPERTY() TObjectPtr<UTextBlock> Prompt;
 UPROPERTY() TObjectPtr<UTextBlock> Stats;
 UPROPERTY() TObjectPtr<UTextBlock> Notice;
 UPROPERTY() TObjectPtr<UProgressBar> Health;
 UPROPERTY() TObjectPtr<UProgressBar> Stamina;
 UPROPERTY() TObjectPtr<UCanvasPanel> MapCanvas;
 UPROPERTY() TObjectPtr<USizeBox> MapContainer;
 void Refresh();
 bool CaptureControl(FKey Key);
 void Upgrade(int Option);
 UFUNCTION() void Settings();
 UFUNCTION() void Track();
 UFUNCTION() void SelectBuild();
 UFUNCTION() void JournalNext();
 UFUNCTION() void BuildDuelist();
 UFUNCTION() void BuildWarden();
 UFUNCTION() void BuildPathfinder();
 UFUNCTION() void EarnTool();
 UFUNCTION() void CraftBinding();
 UFUNCTION() void EarnPrivilege();
 UFUNCTION() void RepairTown();
 UFUNCTION() void Mediate();
 UFUNCTION() void Specialty();
 UFUNCTION() void Rest();
 UFUNCTION() void TextLarger();
 UFUNCTION() void TextSmaller();
 UFUNCTION() void CameraCycle();
 UFUNCTION() void AudioCycle();
 UFUNCTION() void Contrast();
 UFUNCTION() void Flashes();
 UFUNCTION() void PromptMode();
 UFUNCTION() void Tutorials();
 UFUNCTION() void BindAttack();
 UFUNCTION() void BindDodge();
 UFUNCTION() void BindInteract();
 UFUNCTION() void BindGuard();
 UFUNCTION() void BindTechnique();

 void SelectRealm(int Index);
 UFUNCTION() void Journal();
 UFUNCTION() void Equipment();
 UFUNCTION() void Map();
 UFUNCTION() void Dismiss();
 UFUNCTION() void Weapon();
 UFUNCTION() void Armor();
 UFUNCTION() void Charm();
 UFUNCTION() void Heal();
 UFUNCTION() void ChoiceA();
 UFUNCTION() void ChoiceB();
 UFUNCTION() void SelectTechnique();
 UFUNCTION() void NpcLine(FName Id,const FString& Line,bool UsedModel);
 UFUNCTION() void Map0();
 UFUNCTION() void Map1();
 UFUNCTION() void Map2();
 UFUNCTION() void Map3();
 UFUNCTION() void Map4();
 UFUNCTION() void Map5();
 UFUNCTION() void Map6();
 UFUNCTION() void Map7();
 UFUNCTION() void Map8();
};
