#include "World/UnmadePrototypeHub.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Save/UnmadePrototypeSave.h"
#include "Fracture/UnmadeFractureAnchor.h"
#include "Combat/UnmadeEnemyCharacter.h"
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
    if (!RestoredClock.Restore(Save->LivingWorldSeconds) ||
        !RestoredDiscoveries.Restore(Save->DiscoveredLoreMask) ||
        !RestoredVillages.Restore(Save->VisitedSettlementsMask))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid living world state ignored"));
        return;
    }
    Clock = RestoredClock;
    Discoveries = RestoredDiscoveries;
    VillagesVisited = RestoredVillages;
}

bool AUnmadePrototypeHub::WriteWorldSnapshot()
{
    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return false;
    Save->bHasLivingWorldSnapshot = true;
    Save->LivingWorldSeconds = Clock.ElapsedSeconds();
    Save->DiscoveredLoreMask = Discoveries.Snapshot();
    Save->VisitedSettlementsMask = VillagesVisited.Snapshot();
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

    const FVector Position = Player->GetActorLocation();
    const UnmadeCore::SettlementId Village = UnmadeCore::SettlementAt(Position.X, Position.Y);
    if (Village != LastVisitedVillage)
    {
        LastVisitedVillage = Village;
        if (const auto* VillageInfo = UnmadeCore::FindSettlement(Village))
        {
            const int32 PreviouslyVisited = VillagesVisited.Snapshot();
            const bool bNewVillage = VillagesVisited.Visit(Village);
            if (bNewVillage && !WriteWorldSnapshot())
                VillagesVisited.Restore(PreviouslyVisited);
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
