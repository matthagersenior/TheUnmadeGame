#pragma once
// Authored local conflict. Pure C++17: no Unreal, network, local AI, or paid APIs.
namespace UnmadeCore {
enum class ConflictChoice { None = 0, Shelter = 1, Research = 2 };
enum class SupplyStage { AwaitPickup = 0, Carrying = 1, Delivered = 2 };
enum class ConflictResult { NeedsConfirmation, Committed, AlreadyCommitted, InvalidChoice };
struct ConflictSnapshot { int choice = 0; int supplies = 0; };
class ConflictModel final {
public:
    ConflictChoice Choice() const noexcept { return choice_; }
    SupplyStage Supplies() const noexcept { return supplies_; }
    bool ShelterOpen() const noexcept { return choice_ == ConflictChoice::Shelter; }
    bool ArchiveOpen() const noexcept { return choice_ == ConflictChoice::Research; }
    ConflictResult Preview(ConflictChoice choice) const noexcept {
        if (!ValidChoice(choice) || choice == ConflictChoice::None) return ConflictResult::InvalidChoice;
        return choice_ == ConflictChoice::None ? ConflictResult::NeedsConfirmation : ConflictResult::AlreadyCommitted;
    }
    ConflictResult Commit(ConflictChoice choice, bool confirmed) noexcept {
        const ConflictResult preview = Preview(choice);
        if (preview != ConflictResult::NeedsConfirmation || !confirmed) return preview;
        choice_ = choice;
        return ConflictResult::Committed;
    }
    bool CollectSupplies(bool atMarket) noexcept {
        if (!atMarket || supplies_ != SupplyStage::AwaitPickup) return false;
        supplies_ = SupplyStage::Carrying;
        return true;
    }
    bool DeliverSupplies(bool atShelter) noexcept {
        if (!atShelter || supplies_ != SupplyStage::Carrying) return false;
        supplies_ = SupplyStage::Delivered;
        return true;
    }
    ConflictSnapshot Snapshot() const noexcept {
        return {static_cast<int>(choice_), static_cast<int>(supplies_)};
    }
    bool Restore(const ConflictSnapshot& snapshot) noexcept {
        if (snapshot.choice < 0 || snapshot.choice > 2 || snapshot.supplies < 0 || snapshot.supplies > 2)
            return false;
        choice_ = static_cast<ConflictChoice>(snapshot.choice);
        supplies_ = static_cast<SupplyStage>(snapshot.supplies);
        return true;
    }
private:
    static bool ValidChoice(ConflictChoice choice) noexcept {
        return choice == ConflictChoice::None || choice == ConflictChoice::Shelter || choice == ConflictChoice::Research;
    }
    ConflictChoice choice_ = ConflictChoice::None;
    SupplyStage supplies_ = SupplyStage::AwaitPickup;
};
}
