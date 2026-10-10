#pragma once
#include "CoreMinimal.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeRealmGuardianRules.h"
#include "UnmadeRealmGuardian.generated.h"

UCLASS()
class THEUNMADEGAME_API AUnmadeRealmGuardian : public AUnmadeEnemyCharacter
{
    GENERATED_BODY()
public:
    AUnmadeRealmGuardian();
    virtual void Tick(float DeltaSeconds) override;
    void ConfigureGuardian(UnmadeCore::Realm Id);
    UnmadeCore::Realm GetRealm() const {return HomeRealm;}
    void SetGuardianResolved();
private:
    UnmadeCore::Realm HomeRealm=UnmadeCore::Realm::Count;
    UnmadeCore::GuardianBeat Encounter{UnmadeCore::Realm::Count};
    bool bWarningShown=false;
};
