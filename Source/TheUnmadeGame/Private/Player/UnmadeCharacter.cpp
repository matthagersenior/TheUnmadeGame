#include "Player/UnmadeCharacter.h"
#include "Fracture/UnmadeFractureAnchor.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Lexicon/UnmadeLexiconComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
#include "Dialogue/UnmadeLocalDialogueSubsystem.h"
#include "Engine/GameInstance.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Save/UnmadePrototypeSave.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AUnmadeCharacter::AUnmadeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    Combat = CreateDefaultSubobject<UUnmadeCombatComponent>(TEXT("Combat"));
    Lexicon = CreateDefaultSubobject<UUnmadeLexiconComponent>(TEXT("Lexicon"));
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
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AUnmadeCharacter::AttemptMeleeAttack);
    PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &AUnmadeCharacter::StartGuard);
    PlayerInputComponent->BindAction("Guard", IE_Released, this, &AUnmadeCharacter::StopGuard);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUnmadeCharacter::Interact);
    PlayerInputComponent->BindAction("OfferAid", IE_Pressed, this, &AUnmadeCharacter::OfferAid);
    PlayerInputComponent->BindAction("AnomalyPulse", IE_Pressed, this, &AUnmadeCharacter::DemonstrateAnomaly);
    PlayerInputComponent->BindAction("FoldReality", IE_Pressed, this, &AUnmadeCharacter::FoldReality);
    PlayerInputComponent->BindAction("RewriteOpen", IE_Pressed, this, &AUnmadeCharacter::RewriteOpen);
    PlayerInputComponent->BindAction("RewriteSealed", IE_Pressed, this, &AUnmadeCharacter::RewriteSealed);
    PlayerInputComponent->BindAxis("MoveForward", this, &AUnmadeCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AUnmadeCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AUnmadeCharacter::AttemptMeleeAttack()
{
    UWorld* World = GetWorld();
    if (!World || Combat->IsDefeated()) return;
    AUnmadeEnemyCharacter* Target = nullptr;
    double BestDistanceSq = FMath::Square(240.0);
    const FVector Facing = GetActorForwardVector().GetSafeNormal2D();
    for (TActorIterator<AUnmadeEnemyCharacter> It(World); It; ++It)
    {
        if (It->GetCombat()->IsDefeated()) continue;
        FVector Toward = It->GetActorLocation() - GetActorLocation();
        Toward.Z = 0;
        const double DistSq = Toward.SizeSquared();
        if (DistSq >= BestDistanceSq || FVector::DotProduct(Facing, Toward.GetSafeNormal()) < 0.2)
            continue;
        FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeMeleeSight), false);
        Params.AddIgnoredActor(this);
        Params.AddIgnoredActor(*It);
        if (World->LineTraceTestByChannel(GetActorLocation() + FVector(0, 0, 60),
            It->GetActorLocation() + FVector(0, 0, 60), ECC_Visibility, Params))
            continue;
        Target = *It;
        BestDistanceSq = DistSq;
    }
    if (!IsValid(Target))
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Silver,
            TEXT("No enemy within melee reach and facing."));
        return;
    }
    const float Before = Target->GetCombat()->GetHealth();
    const double Now = World->GetTimeSeconds();
    if (Combat->TryStrikeTarget(Target->GetCombat(), Now, true,
        Target->IsFractureExposed(Now)))
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
            FString::Printf(TEXT("HIT: %.0f damage, enemy %.0f health."),
                Before - Target->GetCombat()->GetHealth(), Target->GetCombat()->GetHealth()));
        ReportLocalEvent(FName("Player.Fought"), FName("combat.prototype.enemy"));
    }
}

void AUnmadeCharacter::StartGuard()
{
    if (IsValid(Combat)) Combat->SetGuarding(true);
}

