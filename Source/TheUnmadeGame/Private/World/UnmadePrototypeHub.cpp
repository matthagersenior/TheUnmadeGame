#include "World/UnmadePrototypeHub.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Save/UnmadePrototypeSave.h"
#include "Fracture/UnmadeFractureAnchor.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeBossCharacter.h"
#include "Story/UnmadeConflictGate.h"
#include "Story/UnmadeConflictRules.h"
#include "World/UnmadeLoreSite.h"
#include "World/UnmadeSettlementRegistry.h"
#include "Player/UnmadeCharacter.h"
#include "Engine/Engine.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "TimerManager.h"

AUnmadePrototypeHub::AUnmadePrototypeHub()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AUnmadePrototypeHub::BeginPlay()
{
    Super::BeginPlay();
    BuildForPrototype();
    RestoreLivingWorld();
    RefreshCommunityConsequences();
    RefreshAfterlightWorld();
    RefreshRiteWorldFromSave();
    RefreshDistrictMood();
    LastAmbientPhase = Clock.Phase();
    GetWorldTimerManager().SetTimer(GossipTimer, this, &AUnmadePrototypeHub::SpreadLocalRumors, 8.f, true);
    // Persist passage of time at measured intervals, not every rendered frame.
    GetWorldTimerManager().SetTimer(WorldSaveTimer, this, &AUnmadePrototypeHub::SaveLivingWorld, 60.f, true);
}

int32 AUnmadePrototypeHub::ReadStoryChoice() const
{
    const UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"), 0));
    if (!Save || Save->SchemaVersion != 1 || !Save->bHasConflictSnapshot)
        return 0;
    return FMath::Clamp(Save->LocalConflictChoice, 0, 2);
}

void AUnmadePrototypeHub::RestoreLivingWorld()
{
    const UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"), 0));
    if (!Save || Save->SchemaVersion != 1 || !Save->bHasLivingWorldSnapshot) return;

    UnmadeCore::LivingWorldClock RestoredClock;
    UnmadeCore::DiscoveryLedger RestoredDiscoveries;
    UnmadeCore::SettlementVisits RestoredVillages;
    UnmadeCore::RegionalTaskModel RestoredTasks;
    UnmadeCore::FactionChronicle RestoredChronicle;
    UnmadeCore::FrontierJourney RestoredFrontier;
    UnmadeCore::BellwoldAfterlight RestoredAfterlight;
    if (!RestoredClock.Restore(Save->LivingWorldSeconds) ||
        !RestoredDiscoveries.Restore(Save->DiscoveredLoreMask) ||
        !RestoredVillages.Restore(Save->VisitedSettlementsMask) ||
        !RestoredTasks.Restore({Save->BellwoldTaskStage, Save->PaperhavenTaskStage}))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid living world state ignored"));
        return;
    }
    if (Save->bHasFactionChronicle)
    {
        UnmadeCore::FactionSnapshot State;
        if (Save->FactionStages.Num()!=3 || Save->FactionEndings.Num()!=3)
        {
            UE_LOG(LogTemp,Error,TEXT("Invalid faction array size, save not applied"));
            return;
        }
        for(int32 i=0;i<3;++i)
        {
            State.stages[i]=Save->FactionStages[i];
            State.endings[i]=Save->FactionEndings[i];
        }
        if (!RestoredChronicle.Restore(State))
        {
            UE_LOG(LogTemp,Error,TEXT("Invalid faction state rejected"));
            return;
        }
    }
    if(Save->bHasFrontierSnapshot)
    {
        UnmadeCore::FrontierSnapshot State;
        if(Save->FrontierStages.Num()!=2 || Save->FrontierEndings.Num()!=2)
        {
            UE_LOG(LogTemp,Error,TEXT("Invalid frontier save arrays"));
            return;
        }
        State.visits=Save->VisitedFrontierRealms;
        State.discoveries=Save->DiscoveredFrontierClues;
        for(int32 i=0;i<2;++i)
        {
            State.stages[i]=Save->FrontierStages[i];
            State.endings[i]=Save->FrontierEndings[i];
        }
        if(!RestoredFrontier.Restore(State))
        {
            UE_LOG(LogTemp,Error,TEXT("Corrupt frontier progress rejected"));
            return;
        }
    }
    if(Save->bHasAfterlightSnapshot &&
       !RestoredAfterlight.Restore({Save->BellwoldAfterlightStage,
           Save->BellwoldAfterlightApproach,Save->BellwoldAfterlightOutcome}))
    {
        bAfterlightSaveRejected=true; // No other writer may replace corrupt progress.
        UE_LOG(LogTemp,Error,TEXT("Bellwold Afterlight save rejected; old state preserved"));
        return;
    }
    Afterlight=RestoredAfterlight;
    Frontier=RestoredFrontier;
    Chronicle=RestoredChronicle;
    Clock = RestoredClock;
    Discoveries = RestoredDiscoveries;
    VillagesVisited = RestoredVillages;
    RegionalTasks = RestoredTasks;
}

bool AUnmadePrototypeHub::WriteWorldSnapshot()
{
    if(bAfterlightSaveRejected)return false;
    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return false;
    Save->bHasLivingWorldSnapshot = true;
    Save->LivingWorldSeconds = Clock.ElapsedSeconds();
    Save->DiscoveredLoreMask = Discoveries.Snapshot();
    Save->VisitedSettlementsMask = VillagesVisited.Snapshot();
    const auto Story = RegionalTasks.Snapshot();
    Save->BellwoldTaskStage = Story.bellwold;
    Save->PaperhavenTaskStage = Story.paperhaven;
    const auto FactionState=Chronicle.Snapshot();
    Save->FactionStages.Reset();
    Save->FactionEndings.Reset();
    for(int i=0;i<3;++i)
    {
        Save->FactionStages.Add(FactionState.stages[i]);
        Save->FactionEndings.Add(FactionState.endings[i]);
    }
    Save->bHasFactionChronicle=true;
    const auto FrontierState=Frontier.Snapshot();
    Save->bHasFrontierSnapshot=true;
    Save->VisitedFrontierRealms=FrontierState.visits;
    Save->DiscoveredFrontierClues=FrontierState.discoveries;
    Save->FrontierStages.Reset();
    Save->FrontierEndings.Reset();
    for(int32 i=0;i<2;++i)
    {
        Save->FrontierStages.Add(FrontierState.stages[i]);
        Save->FrontierEndings.Add(FrontierState.endings[i]);
    }
    const auto AfterlightState=Afterlight.Snapshot();
    Save->bHasAfterlightSnapshot=true;
    Save->BellwoldAfterlightStage=AfterlightState.stage;
    Save->BellwoldAfterlightApproach=AfterlightState.approach;
    Save->BellwoldAfterlightOutcome=AfterlightState.outcome;
    return UGameplayStatics::SaveGameToSlot(Save, TEXT("UnmadePrototypeNPC"), 0);
}

