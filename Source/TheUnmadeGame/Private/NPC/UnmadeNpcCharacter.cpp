#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "NPC/UnmadeNpcMotionRules.h"
#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeLivingWorldRules.h"
#include "Player/UnmadeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AUnmadeNpcCharacter::AUnmadeNpcCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 0.25f; // fixed-budget graybox NPC steering
    Memory = CreateDefaultSubobject<UUnmadeMemoryComponent>(TEXT("PersonalMemory"));
    PlaceholderVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TemporaryNPCVisual"));
    PlaceholderVisual->SetupAttachment(GetCapsuleComponent());
    PlaceholderVisual->SetRelativeLocation(FVector(0, 0, -3));
    PlaceholderVisual->SetRelativeScale3D(FVector(0.6, 0.6, 1.55));
    PlaceholderVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (Mesh.Succeeded()) PlaceholderVisual->SetStaticMesh(Mesh.Object);
}

void AUnmadeNpcCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UWorld* World = GetWorld();
    if (!World || NpcId.IsNone()) return;

    const AUnmadeCharacter* Player = Cast<AUnmadeCharacter>(
        UGameplayStatics::GetPlayerCharacter(World, 0));
    if (!IsValid(Player)) return;

    const FVector Current = GetActorLocation();
    const FVector PlayerPosition = Player->GetActorLocation();
    const bool bPlayerNearby = FVector::DistSquared2D(Current, PlayerPosition) < FMath::Square(550.f);
    const auto Action = DecideForPlayer(bPlayerNearby);

    UnmadeCore::NpcMotion Motion = UnmadeCore::NpcMotion::Stay;
    UnmadeCore::Vec2 Target{Current.X, Current.Y};
    double Speed = 70.0;
    double StopRadius = 120.0;

    if (!CachedHub.IsValid())
        CachedHub = Cast<AUnmadePrototypeHub>(
            UGameplayStatics::GetActorOfClass(World, AUnmadePrototypeHub::StaticClass()));
    const auto Phase = CachedHub.IsValid()
        ? CachedHub->GetCurrentPhase() : UnmadeCore::DayPhase::Day;

    switch (Action)
    {
    case UnmadeCore::NpcAction::InvestigateAnomaly:
    case UnmadeCore::NpcAction::ResearchAnomaly:
        Motion = UnmadeCore::NpcMotion::Approach;
        Target = UnmadeCore::LocalInvestigationTarget(HomeSettlement);
        Speed = 80.0;
        break;
    case UnmadeCore::NpcAction::VerifyRumor:
        Motion = UnmadeCore::NpcMotion::Approach;
        Target = UnmadeCore::LocalRumorTarget(HomeSettlement);
        break;
    case UnmadeCore::NpcAction::Intervene:
    case UnmadeCore::NpcAction::ShareKnowledge:
    case UnmadeCore::NpcAction::OfferAid:
        if (bPlayerNearby)
        {
            Motion = UnmadeCore::NpcMotion::Approach;
            Target = {PlayerPosition.X, PlayerPosition.Y};
            StopRadius = 175.0;
        }
        break;
    case UnmadeCore::NpcAction::AvoidPlayer:
    case UnmadeCore::NpcAction::RefuseTrade:
        if (bPlayerNearby)
        {
            Motion = UnmadeCore::NpcMotion::Retreat;
            Target = {PlayerPosition.X, PlayerPosition.Y};
            StopRadius = 360.0;
        }
        break;
    case UnmadeCore::NpcAction::DeliverMessage:
        // The courier now follows day-part waypoints, not a disconnected timer.
        break;
    default:
        break; // working at stall, patrolling in place, etc.
    }

    if (Motion == UnmadeCore::NpcMotion::Stay)
    {
        // Routine goals never replace urgent individual reactions to observed events.
        Motion = UnmadeCore::NpcMotion::Approach;
        Target = UnmadeCore::LocalRoutineTarget(
            HomeSettlement, Role, Phase, {HomeLocation.X, HomeLocation.Y});
        Speed = Role == UnmadeCore::NpcRole::Courier ? 95.0 : 65.0;
        StopRadius = 80.0;
    }

    const auto NewPosition = UnmadeCore::SteerNpc(
        {Current.X, Current.Y}, Target, Motion, Speed, DeltaSeconds, StopRadius);
    const FVector Offset(NewPosition.x - Current.X, NewPosition.y - Current.Y, 0.0);
    if (Offset.SizeSquared2D() > 0.01)
    {
        AddActorWorldOffset(Offset, true); // swept graybox collision, no teleporting
        SetActorRotation(Offset.Rotation());
    }
}

void AUnmadeNpcCharacter::ConfigureIdentity(FName StableId, const FString& DisplayLabel,
    UnmadeCore::NpcRole InRole, UnmadeCore::NpcTemperament InTemperament,
    UnmadeCore::SettlementId InVillage, const FString& InAuthoredLine)
{
    NpcId = StableId;
    NpcDisplayLabel = DisplayLabel;
    HomeLocation = GetActorLocation();
    HomeSettlement = InVillage;
    AuthoredLine = InAuthoredLine;
    Role = InRole;
    Temperament = InTemperament;
    Tags.AddUnique(StableId);
}

