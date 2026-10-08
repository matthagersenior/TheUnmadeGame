#include "Game/UnmadeGameMode.h"
#include "Player/UnmadeCharacter.h"

AUnmadeGameMode::AUnmadeGameMode()
{
    DefaultPawnClass = AUnmadeCharacter::StaticClass();
}
