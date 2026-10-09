#include "Lexicon/UnmadeLexiconComponent.h"
#include "Save/UnmadePrototypeSave.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UUnmadeLexiconComponent::UUnmadeLexiconComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UUnmadeLexiconComponent::BeginPlay()
{
    Super::BeginPlay();
    const UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"), 0));
    if (!Save || Save->SchemaVersion != 1) return;
    std::vector<std::string> Evidence;
    Evidence.reserve(Save->LexiconEvidence.Num());
    for (const FName Entry : Save->LexiconEvidence)
    {
        const FString Text = Entry.ToString();
        Evidence.emplace_back(TCHAR_TO_UTF8(*Text));
    }
    if (!Lexicon.Restore(Evidence))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid lexicon evidence in save ignored"));
    }
}

bool UUnmadeLexiconComponent::RecordEvidence(FName EvidenceId)
{
    const auto Previous = Lexicon.Snapshot();
    const FString Text = EvidenceId.ToString();
    if (!Lexicon.RecordEvidence("term.veyl", std::string(TCHAR_TO_UTF8(*Text))))
        return false;

    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save)
    {
        Lexicon.Restore(Previous);
        return false;
    }

    Save->LexiconEvidence.Reset();
    for (const std::string& Id : Lexicon.Snapshot())
    {
        Save->LexiconEvidence.Add(FName(UTF8_TO_TCHAR(Id.c_str())));
    }

    if (!UGameplayStatics::SaveGameToSlot(Save, TEXT("UnmadePrototypeNPC"), 0))
    {
        Lexicon.Restore(Previous);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red,
            TEXT("Could not save language discovery. Progress not applied."));
        return false;
    }

    if (GEngine)
    {
        const FString Message = UnderstandsVeyl()
            ? TEXT("VEYL UNDERSTOOD: 'the way that remains.' You can read the fracture inscription.")
            : TEXT("VEYL: first of two clues recorded. An archivist might recognize this mark.");
        GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Cyan, Message);
    }
    return true;
}

bool UUnmadeLexiconComponent::UnderstandsVeyl() const
{
    return Lexicon.Interpretation("term.veyl") == UnmadeCore::InterpretationState::Understood;
}

int32 UUnmadeLexiconComponent::GetClueCount() const
{
    return static_cast<int32>(Lexicon.Snapshot().size());
}
