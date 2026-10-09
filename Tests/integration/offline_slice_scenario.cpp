#include "Fracture/UnmadeFractureRules.h"
#include "Combat/UnmadeCombatRules.h"
#include "NPC/UnmadeNpcDecisionRules.h"
#include "NPC/UnmadeNpcMotionRules.h"
#include "Lexicon/UnmadeLexiconRules.h"
#include "Story/UnmadeConflictRules.h"
#include "World/UnmadeLivingWorldRules.h"
#include "World/UnmadeSettlementRegistry.h"
#include "World/UnmadeRegionalTaskRules.h"
#include "Items/UnmadeItemRules.h"
#include "World/UnmadeWorldAtlas.h"
#include "Combat/UnmadeBossRules.h"
#include "Items/UnmadeCraftEconomyRules.h"
#include "World/UnmadeFactionChronicleRules.h"
#include "World/UnmadeFrontierRealmRules.h"

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

    // Act IV: A living region keeps its clock, side stories, and unique discoveries.
    LivingWorldClock world;
    assert(world.MinuteOfDay() == 420);
    assert(world.Restore(780.0) && world.Phase() == DayPhase::Night);
    assert(SelectAmbientCue(District::ShelterThreshold, world.Phase(), 1)
        == AmbientCue::ShelterWelcoming);
    assert(SelectAmbientCue(District::ShelterThreshold, world.Phase(), 2)
        == AmbientCue::ShelterBarred);
    DiscoveryLedger places;
    assert(places.Discover(District::PaperOrchard) && places.Discover(District::EchoWell));
    assert(!places.Discover(District::PaperOrchard));
    LivingWorldClock restoredWorld;
    DiscoveryLedger restoredPlaces;
    assert(restoredWorld.Restore(world.ElapsedSeconds()));
    assert(restoredPlaces.Restore(places.Snapshot()) && restoredPlaces.Count() == 2);
    assert(RoutineTarget(NpcRole::Merchant, DayPhase::Night).y !=
           RoutineTarget(NpcRole::Merchant, DayPhase::Day).y);

    // Multi-settlement regression: real separate villages, local policies, story state.
    assert(Settlements.size() == 3 && Residents.size() == 48);
    assert(SettlementAt(18000,0) == SettlementId::Paperhaven);
    assert(SettlementAt(-18000,0) == SettlementId::Bellwold);
    assert(SettlementAt(0,0) == SettlementId::Crossings);
    assert(SettlementAt(9000,0) == SettlementId::None);
    const ResidentSpec* courier = FindResident("npc.bellwold.courier.001");
    assert(courier && courier->home == SettlementId::Bellwold);
    const Vec2 courierHome = ResidentWorldPosition(*courier);
    const Vec2 courierNight = LocalRoutineTarget(
        courier->home, courier->role, DayPhase::Night, courierHome);
    assert(SettlementAt(courierNight.x,courierNight.y)==SettlementId::Bellwold);
    assert(SettlementTrustDelta(SettlementId::Bellwold,true,true)>0);
    assert(SettlementTrustDelta(SettlementId::Paperhaven,true,true)<0);
    SettlementVisits villageVisits;
    assert(villageVisits.Visit(SettlementId::Crossings));
    assert(villageVisits.Visit(SettlementId::Bellwold));
    assert(villageVisits.Visit(SettlementId::Paperhaven));
    assert(villageVisits.Count()==3);
    RegionalTaskModel tasks;
    assert(tasks.Converse(SettlementId::Bellwold,"npc.bellwold.matron.001")==TaskResult::NoChange);
    assert(tasks.Converse(SettlementId::Bellwold,"npc.bellwold.lamplighter.001")==TaskResult::Advanced);
    assert(tasks.Converse(SettlementId::Bellwold,"npc.bellwold.matron.001")==TaskResult::Completed);
    assert(tasks.Converse(SettlementId::Paperhaven,"npc.paperhaven.scribe.001")==TaskResult::Advanced);
    assert(tasks.Converse(SettlementId::Paperhaven,"npc.paperhaven.registrar.001")==TaskResult::Completed);
    RegionalTaskModel loadedTasks;
    assert(loadedTasks.Restore(tasks.Snapshot()));
    assert(loadedTasks.Progress(SettlementId::Bellwold)==TaskProgress::Completed);
    assert(loadedTasks.Progress(SettlementId::Paperhaven)==TaskProgress::Completed);
    SettlementVisits loadedVillages;
    assert(loadedVillages.Restore(villageVisits.Snapshot()) && loadedVillages.Count()==3);

    // Act VI: an earned inventory changes combat and makes inter-realm routes credible.
    InventoryModel gear;
    assert(gear.Claim(Achievement::Starter)==RewardResult::Awarded);
    assert(gear.Equip(ItemId::SalvagerBlade));
    assert(gear.Equip(ItemId::TravelerMantle));
    assert(gear.AttackBonus()==3 && gear.ArmorBonus()==2);
    assert(gear.Claim(Achievement::BellwoldLanterns)==RewardResult::Awarded);
    assert(gear.Claim(Achievement::PaperhavenTestimony)==RewardResult::Awarded);
    assert(gear.Claim(Achievement::ThreeVillages)==RewardResult::Awarded);
    assert(gear.Claim(Achievement::EchoWell)==RewardResult::Awarded);
    assert(gear.Claim(Achievement::FirstStalker)==RewardResult::Awarded);
    assert(AttunementFromRewards(gear)==1);
    assert(gear.ForgeWaybreaker());
    assert(gear.Equip(ItemId::Waybreaker));
    assert(gear.AttackBonus()>=32);
    assert(AttunementFromRewards(gear)==3);
    assert(gear.Claim(Achievement::AllLandmarks)==RewardResult::Awarded);
    assert(gear.Equip(ItemId::UnwrittenCrown));
    assert(AttunementFromRewards(gear)==4);
    const auto finalRoute=PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,
        AttunementFromRewards(gear));
    assert(finalRoute.size()>3);
    const auto itemSave=gear.Snapshot();
    InventoryModel loadedGear;
    assert(loadedGear.Restore(itemSave));
    assert(loadedGear.AttackBonus()==gear.AttackBonus());
    assert(loadedGear.HasClaimed(Achievement::ThreeVillages));
    assert(loadedGear.Claim(Achievement::BellwoldLanterns)==RewardResult::AlreadyAwarded);

    // Act VII: named boss warnings, no unavoidable instant strikes, rare victory.
    BossEncounter bellBoss(BossId::HollowBell);
    const auto opening=bellBoss.Advance(1.0,1.0,300,false);
    assert(opening.action==BossAction::Telegraph && opening.damage==0);
    assert(bellBoss.Advance(1.2,1.0,300,true).action==BossAction::Stagger);
    assert(bellBoss.Advance(3.0,.5,300,false).action==BossAction::Telegraph);
    assert(bellBoss.Advance(6.0,.5,300,false).action==BossAction::Shockwave);
    assert(gear.Claim(Achievement::HollowBell)==RewardResult::Awarded);
    assert(gear.Quantity(ItemId::BellheartAegis)==1);
    assert(gear.Claim(Achievement::HollowBell)==RewardResult::AlreadyAwarded);

    // Act VIII: recipes spend actual items, skilled professions and barter remain finite.
    RegionalEconomy trade;
    InventoryModel craftBag;
    assert(trade.PayContract(SettlementId::Bellwold,2)==EconomyResult::Completed);
    assert(trade.PayContract(SettlementId::Bellwold,2)==EconomyResult::AlreadyPaid);
    assert(trade.Buy(craftBag,ItemId::WildHerbs,SettlementId::Bellwold,2)==EconomyResult::Completed);
    assert(trade.Buy(craftBag,ItemId::IronScrap,SettlementId::Bellwold,1)==EconomyResult::Completed);
    const int marks=trade.Marks();
    assert(trade.Craft(craftBag,RecipeId::HerbSalve,SettlementId::Paperhaven)==EconomyResult::NotAvailable);
    assert(trade.Marks()==marks);
    assert(trade.Craft(craftBag,RecipeId::HerbSalve,SettlementId::Bellwold)==EconomyResult::Completed);
    assert(craftBag.Quantity(ItemId::HearthSalve)==2);
    RegionalEconomy reloadedTrade;
    assert(reloadedTrade.Restore(trade.Snapshot()) && reloadedTrade.Marks()==marks);
    assert(reloadedTrade.Skill(Profession::Apothecary)==1);

    // Act IX: village storylines cross multiple real witnesses, with explicit end choices.
    FactionChronicle factions;
    assert(factions.Converse("npc.bellwold.lamplighter.001",false)==FactionResult::Advanced);
    assert(factions.Converse("npc.bellwold.matron.001",false)==FactionResult::Advanced);
    assert(factions.Converse("npc.bellwold.guard.001",true)==FactionResult::ChoiceRequired);
    assert(factions.Decide(Faction::Refuge,FactionEnding::Solidarity)==FactionResult::Resolved);
    FactionChronicle reloadedFactions;
    assert(reloadedFactions.Restore(factions.Snapshot()));
    assert(reloadedFactions.Reputation(Faction::Refuge)==40);
    assert(reloadedFactions.Decide(Faction::Refuge,FactionEnding::Truth)==FactionResult::NoChange);

    // Act X: inter-realm source travels lead to distinct inhabited footholds.
    assert(FrontierResidents.size()==16);
    FrontierJourney frontier;
    assert(frontier.Visit(Realm::WidowedRain)==FrontierEvent::Advanced);
    assert(frontier.FindClue(Realm::WidowedRain)==FrontierEvent::Advanced);
    assert(frontier.Converse("npc.saltwake.navigator.001")==FrontierEvent::Advanced);
    assert(frontier.Converse("npc.saltwake.rainkeeper.001")==FrontierEvent::Advanced);
    assert(frontier.Converse("npc.saltwake.harborwarden.001")==FrontierEvent::FinalChoice);
    assert(frontier.Resolve(Realm::WidowedRain,1)==FrontierEvent::Resolved);
    assert(frontier.Visit(Realm::HearthBeneath)==FrontierEvent::Advanced);
    assert(frontier.FindClue(Realm::HearthBeneath)==FrontierEvent::Advanced);
    assert(frontier.Converse("npc.cinderhold.hearthreader.001")==FrontierEvent::Advanced);
    assert(frontier.Converse("npc.cinderhold.healer.001")==FrontierEvent::Advanced);
    assert(frontier.Converse("npc.cinderhold.emberwarden.001")==FrontierEvent::FinalChoice);
    assert(frontier.Resolve(Realm::HearthBeneath,2)==FrontierEvent::Resolved);
    assert(gear.Claim(Achievement::SaltwakeStory)==RewardResult::Awarded);
    assert(gear.Claim(Achievement::CinderholdStory)==RewardResult::Awarded);
    FrontierJourney reloadedFrontier;
    assert(reloadedFrontier.Restore(frontier.Snapshot()));
    assert(reloadedFrontier.IsResolved(Realm::HearthBeneath) &&
           reloadedFrontier.IsResolved(Realm::WidowedRain));

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
