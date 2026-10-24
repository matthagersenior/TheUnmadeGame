#include "Dialogue/UnmadeDialoguePolicy.h"

#include "NPC/UnmadeMemoryComponent.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

FString FUnmadeDialoguePolicy::BuildEvidence(const UUnmadeMemoryComponent* Memory)
{
    if (!IsValid(Memory))
    {
        return TEXT("You have no recorded memories about the player.");
    }

    FString Result = FString::Printf(
        TEXT("Current trust: %d; current fear: %d. These are private feelings, not facts about the world.\n"),
        Memory->GetTrust(), Memory->GetFear());

    const TArray<FUnmadeNpcObservation>& Events = Memory->GetObservations();
    const int32 First = FMath::Max(0, Events.Num() - MaxContextEvents);
    if (Events.Num() == 0)
    {
        Result += TEXT("No directly witnessed or reported events known.\n");
    }
    for (int32 Index = First; Index < Events.Num(); ++Index)
    {
        const FUnmadeNpcObservation& Event = Events[Index];
        const FString Subject = Event.SubjectId.IsNone()
            ? TEXT("unknown subject") : Event.SubjectId.ToString();
        if (Event.Evidence == EUnmadeEvidenceKind::Rumor)
        {
            Result += FString::Printf(
                TEXT("Heard rumor from %s (NOT witnessed): %s about %s.\n"),
                *Event.SpeakerId.ToString(), *Event.EventKind.ToString(), *Subject);
        }
        else
        {
            Result += FString::Printf(
                TEXT("Personally witnessed: %s about %s.\n"),
                *Event.EventKind.ToString(), *Subject);
        }
    }
    return Result;
}

FString FUnmadeDialoguePolicy::BuildOfflineReply(const UUnmadeMemoryComponent* Memory,
                                                 const FString& PlayerUtterance,
                                                 const FString& CharacterVoice,
                                                 const FString& AuthoredPortrayalReply)
{
    const FString Asked = PlayerUtterance.ToLower();
    const bool bAskMemory = Asked.Contains(TEXT("remember")) ||
                            Asked.Contains(TEXT("memory")) ||
                            Asked.Contains(TEXT("about me"));
    const bool bAskConcern = Asked.Contains(TEXT("worry")) || Asked.Contains(TEXT("concern"));
    const bool bAskWitness = Asked.Contains(TEXT("witness")) || Asked.Contains(TEXT("personally")) ||
                             Asked.Contains(TEXT("seen")) || Asked.Contains(TEXT("saw"));
    if (!IsValid(Memory))
        return AuthoredPortrayalReply.IsEmpty()?CharacterVoice:AuthoredPortrayalReply;

    auto Describe = [](FName Kind) -> const TCHAR*
    {
        if (Kind == FName("Player.Helped")) return TEXT("you helped someone nearby");
        if (Kind == FName("Player.HelpedVillage")) return TEXT("you helped people in this settlement");
        if (Kind == FName("Player.DeliveredSupplies")) return TEXT("you delivered supplies");
        if (Kind == FName("Player.Threatened")) return TEXT("you threatened someone");
        if (Kind == FName("World.ConflictResearch")) return TEXT("the residents chose to research the danger");
        if (Kind == FName("World.ConflictShelter")) return TEXT("the residents chose to shelter people");
        if (Kind == FName("World.LocalRepair")) return TEXT("the local damage was repaired");
        if (Kind == FName("World.LocalMediation")) return TEXT("the local dispute was mediated");
        if (Kind == FName("World.BrokenOath")) return TEXT("an oath was broken");
        if (Kind == FName("World.RedeemedOath")) return TEXT("someone tried to make amends for a broken oath");
        if (Kind == FName("Reality.CareWitnessed")) return TEXT("you made a careful passage through a change in reality");
        if (Kind == FName("Reality.InquiryWitnessed")) return TEXT("you examined a change in reality");
        if (Kind == FName("Reality.Anomaly")) return TEXT("a reality fracture appeared");
        return nullptr; // Unknown event IDs stay out of player-facing dialogue.
    };

    const TArray<FUnmadeNpcObservation>& Events = Memory->GetObservations();
    if (bAskConcern)
    {
        for (int32 Index = Events.Num() - 1; Index >= 0; --Index)
        {
            const TCHAR* Summary = Describe(Events[Index].EventKind);
            if (Summary && (Events[Index].EventKind == FName("Player.Threatened") ||
                            Events[Index].EventKind == FName("World.BrokenOath") ||
                            Events[Index].EventKind == FName("Reality.Anomaly")))
            {
                if (Events[Index].Evidence == EUnmadeEvidenceKind::Rumor &&
                    !Events[Index].SpeakerId.IsNone())
                {
                    return FString::Printf(TEXT("I heard from %s that %s. I did not witness it, but the report worries me."),
                        *Events[Index].SpeakerId.ToString(), Summary);
                }
                return FString::Printf(TEXT("What worries me is that %s. I can only speak to what reached me."), Summary);
            }
        }
        return AuthoredPortrayalReply.IsEmpty()
            ?TEXT("I cannot name a particular danger from what I know. I would rather admit that than pretend certainty.")
            :AuthoredPortrayalReply;
    }

    if (bAskWitness || bAskMemory)
    {
        for (int32 Index = Events.Num() - 1; Index >= 0; --Index)
        {
            const FUnmadeNpcObservation& Event = Events[Index];
            const TCHAR* Summary = Describe(Event.EventKind);
            if (!Summary) continue;
            if (Event.Evidence == EUnmadeEvidenceKind::Witnessed)
            {
                return FString::Printf(TEXT("I remember that %s. I saw it myself."), Summary);
            }
            if (bAskMemory && !Event.SpeakerId.IsNone())
            {
                return FString::Printf(TEXT("I heard from %s that %s, but I did not see it myself."),
                    *Event.SpeakerId.ToString(), Summary);
            }
        }
        return bAskWitness
            ? TEXT("I have no firsthand account of that to give you.")
            : TEXT("I cannot point to a particular thing I remember witnessing about you yet.");
    }

    return AuthoredPortrayalReply.IsEmpty()?CharacterVoice:AuthoredPortrayalReply;
}