bool AUnmadePrototypeHub::TryResidentVillageTask(FName ResidentId, bool& bCompleted)
{
    bCompleted = false;
    const FString ResidentName = ResidentId.ToString();
    const FTCHARToUTF8 ResidentUtf8(*ResidentName);
    const UnmadeCore::ResidentSpec* Resident = UnmadeCore::FindResident(ResidentUtf8.Get());
    if (!Resident) return false;
    const auto Previous = RegionalTasks.Snapshot();
    const auto Outcome = RegionalTasks.Converse(Resident->home, ResidentUtf8.Get());
    if (Outcome == UnmadeCore::TaskResult::NoChange) return false;

    if (!WriteWorldSnapshot())
    {
        RegionalTasks.Restore(Previous);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red,
            TEXT("Village story could not be saved. No progress applied."));
        return false;
    }
    RefreshCommunityConsequences();
    bCompleted = Outcome == UnmadeCore::TaskResult::Completed;
    if (GEngine)
    {
        FString Line;
        if (Resident->home == UnmadeCore::SettlementId::Bellwold)
            Line = bCompleted
                ? TEXT("BELLWOLD: the shelter matron accepts the restored lanterns. Refuge light survives the night.")
                : TEXT("BELLWOLD: the lamplighter asks you to carry the shared lanterns to the refuge matron.");
        else
            Line = bCompleted
                ? TEXT("PAPERHAVEN: the registrar preserves the rescued testimony, even though it contradicts the official record.")
                : TEXT("PAPERHAVEN: the copyist entrusts you with a missing testimony. Bring it to the registrar.");
        GEngine->AddOnScreenDebugMessage(-1, 9.f, FColor::Cyan, Line);
    }
    return true;
}

void AUnmadePrototypeHub::TryFactionConversation(FName ResidentId)
{
    if(ResidentId.IsNone())return;
    const FString Id=ResidentId.ToString();
    const FTCHARToUTF8 Utf8(*Id);
    bool bHasEvidence=false;
    for(const auto& Arc:UnmadeCore::FactionArcs)
        if(Arc.first && Arc.second && Arc.third)
        {
            if(FCStringAnsi::Strcmp(Utf8.Get(),Arc.third)!=0)continue;
            bHasEvidence=Arc.faction==UnmadeCore::Faction::Refuge
                ? RegionalTasks.Progress(UnmadeCore::SettlementId::Bellwold)==UnmadeCore::TaskProgress::Completed
                : Arc.faction==UnmadeCore::Faction::Archive
                  ? RegionalTasks.Progress(UnmadeCore::SettlementId::Paperhaven)==UnmadeCore::TaskProgress::Completed
                  : VillagesVisited.Count()==3;
        }
    const auto Before=Chronicle.Snapshot();
    const auto Result=Chronicle.Converse(Utf8.Get(),bHasEvidence);
    if(Result==UnmadeCore::FactionResult::Advanced ||
       Result==UnmadeCore::FactionResult::ChoiceRequired)
    {
        if(Result==UnmadeCore::FactionResult::Advanced ||
           Chronicle.Snapshot().stages!=Before.stages)
        {
            if(!WriteWorldSnapshot())
            {
                Chronicle.Restore(Before);
                return;
            }
        }
    }
    if(GEngine && Result!=UnmadeCore::FactionResult::NoChange)
    {
        const TCHAR* Notice=Result==UnmadeCore::FactionResult::NeedsEvidence
            ? TEXT("FACTION: complete this settlement's local task (or visit all three villages) first.")
            : Result==UnmadeCore::FactionResult::ChoiceRequired
            ? TEXT("FACTION DECISION: near this representative, F7 commits to solidarity, F8 to open truth. This cannot be undone.")
            : TEXT("FACTION: another witness adds their part to the chronicle. Speak to the next representative.");
        GEngine->AddOnScreenDebugMessage(-1,9.f,FColor::Cyan,Notice);
    }
}

bool AUnmadePrototypeHub::ResolveNearbyFaction(UnmadeCore::FactionEnding Outcome)
{
    if(!GetWorld() || Outcome==UnmadeCore::FactionEnding::Unresolved)return false;
    const ACharacter* Player=UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
    if(!IsValid(Player))return false;
    for(const auto& Arc:UnmadeCore::FactionArcs)
    {
        if(Chronicle.Stage(Arc.faction)!=3 ||
           Chronicle.Ending(Arc.faction)!=UnmadeCore::FactionEnding::Unresolved)
            continue;
        for(TActorIterator<AUnmadeNpcCharacter> It(GetWorld());It;++It)
        {
            if(It->GetStableId()!=FName(UTF8_TO_TCHAR(Arc.third)))continue;
            if(FVector::DistSquared(Player->GetActorLocation(),It->GetActorLocation())>FMath::Square(390.f))
                continue;
            const auto Before=Chronicle.Snapshot();
            if(Chronicle.Decide(Arc.faction,Outcome)!=UnmadeCore::FactionResult::Resolved)
                return false;
            if(!WriteWorldSnapshot())
            {
                Chronicle.Restore(Before);
                return false;
            }
            RefreshCommunityConsequences();
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,9.f,FColor::Yellow,
                FString::Printf(TEXT("FACTION ARC RESOLVED: %s. The town remembers your choice."),
                    UTF8_TO_TCHAR(Arc.name)));
            return true;
        }
    }
    return false;
}

FString AUnmadePrototypeHub::GetCurrentRealmName(FVector Position) const
{
    for(const auto& Outpost:UnmadeCore::FrontierOutposts)
        if(FVector::DistSquared2D(Position,FVector(Outpost.centerX,Outpost.centerY,Position.Z))
           <=FMath::Square(2900.f))
            return FString(UTF8_TO_TCHAR(Outpost.settlementName));
    return TEXT("The Threefold Reach");
}

bool AUnmadePrototypeHub::TryTravelFrontier(AUnmadeCharacter* Player)
{
    if(!IsValid(Player) || !GetWorld())return false;
    const FVector P=Player->GetActorLocation();
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        if(FVector::DistSquared(P,It->GetActorLocation())>FMath::Square(390.f))continue;
        FVector Destination;
        UnmadeCore::Realm Next=UnmadeCore::Realm::ThreefoldReach;
        if(It->ActorHasTag(FName("Gateway.ToRain")))
        {
            Next=UnmadeCore::Realm::WidowedRain;
            Destination=FVector(0,-50000,125);
        }
        else if(It->ActorHasTag(FName("Gateway.ToHearth")))
        {
            Next=UnmadeCore::Realm::HearthBeneath;
            Destination=FVector(0,50000,125);
        }
        else if(It->ActorHasTag(FName("Gateway.ReturnRain")))
            Destination=FVector(0,-1250,125);
        else if(It->ActorHasTag(FName("Gateway.ReturnHearth")))
            Destination=FVector(0,1250,125);
        else continue;

        const auto Before=Frontier.Snapshot();
        const bool bNew=Frontier.Visit(Next)==UnmadeCore::FrontierEvent::Advanced;
        if(bNew && !WriteWorldSnapshot())
        {
            Frontier.Restore(Before);
            return false;
        }
        if(!Player->SetActorLocation(Destination,false,nullptr,ETeleportType::TeleportPhysics))
        {
            Frontier.Restore(Before);
            if(bNew)WriteWorldSnapshot();
            return false;
        }
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Cyan,
            FString::Printf(TEXT("CROSSING OPEN: %s. Speak to local witnesses and examine the clue."),
                *GetCurrentRealmName(Destination)));
        return true;
    }
    return false;
}

bool AUnmadePrototypeHub::InspectFrontierClue(AUnmadeCharacter* Player)
{
    if(!IsValid(Player) || !GetWorld())return false;
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        if(FVector::DistSquared(Player->GetActorLocation(),It->GetActorLocation())>FMath::Square(310.f))
            continue;
        UnmadeCore::Realm At=UnmadeCore::Realm::Count;
        if(It->ActorHasTag(FName("Frontier.Clue.Rain")))
            At=UnmadeCore::Realm::WidowedRain;
        else if(It->ActorHasTag(FName("Frontier.Clue.Hearth")))
            At=UnmadeCore::Realm::HearthBeneath;
        if(At==UnmadeCore::Realm::Count)continue;
        const auto Before=Frontier.Snapshot();
        const bool bNew=Frontier.FindClue(At)==UnmadeCore::FrontierEvent::Advanced;
        if(bNew && !WriteWorldSnapshot())
        {
            Frontier.Restore(Before);
            return false;
        }
        const auto* Spec=UnmadeCore::FindFrontier(At);
        if(GEngine && Spec)GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Cyan,
            FString::Printf(TEXT("REALM EVIDENCE: %s"),UTF8_TO_TCHAR(Spec->mystery)));
        return true;
    }
    return false;
}

