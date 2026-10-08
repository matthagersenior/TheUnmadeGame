#include "World/UnmadePrototypeHub.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Save/UnmadePrototypeSave.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/World.h"

AUnmadePrototypeHub::AUnmadePrototypeHub()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AUnmadePrototypeHub::BeginPlay()
{
    Super::BeginPlay();
    BuildForPrototype();
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

void AUnmadePrototypeHub::SpawnCitizen(FName Id, const TCHAR* DisplayName, FVector Position)
{
    if (AUnmadeNpcCharacter* Citizen = GetWorld()->SpawnActor<AUnmadeNpcCharacter>(Position, FRotator::ZeroRotator))
    {
        Citizen->ConfigureIdentity(Id, FString(DisplayName));
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
    SpawnBlock(FVector(0, 530, 90), FVector(0.75, 0.75, 1.8), FName("Hub.AnomalyMarker"));

    SpawnCitizen(FName("npc.merchant.001"), TEXT("The stallkeeper"), FVector(200, -230, 95));
    SpawnCitizen(FName("npc.guard.001"), TEXT("A gate watchkeeper"), FVector(-300, -230, 95));
    SpawnCitizen(FName("npc.wanderer.001"), TEXT("A passing stranger"), FVector(170, 340, 95));
    SpawnCitizen(FName("npc.archivist.001"), TEXT("The records keeper"), FVector(-360, 320, 95));
    SpawnCitizen(FName("npc.courier.001"), TEXT("A courier"), FVector(300, 90, 95));

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
