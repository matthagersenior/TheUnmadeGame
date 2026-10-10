#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnmadePrototypeSave.generated.h"

/** Prototype save: NPC observations and one authored world choice; not yet a full RPG save system. */
UCLASS()
class THEUNMADEGAME_API UUnmadePrototypeSave : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame)
    int32 SchemaVersion = 1;

    /** Optional immutable-origin profile. Older schema-1 files default to none. */
    UPROPERTY(SaveGame)
    bool bHasCharacterIdentity = false;

    UPROPERTY(SaveGame)
    TArray<int32> CharacterIdentityChoices;

    UPROPERTY(SaveGame)
    FString CharacterChosenName;

    UPROPERTY(SaveGame)
    TArray<FUnmadeNpcSnapshot> NpcSnapshots;

    /** Optional language evidence; schema v1 files treat absence as no clues. */
    UPROPERTY(SaveGame)
    TArray<FName> LexiconEvidence;

    /** Separate authored local dispute; older v1 slots default to no choice. */
    UPROPERTY(SaveGame)
    bool bHasConflictSnapshot = false;

    UPROPERTY(SaveGame)
    int32 LocalConflictChoice = 0;

    UPROPERTY(SaveGame)
    int32 SupplyActivityStage = 0;

    /** Twenty-minute in-world clock and six-place exploration; absent in older v1 saves. */
    UPROPERTY(SaveGame)
    bool bHasLivingWorldSnapshot = false;

    UPROPERTY(SaveGame)
    double LivingWorldSeconds = 0.0;

    UPROPERTY(SaveGame)
    int32 DiscoveredLoreMask = 0;
    /** Three separate settlement discoveries; schema-1 predecessors default to 0. */
    UPROPERTY(SaveGame)
    int32 VisitedSettlementsMask = 0;
    /** Independent Bellwold / Paperhaven optional conversation chains. */
    UPROPERTY(SaveGame)
    int32 BellwoldTaskStage = 0;
    UPROPERTY(SaveGame)
    int32 PaperhavenTaskStage = 0;

    UPROPERTY(SaveGame)
    bool bHasFactionChronicle = false;
    UPROPERTY(SaveGame)
    TArray<int32> FactionStages;
    UPROPERTY(SaveGame)
    TArray<int32> FactionEndings;
    /** New realm footholds: visits, unique clues and two authored local arcs. */
    UPROPERTY(SaveGame)
    bool bHasFrontierSnapshot = false;
    UPROPERTY(SaveGame)
    int32 VisitedFrontierRealms = 0;
    UPROPERTY(SaveGame)
    int32 DiscoveredFrontierClues = 0;
    UPROPERTY(SaveGame)
    TArray<int32> FrontierStages;
    UPROPERTY(SaveGame)
    TArray<int32> FrontierEndings;

    /** Bellwold return-visit story: absent on older schema-1 save slots. */
    UPROPERTY(SaveGame)
    bool bHasAfterlightSnapshot=false;
    UPROPERTY(SaveGame)
    int32 BellwoldAfterlightStage=0;
    UPROPERTY(SaveGame)
    int32 BellwoldAfterlightApproach=0;
    UPROPERTY(SaveGame)
    int32 BellwoldAfterlightOutcome=0;

    UPROPERTY(SaveGame)
    UPROPERTY(SaveGame)
    bool bHasRealmAftermathSnapshot = false;
    /** Nine slots follow the append-only Realm enum; six unspawned realms remain 0. */
    UPROPERTY(SaveGame)
    TArray<int32> RealmAftermathStages;
    UPROPERTY(SaveGame)
    TArray<int32> RealmAftermathPrepared;
    UPROPERTY(SaveGame)
    TArray<int32> RealmAftermathApproaches;
    UPROPERTY(SaveGame)
    TArray<int32> RealmAftermathEndings;

    UPROPERTY(SaveGame)
    bool bHasInventorySnapshot = false;

    UPROPERTY(SaveGame)
    TArray<int32> ItemQuantities;

    UPROPERTY(SaveGame)
    TArray<int32> EquippedItems;

    UPROPERTY(SaveGame)
    int64 AwardedMilestoneBits = 0;

    UPROPERTY(SaveGame)
    bool bHasEconomySnapshot = false;
    UPROPERTY(SaveGame)
    int32 TradeMarks = 45;
    UPROPERTY(SaveGame)
    TArray<int32> ProfessionSkills;
    UPROPERTY(SaveGame)
    int32 PaidContractMask = 0;

    UPROPERTY(SaveGame)
    bool bHasFractureSnapshot = false;

    UPROPERTY(SaveGame)
    FName WorldVariantId = NAME_None;

    UPROPERTY(SaveGame)
    double PlayerStrain = 0.0;

    /** Ten offline signature abilities: append-only save fields, absent on old slots. */
    UPROPERTY(SaveGame)
    bool bHasTenfoldChronicle=false;

    UPROPERTY(SaveGame)
    TArray<int32> RiteStages;

    UPROPERTY(SaveGame)
    TArray<int32> RiteChoices;

    UPROPERTY(SaveGame)
    TArray<double> RiteReadyAt;

    UPROPERTY(SaveGame)
    TArray<int32> DisciplineMastery;

    UPROPERTY(SaveGame)
    int32 RiteTrialBits=0;

    UPROPERTY(SaveGame)
    int32 LegacyDeedBits=0;

    UPROPERTY(SaveGame)
    int32 VerifiedRoadBits=0;

    UPROPERTY(SaveGame)
    int32 TomorrowDebtDueDay=0;

    UPROPERTY(SaveGame)
    int32 TomorrowDebtExhaustedUntil=0;

    UPROPERTY(SaveGame)
    int32 BrokenOathCount=0;

    UPROPERTY(SaveGame)
    bool bOathActive=false;
    UPROPERTY(SaveGame)
    bool bOathRedeemed=false;

    /** Multi-discipline challenge continuity, absent on earlier save slots. */
    UPROPERTY(SaveGame)
    bool bHasConfluenceSnapshot=false;
    UPROPERTY(SaveGame)
    TArray<int32> ConfluenceStages;
    UPROPERTY(SaveGame)
    TArray<int32> ConfluenceDecisions;
    UPROPERTY(SaveGame)
    TArray<int32> ConfluenceCastMasks;
    UPROPERTY(SaveGame)
    TArray<double> ConfluenceWindowEnds;

    /** Merges prototype sub-system writes instead of erasing unrelated state. */
    static UUnmadePrototypeSave* LoadOrCreate()
    {
        UUnmadePrototypeSave* Existing = Cast<UUnmadePrototypeSave>(
            UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"), 0));
        if (Existing) return Existing->SchemaVersion == 1 ? Existing : nullptr;
        // Never replace an existing but unreadable save with an empty one.
        if (UGameplayStatics::DoesSaveGameExist(TEXT("UnmadePrototypeNPC"), 0))
            return nullptr;
        return Cast<UUnmadePrototypeSave>(UGameplayStatics::CreateSaveGameObject(StaticClass()));
    }
};
