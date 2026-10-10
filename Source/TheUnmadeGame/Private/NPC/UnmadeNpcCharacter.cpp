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
        Target = (bFrontierResident || bLaterRealmResident) ?
            UnmadeCore::Vec2{HomeLocation.X+130,HomeLocation.Y+300}
            : UnmadeCore::LocalInvestigationTarget(HomeSettlement);
        Speed = 80.0;
        break;
    case UnmadeCore::NpcAction::VerifyRumor:
        Motion = UnmadeCore::NpcMotion::Approach;
        Target = (bFrontierResident || bLaterRealmResident) ?
            UnmadeCore::Vec2{HomeLocation.X-130,HomeLocation.Y+150}
            : UnmadeCore::LocalRumorTarget(HomeSettlement);
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
        Target = (bFrontierResident || bLaterRealmResident) ?
            (Phase == UnmadeCore::DayPhase::Night
                ? UnmadeCore::Vec2{HomeLocation.X+160,HomeLocation.Y-160}
                : UnmadeCore::Vec2{HomeLocation.X,HomeLocation.Y})
            : UnmadeCore::LocalRoutineTarget(
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

void AUnmadeNpcCharacter::ConfigureFrontier(const UnmadeCore::FrontierResident& Resident)
{
    ConfigureIdentity(FName(UTF8_TO_TCHAR(Resident.id)),
        FString(UTF8_TO_TCHAR(Resident.name)),Resident.role,Resident.temperament,
        UnmadeCore::SettlementId::Crossings,FString(UTF8_TO_TCHAR(Resident.authoredLine)));
    bFrontierResident=true; // use independent home-based frontier routines, not Crossings
}

void AUnmadeNpcCharacter::ConfigureLaterRealm(
    const UnmadeCore::LaterRealmSpec& Realm,const char* Identity,
    const FString& Display,UnmadeCore::NpcRole Role,
    UnmadeCore::NpcTemperament Temperament,const char* Line)
{
    if(!Identity || !Line)return;
    ConfigureIdentity(FName(UTF8_TO_TCHAR(Identity)),Display,Role,Temperament,
                      UnmadeCore::SettlementId::None,
                      FString(UTF8_TO_TCHAR(Line)));
    bLaterRealmResident=true;
    LaterHome=Realm.realm;
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
            const int Delta = bFrontierResident ? 0
                : UnmadeCore::SettlementTrustDelta(HomeSettlement, bShelter, bWitnessed);
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
    // A borrowed life affects what people notice, without pretending they know
    // a generated backstory or overwriting this individual's memories.
    if(GetWorld())
    {
        const AUnmadeCharacter* Visitor=Cast<AUnmadeCharacter>(
            UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
        if(IsValid(Visitor) && Visitor->IsBorrowedLifeActive() &&
           FVector::DistSquared(Visitor->GetActorLocation(),GetActorLocation())<FMath::Square(500.f))
            CharacterLine+=Visitor->GetBorrowedLifeRole()==0
                ? TEXT(" You stand like the soldier who guarded this street in a history I never lived.")
                : Visitor->GetBorrowedLifeRole()==1
                ? TEXT(" Your hands carry the marks of a craft you never apprenticed in.")
                : TEXT(" You speak as though you have catalogued a century that never happened.");
    }

    // The resident's own community records what the player has accomplished.
    if (CachedHub.IsValid())
    {
        const auto Progress = CachedHub->GetRegionalTask(HomeSettlement);
            const auto Afterlight=CachedHub->GetAfterlightBenefit();
            if(HomeSettlement==UnmadeCore::SettlementId::Bellwold &&
               Afterlight.worldTag && Afterlight.worldTag[0]!='\0')
            {
                CharacterLine+=TEXT(" ");
                CharacterLine+=FString(UTF8_TO_TCHAR(Afterlight.description));
            }

        if (HomeSettlement == UnmadeCore::SettlementId::Bellwold &&
            Progress == UnmadeCore::TaskProgress::Completed)
            CharacterLine += TEXT(" Our lanterns now belong to every household.");
        else if (HomeSettlement == UnmadeCore::SettlementId::Paperhaven &&
                 Progress == UnmadeCore::TaskProgress::Completed)
            CharacterLine += TEXT(" The rescued testimony is part of our record now.");
    }

    if(bLaterRealmResident && CachedHub.IsValid())
    {
        const int32 Initial=CachedHub->GetLaterRealmOutcome(LaterHome);
        if(Initial!=0)
        {
            const auto* Home=UnmadeCore::FindLaterRealm(LaterHome);
            if(Home)
            {
                CharacterLine+=TEXT(" ");
                CharacterLine+=FString(UTF8_TO_TCHAR(Initial==1?
                    Home->trialChoiceA:Home->trialChoiceB));
            }
        }
        const int32 After=CachedHub->GetRealmAftermathEnding(LaterHome);
        if(After!=0)
        {
            const auto& S=UnmadeCore::RealmAftermathSpecs[static_cast<int>(LaterHome)];
            CharacterLine+=TEXT(" ");
            CharacterLine+=FString(UTF8_TO_TCHAR(After==1?S.careConsequence:S.truthConsequence));
        }
    }

    if(CachedHub.IsValid())
    {
        // Only publicly observable changes in the NPC's own home town.
        // Rumors and firsthand player acts remain separate personal memories.
        UnmadeCore::Community Home=UnmadeCore::Community::Count;
        if(bFrontierResident)
        {
            const FString OwnId=GetStableId().ToString();
            Home=OwnId.StartsWith(TEXT("npc.saltwake."))
                ? UnmadeCore::Community::Saltwake
                : UnmadeCore::Community::Cinderhold;
        }
        else if(HomeSettlement!=UnmadeCore::SettlementId::None)
            Home=static_cast<UnmadeCore::Community>(static_cast<int32>(HomeSettlement));
        if(bFrontierResident)
        {
            const auto Realm=Home==UnmadeCore::Community::Saltwake
                ?UnmadeCore::Realm::WidowedRain:UnmadeCore::Realm::HearthBeneath;
            const int32 Ending=CachedHub->GetRealmAftermathEnding(Realm);
            if(Ending!=0)
            {
                const auto& Spec=UnmadeCore::RealmAftermathSpecs[static_cast<int32>(Realm)];
                CharacterLine+=TEXT(" ");
                CharacterLine+=FString(UTF8_TO_TCHAR(
                    Ending==1?Spec.careConsequence:Spec.truthConsequence));
            }
        }
        if(Home!=UnmadeCore::Community::Count)
        {
            const auto Outcome=CachedHub->GetCommunityOutcome(Home);
            if(Outcome.state!=UnmadeCore::CommunityState::Uncertain)
            {
                CharacterLine+=TEXT(" ");
                CharacterLine+=FString(UTF8_TO_TCHAR(Outcome.visibleChange));
            }
        }
    }

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
        else if (Event.EventKind == FName("World.AfterlightShelter"))
            BeliefLine = bDirect
                ? TEXT("I watched the second-night lamps open a ward for families who had no place in the census.")
                : TEXT("I heard Hessa found room for the families whose names went missing.");
        else if (Event.EventKind == FName("World.AfterlightNames"))
            BeliefLine = bDirect
                ? TEXT("I saw the erased names carved where the registrars could not hide them.")
                : TEXT("They say the names are public now. I will go and look for my own.");
        else if (Event.EventKind == FName("World.SaltwakeCistern"))
            BeliefLine = bDirect ? TEXT("I saw the sluice opened and the shared cistern funded.")
                                 : TEXT("I heard the storm's water was made common. I did not see the decision.");
        else if (Event.EventKind == FName("World.SaltwakeLedger"))
            BeliefLine = bDirect ? TEXT("I witnessed the false rain prices released to the port.")
                                 : TEXT("They say the harbor invoices lied. I want to examine them.");
        else if (Event.EventKind == FName("World.CinderholdKiln"))
            BeliefLine = bDirect ? TEXT("I saw the vent opened and the shared kiln given to the households.")
                                 : TEXT("I heard there is now a common fire for the coldest homes.");
        else if (Event.EventKind == FName("World.CinderholdDeed"))
            BeliefLine = bDirect ? TEXT("I watched the ember deed exposed in the reopened passage.")
                                 : TEXT("Someone says the old ownership claim was forged.");
        else if (Event.EventKind == FName("World.LaterRealmResolved"))
            BeliefLine = bDirect
                ? TEXT("I saw you commit to the new road here. We will have to live with what follows.")
                : TEXT("I heard the traveler changed another realm. I will ask who witnessed it.");
        else if (Event.EventKind == FName("World.RiteProtect"))
            BeliefLine = bDirect ? TEXT("I saw you choose to protect the people from a power that could have harmed them.")
                                 : TEXT("Some say you put the villagers first. I hope they are right.");
        else if (Event.EventKind == FName("World.RiteReveal"))
            BeliefLine = bDirect ? TEXT("I watched you reveal what the authorities wanted hidden.")
                                 : TEXT("They say you released a difficult truth. Did you?");
        else if (Event.EventKind == FName("World.BrokenOath"))
            BeliefLine = bDirect ? TEXT("I watched you abandon a sworn shelter promise.")
                                 : TEXT("People say an oath was broken. I need to know what happened.");
        else if (Event.EventKind == FName("World.RedeemedOath"))
            BeliefLine = bDirect ? TEXT("I saw the hard work of making an oath right again.")
                                 : TEXT("They say the refuge accepted restitution, though the scar remains.");
        if (!BeliefLine.IsEmpty()) break;
    }
    return FString::Printf(TEXT("%s %s %s %s"),
        *NpcDisplayLabel, Line, *CharacterLine, *BeliefLine);
}
