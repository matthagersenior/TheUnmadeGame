#include "World/UnmadeTenfoldComponent.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadePrototypeHub.h"
#include "Save/UnmadePrototypeSave.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Combat/UnmadeBossCharacter.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "Items/UnmadeItemRules.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "CollisionQueryParams.h"

UUnmadeTenfoldComponent::UUnmadeTenfoldComponent()
{
    PrimaryComponentTick.bCanEverTick=true;
    PrimaryComponentTick.TickInterval=.5f;
}

void UUnmadeTenfoldComponent::BeginPlay()
{
    Super::BeginPlay();
    const UUnmadePrototypeSave* Save=Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"),0));
    if(!Save || Save->SchemaVersion!=1 || !Save->bHasTenfoldChronicle)return;
    UnmadeCore::TenfoldSnapshot State;
    if(Save->RiteStages.Num()!=10 || Save->RiteChoices.Num()!=10 ||
       Save->RiteReadyAt.Num()!=10 || Save->DisciplineMastery.Num()!=6 ||
       Save->RiteTrialBits<0 || Save->LegacyDeedBits<0 ||
       Save->VerifiedRoadBits<0)
    {
        bSaveRejected=true;
        UE_LOG(LogTemp,Error,TEXT("Invalid tenfold array lengths; refusing overwrite"));
        return;
    }
    for(int32 i=0;i<10;++i)
    {
        State.stage[i]=Save->RiteStages[i];
        State.choice[i]=Save->RiteChoices[i];
        State.readyAt[i]=Save->RiteReadyAt[i];
    }
    for(int32 i=0;i<6;++i)State.disciplineMastery[i]=Save->DisciplineMastery[i];
    State.trialUsed=static_cast<uint16>(Save->RiteTrialBits);
    State.deeds=static_cast<uint8>(Save->LegacyDeedBits);
    State.verifiedRoutes=static_cast<uint8>(Save->VerifiedRoadBits);
    State.debtDueDay=Save->TomorrowDebtDueDay;
    State.exhaustedUntilDay=Save->TomorrowDebtExhaustedUntil;
    State.brokenOaths=Save->BrokenOathCount;
    State.oathActive=Save->bOathActive;
    if(!Chronicle.Restore(State))
    {
        bSaveRejected=true;
        UE_LOG(LogTemp,Error,TEXT("Invalid tenfold quest state; refusing overwrite"));
    }
}

bool UUnmadeTenfoldComponent::Persist(bool bWriteStrain)
{
    if(bSaveRejected)return false;
    UUnmadePrototypeSave* Save=UUnmadePrototypeSave::LoadOrCreate();
    if(!Save)return false;
    const auto& State=Chronicle.Snapshot();
    Save->RiteStages.Reset();
    Save->RiteChoices.Reset();
    Save->RiteReadyAt.Reset();
    Save->DisciplineMastery.Reset();
    for(int32 i=0;i<10;++i)
    {
        Save->RiteStages.Add(State.stage[i]);
        Save->RiteChoices.Add(State.choice[i]);
        Save->RiteReadyAt.Add(State.readyAt[i]);
    }
    for(int32 i=0;i<6;++i)Save->DisciplineMastery.Add(State.disciplineMastery[i]);
    Save->RiteTrialBits=State.trialUsed;
    Save->LegacyDeedBits=State.deeds;
    Save->VerifiedRoadBits=State.verifiedRoutes;
    Save->TomorrowDebtDueDay=State.debtDueDay;
    Save->TomorrowDebtExhaustedUntil=State.exhaustedUntilDay;
    Save->BrokenOathCount=State.brokenOaths;
    Save->bOathActive=State.oathActive;
    Save->bHasTenfoldChronicle=true;
    if(bWriteStrain)
    {
        const AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner());
        if(!IsValid(Player))return false;
        Save->PlayerStrain=Player->GetRealityStrain();
        Save->bHasFractureSnapshot=true;
    }
    return UGameplayStatics::SaveGameToSlot(Save,TEXT("UnmadePrototypeNPC"),0);
}

