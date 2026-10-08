#include "NPC/UnmadeMemoryComponent.h"

UUnmadeMemoryComponent::UUnmadeMemoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UUnmadeMemoryComponent::KnowsEvent(const FGuid& EventId) const
{
    if (!EventId.IsValid()) return false;
    for (const FUnmadeNpcObservation& Seen : Observations)
    {
        if (Seen.EventId == EventId) return true;
    }
    return false;
}

bool UUnmadeMemoryComponent::AddObservation(const FUnmadeNpcObservation& Observation)
{
    if (!Observation.EventId.IsValid() || Observation.EventKind.IsNone()) return false;
    for (FUnmadeNpcObservation& Existing : Observations)
    {
        if (Existing.EventId == Observation.EventId)
        {
            // A direct observation can correct hearsay, but hearsay never downgrades a witness.
            if (Existing.Evidence == EUnmadeEvidenceKind::Rumor
                && Observation.Evidence == EUnmadeEvidenceKind::Witnessed)
            {
                Existing = Observation;
                RecalculateReactions();
                return true;
            }
            return false;
        }
    }
    // Only keep the most recent salient observations in the first prototype.
    if (Observations.Num() >= 64) Observations.RemoveAt(0);
    Observations.Add(Observation);
    RecalculateReactions();
    return true;
}

bool UUnmadeMemoryComponent::Witness(const FGuid& EventId, FName EventKind)
{
    FUnmadeNpcObservation Entry;
    Entry.EventId = EventId;
    Entry.EventKind = EventKind;
    Entry.Evidence = EUnmadeEvidenceKind::Witnessed;
    return AddObservation(Entry);
}

bool UUnmadeMemoryComponent::HearRumor(const FGuid& EventId, FName EventKind, FName SpeakerId)
{
    if (SpeakerId.IsNone()) return false; // No anonymous omniscient rumors.
    FUnmadeNpcObservation Entry;
    Entry.EventId = EventId;
    Entry.EventKind = EventKind;
    Entry.Evidence = EUnmadeEvidenceKind::Rumor;
    Entry.SpeakerId = SpeakerId;
    return AddObservation(Entry);
}

void UUnmadeMemoryComponent::RecalculateReactions()
{
    Trust = 0;
    Fear = 0;
    for (const FUnmadeNpcObservation& Entry : Observations)
    {
        const bool bDirect = Entry.Evidence == EUnmadeEvidenceKind::Witnessed;
        if (Entry.EventKind == FName("Player.Helped"))
        {
            Trust += bDirect ? 25 : 10;
        }
        else if (Entry.EventKind == FName("Reality.Anomaly"))
        {
            Fear += bDirect ? 30 : 12;
        }
        else if (Entry.EventKind == FName("Player.Threatened"))
        {
            Trust -= bDirect ? 35 : 15;
            Fear += bDirect ? 20 : 8;
        }
    }
    Trust = FMath::Clamp(Trust, -100, 100);
    Fear = FMath::Clamp(Fear, 0, 100);
}

FUnmadeNpcSnapshot UUnmadeMemoryComponent::WriteSnapshot(FName NpcId) const
{
    FUnmadeNpcSnapshot Snapshot;
    Snapshot.NpcId = NpcId;
    Snapshot.Observations = Observations;
    Snapshot.Trust = Trust;
    Snapshot.Fear = Fear;
    return Snapshot;
}

bool UUnmadeMemoryComponent::ReadSnapshot(const FUnmadeNpcSnapshot& Snapshot, FName ExpectedNpcId)
{
    if (ExpectedNpcId.IsNone() || Snapshot.NpcId != ExpectedNpcId || Snapshot.Observations.Num() > 64)
        return false;

    TSet<FGuid> SeenIds;
    for (const FUnmadeNpcObservation& Item : Snapshot.Observations)
    {
        if (!Item.EventId.IsValid() || Item.EventKind.IsNone() || SeenIds.Contains(Item.EventId))
            return false;
        if (Item.Evidence == EUnmadeEvidenceKind::Rumor && Item.SpeakerId.IsNone())
            return false;
        SeenIds.Add(Item.EventId);
    }
    // Atomic in-memory restoration only after the entire snapshot validates.
    Observations = Snapshot.Observations;
    RecalculateReactions(); // Recompute instead of trusting inconsistent cached values.
    return true;
}
