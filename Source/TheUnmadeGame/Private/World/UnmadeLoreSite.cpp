#include "World/UnmadeLoreSite.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "UObject/ConstructorHelpers.h"

AUnmadeLoreSite::AUnmadeLoreSite()
{
    PrimaryActorTick.bCanEverTick = false;
    Marker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LoreMarker"));
    RootComponent = Marker;
    Marker->SetMobility(EComponentMobility::Movable);
    Marker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Marker->SetRelativeScale3D(FVector(0.45, 0.45, 1.1));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded()) Marker->SetStaticMesh(Cube.Object);

    Glow = CreateDefaultSubobject<UPointLightComponent>(TEXT("FractureGlow"));
    Glow->SetupAttachment(Marker);
    Glow->SetRelativeLocation(FVector(0, 0, 170));
    Glow->SetAttenuationRadius(650.0f);
    Glow->SetCastShadows(false);
    SetPhase(UnmadeCore::DayPhase::Day);
}

void AUnmadeLoreSite::Configure(UnmadeCore::District NewDistrict, FName StableId,
    const FString& Name, const FString& DayDescription, const FString& NightDescription)
{
    District = NewDistrict;
    SiteId = StableId;
    SiteName = Name;
    DayLine = DayDescription;
    NightLine = NightDescription;
    Tags.AddUnique(StableId);
}

void AUnmadeLoreSite::SetPhase(UnmadeCore::DayPhase Phase)
{
    const bool bDark = Phase == UnmadeCore::DayPhase::Dusk ||
                       Phase == UnmadeCore::DayPhase::Night;
    Glow->SetIntensity(bDark ? 2600.f : 350.f);
    Glow->SetLightColor(bDark ? FLinearColor(0.3f, 0.12f, 0.95f)
                              : FLinearColor(0.95f, 0.65f, 0.34f));
}

FString AUnmadeLoreSite::GetAmbientText(UnmadeCore::DayPhase Phase, int StoryChoice) const
{
    using namespace UnmadeCore;
    switch (SelectAmbientCue(District, Phase, StoryChoice))
    {
    case AmbientCue::EchoDay: return TEXT("The well repeats a sound before it is made.");
    case AmbientCue::EchoNight: return TEXT("A second moon trembles in water beneath an empty sky.");
    case AmbientCue::PaperDay: return TEXT("Paper leaves carry names the living have never heard.");
    case AmbientCue::PaperNight: return TEXT("The paper trees whisper their names only in the dark.");
    case AmbientCue::SilentDay: return TEXT("The roadway shows footprints that walk in both directions.");
    case AmbientCue::SilentNight: return TEXT("No wind crosses the road; the stones breathe on their own.");
    case AmbientCue::BellDay: return TEXT("A buried bell casts its shadow above the soil.");
    case AmbientCue::BellNight: return TEXT("The bell rings once. Every nearby shadow moves late.");
    case AmbientCue::MarketDay: return TEXT("Merchants weigh objects against remembered promises.");
    case AmbientCue::MarketNight: return TEXT("Closed stalls display prices for things not yet invented.");
    case AmbientCue::ShelterWelcoming: return TEXT("Warm light fills the shelter passage you kept open.");
    case AmbientCue::ShelterBarred: return TEXT("The shelter threshold is cold; the archive has claimed the route.");
    default: return TEXT("Two sets of keys hang beside a door that can favor only one cause.");
    }
}

FString AUnmadeLoreSite::GetInspectionText(UnmadeCore::DayPhase Phase, int StoryChoice) const
{
    const bool bNight = Phase == UnmadeCore::DayPhase::Night ||
                        Phase == UnmadeCore::DayPhase::Dusk;
    FString Line = bNight ? NightLine : DayLine;
    if (District == UnmadeCore::District::ShelterThreshold)
    {
        if (StoryChoice == 1) Line += TEXT(" The displaced have reclaimed the warm passage.");
        else if (StoryChoice == 2) Line += TEXT(" Researchers were given the passage; the displaced wait outside.");
        else Line += TEXT(" Nobody has agreed who should own the only intact passage.");
    }
    return FString::Printf(TEXT("%s — %s"), *SiteName, *Line);
}