UnmadeCore::RiteContext UUnmadeTenfoldComponent::GatherContext()
{
    UnmadeCore::RiteContext Ctx;
    AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner());
    UWorld* World=GetWorld();
    if(!IsValid(Player) || !World)return Ctx;
    Ctx.strain=Player->GetRealityStrain();
    Ctx.now=World->GetTimeSeconds();
    Ctx.day=1;
    const UUnmadePrototypeSave* Save=Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"),0));
    if(Save && Save->SchemaVersion==1)
    {
        Ctx.villageVisits=Save->VisitedSettlementsMask;
        Ctx.landmarkVisits=Save->DiscoveredLoreMask;
        Ctx.frontierVisits=Save->VisitedFrontierRealms;
        Ctx.bellQuestComplete=Save->BellwoldTaskStage==2;
        Ctx.paperQuestComplete=Save->PaperhavenTaskStage==2;
        Ctx.hasVeyl=Save->LexiconEvidence.Contains(FName("evidence.glimpse")) &&
                    Save->LexiconEvidence.Contains(FName("evidence.archivist"));
        if(Save->bHasFactionChronicle && Save->FactionEndings.Num()==3)
            for(int32 i=0;i<3;++i)
                if(Save->FactionEndings[i]!=0)Ctx.factionEndings|=1<<i;
        if(Save->bHasInventorySnapshot)
        {
            const auto claimed=[Save](UnmadeCore::Achievement Reward) {
                return (static_cast<uint64>(Save->AwardedMilestoneBits) &
                    (uint64(1)<<static_cast<int32>(Reward)))!=0;
            };
            if(claimed(UnmadeCore::Achievement::HollowBell))Ctx.bossVictories|=1;
            if(claimed(UnmadeCore::Achievement::RedactedCurator))Ctx.bossVictories|=2;
            if(claimed(UnmadeCore::Achievement::UnfinishedPilgrim))Ctx.bossVictories|=4;
        }
    }
    for(TActorIterator<AUnmadePrototypeHub> Hub(World);Hub;++Hub)
    {
        Ctx.day=Hub->GetGameDay();
        Ctx.now=Hub->GetWorldClockSeconds();
        break;
    }
    Ctx.selectedLaw=SelectedLaw;
    // Only physically close, unobstructed authored ritual stones count.
    double ClosestStone=FMath::Square(380.f);
    for(TActorIterator<AStaticMeshActor> It(World);It;++It)
    {
        const double Dist=FVector::DistSquared(Player->GetActorLocation(),It->GetActorLocation());
        if(Dist>=ClosestStone)continue;
        for(int32 Index=0;Index<10;++Index)
        {
            if(!It->ActorHasTag(FName(*FString::Printf(TEXT("Rite.Site.%d"),Index))))
                continue;
            FCollisionQueryParams Sight(SCENE_QUERY_STAT(TenfoldRiteSight),false);
            Sight.AddIgnoredActor(Player);
            Sight.AddIgnoredActor(*It);
            if(World->LineTraceTestByChannel(Player->GetActorLocation()+FVector(0,0,60),
                    It->GetActorLocation()+FVector(0,0,60),ECC_Visibility,Sight))
                continue;
            Ctx.site=Index;
            ClosestStone=Dist;
        }
    }
    WitnessBuffer.clear();
    double ClosestNpc=FMath::Square(400.f);
    for(TActorIterator<AUnmadeNpcCharacter> It(World);It;++It)
    {
        const double Dist=FVector::DistSquared(Player->GetActorLocation(),It->GetActorLocation());
        if(Dist<ClosestNpc)
        {
            FCollisionQueryParams Sight(SCENE_QUERY_STAT(TenfoldWitnessSight),false);
            Sight.AddIgnoredActor(Player);
            Sight.AddIgnoredActor(*It);
            if(!World->LineTraceTestByChannel(
                Player->GetActorLocation()+FVector(0,0,55),
                It->GetActorLocation()+FVector(0,0,55),ECC_Visibility,Sight))
            {
                const FString Id=It->GetStableId().ToString();
                WitnessBuffer=std::string(TCHAR_TO_UTF8(*Id));
                Ctx.lineOfSight=true;
                ClosestNpc=Dist;
            }
        }
        // This is not "all nearby NPCs": only actual firsthand recorded
        // witnesses of a relevant player/world event qualify as testimony.
        if(Ctx.verifiedWitnesses>=3 || Dist>FMath::Square(1800.f) ||
           !IsValid(It->GetMemory()))continue;
        for(const FUnmadeNpcObservation& Event:It->GetMemory()->GetObservations())
        {
            if(Event.Evidence!=EUnmadeEvidenceKind::Witnessed)continue;
            if(Event.EventKind==FName("Reality.Anomaly") ||
               Event.EventKind==FName("Reality.Rite") ||
               Event.EventKind==FName("Player.Helped") ||
               Event.EventKind==FName("World.FactionResolved"))
            {
                ++Ctx.verifiedWitnesses;
                break;
            }
        }
    }
    Ctx.witness=WitnessBuffer.empty()?nullptr:WitnessBuffer.c_str();
    return Ctx;
}

