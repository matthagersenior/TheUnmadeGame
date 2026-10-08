#include "Game/UnmadeGameMode.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadePrototypeHub.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"

AUnmadeGameMode::AUnmadeGameMode()
{
    DefaultPawnClass = AUnmadeCharacter::StaticClass();
}

void AUnmadeGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    UWorld* World = GetWorld();
    if (!World) return;
    // Provide a valid spawn before players enter the primitive prototype map.
    World->SpawnActor<APlayerStart>(FVector(0, 0, 130), FRotator::ZeroRotator);
    World->SpawnActor<AUnmadePrototypeHub>(FVector::ZeroVector, FRotator::ZeroRotator);
}
