#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadePrototypeHub.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AUnmadeEnemyCharacter::AUnmadeEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    Combat = CreateDefaultSubobject<UUnmadeCombatComponent>(TEXT("Combat"));
    TemporaryEnemyVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeEnemyVisual"));
    TemporaryEnemyVisual->SetupAttachment(GetCapsuleComponent());
    TemporaryEnemyVisual->SetRelativeLocation(FVector(0, 0, -2));
    TemporaryEnemyVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded()) TemporaryEnemyVisual->SetStaticMesh(Cube.Object);
    TemporaryEnemyVisual->SetRelativeScale3D(FVector(0.65, 0.65, 1.2));
    EncounterCrest=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CreatureCrest"));
    EncounterCrest->SetupAttachment(GetCapsuleComponent());
    EncounterCrest->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if(Sphere.Succeeded())EncounterCrest->SetStaticMesh(Sphere.Object);
    EncounterLabel=CreateDefaultSubobject<UTextRenderComponent>(TEXT("CreatureName"));
    EncounterLabel->SetupAttachment(GetCapsuleComponent());
    EncounterLabel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EncounterLabel->SetRelativeLocation(FVector(0,0,125));
    EncounterLabel->SetHorizontalAlignment(EHTA_Center);
    EncounterLabel->SetWorldSize(22.f);
}

void AUnmadeEnemyCharacter::ConfigureStyle(UnmadeCore::EnemyStyle NewStyle)
{
    Style = NewStyle;
    if (Style == UnmadeCore::EnemyStyle::Watcher)
    {
        Combat->Configure(65.0, 10.0, 2.25);
        TemporaryEnemyVisual->SetRelativeScale3D(FVector(1.0, 1.0, 0.8));
    }
    else
    {
        Combat->Configure(85.0, 14.0, 1.4);
        TemporaryEnemyVisual->SetRelativeScale3D(FVector(0.65, 0.65, 1.2));
    }
}

void AUnmadeEnemyCharacter::ConfigureEncounter(FName Identity,const FString& Name,
    const FLinearColor& Accent)
{
    if(Identity.IsNone()||Name.IsEmpty())return;
    Tags.AddUnique(Identity);
    EncounterLabel->SetText(FText::FromString(Name));
    EncounterLabel->SetTextRenderColor(Accent.ToFColor(true));
    const bool bWatch=Style==UnmadeCore::EnemyStyle::Watcher;
    EncounterCrest->SetRelativeLocation(bWatch?FVector(0,0,95):FVector(38,0,45));
    EncounterCrest->SetRelativeScale3D(bWatch?FVector(.18,.5,.55):FVector(.48,.43,.3));
    if(UMaterialInstanceDynamic* Body=TemporaryEnemyVisual->CreateAndSetMaterialInstanceDynamic(0))
        Body->SetVectorParameterValue(TEXT("Color"),Accent);
}

void AUnmadeEnemyCharacter::SetVisualScale(FVector Scale)
{
    if(IsValid(TemporaryEnemyVisual))
        TemporaryEnemyVisual->SetRelativeScale3D(Scale);
}

void AUnmadeEnemyCharacter::ExposeToFold(double Now, double Seconds)
{
    if (FMath::IsFinite(Now) && Now >= 0.0 && Seconds > 0.0)
        FoldExposedUntil = FMath::Max(FoldExposedUntil, Now + Seconds);
}

bool AUnmadeEnemyCharacter::IsFractureExposed(double Now) const
{
    return FMath::IsFinite(Now) && Now >= 0.0 && Now < FoldExposedUntil;
}

void AUnmadeEnemyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!Combat->IsDefeated() && bDeathHandled) return;
    if (Combat->IsDefeated())
    {
        if (!bDeathHandled)
        {
            bDeathHandled = true;
            SetActorEnableCollision(false);
            SetActorHiddenInGame(true); // Prototype defeat; no respawn/loot yet.
            SetActorTickEnabled(false);
        }
        return;
    }

    UWorld* World = GetWorld();
    AUnmadeCharacter* Player = World
        ? Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0))
        : nullptr;
    if (!IsValid(Player) || !IsValid(Player->GetCombat()) || Player->GetCombat()->IsDefeated())
        return;

    if(!CachedRealityHub.IsValid())
        CachedRealityHub=Cast<AUnmadePrototypeHub>(
            UGameplayStatics::GetActorOfClass(World,AUnmadePrototypeHub::StaticClass()));
    const auto Consequence=CachedRealityHub.IsValid()
        ?CachedRealityHub->RealityConsequenceAt(GetActorLocation())
        :UnmadeCore::RealityConsequenceView{};
    FVector Delta = Player->GetActorLocation() - GetActorLocation();
    Delta.Z = 0;
    const double DistanceCm = Delta.Size();
    if (DistanceCm > 1800.0 || DistanceCm < 1.0) return;
    const FVector Direction = Delta.GetSafeNormal();

    FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeEnemySight), false);
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(Player);
    const bool bClearSight = !World->LineTraceTestByChannel(
        GetActorLocation() + FVector(0, 0, 55),
        Player->GetActorLocation() + FVector(0, 0, 55),
        ECC_Visibility, Params);
    const auto Intent = UnmadeCore::ChooseEnemyIntent(
        Style, DistanceCm / 100.0, bClearSight);

    if (Intent == UnmadeCore::EnemyIntent::Approach ||
        Intent == UnmadeCore::EnemyIntent::Retreat)
    {
        const float BaseSpeed = Style == UnmadeCore::EnemyStyle::Stalker ? 130.f : 95.f;
        // Exact same time-bound consequence that changes the Crossings scene
        // changes enemy closing/retreat speed. No permanent hidden buff.
        const float Speed = BaseSpeed *
            (static_cast<float>(Consequence.enemySpeedPercent)/100.f);
        const float Sign = Intent == UnmadeCore::EnemyIntent::Retreat ? -1.f : 1.f;
        // Swept root movement is only a graybox stand-in for navmesh-backed AI.
        AddActorWorldOffset(Direction * Speed * Sign * FMath::Max(DeltaSeconds, 0.f), true);
    }
    SetActorRotation(Direction.Rotation());

    if (Intent == UnmadeCore::EnemyIntent::Attack && bClearSight)
    {
        const float Before = Player->GetCombat()->GetHealth();
        if (Combat->TryStrikeTarget(Player->GetCombat(), World->GetTimeSeconds(), true, false) &&
            GEngine)
        {
            const FString AttackType = Style == UnmadeCore::EnemyStyle::Stalker
                ? TEXT("Stalker strike") : TEXT("Watcher pulse (prototype ranged impact)");
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
                FString::Printf(TEXT("%s: -%.0f health (remaining %.0f)"),
                    *AttackType, Before - Player->GetCombat()->GetHealth(),
                    Player->GetCombat()->GetHealth()));
        }
    }
}