void AUnmadePrototypeHub::TryFrontierConversation(FName ResidentId)
{
    if(ResidentId.IsNone())return;
    const FString Id=ResidentId.ToString();
    const FTCHARToUTF8 Utf8(*Id);
    if(!UnmadeCore::FindFrontierResident(Utf8.Get()))return;
    const auto Before=Frontier.Snapshot();
    const auto Event=Frontier.Converse(Utf8.Get());
    if(Event==UnmadeCore::FrontierEvent::Advanced ||
       Event==UnmadeCore::FrontierEvent::FinalChoice)
    {
        if(Frontier.Snapshot().stages!=Before.stages && !WriteWorldSnapshot())
        {
            Frontier.Restore(Before);
            return;
        }
    }
    if(GEngine && Event!=UnmadeCore::FrontierEvent::NoChange)
    {
        const TCHAR* Notice=Event==UnmadeCore::FrontierEvent::NeedClue
            ? TEXT("REALM STORY: inspect the local evidence before returning to the third witness.")
            : Event==UnmadeCore::FrontierEvent::FinalChoice
            ? TEXT("REALM CHOICE: F7 stands with the people; F8 releases the disputed truth.")
            : TEXT("REALM STORY: the next witness waits elsewhere in this settlement.");
        GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Cyan,Notice);
    }
}

bool AUnmadePrototypeHub::ResolveNearbyFrontier(int32 Ending)
{
    if(!GetWorld() || (Ending!=1 && Ending!=2))return false;
    const ACharacter* Player=UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
    if(!IsValid(Player))return false;
    for(const auto& Outpost:UnmadeCore::FrontierOutposts)
    {
        const int idx=UnmadeCore::FrontierIndex(Outpost.realm);
        if(idx<0 || Frontier.Snapshot().stages[idx]!=3 ||
           Frontier.Snapshot().endings[idx]!=0)continue;
        const FName FinalId(Outpost.realm==UnmadeCore::Realm::WidowedRain
            ? TEXT("npc.saltwake.harborwarden.001")
            : TEXT("npc.cinderhold.emberwarden.001"));
        for(TActorIterator<AUnmadeNpcCharacter> It(GetWorld());It;++It)
        {
            if(It->GetStableId()!=FinalId ||
               FVector::DistSquared(Player->GetActorLocation(),It->GetActorLocation())>FMath::Square(390.f))
                continue;
            const auto Before=Frontier.Snapshot();
            if(Frontier.Resolve(Outpost.realm,Ending)!=UnmadeCore::FrontierEvent::Resolved)
                return false;
            if(!WriteWorldSnapshot())
            {
                Frontier.Restore(Before);
                return false;
            }
            RefreshCommunityConsequences();
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,9.f,FColor::Yellow,
                FString::Printf(TEXT("REALM STORY RESOLVED: %s."),
                    UTF8_TO_TCHAR(Outpost.settlementName)));
            return true;
        }
    }
    return false;
}

void AUnmadePrototypeHub::BuildBellwoldAfterlight()
{
    // Separate physical evidence and mutually exclusive final landmarks.
    SpawnBlock(FVector(-19350,1100,90),FVector(.55,.42,1.6),
        FName("Bellwold.Afterlight.Relief"));
    SpawnBlock(FVector(-18500,-1450,90),FVector(.55,.42,1.6),
        FName("Bellwold.Afterlight.Census"));
    SpawnBlock(FVector(-19100,1490,155),FVector(3.6,1.15,3.1),
        FName("Bellwold.Afterlight.SafeWard"));
    SpawnBlock(FVector(-18350,-1360,195),FVector(1.5,1.4,3.9),
        FName("Bellwold.Afterlight.OpenCensus"));
    SetRiteWorldActorState(FName("Bellwold.Afterlight.SafeWard"),false);
    SetRiteWorldActorState(FName("Bellwold.Afterlight.OpenCensus"),false);
}
void AUnmadePrototypeHub::RefreshAfterlightWorld()
{
    const auto Outcome=Afterlight.Outcome();
    SetRiteWorldActorState(FName("Bellwold.Afterlight.SafeWard"),
        Outcome==UnmadeCore::AfterlightChoice::Relief);
    SetRiteWorldActorState(FName("Bellwold.Afterlight.OpenCensus"),
        Outcome==UnmadeCore::AfterlightChoice::Revelation);
}
bool AUnmadePrototypeHub::TryAfterlightConversation(FName ResidentId)
{
    if(ResidentId.IsNone() || bAfterlightSaveRejected)return false;
    const FString Identity=ResidentId.ToString();
    const FTCHARToUTF8 Utf8(*Identity);
    const auto Before=Afterlight.Snapshot();
    auto Result=UnmadeCore::AfterlightResult::NoChange;
    if(Afterlight.Stage()==0)
        Result=Afterlight.Begin(
            static_cast<int>(RegionalTasks.Progress(UnmadeCore::SettlementId::Bellwold)),
            static_cast<int>(Chronicle.Ending(UnmadeCore::Faction::Refuge)),Utf8.Get());
    else Result=Afterlight.Converse(Utf8.Get());
    if(Result!=UnmadeCore::AfterlightResult::Started &&
       Result!=UnmadeCore::AfterlightResult::Witnessed)return false;
    if(!WriteWorldSnapshot())
    {
        Afterlight.Restore(Before);
        return false;
    }
    if(GEngine)
    {
        const TCHAR* Line=Result==UnmadeCore::AfterlightResult::Started
            ? TEXT("AFTERLIGHT: Hessa needs you back after the Refuge Compact. The second night draws near: examine the shelter's relief cache or the lost-name census.")
            : TEXT("AFTERLIGHT: testimony verified. Return to Hessa. F7 protects those without shelter; F8 publishes the lost names. Your evidence determines what can be chosen.");
        GEngine->AddOnScreenDebugMessage(-1,12.f,FColor::Cyan,Line);
    }
    return true;
}
bool AUnmadePrototypeHub::InspectAfterlightClue(AUnmadeCharacter* Player)
{
    if(!IsValid(Player) || !GetWorld() || bAfterlightSaveRejected ||
       (Afterlight.Stage()!=1 && Afterlight.Stage()!=2))return false;
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        if(FVector::DistSquared(Player->GetActorLocation(),It->GetActorLocation())>
            FMath::Square(310.f))continue;
        UnmadeCore::AfterlightChoice Approach=UnmadeCore::AfterlightChoice::None;
        if(It->ActorHasTag(FName("Bellwold.Afterlight.Relief")))
            Approach=UnmadeCore::AfterlightChoice::Relief;
        else if(It->ActorHasTag(FName("Bellwold.Afterlight.Census")))
            Approach=UnmadeCore::AfterlightChoice::Revelation;
        if(Approach==UnmadeCore::AfterlightChoice::None)continue;
        FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeAfterlightSight),false);
        Sight.AddIgnoredActor(Player);
        Sight.AddIgnoredActor(*It);
        if(GetWorld()->LineTraceTestByChannel(Player->GetActorLocation()+FVector(0,0,50),
            It->GetActorLocation()+FVector(0,0,50),ECC_Visibility,Sight))continue;
        const auto Before=Afterlight.Snapshot();
        const FString Tag=Approach==UnmadeCore::AfterlightChoice::Relief
            ? TEXT("Bellwold.Afterlight.Relief"):TEXT("Bellwold.Afterlight.Census");
        const FTCHARToUTF8 Utf8(*Tag);
        if(Afterlight.Inspect(Approach,Utf8.Get())!=
           UnmadeCore::AfterlightResult::EvidenceFound)return false;
        if(!WriteWorldSnapshot())
        {
            Afterlight.Restore(Before);
            return false;
        }
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,13.f,FColor::Cyan,
            Approach==UnmadeCore::AfterlightChoice::Relief
            ? TEXT("CACHE: the lamps could save the unregistered families. Ask Sorin the healer what this cold will cost.")
            : TEXT("CENSUS: these erased names prove the shelter's history was altered. Ask Ivera the tutor what the children remember."));
        return true;
    }
    return false;
}
bool AUnmadePrototypeHub::ResolveNearbyAfterlight(int32 Choice)
{
    if(!GetWorld() || bAfterlightSaveRejected || Afterlight.Stage()!=3 ||
       (Choice!=1 && Choice!=2))return false;
    const ACharacter* Player=UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
    if(!IsValid(Player))return false;
    for(TActorIterator<AUnmadeNpcCharacter> It(GetWorld());It;++It)
    {
        if(It->GetStableId()!=FName("npc.bellwold.matron.001") ||
           FVector::DistSquared(Player->GetActorLocation(),It->GetActorLocation())>
           FMath::Square(390.f))continue;
        const auto Before=Afterlight.Snapshot();
        const auto Result=Afterlight.Resolve(static_cast<UnmadeCore::AfterlightChoice>(Choice));
        if(Result==UnmadeCore::AfterlightResult::WrongEvidence)
        {
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Yellow,
                TEXT("The choice needs the other evidence and witness. Inspect that path before you commit."));
            return false;
        }
        if(Result!=UnmadeCore::AfterlightResult::Resolved)return false;
        if(!WriteWorldSnapshot())
        {
            Afterlight.Restore(Before);
            return false;
        }
        RefreshAfterlightWorld();
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,12.f,FColor::Green,
            FString::Printf(TEXT("THE SECOND NIGHT: %s"),
                UTF8_TO_TCHAR(Afterlight.Effect().description)));
        return true;
    }
    return false;
}