UnmadeCore::NpcAction AUnmadeNpcCharacter::DecideForPlayer(bool bPlayerNearby) const
{
    UnmadeCore::NpcDecisionInput Input;
    Input.role = Role;
    Input.temperament = Temperament;
    Input.playerNearby = bPlayerNearby;
    Input.trust = Memory->GetTrust();
    Input.fear = Memory->GetFear();
    for (const FUnmadeNpcObservation& Observation : Memory->GetObservations())
    {
        const bool bWitnessed = Observation.Evidence == EUnmadeEvidenceKind::Witnessed;
        if (Observation.EventKind == FName("World.ConflictShelter") ||
            Observation.EventKind == FName("World.ConflictResearch"))
        {
            const bool bShelter = Observation.EventKind == FName("World.ConflictShelter");
            const int Delta = UnmadeCore::SettlementTrustDelta(HomeSettlement, bShelter, bWitnessed);
            Input.trust += Delta;
            if (Delta < 0) Input.fear -= Delta;
        }
        if (Observation.EventKind == FName("Player.Threatened") && bWitnessed)
            Input.witnessedThreat = true;
        if (Observation.EventKind == FName("Reality.Anomaly"))
        {
            if (bWitnessed) Input.witnessedAnomaly = true;
            else Input.heardAnomaly = true;
        }
    }
    return UnmadeCore::ChooseNpcAction(Input);
}

FName AUnmadeNpcCharacter::GetCurrentActionId(bool bPlayerNearby) const
{
    using UnmadeCore::NpcAction;
    switch (DecideForPlayer(bPlayerNearby))
    {
    case NpcAction::OfferTrade: return FName("npc.action.offer_trade");
    case NpcAction::OfferDiscount: return FName("npc.action.offer_discount");
    case NpcAction::RefuseTrade: return FName("npc.action.refuse_trade");
    case NpcAction::AvoidPlayer: return FName("npc.action.avoid");
    case NpcAction::VerifyRumor: return FName("npc.action.verify_rumor");
    case NpcAction::InvestigateAnomaly: return FName("npc.action.investigate");
    case NpcAction::Intervene: return FName("npc.action.intervene");
    case NpcAction::ResearchAnomaly: return FName("npc.action.research");
    case NpcAction::ShareKnowledge: return FName("npc.action.share_knowledge");
    case NpcAction::DeliverMessage: return FName("npc.action.deliver");
    case NpcAction::OfferAid: return FName("npc.action.offer_aid");
    default: return FName("npc.action.routine");
    }
}

bool AUnmadeNpcCharacter::CanTradeWithPlayer() const
{
    using UnmadeCore::NpcAction;
    const auto Action = DecideForPlayer(true);
    return Role == UnmadeCore::NpcRole::Merchant &&
        (Action == NpcAction::OfferTrade || Action == NpcAction::OfferDiscount);
}

FString AUnmadeNpcCharacter::GetReactionText() const
{
    // Canonical authored dialogue never requires a model. AI can paraphrase,
    // but it cannot overwrite these deterministic permissions/choices.
    using UnmadeCore::NpcAction;
    const TCHAR* Line = TEXT("continues their routine, watching you quietly.");
    switch (DecideForPlayer(true))
    {
    case NpcAction::OfferTrade: Line = TEXT("offers ordinary goods and a cautious greeting."); break;
    case NpcAction::OfferDiscount: Line = TEXT("remembers your help and offers a better price."); break;
    case NpcAction::RefuseTrade: Line = TEXT("refuses to trade after what they witnessed."); break;
    case NpcAction::AvoidPlayer: Line = TEXT("steps back, unwilling to speak closely."); break;
    case NpcAction::VerifyRumor: Line = TEXT("asks witnesses to confirm what was said, rather than assuming it is true."); break;
    case NpcAction::InvestigateAnomaly: Line = TEXT("prepares to investigate the fracture they saw."); break;
    case NpcAction::Intervene: Line = TEXT("moves to intervene after personally witnessing a threat."); break;
    case NpcAction::ResearchAnomaly: Line = TEXT("examines the anomaly and begins recording observations."); break;
    case NpcAction::ShareKnowledge: Line = TEXT("recognizes your trustworthiness and offers a clue."); break;
    case NpcAction::DeliverMessage: Line = TEXT("keeps to their deliveries."); break;
    case NpcAction::OfferAid: Line = TEXT("offers to help you on the road."); break;
    default: break;
    }
    // Every one of the 48 residents has an authored personal line from their identity.
    FString CharacterLine = AuthoredLine;
    const auto Phase = CachedHub.IsValid()
        ? CachedHub->GetCurrentPhase() : UnmadeCore::DayPhase::Day;
    if (Phase == UnmadeCore::DayPhase::Night)
        CharacterLine += TEXT(" It feels like the dark has started listening.");

    FString BeliefLine;
    const TArray<FUnmadeNpcObservation>& Observations = Memory->GetObservations();
    for (int32 Index = Observations.Num() - 1; Index >= 0; --Index)
    {
        const FUnmadeNpcObservation& Event = Observations[Index];
        const bool bDirect = Event.Evidence == EUnmadeEvidenceKind::Witnessed;
        if (Event.EventKind == FName("World.ConflictShelter"))
            BeliefLine = bDirect ? TEXT("I saw you keep the shelter route open.")
                                 : TEXT("I heard you protected the shelter. Is it true?");
        else if (Event.EventKind == FName("World.ConflictResearch"))
            BeliefLine = bDirect ? TEXT("I witnessed the archive claim the passage.")
                                 : TEXT("Someone says the archive controls the passage.");
        else if (Event.EventKind == FName("Reality.Anomaly"))
            BeliefLine = bDirect ? TEXT("I saw the street move beneath your hand.")
                                 : TEXT("Someone mentioned a moving street. I did not see it.");
        if (!BeliefLine.IsEmpty()) break;
    }
    return FString::Printf(TEXT("%s %s %s %s"),
        *NpcDisplayLabel, Line, *CharacterLine, *BeliefLine);
}
