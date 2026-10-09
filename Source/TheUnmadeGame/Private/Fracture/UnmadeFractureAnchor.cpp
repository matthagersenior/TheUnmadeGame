#include "Fracture/UnmadeFractureAnchor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AUnmadeFractureAnchor::AUnmadeFractureAnchor()
{
    PrimaryActorTick.bCanEverTick = false;
    Barrier = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FractureBarrier"));
    RootComponent = Barrier;
    Barrier->SetMobility(EComponentMobility::Movable);
    Barrier->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Barrier->SetRelativeScale3D(FVector(1.5, 0.5, 2.2));

    Clue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AlternateHistoryClue"));
    Clue->SetupAttachment(Barrier);
    Clue->SetRelativeLocation(FVector(0, 210, 0));
    Clue->SetRelativeScale3D(FVector(0.25, 0.25, 0.9));
    Clue->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Clue->SetVisibility(false);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded())
    {
        Barrier->SetStaticMesh(Cube.Object);
        Clue->SetStaticMesh(Cube.Object);
    }
}

void AUnmadeFractureAnchor::ApplyFractureState(bool bGlimpsing, bool bFolded, FName Variant)
{
    const bool bOpen = Variant == FName("variant.open");
    const bool bSealed = Variant == FName("variant.sealed");
    const bool bBlocking = !bOpen && !bFolded;
    Barrier->SetVisibility(bBlocking);
    Barrier->SetCollisionEnabled(bBlocking ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    Barrier->SetRelativeScale3D(bSealed ? FVector(2.25, 0.75, 2.7) : FVector(1.5, 0.5, 2.2));
    Clue->SetVisibility(bGlimpsing && !bSealed);
}

bool AUnmadeFractureAnchor::IsBarrierBlocking() const
{
    return Barrier->GetCollisionEnabled() != ECollisionEnabled::NoCollision;
}
bool AUnmadeFractureAnchor::IsClueVisible() const
{
    return Clue->IsVisible();
}
