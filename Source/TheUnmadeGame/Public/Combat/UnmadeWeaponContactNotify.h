#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UnmadeWeaponContactNotify.generated.h"
UCLASS()
class THEUNMADEGAME_API UUnmadeWeaponContactNotify : public UAnimNotify {
 GENERATED_BODY()
public:
 virtual void Notify(USkeletalMeshComponent* Mesh,UAnimSequenceBase* Animation,const FAnimNotifyEventReference& Reference)override;
};
