#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnmadeRpgWidget.generated.h"
class UButton;class USizeBox;class UTextBlock;class UBorder;class UProgressBar;class UCanvasPanel;
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
 virtual FReply NativeOnKeyDown(const FGeometry& Geometry,const FKeyEvent& Event)override;
private:
 int Page=0,SelectedRealm=0;bool bOpen=false,bPausedWorld=false;double NextRefresh=0,NoticeUntil=0;
 FString LastNotice;uint64 LastNoticeSerial=0;
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
