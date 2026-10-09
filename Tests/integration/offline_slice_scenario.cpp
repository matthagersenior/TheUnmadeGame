#include "Fracture/UnmadeFractureRules.h"
#include "Combat/UnmadeCombatRules.h"
#include "NPC/UnmadeNpcDecisionRules.h"
#include "NPC/UnmadeNpcMotionRules.h"
#include "Lexicon/UnmadeLexiconRules.h"
#include "Story/UnmadeConflictRules.h"

#include <cassert>
#include <iostream>

int main() {
    using namespace UnmadeCore;

    // Act I: Seeing impossible geometry never requires an LLM or paid API.
    FractureModel fracture("region.prototype.hub", {"variant.open","variant.sealed"});
    LexiconModel lexicon;
    assert(fracture.Glimpse(true, 1.0) == Result::Applied);
    assert(fracture.WorldVariant().empty()); // discovery is not an irreversible world change
    assert(lexicon.RecordEvidence("term.veyl","evidence.glimpse"));
    assert(lexicon.Interpretation("term.veyl") == InterpretationState::Partial);

    // A scholar uses direct evidence to investigate, while a rumor-only guard verifies.
    NpcDecisionInput scholar;
    scholar.role = NpcRole::Scholar;
    scholar.witnessedAnomaly = true;
    assert(ChooseNpcAction(scholar) == NpcAction::ResearchAnomaly);

    NpcDecisionInput guard;
    guard.role = NpcRole::Guard;
    guard.heardAnomaly = true;
    assert(ChooseNpcAction(guard) == NpcAction::VerifyRumor);
    assert(ChooseEnemyIntent(EnemyStyle::Stalker, 1.8, true) == EnemyIntent::Attack);
    assert(ChooseEnemyIntent(EnemyStyle::Watcher, 2.0, true) == EnemyIntent::Retreat);

    // Act II: Combat is independent of dialogue inference. Guard prevents full damage.
    Combatant player(100, 24, 0.75), enemy(65, 12, 1.4);
    assert(player.SetGuarding(true));
    const auto enemySwing = enemy.TryAttack(1.0);
    assert(enemySwing.outcome == AttackOutcome::Started);
    assert(player.ReceiveHit(901, enemySwing.swingId, enemySwing.baseDamage, false)
        == HitOutcome::Applied);
    assert(player.Health() == 97.0);
    assert(player.SetGuarding(false));
    const auto swing = player.TryAttack(2.0);
    assert(swing.outcome == AttackOutcome::Started);
    assert(enemy.ReceiveHit(902, swing.swingId, swing.baseDamage, true)
        == HitOutcome::Applied);
    assert(enemy.Health() == 29.0); // fracture exposure changes combat
    assert(enemy.ReceiveHit(902, swing.swingId, swing.baseDamage, true)
        == HitOutcome::Duplicate);

    // Act III: Language interpretation has two authored sources; neither is required to choose.
    assert(lexicon.RecordEvidence("term.veyl","evidence.archivist"));
    assert(lexicon.Interpretation("term.veyl") == InterpretationState::Understood);

    ConflictModel conflict;
    assert(conflict.Preview(ConflictChoice::Shelter) == ConflictResult::NeedsConfirmation);
    assert(!conflict.ShelterOpen());
    assert(conflict.Commit(ConflictChoice::Shelter,false) == ConflictResult::NeedsConfirmation);
    assert(conflict.Commit(ConflictChoice::Shelter,true) == ConflictResult::Committed);
    assert(conflict.ShelterOpen() && !conflict.ArchiveOpen());
    assert(conflict.Commit(ConflictChoice::Research,true) == ConflictResult::AlreadyCommitted);
    assert(conflict.CollectSupplies(true));
    assert(conflict.DeliverSupplies(true));

    // A witness's trust affects trade; hearsay is weaker.
    NpcDecisionInput eyewitnessMerchant;
    eyewitnessMerchant.role = NpcRole::Merchant;
    eyewitnessMerchant.playerNearby = true;
    eyewitnessMerchant.trust = 25;
    NpcDecisionInput rumorMerchant = eyewitnessMerchant;
    rumorMerchant.trust = 8;
    assert(ChooseNpcAction(eyewitnessMerchant) == NpcAction::OfferDiscount);
    assert(ChooseNpcAction(rumorMerchant) == NpcAction::OfferTrade);

    // A native NPC movement update is bounded during frame spikes.
    const Vec2 movement = SteerNpc({0,0}, {500,0}, NpcMotion::Approach, 80, 4.0, 100);
    assert(movement.x == 20.0 && movement.y == 0.0);

    // Returning to the region retains both authored outcomes and clues.
    FractureModel loadedFracture("region.prototype.hub", {"variant.open","variant.sealed"});
    LexiconModel loadedLexicon;
    ConflictModel loadedConflict;
    assert(loadedFracture.Restore(fracture.TakeSnapshot()));
    assert(loadedLexicon.Restore(lexicon.Snapshot()));
    assert(loadedConflict.Restore(conflict.Snapshot()));
    assert(loadedLexicon.Interpretation("term.veyl") == InterpretationState::Understood);
    assert(loadedConflict.ShelterOpen());
    assert(loadedConflict.Supplies() == SupplyStage::Delivered);

    std::cout << "PASS: offline integrated story/combat/lexicon/fracture/NPC/save simulation\n";
    std::cout << "NOTE: simulated domain models only; no Unreal map, animations or runtime tested\n";
}
