#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "UI/UnmadePlayerPreferences.h"
namespace UnmadePresentation {
inline void Sound(AActor* actor,const TCHAR* cue){if(!IsValid(actor))return;FString name=FString(TEXT("SFX_ID_"))+cue;FString path=TEXT("/Game/UnmadeProduction/Audio/Identity/")+name+TEXT(".")+name;if(auto* sound=LoadObject<USoundBase>(nullptr,*path,nullptr,LOAD_NoWarn))UGameplayStatics::PlaySoundAtLocation(actor,sound,actor->GetActorLocation(),.55f*GetDefault<UUnmadePlayerPreferences>()->AudioVolume);}
inline void Motion(USkeletalMeshComponent* mesh,const TCHAR* rig,const TCHAR* cue,bool loop=false){if(!IsValid(mesh)||!mesh->GetSkeletalMeshAsset())return;FString name=FString::Printf(TEXT("AN_Unmade%s_%s"),rig,cue);FString path=FString::Printf(TEXT("/Game/UnmadeProduction/Characters/%s/%s.%s"),rig,*name,*name);if(auto* clip=LoadObject<UAnimSequence>(nullptr,*path,nullptr,LOAD_NoWarn))mesh->PlayAnimation(clip,loop);}
}
