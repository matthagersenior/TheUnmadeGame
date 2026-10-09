#include "World/UnmadePrototypeHub.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Save/UnmadePrototypeSave.h"
#include "Fracture/UnmadeFractureAnchor.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "TimerManager.h"

AUnmadePrototypeHub::AUnmadePrototypeHub()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AUnmadePrototypeHub::BeginPlay()
{
    Super::BeginPlay();
    BuildForPrototype();
    GetWorldTimerManager().SetTimer(GossipTimer, this, &AUnmadePrototypeHub::SpreadLocalRumors, 8.f, true);
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

    // All geometry uses Unreal's primitive cube assets: temporary untextured blockout.
    // Walking ground: top at Z=0; center at -50 with 1*100 cm tall mesh.
    SpawnBlock(FVector(0, 0, -50), FVector(26, 26, 1), FName("Hub.Ground"));
    SpawnBlock(FVector(520, -470, 210), FVector(5, 5, 4.2), FName("Hub.Market"));
    SpawnBlock(FVector(-650, -440, 160), FVector(3, 3, 3.2), FName("Hub.Watch"));
    SpawnBlock(FVector(650, 570, 140), FVector(4, 3, 2.8), FName("Hub.Store"));
    SpawnBlock(FVector(-590, 660, 190), FVector(4, 4, 3.8), FName("Hub.Shelter"));
    // A single authored prototype target for temporary and permanent fractures.
    if (AUnmadeFractureAnchor* Anchor = GetWorld()->SpawnActor<AUnmadeFractureAnchor>(
        FVector(0, 530, 110), FRotator::ZeroRotator))
    {
        Anchor->Tags.AddUnique(FName("Hub.AnomalyMarker"));
    }

    SpawnCitizen(FName("npc.merchant.001"), TEXT("The stallkeeper"), FVector(200, -230, 95), UnmadeCore::NpcRole::Merchant, UnmadeCore::NpcTemperament::Cautious);
    SpawnCitizen(FName("npc.guard.001"), TEXT("A gate watchkeeper"), FVector(-300, -230, 95), UnmadeCore::NpcRole::Guard, UnmadeCore::NpcTemperament::Steady);
    SpawnCitizen(FName("npc.wanderer.001"), TEXT("A passing stranger"), FVector(170, 340, 95), UnmadeCore::NpcRole::Wanderer, UnmadeCore::NpcTemperament::Steady);
    SpawnCitizen(FName("npc.archivist.001"), TEXT("The records keeper"), FVector(-360, 320, 95), UnmadeCore::NpcRole::Scholar, UnmadeCore::NpcTemperament::Curious);
    SpawnCitizen(FName("npc.courier.001"), TEXT("A courier"), FVector(300, 90, 95), UnmadeCore::NpcRole::Courier, UnmadeCore::NpcTemperament::Steady);

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