void AUnmadePrototypeHub::BuildFrontiers()
{
    SpawnBlock(FVector(0,-1950,130),FVector(.45,.45,2.4),FName("Gateway.ToRain"));
    SpawnBlock(FVector(0,1950,130),FVector(.45,.45,2.4),FName("Gateway.ToHearth"));
    for(const auto& Outpost:UnmadeCore::FrontierOutposts)
    {
        const FVector Origin(Outpost.centerX,Outpost.centerY,0);
        const bool bRain=Outpost.realm==UnmadeCore::Realm::WidowedRain;
        SpawnBlock(Origin+FVector(0,0,-50),FVector(52,52,1),
            FName(bRain?TEXT("Realm.Rain.Ground"):TEXT("Realm.Hearth.Ground")));
        if(bRain)
        {
            SpawnBlock(Origin+FVector(-1500,-900,130),FVector(7,.65,2.6),FName("Rain.BeachHull"));
            SpawnBlock(Origin+FVector(1300,850,225),FVector(.65,2.5,4.5),FName("Rain.Lighthouse"));
            SpawnBlock(Origin+FVector(-100,1550,125),FVector(4,1.2,2.5),FName("Rain.Rainhouse"));
            SpawnBlock(Origin+FVector(1480,-1500,160),FVector(2.8,.8,3.2),FName("Rain.SaltVault"));
        }
        else
        {
            SpawnBlock(Origin+FVector(-1420,-1300,380),FVector(1.4,4,7.6),FName("Hearth.DeepWall"));
            SpawnBlock(Origin+FVector(1550,1000,290),FVector(1.2,3,5.8),FName("Hearth.StoneArch"));
            SpawnBlock(Origin+FVector(-100,1480,150),FVector(3.3,2.2,3),FName("Hearth.WarmCommons"));
            SpawnBlock(Origin+FVector(1150,-1550,150),FVector(2.2,2,3),FName("Hearth.ForgeChamber"));
        }
        SpawnBlock(Origin+FVector(1550,0,130),FVector(.45,.45,2.4),
            FName(bRain?TEXT("Gateway.ReturnRain"):TEXT("Gateway.ReturnHearth")));
        SpawnBlock(Origin+FVector(-400,0,110),FVector(.65,.45,2.1),
            FName(bRain?TEXT("Frontier.Clue.Rain"):TEXT("Frontier.Clue.Hearth")));
    }
    for(const auto& Resident:UnmadeCore::FrontierResidents)
    {
        const auto* Outpost=UnmadeCore::FindFrontier(Resident.home);
        if(!Outpost)continue;
        if(AUnmadeNpcCharacter* Npc=GetWorld()->SpawnActor<AUnmadeNpcCharacter>(
            FVector(Outpost->centerX+Resident.localX,
                    Outpost->centerY+Resident.localY,95),FRotator::ZeroRotator))
            Npc->ConfigureFrontier(Resident);
    }
}

void AUnmadePrototypeHub::SaveLivingWorld()
{
    if (!WriteWorldSnapshot())
        UE_LOG(LogTemp, Warning, TEXT("Living-world progress could not be saved"));
}

void AUnmadePrototypeHub::RefreshDistrictMood()
{
    if (!GetWorld()) return;
    const auto Phase = Clock.Phase();
    for (TActorIterator<AUnmadeLoreSite> It(GetWorld()); It; ++It)
        It->SetPhase(Phase);

    if (IsValid(Sunlight) && IsValid(Sunlight->GetComponent()))
    {
        const bool bNight = Phase == UnmadeCore::DayPhase::Night;
        const bool bTransition = Phase == UnmadeCore::DayPhase::Dawn ||
                                 Phase == UnmadeCore::DayPhase::Dusk;
        Sunlight->GetComponent()->SetIntensity(bNight ? 0.12f : bTransition ? 2.f : 9.f);
        Sunlight->GetComponent()->SetLightColor(bNight
            ? FLinearColor(0.36f, 0.45f, 0.8f)
            : bTransition ? FLinearColor(1.0f, 0.53f, 0.31f)
                          : FLinearColor(1.0f, 0.95f, 0.82f), false);
        Sunlight->SetActorRotation(FRotator(bNight ? 25.f : bTransition ? -12.f : -50.f, 0.f, 0.f));
    }
}

bool AUnmadePrototypeHub::InspectSite(AUnmadeLoreSite* Site)
{
    if (!IsValid(Site) || Site->GetWorld() != GetWorld() || !GetWorld()) return false;
    const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!IsValid(Player) ||
        FVector::DistSquared(Player->GetActorLocation(), Site->GetActorLocation()) > FMath::Square(295.f))
        return false;

    const int32 OldMask = Discoveries.Snapshot();
    const bool bNew = Discoveries.Discover(Site->GetDistrict());
    if (bNew && !WriteWorldSnapshot())
    {
        Discoveries.Restore(OldMask);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Red,
            TEXT("Exploration progress could not be saved."));
        return false;
    }
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 11.f, FColor::Cyan,
            Site->GetInspectionText(Clock.Phase(), ReadStoryChoice()));
        if (bNew)
            GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Yellow,
                FString::Printf(TEXT("LANDMARK DISCOVERED: %d of 6."), Discoveries.Count()));
    }
    return true;
}

