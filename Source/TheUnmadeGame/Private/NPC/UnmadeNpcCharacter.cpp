#include "NPC/UnmadeNpcCharacter.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AUnmadeNpcCharacter::AUnmadeNpcCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    Memory = CreateDefaultSubobject<UUnmadeMemoryComponent>(TEXT("PersonalMemory"));
    PlaceholderVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TemporaryNPCVisual"));
    PlaceholderVisual->SetupAttachment(GetCapsuleComponent());
    PlaceholderVisual->SetRelativeLocation(FVector(0, 0, -3));
    PlaceholderVisual->SetRelativeScale3D(FVector(0.6, 0.6, 1.55));
    PlaceholderVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (Mesh.Succeeded()) PlaceholderVisual->SetStaticMesh(Mesh.Object);
}

void AUnmadeNpcCharacter::ConfigureIdentity(FName StableId, const FString& DisplayLabel,
    UnmadeCore::NpcRole InRole, UnmadeCore::NpcTemperament InTemperament)
{
    NpcId = StableId;
    NpcDisplayLabel = DisplayLabel;
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
    return FString::Printf(TEXT("%s %s"), *NpcDisplayLabel, Line);
}
