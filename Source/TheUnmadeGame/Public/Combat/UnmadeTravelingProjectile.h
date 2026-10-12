#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Combat/UnmadeCombatStyleRules.h"
#include "UnmadeTravelingProjectile.generated.h"
UCLASS()
class THEUNMADEGAME_API AUnmadeTravelingProjectile : public AActor {
 GENERATED_BODY()
public:
 AUnmadeTravelingProjectile();
 virtual void Tick(float Delta)override;
 bool Fire(AActor* Source,const FVector& Aim,double Damage,uint64 Receipt,float Speed=1200,float Range=1400,UnmadeCore::CombatStatus Status=UnmadeCore::CombatStatus::None);
private:
 UPROPERTY() TObjectPtr<class UStaticMeshComponent> Visual;
 TWeakObjectPtr<AActor> Shooter;
 UnmadeCore::CombatStatus ShotStatus=UnmadeCore::CombatStatus::None;
 FVector Velocity=FVector::ZeroVector;
 double Damage=0,Expires=-1;uint64 Receipt=0;
};