void AUnmadePrototypeHub::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!Clock.Advance(DeltaSeconds) || !GetWorld()) return;
    // Unmastered bridges and overlapping histories are temporary physical
    // states. World mastering only stabilizes explicitly allowed structures.
    for(auto It=TemporaryRiteWorldEffects.CreateIterator();It;++It)
    {
        if(Clock.ElapsedSeconds()<It.Value())continue;
        SetRiteWorldActorState(It.Key(),false);
        It.RemoveCurrent();
    }
    const auto Phase = Clock.Phase();
    const bool bPhaseChanged = Phase != LastAmbientPhase;
    if (bPhaseChanged)
    {
        LastAmbientPhase = Phase;
        RefreshDistrictMood();
        if (GEngine)
        {
            const TCHAR* PhaseName = Phase == UnmadeCore::DayPhase::Dawn ? TEXT("dawn")
                : Phase == UnmadeCore::DayPhase::Day ? TEXT("day")
                : Phase == UnmadeCore::DayPhase::Dusk ? TEXT("dusk") : TEXT("night");
            GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Purple,
                FString::Printf(TEXT("THE SETTLEMENT: %s settles across the district."), PhaseName));
        }
    }

    const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!IsValid(Player)) return;

    const FVector Position = Player->GetActorLocation();
    const UnmadeCore::SettlementId Village = UnmadeCore::SettlementAt(Position.X, Position.Y);
    if (Village != LastVisitedVillage)
    {
        LastVisitedVillage = Village;
        if (const auto* VillageInfo = UnmadeCore::FindSettlement(Village))
        {
            const int32 PreviouslyVisited = VillagesVisited.Snapshot();
            const bool bNewVillage = VillagesVisited.Visit(Village);
            if (bNewVillage)
            {
                if (!WriteWorldSnapshot())
                    VillagesVisited.Restore(PreviouslyVisited);
                else if (AUnmadeCharacter* UnmadePlayer = Cast<AUnmadeCharacter>(
                    UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
                    UnmadePlayer->ReconcileEarnedRewards();
            }
            if (GEngine)
            {
                const TCHAR* Line = (Phase == UnmadeCore::DayPhase::Night ||
                                     Phase == UnmadeCore::DayPhase::Dusk)
                    ? UTF8_TO_TCHAR(VillageInfo->night)
                    : UTF8_TO_TCHAR(VillageInfo->welcome);
                GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Yellow,
                    FString::Printf(TEXT("ARRIVED: %s | %s"),
                        UTF8_TO_TCHAR(VillageInfo->name), Line));
            }
        }
        else if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver,
                TEXT("ON THE ROAD: Two settlements wait beyond the horizon."));
        }
    }

    AUnmadeLoreSite* Near = nullptr;
    double BestSq = FMath::Square(625.0);
    for (TActorIterator<AUnmadeLoreSite> It(GetWorld()); It; ++It)
    {
        const double DistSq = FVector::DistSquared2D(Player->GetActorLocation(), It->GetActorLocation());
        if (DistSq < BestSq) { Near = *It; BestSq = DistSq; }
    }
    if (!Near)
    {
        LastAmbientSite = NAME_None; // Re-entry can trigger a new local sensory cue.
    }
    else if (LastAmbientSite != Near->GetSiteId() || bPhaseChanged)
    {
        LastAmbientSite = Near->GetSiteId();
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Cyan,
            Near->GetAmbientText(Phase, ReadStoryChoice()));
    }
}

void AUnmadePrototypeHub::SpawnLoreSite(UnmadeCore::District District, FName SiteId,
    const TCHAR* Name, const TCHAR* DayLine, const TCHAR* NightLine, FVector Position)
{
    if (!GetWorld()) return;
    if (AUnmadeLoreSite* Site = GetWorld()->SpawnActor<AUnmadeLoreSite>(Position, FRotator::ZeroRotator))
        Site->Configure(District, SiteId, FString(Name), FString(DayLine), FString(NightLine));
}

void AUnmadePrototypeHub::SpawnBlock(FVector Center, FVector Scale, FName Label)
{
    UWorld* World = GetWorld();
    UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (!World || !Cube) return;
    AStaticMeshActor* Block = World->SpawnActor<AStaticMeshActor>(Center, FRotator::ZeroRotator);
    if (!Block) return;
    Block->Tags.AddUnique(Label);
    UStaticMeshComponent* Mesh = Block->GetStaticMeshComponent();
    Mesh->SetMobility(EComponentMobility::Movable);
    Mesh->SetStaticMesh(Cube);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Block->SetActorScale3D(Scale);
}

void AUnmadePrototypeHub::SpawnCitizen(const UnmadeCore::ResidentSpec& Resident)
{
    if (!GetWorld()) return;
    const UnmadeCore::Vec2 Position = UnmadeCore::ResidentWorldPosition(Resident);
    if (AUnmadeNpcCharacter* Citizen = GetWorld()->SpawnActor<AUnmadeNpcCharacter>(
        FVector(Position.x, Position.y, 95.0), FRotator::ZeroRotator))
    {
        Citizen->ConfigureIdentity(
            FName(UTF8_TO_TCHAR(Resident.id)), FString(UTF8_TO_TCHAR(Resident.name)),
            Resident.role, Resident.temperament, Resident.home,
            FString(UTF8_TO_TCHAR(Resident.authoredLine)));
    }
}

FString AUnmadePrototypeHub::GetCurrentVillageName() const
{
    const ACharacter* Player = GetWorld() ? UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) : nullptr;
    if (!IsValid(Player)) return TEXT("unknown");
    const FVector Position = Player->GetActorLocation();
    const auto* Village = UnmadeCore::FindSettlement(
        UnmadeCore::SettlementAt(Position.X, Position.Y));
    return Village ? FString(UTF8_TO_TCHAR(Village->name)) : FString(TEXT("the open road"));
}

void AUnmadePrototypeHub::BuildVillages()
{
    // Individual traversable village floors + broad uninterrupted roads.
    // 18,000 cm (180 m) separates each village. Built-in cubes only.
    SpawnBlock(FVector(-9000, 0, -50), FVector(180, 7, 1), FName("Route.WestCauseway"));
    SpawnBlock(FVector(9000, 0, -50), FVector(180, 7, 1), FName("Route.EastCauseway"));
    for (const auto& Village : UnmadeCore::Settlements)
    {
        if (Village.id == UnmadeCore::SettlementId::Crossings) continue;
        const FVector Base(Village.x, Village.y, 0);
        const FName Prefix = Village.id == UnmadeCore::SettlementId::Bellwold
            ? FName("Village.Bellwold") : FName("Village.Paperhaven");
        SpawnBlock(Base + FVector(0, 0, -50), FVector(52, 52, 1), Prefix);
        // Buildings deliberately avoid the central lane and resident arrival markers.
        SpawnBlock(Base + FVector(-1100, -1030, 155), FVector(3.4, 2.5, 3.1), FName("Village.Commons"));
        SpawnBlock(Base + FVector(1100, -1060, 130), FVector(2.7, 2.7, 2.6), FName("Village.Trades"));
        SpawnBlock(Base + FVector(-1110, 1150, 160), FVector(2.8, 3.0, 3.2), FName("Village.Housing"));
        SpawnBlock(Base + FVector(1160, 1210, 135), FVector(2.5, 2.6, 2.7), FName("Village.Watchpost"));
        if (Village.id == UnmadeCore::SettlementId::Bellwold)
        {
            // Bells and a common shelter establish its refuge character.
            SpawnBlock(Base + FVector(-1640, 0, 430), FVector(1.2, 1.2, 8.6), FName("Bellwold.Belltower"));
            SpawnBlock(Base + FVector(180, 1550, 190), FVector(4.5, 3.1, 3.8), FName("Bellwold.Refuge"));
            SpawnBlock(Base + FVector(1750, -550, 115), FVector(2.0, 3.0, 2.3), FName("Bellwold.Workshop"));
        }
        else
        {
            // Taller registry tower, archive galleries and paper storehouses.
            SpawnBlock(Base + FVector(1570, -150, 580), FVector(1.5, 1.5, 11.6), FName("Paperhaven.ArchiveTower"));
            SpawnBlock(Base + FVector(-150, 1620, 210), FVector(5.0, 2.9, 4.2), FName("Paperhaven.Registry"));
            SpawnBlock(Base + FVector(-1730, -640, 120), FVector(2.0, 3.0, 2.4), FName("Paperhaven.Scriptorium"));
        }
    }
}

