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
