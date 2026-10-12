#include "World/UnmadePrototypeHub.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "World/UnmadeTenfoldComponent.h"
#include "Lexicon/UnmadeLexiconComponent.h"
#include "EngineUtils.h"
#include "Save/UnmadeSaveSession.h"
#include "Save/UnmadePrototypeSave.h"
#include "Save/UnmadeSessionRules.h"
#include "UI/UnmadePlayerPreferences.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Player/UnmadeCharacter.h"
namespace {
TWeakObjectPtr<AUnmadeCharacter> SessionPlayer;
bool Switching=false;
UUnmadePrototypeSave* Read(const FString& slot){auto* s=Cast<UUnmadePrototypeSave>(UGameplayStatics::LoadGameFromSlot(slot,0));if(!s||s->SchemaVersion!=1)return nullptr;if(s->bHasPlayerResume&&!UnmadeCore::ValidResume(s->ResumePosition.X,s->ResumePosition.Y,s->ResumePosition.Z,s->ResumeHealth,s->ResumeStamina))return nullptr;return s;}
}
namespace UnmadeSave {
void RegisterPlayer(AUnmadeCharacter* p){SessionPlayer=p;Switching=false;}
FString SlotName(int p){if(p<0)p=GetDefault<UUnmadePlayerPreferences>()->ActiveProfile;p=UnmadeCore::ValidProfile(p)?p:0;return p==0?TEXT("UnmadePrototypeNPC"):FString::Printf(TEXT("UnmadeProfile%d"),p);}
bool HasRecord(){FString slot=SlotName();return UGameplayStatics::DoesSaveGameExist(slot,0)||UGameplayStatics::DoesSaveGameExist(slot+TEXT("_Backup"),0);}
USaveGame* LoadGameFromSlot(){FString slot=SlotName();auto* raw=UGameplayStatics::LoadGameFromSlot(slot,0);auto* typed=Cast<UUnmadePrototypeSave>(raw);bool compatible=!raw||(typed&&typed->SchemaVersion==1);auto* primary=Read(slot);auto* backup=Read(slot+TEXT("_Backup"));auto selected=UnmadeCore::ChooseStoredRecord(raw!=nullptr,compatible,primary!=nullptr,backup!=nullptr,GetDefault<UUnmadePlayerPreferences>()->UseBackup);return selected==UnmadeCore::StoredRecord::Incompatible?raw:selected==UnmadeCore::StoredRecord::Primary?primary:selected==UnmadeCore::StoredRecord::Backup?backup:nullptr;}
bool SaveGameToSlot(USaveGame* raw){if(Switching)return false;if(SessionPlayer.IsValid()){auto* p=SessionPlayer.Get();if(p->HasRejectedSave()||(p->GetEquipment()&&p->GetEquipment()->HasRejectedSave()))return false;if(auto* rite=p->FindComponentByClass<UUnmadeTenfoldComponent>())if(rite->HasRejectedSave())return false;if(auto* lex=p->FindComponentByClass<UUnmadeLexiconComponent>())if(lex->HasRejectedSave())return false;for(TActorIterator<AUnmadePrototypeHub> h(p->GetWorld());h;++h)if(!h->IsWorldReady()||h->HasRejectedWorld())return false;}if(auto* stored=UGameplayStatics::LoadGameFromSlot(SlotName(),0)){auto* existing=Cast<UUnmadePrototypeSave>(stored);if(!existing||existing->SchemaVersion!=1)return false;}auto* s=Cast<UUnmadePrototypeSave>(raw);if(!s||s->SchemaVersion!=1)return false;if(SessionPlayer.IsValid())SessionPlayer->CaptureSession(s);FString slot=SlotName();auto* settings=GetMutableDefault<UUnmadePlayerPreferences>();auto* previous=settings->UseBackup?nullptr:Read(slot);if(!UnmadeCore::CommitWithPrevious(*s,previous,[&](const UUnmadePrototypeSave& record,bool backup){return UGameplayStatics::SaveGameToSlot(const_cast<UUnmadePrototypeSave*>(&record),slot+(backup?TEXT("_Backup"):TEXT("")),0);}))return false;if(settings->UseBackup){settings->UseBackup=false;settings->SaveConfig();}return true;}
bool SelectProfile(UObject* context,int p,bool backup){if(!context||!context->GetWorld()||!UnmadeCore::ValidProfile(p)||!Read(SlotName(p)+(backup?TEXT("_Backup"):TEXT(""))))return false;auto* settings=GetMutableDefault<UUnmadePlayerPreferences>();Switching=true;settings->ActiveProfile=p;settings->UseBackup=backup;settings->SaveConfig();UGameplayStatics::OpenLevel(context,FName(*UGameplayStatics::GetCurrentLevelName(context,true)));return true;}
bool CreateUnusedProfile(UObject* context){if(!context||!context->GetWorld())return false;for(int p=1;p<=3;++p){auto slot=SlotName(p);if(UGameplayStatics::DoesSaveGameExist(slot,0)||UGameplayStatics::DoesSaveGameExist(slot+TEXT("_Backup"),0))continue;auto* s=Cast<UUnmadePrototypeSave>(UGameplayStatics::CreateSaveGameObject(UUnmadePrototypeSave::StaticClass()));if(!s||!UGameplayStatics::SaveGameToSlot(s,slot,0))return false;return SelectProfile(context,p);}return false;}
FString DescribeProfiles(){FString t=TEXT("SAVE PROFILES\nNew Game creates an unused profile; existing records are never erased. Resume uses the last saved safe settlement and recorded health/stamina. A backup holds the preceding committed record.\n");for(int p=0;p<=3;++p){FString slot=SlotName(p);t+=FString::Printf(TEXT("%s %d: %s / backup %s%s\n"),p==0?TEXT("Legacy"):TEXT("Profile"),p,Read(slot)?TEXT("available"):UGameplayStatics::DoesSaveGameExist(slot,0)?TEXT("unreadable or incompatible"):TEXT("empty"),Read(slot+TEXT("_Backup"))?TEXT("available"):TEXT("none"),GetDefault<UUnmadePlayerPreferences>()->ActiveProfile==p?TEXT(" / active"):TEXT(""));}return t;}
}