void UUnmadeTenfoldComponent::ExplainResult(UnmadeCore::RiteResult Result) const
{
    if(!GEngine)return;
    const TCHAR* Message=TEXT("Rite unavailable. Check your current quest chapter.");
    switch(Result)
    {
    case UnmadeCore::RiteResult::WrongLocation:
        Message=TEXT("Approach this discipline's inscribed ritual stone.");break;
    case UnmadeCore::RiteResult::WrongWitness:
        Message=TEXT("Find the named firsthand witness listed in the rite journal.");break;
    case UnmadeCore::RiteResult::NeedsEvidence:
        Message=TEXT("Insufficient real evidence: explore, finish earlier quests, or gather two firsthand witnesses.");break;
    case UnmadeCore::RiteResult::NeedAbilityTrial:
        Message=TEXT("Perform this new ability at its ritual stone before continuing.");break;
    case UnmadeCore::RiteResult::InsufficientStability:
        Message=TEXT("Reality Strain is too high to invoke this power.");break;
    case UnmadeCore::RiteResult::Cooldown:
        Message=TEXT("The rite is recharging. You cannot spam reality changes.");break;
    case UnmadeCore::RiteResult::DebtOutstanding:
        Message=TEXT("Tomorrow's debt is still owed. Borrowing twice is forbidden.");break;
    case UnmadeCore::RiteResult::Exhausted:
        Message=TEXT("The debt has been repaid, but the exhaustion remains until the following day.");break;
    case UnmadeCore::RiteResult::BrokenOath:
        Message=TEXT("Your former oath remembers the betrayal. It cannot be reclaimed.");break;
    case UnmadeCore::RiteResult::AlreadyCompleted:
        Message=TEXT("This five-chapter quest is mastered. Repeat actions give no new rewards.");break;
    default: break;
    }
    GEngine->AddOnScreenDebugMessage(-1,7.f,FColor::Yellow,Message);
}

