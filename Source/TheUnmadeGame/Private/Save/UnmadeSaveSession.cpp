#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeFieldSurveySave.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "World/UnmadeTenfoldComponent.h"
#include "Lexicon/UnmadeLexiconComponent.h"
#include "EngineUtils.h"
#include "Save/UnmadeSaveSession.h"
#include "Save/UnmadePrototypeSave.h"
#include "Save/UnmadeSessionRules.h"
#include "Save/UnmadeProfileHandoffRules.h"
#include "Engine/Engine.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/GameInstance.h"
#include "Save/UnmadeCombinedSaveRules.h"
#include "Save/UnmadeCaptureRules.h"
#include "UI/UnmadeRpgFeedback.h"
#include "UI/UnmadePlayerPreferences.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Player/UnmadeCharacter.h"
namespace {
TWeakObjectPtr<AUnmadeCharacter> SessionPlayer;
UnmadeCore::ProfileHandoff Handoff;
TWeakObjectPtr<UWorld> OriginWorld;
TWeakObjectPtr<UWorld> FailedWorld;
TWeakObjectPtr<UGameInstance> OriginInstance;
FDelegateHandle TravelFailureHandle;
UnmadeCore::ProfileSelection EffectiveSelection(){
    const auto* settings=GetDefault<UUnmadePlayerPreferences>();
    return Handoff.Effective({UnmadeCore::ValidProfile(settings->ActiveProfile)?settings->ActiveProfile:0,settings->UseBackup});
}
FString LastLoadNotice;
std::string SaveText(const FString& value){return std::string(TCHAR_TO_UTF8(*value));}
std::string SaveText(const FName& value){return value.IsNone()?std::string{}:SaveText(value.ToString());}
bool Valid(const UUnmadePrototypeSave& record){
    return UnmadeCore::ValidCombinedRecord(record,[](const auto& value){return SaveText(value);});
}
bool ValidStored(const UUnmadePrototypeSave& save,const FString& slot){
    if(!Valid(save))return false;
    // The legacy profile can still read its original independent fieldwork file.
    // Check that source too before advertising a record which will import it.
    if(!save.bHasFieldSurveys&&(slot==TEXT("UnmadePrototypeNPC")||slot==TEXT("UnmadePrototypeNPC_Backup"))&&
       UGameplayStatics::DoesSaveGameExist(TEXT("UnmadeOptionalFieldSurveys"),0)){
        auto* legacy=Cast<UUnmadeFieldSurveySave>(UGameplayStatics::LoadGameFromSlot(TEXT("UnmadeOptionalFieldSurveys"),0));
        return legacy&&UnmadeCore::ValidSurveyRecord(legacy->Version,legacy->Stages);
    }
    return true;
}
UUnmadePrototypeSave* Read(const FString& slot){
    auto* save=Cast<UUnmadePrototypeSave>(UGameplayStatics::LoadGameFromSlot(slot,0));
    return save&&ValidStored(*save,slot)?save:nullptr;
}
void LoadNotice(const FString& message){
    if(Handoff.Pending())return; // Destination feedback belongs to its arriving player.
    if(SessionPlayer.IsValid()&&LastLoadNotice!=message){
        LastLoadNotice=message;UnmadeRpgFeedback(-1,12.f,FColor::Orange,message);
    }
}
void ClearTravelFailureHandler(){
    if(GEngine&&TravelFailureHandle.IsValid())GEngine->OnTravelFailure().Remove(TravelFailureHandle);
    TravelFailureHandle.Reset();
}
void ProfileTravelFailed(UWorld* World,ETravelFailure::Type Failure,const FString& Error){
    (void)Failure;(void)Error;
    if(!Handoff.Pending())return;
    if(World&&World!=OriginWorld.Get()&&World->GetGameInstance()!=OriginInstance.Get())return;
    const bool Resume=OriginWorld.IsValid()&&OriginInstance.IsValid()&&
        OriginInstance->GetWorld()==OriginWorld.Get()&&SessionPlayer.IsValid()&&
        SessionPlayer->GetWorld()==OriginWorld.Get()&&
        UGameplayStatics::GetPlayerCharacter(OriginWorld.Get(),0)==SessionPlayer.Get();
    const auto Original=Handoff.Original();
    if(!Handoff.Fail(Resume))return;
    ClearTravelFailureHandler();
    FailedWorld=Resume?nullptr:(World?World:(OriginInstance.IsValid()?OriginInstance->GetWorld():nullptr));
    auto* settings=GetMutableDefault<UUnmadePlayerPreferences>();
    settings->ActiveProfile=Original.profile;settings->UseBackup=Original.backup;
    settings->SaveConfig();
    OriginWorld.Reset();OriginInstance.Reset();
    UnmadeRpgFeedback(-1,14.f,FColor::Red,Resume?
        TEXT("PROFILE LOAD FAILED: your original profile is still active. Its unsaved session is retained; try again or continue playing."):
        TEXT("PROFILE LOAD FAILED: your original profile selection was restored. Choose a profile again to load a fresh world; saved records are protected."));
}
void EnsureTravelFailureHandler(){
    if(GEngine&&!TravelFailureHandle.IsValid())
        TravelFailureHandle=GEngine->OnTravelFailure().AddStatic(&ProfileTravelFailed);
}
bool WriteFailed(){
    UnmadeRpgFeedback(-1,8.f,FColor::Red,TEXT("SAVE FAILED: progress was not committed. Retry saving or use Save Profiles to recover."));
    return false;
}
}
namespace UnmadeSave {
void RegisterPlayer(AUnmadeCharacter* p){
    if(!IsValid(p)||!p->GetWorld())return;
    const auto Target=Handoff.Target();const auto Original=Handoff.Original();
    const auto Arrival=Handoff.RegisterOwner(OriginWorld.Get()==p->GetWorld(),
        OriginInstance.Get()==p->GetWorld()->GetGameInstance(),FailedWorld.Get()!=p->GetWorld());
    if(Arrival==UnmadeCore::ProfileArrival::Waiting)return;
    ClearTravelFailureHandler();
    if(Arrival==UnmadeCore::ProfileArrival::TargetReady||Arrival==UnmadeCore::ProfileArrival::Cancelled){
        const auto Selected=Arrival==UnmadeCore::ProfileArrival::TargetReady?Target:Original;
        auto* settings=GetMutableDefault<UUnmadePlayerPreferences>();
        settings->ActiveProfile=Selected.profile;settings->UseBackup=Selected.backup;
        settings->SaveConfig();
    }
    if(Arrival==UnmadeCore::ProfileArrival::Cancelled){
        FailedWorld=p->GetWorld();OriginWorld.Reset();OriginInstance.Reset();
        return; // This world may already contain destination-loaded owners.
    }
    SessionPlayer=p;OriginWorld.Reset();OriginInstance.Reset();FailedWorld.Reset();LastLoadNotice.Empty();
}
FString SlotName(int p){if(p<0)p=EffectiveSelection().profile;p=UnmadeCore::ValidProfile(p)?p:0;return p==0?TEXT("UnmadePrototypeNPC"):FString::Printf(TEXT("UnmadeProfile%d"),p);}
bool HasRecord(){FString slot=SlotName();return UGameplayStatics::DoesSaveGameExist(slot,0)||UGameplayStatics::DoesSaveGameExist(slot+TEXT("_Backup"),0);}
USaveGame* LoadGameFromSlot(){
    const FString slot=SlotName();
    auto* raw=UGameplayStatics::LoadGameFromSlot(slot,0);
    auto* typed=Cast<UUnmadePrototypeSave>(raw);
    const bool compatible=!raw||(typed&&typed->SchemaVersion==1);
    auto* primary=typed&&ValidStored(*typed,slot)?typed:nullptr;
    auto* backup=Read(slot+TEXT("_Backup"));
    auto* settings=GetMutableDefault<UUnmadePlayerPreferences>();
    const auto selected=UnmadeCore::ChooseStoredRecord(raw!=nullptr,compatible,
        primary!=nullptr,backup!=nullptr,EffectiveSelection().backup);
    if(selected==UnmadeCore::StoredRecord::Backup){
        // Pin recovery until a primary write succeeds, including automatic fallback.
        // Otherwise a later owner or retry could rotate over the recovery source.
        if(Handoff.Pending())Handoff.RecoverTarget();
        else if(!settings->UseBackup){settings->UseBackup=true;settings->SaveConfig();}
        LoadNotice(TEXT("BACKUP RECOVERED: the preceding complete record is active. Some recent progress may be absent. The backup is protected until a new primary save succeeds."));
        return backup;
    }
    if(selected==UnmadeCore::StoredRecord::Primary)return primary;
    if(HasRecord())LoadNotice(TEXT("LOAD FAILED: no complete compatible record could be loaded. Saved files are protected; use Save Profiles to select another profile or recovery record."));
    // Never expose even a partially valid serialized record to a subsystem.
    return nullptr;
}
bool IsSessionWritable(const AUnmadeCharacter* Player){
    if(Handoff.WritesBlocked()||!SessionPlayer.IsValid())return false;
    auto* p=SessionPlayer.Get();
    if((Player&&Player!=p)||!p->HasActorBegunPlay()||!p->IsSessionRestored()||!p->GetWorld())return false;
    if(p->HasRejectedSave()||!p->GetEquipment()||p->GetEquipment()->HasRejectedSave())return false;
    if(auto* rite=p->FindComponentByClass<UUnmadeTenfoldComponent>())if(rite->HasRejectedSave())return false;
    if(auto* lex=p->FindComponentByClass<UUnmadeLexiconComponent>())if(lex->HasRejectedSave())return false;
    bool found=false;
    for(TActorIterator<AUnmadePrototypeHub> h(p->GetWorld());h;++h){
        found=true;if(!h->IsWorldReady()||h->HasRejectedWorld())return false;
    }
    return found;
}
bool IsWorldWritable(const UWorld* World){
 return SessionPlayer.IsValid()&&UnmadeCore::SameSessionWorld(SessionPlayer->GetWorld(),World)&&IsSessionWritable();
}
bool SaveGameToSlot(USaveGame* raw,const UObject* Owner){
    if(Handoff.WritesBlocked())return WriteFailed();
    if(!IsValid(Owner)||!IsWorldWritable(Owner->GetWorld()))return WriteFailed();
    const FString slot=SlotName();
    auto* stored=UGameplayStatics::LoadGameFromSlot(slot,0);
    auto* existing=Cast<UUnmadePrototypeSave>(stored);
    if(stored&&(!existing||existing->SchemaVersion!=1))return WriteFailed();
    auto* s=Cast<UUnmadePrototypeSave>(raw);
    if(!s)return WriteFailed();
    if(!UnmadeCore::PrepareCombinedCapture(*s,
        [&](const auto& record){return ValidStored(record,slot);},
        [&](auto&){
            SessionPlayer->CaptureSession(s);
            for(TActorIterator<AUnmadePrototypeHub> h(Owner->GetWorld());h;++h)
                return h->CaptureWorldSnapshot(s);
            return false;
        }))return WriteFailed();
    if(existing&&!ValidStored(*existing,slot))existing=nullptr;
    auto* settings=GetMutableDefault<UUnmadePlayerPreferences>();
    auto* backup=Read(slot+TEXT("_Backup"));
    // CommitCombinedRecord gates CommitWithPrevious on all subsystem validators.
    if(!UnmadeCore::CommitCombinedRecord(*s,existing,backup,settings->UseBackup,
        HasRecord(),[](const auto& value){return SaveText(value);},
        [&](const UUnmadePrototypeSave& record,bool isBackup){
            return UGameplayStatics::SaveGameToSlot(const_cast<UUnmadePrototypeSave*>(&record),
                slot+(isBackup?TEXT("_Backup"):TEXT("")),0);
        }))return WriteFailed();
    if(settings->UseBackup){settings->UseBackup=false;settings->SaveConfig();}
    return true;
}
bool SelectProfile(UObject* context,int p,bool backup){
    if(Handoff.Pending()||!context||!context->GetWorld()||!GEngine||
       !context->GetWorld()->GetGameInstance()||!UnmadeCore::ValidProfile(p))return false;
    const FString slot=SlotName(p);
    auto* raw=UGameplayStatics::LoadGameFromSlot(slot,0);
    auto* typed=Cast<UUnmadePrototypeSave>(raw);
    if(raw&&(!typed||typed->SchemaVersion!=1))return false;
    if(!Read(slot+(backup?TEXT("_Backup"):TEXT(""))))return false;
    const FString Level=UGameplayStatics::GetCurrentLevelName(context,true);
    if(Level.IsEmpty())return false;
    EnsureTravelFailureHandler();
    if(!TravelFailureHandle.IsValid()||!Handoff.Begin(EffectiveSelection(),{p,backup}))return false;
    OriginWorld=context->GetWorld();OriginInstance=OriginWorld->GetGameInstance();
    // Pending routing serves the destination's BeginPlay reads; settings are
    // persisted only after its new player registers. Old-world writes stay shut.
    UGameplayStatics::OpenLevel(context,FName(*Level));
    return true;
}
bool CreateUnusedProfile(UObject* context){if(Handoff.Pending()||!context||!context->GetWorld())return false;for(int p=1;p<=3;++p){auto slot=SlotName(p);if(UGameplayStatics::DoesSaveGameExist(slot,0)||UGameplayStatics::DoesSaveGameExist(slot+TEXT("_Backup"),0))continue;auto* s=Cast<UUnmadePrototypeSave>(UGameplayStatics::CreateSaveGameObject(UUnmadePrototypeSave::StaticClass()));if(!s||!UGameplayStatics::SaveGameToSlot(s,slot,0))return false;return SelectProfile(context,p);}return false;}
FString DescribeProfiles(){
    FString text=TEXT("SAVE PROFILES\nNew Game creates an unused profile; existing records are never erased. Resume uses the last saved safe settlement and recorded health/stamina. A backup holds the preceding committed record. Availability requires every present subsystem to validate.\n");
    const auto* settings=GetDefault<UUnmadePlayerPreferences>();
    for(int p=0;p<=3;++p){
        const FString slot=SlotName(p),backupSlot=slot+TEXT("_Backup");
        auto* raw=UGameplayStatics::LoadGameFromSlot(slot,0);
        auto* typed=Cast<UUnmadePrototypeSave>(raw);
        const bool incompatible=raw&&(!typed||typed->SchemaVersion!=1);
        const TCHAR* primary=Read(slot)?TEXT("available"):
            UGameplayStatics::DoesSaveGameExist(slot,0)?TEXT("unreadable or incompatible"):TEXT("empty");
        const TCHAR* backup=Read(backupSlot)?
            (incompatible?TEXT("protected by incompatible primary"):TEXT("available")):
            UGameplayStatics::DoesSaveGameExist(backupSlot,0)?TEXT("unreadable or incompatible"):TEXT("none");
        text+=FString::Printf(TEXT("%s %d: %s / backup %s%s\n"),
            p==0?TEXT("Legacy"):TEXT("Profile"),p,primary,backup,
            settings->ActiveProfile==p?(settings->UseBackup?TEXT(" / active, recovering backup"):TEXT(" / active")):TEXT(""));
    }
    return text;
}
}
