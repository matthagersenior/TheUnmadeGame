#pragma once
#include "CoreMinimal.h"
// Routes legacy gameplay notices into the active local RPG interface.
void UnmadeRpgFeedback(uint64 Key,float Duration,FColor Color,const FString& Text,bool NewerOnTop=true,FVector2D Scale=FVector2D(1,1));
