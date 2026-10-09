#include "World/UnmadePrototypeHub.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Save/UnmadePrototypeSave.h"
#include "Fracture/UnmadeFractureAnchor.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Story/UnmadeConflictGate.h"
#include "Story/UnmadeConflictRules.h"
#include "World/UnmadeLoreSite.h"
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
    if (!RestoredClock.Restore(Save->LivingWorldSeconds) ||
        !RestoredDiscoveries.Restore(Save->DiscoveredLoreMask))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid living world state ignored"));
        return;
    }
    Clock = RestoredClock;
    Discoveries = RestoredDiscoveries;
}

bool AUnmadePrototypeHub::WriteWorldSnapshot()
{
    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return false;
    Save->bHasLivingWorldSnapshot = true;
    Save->LivingWorldSeconds = Clock.ElapsedSeconds();
    Save->DiscoveredLoreMask = Discoveries.Snapshot();
    return UGameplayStatics::SaveGameToSlot(Save, TEXT("UnmadePrototypeNPC"), 0);
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

void AUnmadePrototypeHub::SpawnCitizen(FName Id, const TCHAR* DisplayName, FVector Position,
    UnmadeCore::NpcRole Role, UnmadeCore::NpcTemperament Temperament)
{
    if (AUnmadeNpcCharacter* Citizen = GetWorld()->SpawnActor<AUnmadeNpcCharacter>(Position, FRotator::ZeroRotator))
    {
        Citizen->ConfigureIdentity(Id, FString(DisplayName), Role, Temperament);
    }
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

    SpawnCitizen(FName("npc.merchant.001"), TEXT("The stallkeeper"), FVector(200, -230, 95), UnmadeCore::NpcRole::Merchant, UnmadeCore::NpcTemperament::Cautious);
    SpawnCitizen(FName("npc.guard.001"), TEXT("A gate watchkeeper"), FVector(-300, -230, 95), UnmadeCore::NpcRole::Guard, UnmadeCore::NpcTemperament::Steady);
    SpawnCitizen(FName("npc.wanderer.001"), TEXT("A passing stranger"), FVector(170, 340, 95), UnmadeCore::NpcRole::Wanderer, UnmadeCore::NpcTemperament::Steady);
    SpawnCitizen(FName("npc.archivist.001"), TEXT("The records keeper"), FVector(-360, 320, 95), UnmadeCore::NpcRole::Scholar, UnmadeCore::NpcTemperament::Curious);
    SpawnCitizen(FName("npc.courier.001"), TEXT("A courier"), FVector(300, 90, 95), UnmadeCore::NpcRole::Courier, UnmadeCore::NpcTemperament::Steady);

    // Additional named residents make the outer sites inhabited, not just scenic.
    // Stable IDs permit distinct persistent memories even when roles are shared.
    SpawnCitizen(FName("npc.welllistener.001"), TEXT("The well listener"),
        FVector(-1540, 680, 95), UnmadeCore::NpcRole::Scholar, UnmadeCore::NpcTemperament::Curious);
    SpawnCitizen(FName("npc.orchardexile.001"), TEXT("The displaced orchard keeper"),
        FVector(1120, 1260, 95), UnmadeCore::NpcRole::Wanderer, UnmadeCore::NpcTemperament::Cautious);
    SpawnCitizen(FName("npc.tollbroker.001"), TEXT("The keeper of future debts"),
        FVector(1000, -1170, 95), UnmadeCore::NpcRole::Merchant, UnmadeCore::NpcTemperament::Cautious);
    SpawnCitizen(FName("npc.roadwarden.001"), TEXT("The road warden"),
        FVector(1690, 390, 95), UnmadeCore::NpcRole::Guard, UnmadeCore::NpcTemperament::Steady);
    SpawnCitizen(FName("npc.bellmaker.001"), TEXT("The bell maker"),
        FVector(-1580, -1020, 95), UnmadeCore::NpcRole::Merchant, UnmadeCore::NpcTemperament::Steady);
    SpawnCitizen(FName("npc.nightcourier.001"), TEXT("The night courier"),
        FVector(1010, 1040, 95), UnmadeCore::NpcRole::Courier, UnmadeCore::NpcTemperament::Cautious);

    // Two visually distinct graybox enemies. No quest reward or respawn system yet.
    if (AUnmadeEnemyCharacter* Stalker = GetWorld()->SpawnActor<AUnmadeEnemyCharacter>(
        FVector(920, 920, 100), FRotator::ZeroRotator))
        Stalker->ConfigureStyle(UnmadeCore::EnemyStyle::Stalker);
    if (AUnmadeEnemyCharacter* Watcher = GetWorld()->SpawnActor<AUnmadeEnemyCharacter>(
        FVector(-910, 920, 100), FRotator::ZeroRotator))
        Watcher->ConfigureStyle(UnmadeCore::EnemyStyle::Watcher);
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
                    bNewMemory = true;
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