void AUnmadeCharacter::StopGuard()
{
    if (IsValid(Combat)) Combat->SetGuarding(false);
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
    if (!Target)
    {
        if (IsValid(FindNearbyFractureAnchor()) && GEngine)
        {
            const FString Inscription = Lexicon && Lexicon->UnderstandsVeyl()
                ? TEXT("The inscription: VEYL - the way that remains. It marks a surviving passage.")
                : TEXT("The inscription is unfamiliar. Investigate it with Glimpse and ask the archivist.");
            GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Cyan, Inscription);
        }
        else if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Cyan,
                TEXT("Nobody close enough to speak to."));
        }
        return;
    }
    if (Lexicon && Target->GetStableId() == FName("npc.archivist.001"))
        Lexicon->RecordEvidence(FName("evidence.archivist"));
    UUnmadeLocalDialogueSubsystem* Dialogue = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UUnmadeLocalDialogueSubsystem>()
        : nullptr;
    if (Dialogue)
    {
        // Placeholder greeting until the in-game dialogue UI captures player speech.
        Dialogue->RequestDialogue(Target, TEXT("Hello."));
    }
    else if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Cyan, Target->GetReactionText());
    }
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
        if (Observation.EventKind == FName("Player.Helped") && Observation.SubjectId == Target->GetStableId())
        {
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                TEXT("You have already helped this resident."));
            return;
        }
    }
    ReportLocalEvent(FName("Player.Helped"), Target->GetStableId());
}

void AUnmadeCharacter::DemonstrateAnomaly()
{
    AUnmadeFractureAnchor* Anchor = FindNearbyFractureAnchor();
    if (!GetWorld()) return;
    const auto Result = FractureModel.Glimpse(IsValid(Anchor), GetWorld()->GetTimeSeconds());
    if (Result != UnmadeCore::Result::Applied)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Glimpse unavailable. Get closer to the fracture or recover Strain."));
        return;
    }
    ApplyFractureVisuals();
    if (Lexicon) Lexicon->RecordEvidence(FName("evidence.glimpse"));
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        TEXT("GLIMPSE: another possible version flickers into view (+8 Strain)."));
    ReportLocalEvent(FName("Reality.Anomaly"), FName("region.prototype.hub"));
    SaveFractureState();
}

void AUnmadeCharacter::FoldReality()
{
    AUnmadeFractureAnchor* Anchor = FindNearbyFractureAnchor();
    if (!GetWorld()) return;
    const auto Result = FractureModel.Fold(IsValid(Anchor), GetWorld()->GetTimeSeconds());
    if (Result != UnmadeCore::Result::Applied)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Fold unavailable: already active, out of range, or Strain too high."));
        return;
    }
    ApplyFractureVisuals();
    for (TActorIterator<AUnmadeEnemyCharacter> It(GetWorld()); It; ++It)
    {
        if (!It->GetCombat()->IsDefeated() &&
            FVector::DistSquared(It->GetActorLocation(), Anchor->GetActorLocation()) < FMath::Square(800.f))
            It->ExposeToFold(GetWorld()->GetTimeSeconds(), 4.0);
    }
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        TEXT("FOLD: the barrier vanishes for six seconds (+24 Strain)."));
    ReportLocalEvent(FName("Reality.Anomaly"), FName("region.prototype.hub"));
    SaveFractureState();
}

void AUnmadeCharacter::RewriteOpen()
{
    RewriteChoice(FName("variant.open"));
}

void AUnmadeCharacter::RewriteSealed()
{
    RewriteChoice(FName("variant.sealed"));
}

