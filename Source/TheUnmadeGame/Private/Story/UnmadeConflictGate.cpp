#include "Story/UnmadeConflictGate.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AUnmadeConflictGate::AUnmadeConflictGate()
{
    PrimaryActorTick.bCanEverTick = false;

    GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PassageGate"));
    RootComponent = GateMesh;
    GateMesh->SetMobility(EComponentMobility::Movable);
    GateMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GateMesh->SetRelativeScale3D(FVector(1.5, 0.5, 2.2));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Mesh.Succeeded()) GateMesh->SetStaticMesh(Mesh.Object);
}

void AUnmadeConflictGate::Configure(FName StableGateId)
{
    GateId = StableGateId;
    Tags.AddUnique(StableGateId);
    SetAccess(false);
}

void AUnmadeConflictGate::SetAccess(bool bAllowPassage)
{
    bPassable = bAllowPassage;
    GateMesh->SetVisibility(!bPassable);
    GateMesh->SetCollisionEnabled(
        bPassable ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
}
