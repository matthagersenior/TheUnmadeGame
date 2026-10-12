#include "Combat/UnmadeWeaponContactNotify.h"
#include "Player/UnmadeCharacter.h"
#include "Components/SkeletalMeshComponent.h"
void UUnmadeWeaponContactNotify::Notify(USkeletalMeshComponent* Mesh,UAnimSequenceBase* Animation,const FAnimNotifyEventReference& Reference){Super::Notify(Mesh,Animation,Reference);if(Mesh)if(auto* p=Cast<AUnmadeCharacter>(Mesh->GetOwner()))p->NotifyWeaponContact();}