void UUnmadeTenfoldComponent::ShowRite()
{
    if(!GEngine)return;
    const auto& Spec=UnmadeCore::RiteSpecs[SelectedRite];
    const int32 Stage=Chronicle.Stage(Spec.id);
    const FString Chapter=Stage>=5?TEXT("MASTERED: return to the world and use this power responsibly.")
        :FString(UTF8_TO_TCHAR(Spec.chapters[Stage]));
    GEngine->AddOnScreenDebugMessage(-1,16.f,FColor::Cyan,
        FString::Printf(TEXT("RITE %d/10 | %s | Chapter %d/5 | %s"),
            SelectedRite+1,UTF8_TO_TCHAR(Spec.name),FMath::Min(5,Stage+1),*Chapter));
}
void UUnmadeTenfoldComponent::NextRite()
{
    SelectedRite=(SelectedRite+1)%10;ShowRite();
}
void UUnmadeTenfoldComponent::PreviousRite()
{
    SelectedRite=(SelectedRite+9)%10;ShowRite();
}
void UUnmadeTenfoldComponent::CycleWorldLaw()
{
    SelectedLaw=(SelectedLaw+1)%3;
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Silver,
        FString::Printf(TEXT("Unwrite Law: %s"),
            SelectedLaw==0?TEXT("Gravity"):SelectedLaw==1?TEXT("Sound"):TEXT("Momentum")));
}
void UUnmadeTenfoldComponent::StudyRite()
{
    if(bSaveRejected){ExplainResult(UnmadeCore::RiteResult::Invalid);return;}
    const auto Id=static_cast<UnmadeCore::RiteId>(SelectedRite);
    const int Stage=Chronicle.Stage(Id);
    if(Stage==2 || Stage==3) {
        ShowRite();return;
    }
    if(Stage==5){ExplainResult(UnmadeCore::RiteResult::AlreadyCompleted);return;}
    auto Context=GatherContext();
    const auto Before=Chronicle.Snapshot();
    const auto Result=Chronicle.Advance(Id,
        Stage==0?UnmadeCore::RiteAction::Discover:
        Stage==1?UnmadeCore::RiteAction::Testify:UnmadeCore::RiteAction::Master,
        Context);
    if(Result!=UnmadeCore::RiteResult::Advanced &&
       Result!=UnmadeCore::RiteResult::Completed)
    {
        ExplainResult(Result);return;
    }
    if(!Persist()) {Chronicle.Restore(Before);ExplainResult(UnmadeCore::RiteResult::Invalid);return;}
    if(Result==UnmadeCore::RiteResult::Completed)
    {
        if(AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner()))
            Player->ReconcileEarnedRewards();
    }
    ShowRite();
}
void UUnmadeTenfoldComponent::DecideRite(int32 Choice)
{
    if(bSaveRejected)return;
    const auto Id=static_cast<UnmadeCore::RiteId>(SelectedRite);
    const auto Before=Chronicle.Snapshot();
    const auto Result=Chronicle.Advance(Id,UnmadeCore::RiteAction::Decide,
        GatherContext(),Choice);
    if(Result!=UnmadeCore::RiteResult::Advanced){ExplainResult(Result);return;}
    if(!Persist()){Chronicle.Restore(Before);return;}
    if(AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner()))
        Player->ReportRiteWitnessEvent();
    ShowRite();
}
void UUnmadeTenfoldComponent::DecideSolidarity(){DecideRite(1);}
void UUnmadeTenfoldComponent::DecideTruth(){DecideRite(2);}

void UUnmadeTenfoldComponent::InvokeRite()
{
    AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner());
    if(bSaveRejected || !IsValid(Player) || !GetWorld() || Player->GetCombat()->IsDefeated())return;
    const auto Id=static_cast<UnmadeCore::RiteId>(SelectedRite);
    const auto Context=GatherContext();
    if(Id==UnmadeCore::RiteId::UnderstandingBosses && !Player->HasNearbyHollowKeeper())
    {
        ExplainResult(UnmadeCore::RiteResult::WrongLocation);
        return;
    }
    const auto Previous=Chronicle.Snapshot();
    const auto Power=Chronicle.Invoke(Id,Context);
    if(Power.result!=UnmadeCore::RiteResult::Applied)
    {
        ExplainResult(Power.result);return;
    }
    if(!Player->SpendRealityStrain(Power.cost))
    {
        Chronicle.Restore(Previous);
        ExplainResult(UnmadeCore::RiteResult::InsufficientStability);
        return;
    }
    if(Chronicle.Stage(Id)==2)
    {
        const auto Advanced=Chronicle.Advance(Id,UnmadeCore::RiteAction::Trial,Context);
        if(Advanced!=UnmadeCore::RiteResult::Advanced)
        {
            Chronicle.Restore(Previous);
            Player->RefundRealityStrain(Power.cost);
            ExplainResult(Advanced);return;
        }
    }
    if(!Persist(true))
    {
        Chronicle.Restore(Previous);
        Player->RefundRealityStrain(Power.cost);
        ExplainResult(UnmadeCore::RiteResult::Invalid);return;
    }
    // Runtime effects always follow the durable, validated state transition.
    if(Id==UnmadeCore::RiteId::BorrowedLives ||
       Id==UnmadeCore::RiteId::LegacyForging ||
       Id==UnmadeCore::RiteId::TomorrowDebt ||
       Id==UnmadeCore::RiteId::Oathbinding)
    {
        ActiveAttackBonus=Id==UnmadeCore::RiteId::BorrowedLives ||
            Id==UnmadeCore::RiteId::LegacyForging ||
            Id==UnmadeCore::RiteId::TomorrowDebt ? Power.impact : 0;
        ActiveArmorBonus=Id==UnmadeCore::RiteId::Oathbinding?Power.impact:0;
        BonusExpiresAt=Context.now+Power.duration;
        if(IsValid(Player->GetEquipment()))
            Player->GetEquipment()->SetTemporaryBonuses(ActiveAttackBonus,ActiveArmorBonus);
    }
    Player->ApplyRiteAbility(Id,Power,SelectedLaw);
    if(AUnmadePrototypeHub* Hub=Cast<AUnmadePrototypeHub>(
        UGameplayStatics::GetActorOfClass(GetWorld(),AUnmadePrototypeHub::StaticClass())))
        Hub->ApplyRiteEnvironment(Id,Power.duration,Chronicle.IsMastered(Id));
    Player->ReportRiteWitnessEvent();
    ShowRite();
}

