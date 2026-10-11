#include "UI/UnmadeRpgFeedback.h"
#include "World/UnmadePrototypeHub.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "EngineUtils.h"
void UnmadeRpgFeedback(uint64 Key,float Duration,FColor Color,const FString& Text,bool NewerOnTop,FVector2D Scale){
 UWorld* World=GEngine&&GEngine->GameViewport?GEngine->GameViewport->GetWorld():nullptr;
 if(World)for(TActorIterator<AUnmadePrototypeHub> Hub(World);Hub;++Hub){Hub->NotifyRpg(Text);return;}
 // Early initialization, dedicated servers and missing local viewport retain diagnostics.
 if(GEngine)GEngine->AddOnScreenDebugMessage(Key,Duration,Color,Text,NewerOnTop,Scale);
}
