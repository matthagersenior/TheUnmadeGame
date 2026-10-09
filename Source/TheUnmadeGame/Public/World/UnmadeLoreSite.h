#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/UnmadeLivingWorldRules.h"
#include "UnmadeLoreSite.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;

/** A location with authored day/night and story-aware atmosphere. No generative AI. */
UCLASS()
class THEUNMADEGAME_API AUnmadeLoreSite : public AActor
{
    GENERATED_BODY()
public:
    AUnmadeLoreSite();
    void Configure(UnmadeCore::District NewDistrict, FName StableId,
                   const FString& Name, const FString& DayDescription,
                   const FString& NightDescription);
    void SetPhase(UnmadeCore::DayPhase Phase);

    FName GetSiteId() const { return SiteId; }
    UnmadeCore::District GetDistrict() const { return District; }
    FString GetInspectionText(UnmadeCore::DayPhase Phase, int StoryChoice) const;
    FString GetAmbientText(UnmadeCore::DayPhase Phase, int StoryChoice) const;
private:
    UPROPERTY(VisibleAnywhere, Category="Unmade|World")
    TObjectPtr<UStaticMeshComponent> Marker;
    UPROPERTY(VisibleAnywhere, Category="Unmade|World")
    TObjectPtr<UPointLightComponent> Glow;

    UPROPERTY()
    FName SiteId = NAME_None;
    UPROPERTY()
    FString SiteName;
    UPROPERTY()
    FString DayLine;
    UPROPERTY()
    FString NightLine;
    UnmadeCore::District District = UnmadeCore::District::EchoWell;
};