void UUnmadeTenfoldComponent::BreakChosenOath()
{
    if(bSaveRejected)return;
    const auto Before=Chronicle.Snapshot();
    if(!Chronicle.BreakOath()){ExplainResult(UnmadeCore::RiteResult::BrokenOath);return;}
    if(!Persist()){Chronicle.Restore(Before);return;}
    ActiveArmorBonus=0;
    if(AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner()))
    {
        if(IsValid(Player->GetEquipment()))
            Player->GetEquipment()->SetTemporaryBonuses(ActiveAttackBonus,0);
        Player->ReportBrokenOathEvent();
    }
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Red,
        TEXT("OATH BROKEN: the choice persists, and the oath's power is permanently lost."));
}

void UUnmadeTenfoldComponent::RecordLegacyDeed(UnmadeCore::Deed Deed)
{
    if(bSaveRejected)return;
    const auto Before=Chronicle.Snapshot();
    if(Chronicle.RecordDeed(Deed) && !Persist())Chronicle.Restore(Before);
}
void UUnmadeTenfoldComponent::VerifyExploredFrontier(int32 RealmIndex)
{
    if(bSaveRejected || Chronicle.Stage(UnmadeCore::RiteId::Cartography)<2)return;
    const auto Before=Chronicle.Snapshot();
    if(Chronicle.VerifyRoute(RealmIndex,2,true) && !Persist())Chronicle.Restore(Before);
}
void UUnmadeTenfoldComponent::RefreshTemporaryPowers(double CurrentTime)
{
    if(BonusExpiresAt<=0 || CurrentTime<BonusExpiresAt)return;
    BonusExpiresAt=0;
    ActiveAttackBonus=ActiveArmorBonus=0;
    if(AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner()))
        if(IsValid(Player->GetEquipment()))
            Player->GetEquipment()->SetTemporaryBonuses(0,0);
}
void UUnmadeTenfoldComponent::TickComponent(float DeltaSeconds,ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaSeconds,TickType,ThisTickFunction);
    if(bSaveRejected || !GetWorld())return;
    double Now=GetWorld()->GetTimeSeconds();
    int Day=1;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        Now=Hub->GetWorldClockSeconds();
        Day=Hub->GetGameDay();
        break;
    }
    RefreshTemporaryPowers(Now);
    if(Now>LastTickDayCheck+3)
    {
        LastTickDayCheck=Now;
        const auto Before=Chronicle.Snapshot();
        if(Chronicle.PassDay(Day) && !Persist())Chronicle.Restore(Before);
    }
}
