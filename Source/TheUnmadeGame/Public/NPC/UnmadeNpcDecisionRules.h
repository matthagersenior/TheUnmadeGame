#pragma once

// Deterministic NPC gameplay policy. No models, network, Unreal, randomness or prompt dependencies.
namespace UnmadeCore {
enum class NpcRole { Merchant, Guard, Scholar, Courier, Wanderer };
enum class NpcTemperament { Steady, Cautious, Curious };
enum class NpcAction {
    KeepRoutine, OfferTrade, OfferDiscount, RefuseTrade, AvoidPlayer, VerifyRumor,
    InvestigateAnomaly, Intervene, ResearchAnomaly, ShareKnowledge, DeliverMessage, OfferAid
};
struct NpcDecisionInput {
    NpcRole role = NpcRole::Wanderer;
    NpcTemperament temperament = NpcTemperament::Steady;
    int trust = 0;
    int fear = 0;
    bool witnessedThreat = false;
    bool witnessedAnomaly = false;
    bool heardAnomaly = false;
    bool playerNearby = false;
};

inline NpcAction ChooseNpcAction(const NpcDecisionInput& in) noexcept {
    switch (in.role) {
    case NpcRole::Merchant:
        if (!in.playerNearby) return NpcAction::KeepRoutine;
        if (in.witnessedThreat || (in.fear >= 50 && in.temperament == NpcTemperament::Cautious))
            return NpcAction::RefuseTrade;
        if (in.fear >= 30 && in.temperament == NpcTemperament::Cautious)
            return NpcAction::AvoidPlayer;
        if (in.trust >= 20 && in.fear < 40) return NpcAction::OfferDiscount;
        return NpcAction::OfferTrade;
    case NpcRole::Guard:
        if (in.witnessedThreat && in.playerNearby) return NpcAction::Intervene;
        if (in.witnessedAnomaly) return NpcAction::InvestigateAnomaly;
        if (in.heardAnomaly) return NpcAction::VerifyRumor;
        return NpcAction::KeepRoutine;
    case NpcRole::Scholar:
        if (in.witnessedAnomaly) return NpcAction::ResearchAnomaly;
        if (in.heardAnomaly) return NpcAction::VerifyRumor;
        if (in.playerNearby && in.trust >= 20) return NpcAction::ShareKnowledge;
        return NpcAction::KeepRoutine;
    case NpcRole::Courier:
        if (in.playerNearby && (in.witnessedThreat || in.fear >= 60))
            return NpcAction::AvoidPlayer;
        return NpcAction::DeliverMessage;
    case NpcRole::Wanderer:
        if (!in.playerNearby) return NpcAction::KeepRoutine;
        if (in.witnessedThreat || in.fear >= 60) return NpcAction::AvoidPlayer;
        if (in.trust >= 20) return NpcAction::OfferAid;
        return NpcAction::KeepRoutine;
    }
    return NpcAction::KeepRoutine;
}
}
