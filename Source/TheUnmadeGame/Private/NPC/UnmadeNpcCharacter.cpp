#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AUnmadeNpcCharacter::AUnmadeNpcCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    Memory = CreateDefaultSubobject<UUnmadeMemoryComponent>(TEXT("PersonalMemory"));
    PlaceholderVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TemporaryNPCVisual"));
    PlaceholderVisual->SetupAttachment(GetCapsuleComponent());
    PlaceholderVisual->SetRelativeLocation(FVector(0, 0, -3));
    PlaceholderVisual->SetRelativeScale3D(FVector(0.6, 0.6, 1.55));
    PlaceholderVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (Mesh.Succeeded()) PlaceholderVisual->SetStaticMesh(Mesh.Object);
}

void AUnmadeNpcCharacter::ConfigureIdentity(FName StableId, const FString& DisplayLabel)
{
    NpcId = StableId;
    NpcDisplayLabel = DisplayLabel;
    Tags.AddUnique(StableId);
}

FString AUnmadeNpcCharacter::GetReactionText() const
{
    const int32 Trust = Memory->GetTrust();
    const int32 Fear = Memory->GetFear();
    FString Mood = TEXT("watches you cautiously");
    if (Trust >= 20 && Fear >= 20) Mood = TEXT("thanks you, but keeps their distance");
    else if (Fear >= 20) Mood = TEXT("backs away, disturbed by what they witnessed");
    else if (Trust >= 20) Mood = TEXT("greets you warmly and offers help");
    return FString::Printf(TEXT("%s %s. (trust %d, fear %d; memories %d)"),
        *NpcDisplayLabel, *Mood, Trust, Fear, Memory->GetObservationCount());
}
