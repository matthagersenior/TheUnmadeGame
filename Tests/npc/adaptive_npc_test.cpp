#include "NPC/UnmadeNpcDecisionRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main() {
    NpcDecisionInput merchant;
    merchant.role = NpcRole::Merchant;
    merchant.playerNearby = true;
    assert(ChooseNpcAction(merchant) == NpcAction::OfferTrade);
    merchant.trust = 25;
    assert(ChooseNpcAction(merchant) == NpcAction::OfferDiscount);
    merchant.fear = 30;
    merchant.temperament = NpcTemperament::Cautious;
    assert(ChooseNpcAction(merchant) == NpcAction::AvoidPlayer);
    merchant.fear = 55;
    assert(ChooseNpcAction(merchant) == NpcAction::RefuseTrade);
    merchant.fear = 0;
    merchant.witnessedThreat = true;
    assert(ChooseNpcAction(merchant) == NpcAction::RefuseTrade);
    merchant.witnessedThreat = false;
    merchant.trust = 0;
    merchant.fear = 12;
    merchant.heardAnomaly = true;
    assert(ChooseNpcAction(merchant) == NpcAction::OfferTrade);
    merchant.playerNearby = false;
    assert(ChooseNpcAction(merchant) == NpcAction::KeepRoutine);

    NpcDecisionInput guard;
    guard.role = NpcRole::Guard;
    guard.playerNearby = true;
    assert(ChooseNpcAction(guard) == NpcAction::KeepRoutine);
    guard.heardAnomaly = true;
    assert(ChooseNpcAction(guard) == NpcAction::VerifyRumor);
    guard.witnessedAnomaly = true;
    assert(ChooseNpcAction(guard) == NpcAction::InvestigateAnomaly);
    guard.witnessedThreat = true;
    assert(ChooseNpcAction(guard) == NpcAction::Intervene);
    guard.playerNearby = false;
    assert(ChooseNpcAction(guard) == NpcAction::InvestigateAnomaly);

    NpcDecisionInput archivist;
    archivist.role = NpcRole::Scholar;
    archivist.playerNearby = true;
    archivist.trust = 25;
    assert(ChooseNpcAction(archivist) == NpcAction::ShareKnowledge);
    archivist.heardAnomaly = true;
    assert(ChooseNpcAction(archivist) == NpcAction::VerifyRumor);
    archivist.witnessedAnomaly = true;
    assert(ChooseNpcAction(archivist) == NpcAction::ResearchAnomaly);

    NpcDecisionInput traveler;
    traveler.role = NpcRole::Wanderer;
    traveler.playerNearby = true;
    traveler.trust = 25;
    assert(ChooseNpcAction(traveler) == NpcAction::OfferAid);
    traveler.fear = 75;
    assert(ChooseNpcAction(traveler) == NpcAction::AvoidPlayer);

    // This gameplay logic has no AI mode or API parameter: same facts, same result.
    for (int i = 0; i < 100; ++i)
        assert(ChooseNpcAction(archivist) == NpcAction::ResearchAnomaly);
    std::cout << "PASS: offline NPC actions, provenance, personalities and repeatability\n";
}
