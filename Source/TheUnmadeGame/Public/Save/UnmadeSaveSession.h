#pragma once
#include "CoreMinimal.h"
class USaveGame;class UObject;class AUnmadeCharacter;
namespace UnmadeSave {
 THEUNMADEGAME_API void RegisterPlayer(AUnmadeCharacter* Player);
 THEUNMADEGAME_API FString SlotName(int Profile=-1);
 THEUNMADEGAME_API USaveGame* LoadGameFromSlot();
 THEUNMADEGAME_API bool SaveGameToSlot(USaveGame* Save);
 THEUNMADEGAME_API bool IsSessionWritable(const AUnmadeCharacter* Player=nullptr);
 THEUNMADEGAME_API bool HasRecord();
 THEUNMADEGAME_API bool SelectProfile(UObject* Context,int Profile,bool Backup=false);
 THEUNMADEGAME_API bool CreateUnusedProfile(UObject* Context);
 THEUNMADEGAME_API FString DescribeProfiles();
}