void AUnmadePrototypeHub::BuildCommunityConsequences()
{
    // Source graybox: each community has three exclusive physical landmarks:
    // restoration works, a freely shared institution, or a public testimony.
    const FVector Centers[5]={
        FVector(0,0,0),FVector(-18000,0,0),FVector(18000,0,0),
        FVector(0,-50000,0),FVector(0,50000,0)
    };
    for(int32 i=0;i<5;++i)
    {
        for(int32 state=1;state<4;++state)
        {
            const FVector Position=Centers[i]+FVector(
                -1050.f+state*510.f,1650.f,190.f);
            const FVector Scale=state==1?FVector(2.4,1,3.8):
                state==2?FVector(3.4,1.2,3.8):FVector(.75,3.5,3.8);
            const FName Tag(*FString::Printf(TEXT("Community.%d.%d"),i,state));
            SpawnBlock(Position,Scale,Tag);
            SetRiteWorldActorState(Tag,false);
        }
    }
}

void AUnmadePrototypeHub::RefreshCommunityConsequences()
{
    UnmadeCore::ConsequenceFacts Facts;
    Facts.crossingDecision=ReadStoryChoice();
    Facts.local=RegionalTasks.Snapshot();
    Facts.factionEndings=Chronicle.Snapshot().endings;
    Facts.frontierEndings=Frontier.Snapshot().endings;
    if(!UnmadeCore::ValidFacts(Facts))
    {
        UE_LOG(LogTemp,Warning,TEXT("Invalid community source facts; retaining previous environment"));
        return;
    }
    const auto Next=UnmadeCore::EvaluateAllCommunities(Facts);
    for(int32 i=0;i<5;++i)
    {
        if(Next[i].state==CommunityEffects[i].state)continue;
        for(int32 State=1;State<=3;++State)
        {
            const FName Tag(*FString::Printf(TEXT("Community.%d.%d"),i,State));
            SetRiteWorldActorState(Tag,static_cast<int32>(Next[i].state)==State);
        }
        if(GEngine && Next[i].state!=UnmadeCore::CommunityState::Uncertain)
            GEngine->AddOnScreenDebugMessage(-1,9.f,FColor::Cyan,
                FString::Printf(TEXT("WORLD CHANGED: %s — %s"),
                    UTF8_TO_TCHAR(UnmadeCore::CommunityNames[i]),
                    UTF8_TO_TCHAR(Next[i].visibleChange)));
    }
    CommunityEffects=Next;
}

UnmadeCore::CommunityConsequence AUnmadePrototypeHub::GetCommunityOutcome(
    UnmadeCore::Community Id) const
{
    const int32 Index=static_cast<int32>(Id);
    return Index>=0 && Index<5?CommunityEffects[Index]:
        UnmadeCore::CommunityConsequence{};
}

FString AUnmadePrototypeHub::DescribeCommunityAt(FVector Position) const
{
    int32 Index=-1;
    const auto Village=UnmadeCore::SettlementAt(Position.X,Position.Y);
    if(Village!=UnmadeCore::SettlementId::None)
        Index=static_cast<int32>(Village);
    else if(FVector::DistSquared2D(Position,FVector(0,-50000,Position.Z))
        <FMath::Square(2850.f))Index=3;
    else if(FVector::DistSquared2D(Position,FVector(0,50000,Position.Z))
        <FMath::Square(2850.f))Index=4;
    return Index>=0
        ? FString(UTF8_TO_TCHAR(CommunityEffects[Index].visibleChange))
        : TEXT("The unclaimed road carries no settlement law.");
}

void AUnmadePrototypeHub::SetRiteWorldActorState(FName Tag,bool bEnabled)
{
    if(!GetWorld())return;
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        if(!It->ActorHasTag(Tag))continue;
        It->SetActorHiddenInGame(!bEnabled);
        It->SetActorEnableCollision(bEnabled);
    }
}

void AUnmadePrototypeHub::RefreshRiteWorldFromSave()
{
    const UUnmadePrototypeSave* Save=Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"),0));
    if(!Save || !Save->bHasTenfoldChronicle || Save->RiteStages.Num()!=10)return;
    if(Save->RiteStages[1]==5)
        SetRiteWorldActorState(FName("Rite.WitnessBridge"),true);
    if(Save->RiteStages[4]==5)
        SetRiteWorldActorState(FName("Rite.CommonCauseway"),true);
    if(Save->RiteStages[7]==5 && Save->RiteChoices.Num()==10 &&
       Save->RiteChoices[7]==2)
        SetRiteWorldActorState(FName("Rite.ParadoxIntact"),true);
    if(Save->VerifiedRoadBits>0)
        SetRiteWorldActorState(FName("Rite.MapRoute"),true);
}

