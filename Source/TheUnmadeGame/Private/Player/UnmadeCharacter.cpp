#include "Player/UnmadeCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Save/UnmadePrototypeSave.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AUnmadeCharacter::AUnmadeCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 340.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    PlaceholderBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypePlayerBody"));
    PlaceholderBody->SetupAttachment(GetCapsuleComponent());
    PlaceholderBody->SetRelativeLocation(FVector(0.f, 0.f, -3.f));
    PlaceholderBody->SetRelativeScale3D(FVector(.54f, .54f, 1.58f));
    PlaceholderBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (BodyMesh.Succeeded()) PlaceholderBody->SetStaticMesh(BodyMesh.Object);
}

void AUnmadeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Early bootstrap mappings. Replace with data-driven Enhanced Input contexts in an engine editor.
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUnmadeCharacter::Interact);
    PlayerInputComponent->BindAction("OfferAid", IE_Pressed, this, &AUnmadeCharacter::OfferAid);
    PlayerInputComponent->BindAction("AnomalyPulse", IE_Pressed, this, &AUnmadeCharacter::DemonstrateAnomaly);
    PlayerInputComponent->BindAxis("MoveForward", this, &AUnmadeCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AUnmadeCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AUnmadeCharacter::MoveForward(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator YawOnly(0.f, Controller->GetControlRotation().Yaw, 0.f);
    AddMovementInput(FRotationMatrix(YawOnly).GetUnitAxis(EAxis::X), Value);
}

void AUnmadeCharacter::MoveRight(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator YawOnly(0.f, Controller->GetControlRotation().Yaw, 0.f);
    AddMovementInput(FRotationMatrix(YawOnly).GetUnitAxis(EAxis::Y), Value);
}

namespace
{
AUnmadeNpcCharacter* FindNearbyCitizen(UWorld* World, FVector Origin, float MaxDistance)
{
    AUnmadeNpcCharacter* Nearest = nullptr;
    float BestDistSq = FMath::Square(MaxDistance);
    for (TActorIterator<AUnmadeNpcCharacter> It(World); It; ++It)
    {
        const float DistSq = FVector::DistSquared(Origin, It->GetActorLocation());
        if (DistSq < BestDistSq)
        {
            Nearest = *It;
            BestDistSq = DistSq;
        }
    }
    return Nearest;
}
}

void AUnmadeCharacter::Interact()
{
    AUnmadeNpcCharacter* Target = FindNearbyCitizen(GetWorld(), GetActorLocation(), 260.f);
    const FString Line = Target ? Target->GetReactionText() : TEXT("Nobody close enough to speak to.");
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Cyan, Line);
}

void AUnmadeCharacter::OfferAid()
{
    AUnmadeNpcCharacter* Target = FindNearbyCitizen(GetWorld(), GetActorLocation(), 260.f);
    if (!Target)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Approach a resident before offering aid."));
        return;
    }
    for (const FUnmadeNpcObservation& Observation : Target->GetMemory()->GetObservations())
    {
        if (Observation.EventKind == FName("Player.Helped"))
        {
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                TEXT("You have already helped this resident."));
            return;
        }
    }
    ReportLocalEvent(FName("Player.Helped"));
}

void AUnmadeCharacter::DemonstrateAnomaly()
{
    // Prototype-only reaction signal. This is not a completed reality-fracture ability.
    bool bNearMarker = false;
    for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
    {
        if (It->ActorHasTag(FName("Hub.AnomalyMarker"))
            && FVector::DistSquared(It->GetActorLocation(), GetActorLocation()) < FMath::Square(360.f))
        {
            bNearMarker = true;
            break;
        }
    }
    if (!bNearMarker)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Move toward the strange marker to investigate the anomaly."));
        return;
    }
    ReportLocalEvent(FName("Reality.Anomaly"));
}

void AUnmadeCharacter::ReportLocalEvent(FName EventKind)
{
    UWorld* World = GetWorld();
    if (!World) return;
    const FGuid EventId = FGuid::NewGuid();
    int32 Witnesses = 0;
    for (TActorIterator<AUnmadeNpcCharacter> It(World); It; ++It)
    {
        if (FVector::DistSquared(GetActorLocation(), It->GetActorLocation()) > FMath::Square(720.f))
            continue;

        // Simple sight approximation: solid level geometry blocks direct observation.
        FCollisionQueryParams VisibilityParams(SCENE_QUERY_STAT(UnmadeNPCWitness), false);
        VisibilityParams.AddIgnoredActor(this);
        VisibilityParams.AddIgnoredActor(*It);
        const FVector Observer = It->GetActorLocation() + FVector(0, 0, 70);
        const FVector Performer = GetActorLocation() + FVector(0, 0, 70);
        if (World->LineTraceTestByChannel(Observer, Performer, ECC_Visibility, VisibilityParams))
            continue;
        if (It->GetMemory()->Witness(EventId, EventKind)) ++Witnesses;
    }
    SaveNearbyNpcMemories();
    const FString Line = FString::Printf(TEXT("%s witnessed by %d nearby resident(s)."),
        *EventKind.ToString(), Witnesses);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Yellow, Line);
}

void AUnmadeCharacter::SaveNearbyNpcMemories()
{
    UWorld* World = GetWorld();
    if (!World) return;
    UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(
        UGameplayStatics::CreateSaveGameObject(UUnmadePrototypeSave::StaticClass()));
    if (!Save) return;
    for (TActorIterator<AUnmadeNpcCharacter> It(World); It; ++It)
    {
        if (!It->GetStableId().IsNone())
            Save->NpcSnapshots.Add(It->GetMemory()->WriteSnapshot(It->GetStableId()));
    }
    if (!UGameplayStatics::SaveGameToSlot(Save, TEXT("UnmadePrototypeNPC"), 0))
    {
        UE_LOG(LogTemp, Error, TEXT("Unmade prototype NPC memory save failed"));
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red,
            TEXT("Could not save NPC memories."));
    }
}