bool FUnmadeDialoguePolicy::TryExtractLine(const FString& ResponseJson, FString& OutLine)
{
    OutLine.Empty();
    if (ResponseJson.IsEmpty() || ResponseJson.Len() > 8192)
    {
        return false;
    }

    TSharedPtr<FJsonObject> Outer;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseJson);
    if (!FJsonSerializer::Deserialize(Reader, Outer) || !Outer.IsValid())
    {
        return false;
    }

    bool bDone = false;
    if (!Outer->TryGetBoolField(TEXT("done"), bDone) || !bDone ||
        !Outer->HasTypedField<EJson::Object>(TEXT("message")))
    {
        return false;
    }
    const TSharedPtr<FJsonObject> Message = Outer->GetObjectField(TEXT("message"));
    FString Content;
    if (!Message.IsValid() || !Message->TryGetStringField(TEXT("content"), Content) ||
        Content.IsEmpty() || Content.Len() > 4096)
    {
        return false;
    }

    TSharedPtr<FJsonObject> Reply;
    TSharedRef<TJsonReader<>> ReplyReader = TJsonReaderFactory<>::Create(Content);
    if (!FJsonSerializer::Deserialize(ReplyReader, Reply) || !Reply.IsValid())
    {
        return false;
    }

    FString Candidate;
    if (!Reply->TryGetStringField(TEXT("line"), Candidate))
    {
        return false;
    }
    Candidate.TrimStartAndEndInline();
    if (Candidate.IsEmpty() || Candidate.Len() > MaxDialogueCharacters)
    {
        return false;
    }
    for (TCHAR Ch : Candidate)
    {
        // No multiline responses, invisible control sequences, or injected overlays.
        if (Ch < 0x20 || Ch == 0x7F)
        {
            return false;
        }
    }
    OutLine = MoveTemp(Candidate);
    return true;
}