void AUnmadePrototypeHub::ApplyRiteEnvironment(UnmadeCore::RiteId Id,
    double Duration,bool Mastered)
{
    if(!GetWorld() || !FMath::IsFinite(Duration) || Duration<=0)return;
    FName Tag;
    switch(Id)
    {
    case UnmadeCore::RiteId::Witnesscraft:Tag=FName("Rite.WitnessBridge");break;
    case UnmadeCore::RiteId::LivingRoads:Tag=FName("Rite.CommonCauseway");break;
    case UnmadeCore::RiteId::ParadoxConvergence:Tag=FName("Rite.ParadoxIntact");break;
    case UnmadeCore::RiteId::Cartography:Tag=FName("Rite.MapRoute");break;
    default:return;
    }
    SetRiteWorldActorState(Tag,true);
    bool bPermanent=Mastered && Id!=UnmadeCore::RiteId::ParadoxConvergence;
    if(Mastered && Id==UnmadeCore::RiteId::ParadoxConvergence)
    {
        const UUnmadePrototypeSave* Save=Cast<UUnmadePrototypeSave>(
            UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"),0));
        bPermanent=Save && Save->bHasTenfoldChronicle &&
            Save->RiteChoices.Num()==10 && Save->RiteChoices[7]==2;
    }
    if(bPermanent)TemporaryRiteWorldEffects.Remove(Tag);
    else TemporaryRiteWorldEffects.Add(Tag,Clock.ElapsedSeconds()+Duration);
}

void AUnmadePrototypeHub::BuildForPrototype()
{
    if (bBuilt || !GetWorld()) return;
    bBuilt = true;

    // Engine-native directional light: temporary global time-of-day illumination.
    Sunlight = GetWorld()->SpawnActor<ADirectionalLight>(
        FVector(0, 0, 1200), FRotator(-50.f, 0.f, 0.f));

    // All geometry uses Unreal's primitive cube assets: temporary untextured blockout.
    // Walking ground: top at Z=0; center at -50 with 1*100 cm tall mesh.
    SpawnBlock(FVector(0, 0, -50), FVector(52, 52, 1), FName("Hub.Ground"));
    BuildVillages();
    BuildFrontiers();
    BuildCommunityConsequences();
    BuildBellwoldAfterlight();
    // Ten inscriptions across five communities. Each marker is world-space,
    // independently discoverable, and checked for distance and visibility.
    const FVector RitualSites[10]={
        FVector(-1800,-1020,90), // Bellgrave: the Law of the Bell
        FVector(-1710,680,90),  // Echo Well: Witnesscraft
        FVector(1300,1260,90),  // Paper Orchard: Borrowed Lives
        FVector(-17440,-710,90),// Bellwold Forge: Legacy
        FVector(1850,390,90),   // Silent Mile: Common Roads
        FVector(1170,-1170,90), // Debt Market: Borrowed Dawn
        FVector(-18000,-1720,90),// Bellwold Ossuary: boss mercy
        FVector(17880,-1250,90),// Paperhaven Scribe: dual histories
        FVector(-17330,540,90),// Bellwold Refuge: Oath
        FVector(-350,-49600,90)// Saltwake Harbor: Unreliable Map
    };
    for(int32 Index=0;Index<10;++Index)
        SpawnBlock(RitualSites[Index],FVector(.48,.48,1.8),
            FName(*FString::Printf(TEXT("Rite.Site.%d"),Index)));
    // Alternating reality props: nondefault state is invisible/noncolliding
    // until a player performs the actual rite, not until NPC dialogue says so.
    SpawnBlock(FVector(-1250,270,-50),FVector(3.5,1.4,1),FName("Rite.WitnessBridge"));
    SpawnBlock(FVector(1720,870,-50),FVector(2.2,1.4,1),FName("Rite.CommonCauseway"));
    SpawnBlock(FVector(17520,-1440,330),FVector(2.1,2.1,6.6),FName("Rite.ParadoxIntact"));
    SpawnBlock(FVector(-400,-49400,-50),FVector(2.2,1.4,1),FName("Rite.MapRoute"));
    SetRiteWorldActorState(FName("Rite.WitnessBridge"),false);
    SetRiteWorldActorState(FName("Rite.CommonCauseway"),false);
    SetRiteWorldActorState(FName("Rite.ParadoxIntact"),false);
    SetRiteWorldActorState(FName("Rite.MapRoute"),false);
    // Six late-campaign confluence puzzles combine pairs of earned disciplines.
    // These are separate from the ten learning stones and must be visited.
    const FVector ConfluenceSites[6]={
        FVector(-18200,-1700,90),   // Bellwold: silent alarm
        FVector(-17480,-800,90),    // Bellwold: two names
        FVector(1600,520,90),       // Silent Mile: unmapped way
        FVector(970,-1070,90),      // Debt Market: tomorrow's promise
        FVector(17760,-1350,90),   // Paperhaven: the two keepers
        FVector(120,-49850,90)      // Saltwake: first absence
    };
    for(int32 Index=0;Index<6;++Index)
        SpawnBlock(ConfluenceSites[Index],FVector(.85,.85,2.5),
            FName(*FString::Printf(TEXT("Confluence.Site.%d"),Index)));


    SpawnBlock(FVector(520, -470, 210), FVector(5, 5, 4.2), FName("Hub.Market"));
    SpawnBlock(FVector(-650, -440, 160), FVector(3, 3, 3.2), FName("Hub.Watch"));
    SpawnBlock(FVector(650, 570, 140), FVector(4, 3, 2.8), FName("Hub.Store"));
    SpawnBlock(FVector(-590, 660, 190), FVector(4, 4, 3.8), FName("Hub.Shelter"));
    // Peripheral ruins frame a longer loop around the original hub; passage between
    // landmarks remains open, with geometry still made only from engine primitives.
    SpawnBlock(FVector(-1880, 1320, 90), FVector(0.6, 2.4, 1.8), FName("Outer.WellRuins"));
    SpawnBlock(FVector(1500, 1830, 140), FVector(3.2, 0.5, 2.8), FName("Outer.PaperWall"));
    SpawnBlock(FVector(2040, 560, 150), FVector(0.7, 2.9, 3), FName("Outer.UnfinishedRoad"));
    SpawnBlock(FVector(-2030, -900, 110), FVector(0.7, 2.2, 2.2), FName("Outer.BuriedBell"));
    SpawnBlock(FVector(1170, -1430, 140), FVector(1.6, 1.8, 2.8), FName("Outer.MarketAnnex"));

    SpawnLoreSite(UnmadeCore::District::EchoWell, FName("site.echo_well"),
        TEXT("The Well of Returned Voices"),
        TEXT("Someone whispered a warning in your voice. You have not said it yet."),
        TEXT("A second moon moves through this water although there is only one sky."),
        FVector(-1720, 850, 90));
    SpawnLoreSite(UnmadeCore::District::PaperOrchard, FName("site.paper_orchard"),
        TEXT("The Orchard of Unwritten Names"),
        TEXT("Its leaves are signed by people no record admits were born."),
        TEXT("Every sheet turns toward the footsteps of someone walking behind you."),
        FVector(1260, 1490, 90));
    SpawnLoreSite(UnmadeCore::District::SilentMile, FName("site.silent_mile"),
        TEXT("The Silent Mile"),
        TEXT("Footprints travel both directions. Not one of them begins here."),
        TEXT("The stones tremble without wind, and even your boots forget their sound."),
        FVector(1890, 210, 90));
    SpawnLoreSite(UnmadeCore::District::BellGrave, FName("site.bell_grave"),
        TEXT("The Grave of the Last Bell"),
        TEXT("The buried bell casts a shadow upward through the dirt."),
        TEXT("Something tolls beneath the earth. Your shadow turns a moment too late."),
        FVector(-1790, -1270, 90));
    SpawnLoreSite(UnmadeCore::District::MarketLedger, FName("site.market_ledger"),
        TEXT("The Debt Market"),
        TEXT("This ledger lists purchases dated tomorrow, including one in your name."),
        TEXT("The price of each empty stall is written as a memory no one will confess."),
        FVector(850, -1440, 90));
    SpawnLoreSite(UnmadeCore::District::ShelterThreshold, FName("site.shelter_threshold"),
        TEXT("The Threshold of Two Claims"),
        TEXT("Two factions each hold a key. The only intact passage can serve one."),
        TEXT("Candlelight stops at the threshold as if the darkness has been granted ownership."),
        FVector(-1470, 250, 90));

    // A single authored prototype target for temporary and permanent fractures.
    if (AUnmadeFractureAnchor* Anchor = GetWorld()->SpawnActor<AUnmadeFractureAnchor>(
        FVector(0, 530, 110), FRotator::ZeroRotator))
    {
        Anchor->Tags.AddUnique(FName("Hub.AnomalyMarker"));
    }

    // Two distinct blocked routes; the player's persistent local choice opens exactly one.
    if (AUnmadeConflictGate* ShelterGate = GetWorld()->SpawnActor<AUnmadeConflictGate>(
        FVector(-420, 930, 110), FRotator::ZeroRotator))
        ShelterGate->Configure(FName("gate.prototype.shelter"));
    if (AUnmadeConflictGate* ArchiveGate = GetWorld()->SpawnActor<AUnmadeConflictGate>(
        FVector(420, 930, 110), FRotator::ZeroRotator))
        ArchiveGate->Configure(FName("gate.prototype.archive"));

    // Restore physical access independently of player BeginPlay order.
    const UUnmadePrototypeSave* SavedConflict = Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"), 0));
    if (SavedConflict && SavedConflict->SchemaVersion == 1 && SavedConflict->bHasConflictSnapshot)
    {
        UnmadeCore::ConflictModel Restored;
        if (Restored.Restore({SavedConflict->LocalConflictChoice, SavedConflict->SupplyActivityStage}))
        {
            for (TActorIterator<AUnmadeConflictGate> It(GetWorld()); It; ++It)
            {
                if (It->GetGateId() == FName("gate.prototype.shelter"))
                    It->SetAccess(Restored.ShelterOpen());
                else if (It->GetGateId() == FName("gate.prototype.archive"))
                    It->SetAccess(Restored.ArchiveOpen());
            }
        }
    }

    // Data-driven identities: 16 per village, 48 independently remembered residents.
    for (const UnmadeCore::ResidentSpec& Resident : UnmadeCore::Residents)
        SpawnCitizen(Resident);

    // Two visually distinct graybox enemies. No quest reward or respawn system yet.
    if (AUnmadeEnemyCharacter* Stalker = GetWorld()->SpawnActor<AUnmadeEnemyCharacter>(
        FVector(920, 920, 100), FRotator::ZeroRotator))
        Stalker->ConfigureStyle(UnmadeCore::EnemyStyle::Stalker);
    if (AUnmadeEnemyCharacter* Watcher = GetWorld()->SpawnActor<AUnmadeEnemyCharacter>(
        FVector(-910, 920, 100), FRotator::ZeroRotator))
        Watcher->ConfigureStyle(UnmadeCore::EnemyStyle::Watcher);
    // Three authored arenas on the village outskirts. Fracture anchors allow
    // the existing Fold power to interrupt the boss' telegraphed attacks.
    struct BossPlacement { UnmadeCore::BossId Id; FVector Position; };
    const BossPlacement Encounters[]={
        {UnmadeCore::BossId::HollowBell,FVector(-18000,-2050,100)},
        {UnmadeCore::BossId::RedactedCurator,FVector(18000,2030,100)},
        {UnmadeCore::BossId::UnfinishedPilgrim,FVector(0,-2120,100)}
    };
    const UUnmadePrototypeSave* EncounterSave=Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"),0));
    for(const BossPlacement& Location:Encounters)
    {
        UnmadeCore::Achievement BossReward=UnmadeCore::Achievement::Count;
        if(Location.Id==UnmadeCore::BossId::HollowBell)
            BossReward=UnmadeCore::Achievement::HollowBell;
        else if(Location.Id==UnmadeCore::BossId::RedactedCurator)
            BossReward=UnmadeCore::Achievement::RedactedCurator;
        else if(Location.Id==UnmadeCore::BossId::UnfinishedPilgrim)
            BossReward=UnmadeCore::Achievement::UnfinishedPilgrim;
        const bool bPreviouslyDefeated=EncounterSave && EncounterSave->SchemaVersion==1 &&
            EncounterSave->bHasInventorySnapshot && BossReward!=UnmadeCore::Achievement::Count &&
            (static_cast<uint64>(EncounterSave->AwardedMilestoneBits) &
                (uint64(1)<<static_cast<int32>(BossReward)))!=0;
        if(!bPreviouslyDefeated)
        {
            if(AUnmadeBossCharacter* Boss=GetWorld()->SpawnActor<AUnmadeBossCharacter>(
                Location.Position,FRotator::ZeroRotator))
                Boss->ConfigureBoss(Location.Id);
        }
        if(AUnmadeFractureAnchor* Anchor=GetWorld()->SpawnActor<AUnmadeFractureAnchor>(
            Location.Position+FVector(500,0,15),FRotator::ZeroRotator))
            Anchor->Tags.AddUnique(FName("Boss.FractureCounter"));
    }

    RestoreCitizens();
}

void AUnmadePrototypeHub::RestoreCitizens()
{
    const USaveGame* Raw = UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"), 0);
    const UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(Raw);
    if (!Save || Save->SchemaVersion != 1) return;
    for (TActorIterator<AUnmadeNpcCharacter> It(GetWorld()); It; ++It)
    {
        for (const FUnmadeNpcSnapshot& Snapshot : Save->NpcSnapshots)
        {
            if (Snapshot.NpcId == It->GetStableId())
            {
                It->GetMemory()->ReadSnapshot(Snapshot, It->GetStableId());
                break;
            }
        }
    }
}

void AUnmadePrototypeHub::SpreadLocalRumors()
{
    UWorld* World = GetWorld();
    if (!World) return;
    TArray<AUnmadeNpcCharacter*> Citizens;
    for (TActorIterator<AUnmadeNpcCharacter> It(World); It; ++It)
        Citizens.Add(*It);

    bool bNewMemory = false;
    bool bOverheardOne = false;
    const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(World, 0);
    // Nearby named individuals talk about events they directly observed.
    // Hearsay is not silently upgraded to evidence and gossip cannot teleport.
    for (AUnmadeNpcCharacter* Speaker : Citizens)
    {
        for (AUnmadeNpcCharacter* Listener : Citizens)
        {
            if (Speaker == Listener || Speaker->GetStableId().IsNone() || Listener->GetStableId().IsNone())
                continue;
            if (FVector::DistSquared(Speaker->GetActorLocation(), Listener->GetActorLocation()) > FMath::Square(400.f))
                continue;
            FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeNpcGossip), false);
            Params.AddIgnoredActor(Speaker);
            Params.AddIgnoredActor(Listener);
            if (World->LineTraceTestByChannel(
                Speaker->GetActorLocation() + FVector(0, 0, 65),
                Listener->GetActorLocation() + FVector(0, 0, 65),
                ECC_Visibility, Params))
                continue;

            for (const FUnmadeNpcObservation& Event : Speaker->GetMemory()->GetObservations())
            {
                // First version only permits one hop from a direct witness.
                if (Event.Evidence == EUnmadeEvidenceKind::Witnessed &&
                    Listener->GetMemory()->HearRumor(Event.EventId, Event.EventKind, Speaker->GetStableId(), Event.SubjectId))
                {
                    bNewMemory = true;
                    // A single local line, only if the player can physically overhear.
                    if (!bOverheardOne && IsValid(Player) && GEngine &&
                        FVector::DistSquared2D(Player->GetActorLocation(), Speaker->GetActorLocation()) < FMath::Square(520.f))
                    {
                        FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeOverheardGossip), false);
                        Params.AddIgnoredActor(Player);
                        Params.AddIgnoredActor(Speaker);
                        if (!World->LineTraceTestByChannel(
                            Player->GetActorLocation() + FVector(0, 0, 55),
                            Speaker->GetActorLocation() + FVector(0, 0, 55),
                            ECC_Visibility, Params))
                        {
                            const TCHAR* Topic = TEXT("something odd in the district");
                            if (Event.EventKind == FName("Reality.Anomaly"))
                                Topic = TEXT("the street shifting beneath the stranger");
                            else if (Event.EventKind == FName("Player.Helped"))
                                Topic = TEXT("the stranger helping a neighbor");
                            else if (Event.EventKind == FName("Player.DeliveredSupplies"))
                                Topic = TEXT("provisions reaching the shelter");
                            else if (Event.EventKind == FName("World.ConflictShelter"))
                                Topic = TEXT("the shelter passage being protected");
                            else if (Event.EventKind == FName("World.ConflictResearch"))
                                Topic = TEXT("the archive taking control of the route");
                            GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Silver,
                                FString::Printf(TEXT("OVERHEARD | %s to %s: I saw %s."),
                                    *Speaker->GetDisplayLabel(), *Listener->GetDisplayLabel(), Topic));
                            bOverheardOne = true;
                        }
                    }
                }
            }
        }
    }
    if (bNewMemory) SaveCitizens();
}

void AUnmadePrototypeHub::SaveCitizens()
{
    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return;
    Save->NpcSnapshots.Reset(); // Avoid duplicates on repeated gossip saves.
    for (TActorIterator<AUnmadeNpcCharacter> It(GetWorld()); It; ++It)
    {
        if (!It->GetStableId().IsNone())
            Save->NpcSnapshots.Add(It->GetMemory()->WriteSnapshot(It->GetStableId()));
    }
    if (!UGameplayStatics::SaveGameToSlot(Save, TEXT("UnmadePrototypeNPC"), 0))
        UE_LOG(LogTemp, Error, TEXT("Unmade NPC social update save failed"));
}