void AUnmadeCharacter::RewriteChoice(FName ChoiceId)
{
    if (!GetWorld() || !IsValid(FindNearbyFractureAnchor()))
    {
        PendingRewriteChoice = NAME_None;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Approach the anomaly before attempting Rewrite."));
        return;
    }
    const double Now = GetWorld()->GetTimeSeconds();
    const FString ChoiceString = ChoiceId.ToString();
    const std::string Choice(TCHAR_TO_UTF8(*ChoiceString));
    const bool bConfirmed = PendingRewriteChoice == ChoiceId && Now <= PendingRewriteExpiresAt;
    if (!bConfirmed)
    {
        const auto Preview = FractureModel.Rewrite("region.prototype.hub", Choice, false);
        PendingRewriteChoice = NAME_None;
        if (Preview != UnmadeCore::Result::NeedsConfirmation)
        {
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                TEXT("This world has already been committed or the choice is unavailable."));
            return;
        }
        PendingRewriteChoice = ChoiceId;
        PendingRewriteExpiresAt = Now + 6.0;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Orange,
            FString::Printf(TEXT("PERMANENT REWRITE %s (+60 Strain). Press the same key again within 6 seconds to confirm."), *ChoiceString));
        return;
    }
    PendingRewriteChoice = NAME_None;
    const UnmadeCore::FractureSnapshot Previous = FractureModel.TakeSnapshot();
    const auto Result = FractureModel.Rewrite("region.prototype.hub", Choice, true);
    if (Result != UnmadeCore::Result::Applied)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Rewrite refused. Strain may be too high."));
        return;
    }
    ApplyFractureVisuals();
    if (!SaveFractureState())
    {
        FractureModel.Restore(Previous);
        ApplyFractureVisuals();
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red,
            TEXT("Rewrite failed to save: world and Strain restored."));
        return;
    }
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Yellow,
        FString::Printf(TEXT("REWRITE COMMITTED: %s. This outcome persists across sessions."), *ChoiceString));
    ReportLocalEvent(FName("Reality.Anomaly"), FName("region.prototype.hub"));
}

AUnmadeFractureAnchor* AUnmadeCharacter::FindNearbyFractureAnchor() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;
    for (TActorIterator<AUnmadeFractureAnchor> It(World); It; ++It)
    {
        if (FVector::DistSquared(GetActorLocation(), It->GetActorLocation()) <= FMath::Square(490.f))
            return *It;
    }
    return nullptr;
}

void AUnmadeCharacter::ApplyFractureVisuals()
{
    if (!GetWorld()) return;
    const double Now = GetWorld()->GetTimeSeconds();
    const FString VariantString(UTF8_TO_TCHAR(FractureModel.WorldVariant().c_str()));
    for (TActorIterator<AUnmadeFractureAnchor> It(GetWorld()); It; ++It)
    {
        It->ApplyFractureState(FractureModel.IsGlimpsing(Now),
            FractureModel.IsFolded(Now), FName(*VariantString));
    }
}

bool AUnmadeCharacter::SaveFractureState()
{
    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return false;
    const auto Snapshot = FractureModel.TakeSnapshot();
    Save->WorldVariantId = Snapshot.variant.empty()
        ? NAME_None : FName(UTF8_TO_TCHAR(Snapshot.variant.c_str()));
    Save->PlayerStrain = Snapshot.strain;
    Save->bHasFractureSnapshot = true;
    return UGameplayStatics::SaveGameToSlot(Save, TEXT("UnmadePrototypeNPC"), 0);
}

void AUnmadeCharacter::BeginPlay()
{
    Super::BeginPlay();
    const UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"), 0));
    if (Save && Save->SchemaVersion == 1 && Save->bHasFractureSnapshot)
    {
        const FString Variant = Save->WorldVariantId.IsNone()
            ? TEXT("") : Save->WorldVariantId.ToString();
        if (!FractureModel.Restore({std::string(TCHAR_TO_UTF8(*Variant)), Save->PlayerStrain}))
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid fracture save ignored"));
        }
    }
    ApplyFractureVisuals();
}

void AUnmadeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    FractureModel.Recover(static_cast<double>(DeltaSeconds));
    if (Combat->IsDefeated() && !bPlayerDefeatHandled)
    {
        bPlayerDefeatHandled = true;
        GetCharacterMovement()->DisableMovement();
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red,
            TEXT("You fell in the prototype encounter. Checkpoint/respawn is not implemented yet."));
    }
    ApplyFractureVisuals();
}

void AUnmadeCharacter::ReportLocalEvent(FName EventKind, FName SubjectId)
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
        if (It->GetMemory()->Witness(EventId, EventKind, SubjectId)) ++Witnesses;
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
    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return;
    Save->NpcSnapshots.Reset(); // Keep one snapshot per stable NPC ID.
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
