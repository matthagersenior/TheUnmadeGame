#pragma once
// Offline authored settlement + character registry. No Unreal, cloud, LLM, or procedural names.
#include "World/UnmadeLivingWorldRules.h"
#include <array>
#include <cmath>
#include <cstring>

namespace UnmadeCore {
enum class SettlementId { None = -1, Crossings = 0, Bellwold = 1, Paperhaven = 2 };
enum class SettlementLean { Independent, Shelter, Research };
struct SettlementSpec {
    SettlementId id;
    const char* name;
    double x;
    double y;
    SettlementLean leaning;
    const char* welcome;
    const char* night;
};
struct ResidentSpec {
    const char* id;
    const char* name;
    NpcRole role;
    NpcTemperament temperament;
    SettlementId home;
    double localX;
    double localY;
    const char* authoredLine;
};
inline constexpr std::array<SettlementSpec, 3> Settlements = {{
    {SettlementId::Crossings, "The Crossings", 0.0, 0.0, SettlementLean::Independent,
     "Three roads meet where two histories disagree. Shelter and inquiry still argue over the passage.",
     "Every door in the Crossings casts a different shadow than its owner."},
    {SettlementId::Bellwold, "Bellwold Refuge", -18000.0, 0.0, SettlementLean::Shelter,
     "The bell-keepers offer sanctuary first and questions later. They live by shared labor and remembered loss.",
     "Lanterns burn for people erased from the official ledgers."},
    {SettlementId::Paperhaven, "Paperhaven Archive", 18000.0, 0.0, SettlementLean::Research,
     "Names are registered, testimonies argued, and impossible histories bound into books.",
     "The archive locks its doors, but missing pages whisper after sundown."}
}};
inline constexpr std::array<ResidentSpec, 48> Residents = {{
    {"npc.merchant.001", "The stallkeeper", NpcRole::Merchant, NpcTemperament::Cautious, SettlementId::Crossings, 200.0, -230.0, "You can buy anything except the day before yesterday."},
    {"npc.guard.001", "A gate watchkeeper", NpcRole::Guard, NpcTemperament::Steady, SettlementId::Crossings, -300.0, -230.0, "I do not guard the gates. I guard who comes through."},
    {"npc.wanderer.001", "A passing stranger", NpcRole::Wanderer, NpcTemperament::Steady, SettlementId::Crossings, 170.0, 340.0, "There is a road that leaves every village twice."},
    {"npc.archivist.001", "The records keeper", NpcRole::Scholar, NpcTemperament::Curious, SettlementId::Crossings, -360.0, 320.0, "The ink changes whenever someone remembers differently."},
    {"npc.courier.001", "A courier", NpcRole::Courier, NpcTemperament::Steady, SettlementId::Crossings, 300.0, 90.0, "Letters arrive from people who never left."},
    {"npc.welllistener.001", "The well listener", NpcRole::Scholar, NpcTemperament::Curious, SettlementId::Crossings, -1540.0, 680.0, "The well knows tomorrow's answers, not which questions survive."},
    {"npc.orchardexile.001", "The displaced orchard keeper", NpcRole::Wanderer, NpcTemperament::Cautious, SettlementId::Crossings, 1120.0, 1260.0, "Every leaf bears a name I was meant to remember."},
    {"npc.tollbroker.001", "The keeper of future debts", NpcRole::Merchant, NpcTemperament::Cautious, SettlementId::Crossings, 1000.0, -1170.0, "Your debt was recorded before you arrived."},
    {"npc.roadwarden.001", "The road warden", NpcRole::Guard, NpcTemperament::Steady, SettlementId::Crossings, 1690.0, 390.0, "I guard a road that changes its destination while I sleep."},
    {"npc.bellmaker.001", "The bell maker", NpcRole::Merchant, NpcTemperament::Steady, SettlementId::Crossings, -1580.0, -1020.0, "I can mend a bell, but not the hour it rings."},
    {"npc.nightcourier.001", "The night courier", NpcRole::Courier, NpcTemperament::Cautious, SettlementId::Crossings, 1010.0, 1040.0, "I deliver letters people swear they never wrote."},
    {"npc.metalworker.001", "Mara the metalworker", NpcRole::Merchant, NpcTemperament::Steady, SettlementId::Crossings, -690.0, -860.0, "Good steel holds shape even when the world forgets it."},
    {"npc.inkmender.001", "Tovin the ink mender", NpcRole::Scholar, NpcTemperament::Steady, SettlementId::Crossings, -220.0, 910.0, "I keep two copies, because each one disagrees."},
    {"npc.hearthwatch.001", "Nira of the hearth", NpcRole::Wanderer, NpcTemperament::Cautious, SettlementId::Crossings, 680.0, 920.0, "We count those who return, not those who promise."},
    {"npc.bridgekeeper.001", "Orrel the bridge keeper", NpcRole::Guard, NpcTemperament::Cautious, SettlementId::Crossings, -1070.0, 90.0, "The bridges have different memories of the river."},
    {"npc.marketporter.001", "Kesta the porter", NpcRole::Courier, NpcTemperament::Curious, SettlementId::Crossings, 750.0, -830.0, "Cargo gets lighter near the fracture. Nobody asks why."},
    {"npc.bellwold.matron.001", "Hessa the refuge matron", NpcRole::Wanderer, NpcTemperament::Steady, SettlementId::Bellwold, -600.0, 250.0, "The refuge never asks where you came from."},
    {"npc.bellwold.stonewright.001", "Corvin the stonewright", NpcRole::Merchant, NpcTemperament::Steady, SettlementId::Bellwold, 340.0, -710.0, "The old bell was never meant to hang."},
    {"npc.bellwold.guard.001", "Bram the watch captain", NpcRole::Guard, NpcTemperament::Cautious, SettlementId::Bellwold, 850.0, 540.0, "The west road belongs to anyone still breathing."},
    {"npc.bellwold.healer.001", "Sorin the healer", NpcRole::Scholar, NpcTemperament::Curious, SettlementId::Bellwold, -900.0, -440.0, "A scar is proof that something survived."},
    {"npc.bellwold.courier.001", "Elsa the message runner", NpcRole::Courier, NpcTemperament::Steady, SettlementId::Bellwold, 230.0, 890.0, "I carry names between the empty houses."},
    {"npc.bellwold.shepherd.001", "Anwen the shepherd", NpcRole::Wanderer, NpcTemperament::Cautious, SettlementId::Bellwold, -1210.0, 740.0, "Our animals turn toward the bell before it rings."},
    {"npc.bellwold.tanner.001", "Perrin the tanner", NpcRole::Merchant, NpcTemperament::Cautious, SettlementId::Bellwold, 1100.0, -760.0, "No two hides bear the same year twice."},
    {"npc.bellwold.vigil.001", "Dessa of the night vigil", NpcRole::Guard, NpcTemperament::Steady, SettlementId::Bellwold, -1050.0, -1030.0, "At night the passage counts its own sentries."},
    {"npc.bellwold.childtutor.001", "Ivera the tutor", NpcRole::Scholar, NpcTemperament::Steady, SettlementId::Bellwold, -120.0, 1160.0, "The children remember buildings we never built."},
    {"npc.bellwold.baker.001", "Tessa the baker", NpcRole::Merchant, NpcTemperament::Curious, SettlementId::Bellwold, 540.0, 320.0, "Dough rises differently when tomorrow is near."},
    {"npc.bellwold.lamplighter.001", "Rook the lamplighter", NpcRole::Courier, NpcTemperament::Cautious, SettlementId::Bellwold, -640.0, 960.0, "I leave a light for the people history forgot."},
    {"npc.bellwold.millhand.001", "Vale the millhand", NpcRole::Wanderer, NpcTemperament::Steady, SettlementId::Bellwold, 1220.0, 280.0, "The wheel still turns though the stream went away."},
    {"npc.bellwold.pathfinder.001", "Oren the pathfinder", NpcRole::Guard, NpcTemperament::Curious, SettlementId::Bellwold, -280.0, -1160.0, "I chart exits. The map charts entrances."},
    {"npc.bellwold.relickeeper.001", "Isma the relic keeper", NpcRole::Scholar, NpcTemperament::Cautious, SettlementId::Bellwold, 890.0, -1220.0, "Everything in this cabinet has been returned twice."},
    {"npc.bellwold.fletcher.001", "Fen the fletcher", NpcRole::Merchant, NpcTemperament::Steady, SettlementId::Bellwold, 300.0, -1280.0, "The arrows know which direction the wind will choose."},
    {"npc.bellwold.woodcarrier.001", "Belyn the wood carrier", NpcRole::Courier, NpcTemperament::Steady, SettlementId::Bellwold, -1370.0, 120.0, "The shelter's fires belong to everyone."},
    {"npc.paperhaven.registrar.001", "Sevrin the registrar", NpcRole::Scholar, NpcTemperament::Cautious, SettlementId::Paperhaven, -500.0, 350.0, "We cannot recognize a person whose name does not exist."},
    {"npc.paperhaven.guard.001", "Alis the archive sentinel", NpcRole::Guard, NpcTemperament::Steady, SettlementId::Paperhaven, 810.0, 630.0, "We preserve evidence even when the truth hurts."},
    {"npc.paperhaven.scribe.001", "Toma the copyist", NpcRole::Scholar, NpcTemperament::Curious, SettlementId::Paperhaven, 200.0, -790.0, "Every transcription changes one word by itself."},
    {"npc.paperhaven.merchant.001", "Myra the book trader", NpcRole::Merchant, NpcTemperament::Steady, SettlementId::Paperhaven, 920.0, -470.0, "A banned book is still a book, if you can find its price."},
    {"npc.paperhaven.courier.001", "Rell the paper runner", NpcRole::Courier, NpcTemperament::Curious, SettlementId::Paperhaven, -920.0, 770.0, "I deliver decisions sealed before they are made."},
    {"npc.paperhaven.votary.001", "Edris of the index", NpcRole::Wanderer, NpcTemperament::Cautious, SettlementId::Paperhaven, -850.0, -650.0, "The index remembers the forgotten better than I do."},
    {"npc.paperhaven.archivist.001", "Calder the keeper of hours", NpcRole::Scholar, NpcTemperament::Steady, SettlementId::Paperhaven, 110.0, 1160.0, "A missing hour is not the same as an empty hour."},
    {"npc.paperhaven.guard2.001", "Yara of the seal", NpcRole::Guard, NpcTemperament::Cautious, SettlementId::Paperhaven, -1240.0, 160.0, "Nobody opens the vault without someone watching."},
    {"npc.paperhaven.binding.001", "Ilen the bookbinder", NpcRole::Merchant, NpcTemperament::Curious, SettlementId::Paperhaven, 1100.0, 800.0, "I mend spines. I have not learned to mend stories."},
    {"npc.paperhaven.mapmaker.001", "Voss the mapmaker", NpcRole::Scholar, NpcTemperament::Curious, SettlementId::Paperhaven, -290.0, -1250.0, "Our maps change when someone returns alive."},
    {"npc.paperhaven.surveyor.001", "Dara the surveyor", NpcRole::Courier, NpcTemperament::Steady, SettlementId::Paperhaven, 1300.0, -240.0, "I measure distance, never certainty."},
    {"npc.paperhaven.gardener.001", "Linn the dry gardener", NpcRole::Wanderer, NpcTemperament::Steady, SettlementId::Paperhaven, 670.0, 1130.0, "These roots drink the ink from discarded laws."},
    {"npc.paperhaven.watchman.001", "Bret the last watchman", NpcRole::Guard, NpcTemperament::Cautious, SettlementId::Paperhaven, -1300.0, -950.0, "The archive's silence is an oath I took unwillingly."},
    {"npc.paperhaven.glassworker.001", "Meri the glassworker", NpcRole::Merchant, NpcTemperament::Curious, SettlementId::Paperhaven, 330.0, -1280.0, "When a glass cracks the world changes on the other side."},
    {"npc.paperhaven.cataloguer.001", "Otho the cataloguer", NpcRole::Scholar, NpcTemperament::Steady, SettlementId::Paperhaven, -1080.0, 1210.0, "I can tell you where a book was, not where it is now."},
    {"npc.paperhaven.lettercarrier.001", "Sela the letter carrier", NpcRole::Courier, NpcTemperament::Steady, SettlementId::Paperhaven, 1100.0, -1170.0, "Some envelopes refuse to leave this town."}
}};
inline const SettlementSpec* FindSettlement(SettlementId id) noexcept {
    for (const auto& settlement : Settlements)
        if (settlement.id == id) return &settlement;
    return nullptr;
}
inline const ResidentSpec* FindResident(const char* id) noexcept {
    if (!id) return nullptr;
    for (const auto& resident : Residents)
        if (std::strcmp(resident.id, id) == 0) return &resident;
    return nullptr;
}
inline Vec2 ResidentWorldPosition(const ResidentSpec& resident) noexcept {
    const auto* settlement = FindSettlement(resident.home);
    return settlement ? Vec2{settlement->x + resident.localX, settlement->y + resident.localY} : Vec2{};
}
inline SettlementId SettlementAt(double x, double y, double radius = 2850.0) noexcept {
    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(radius) || radius <= 0.0)
        return SettlementId::None;
    for (const auto& settlement : Settlements)
        if (std::hypot(x - settlement.x, y - settlement.y) <= radius)
            return settlement.id;
    return SettlementId::None;
}
inline Vec2 LocalRoutineTarget(SettlementId home, NpcRole role, DayPhase phase, Vec2 assignedHome) noexcept {
    const SettlementSpec* village = FindSettlement(home);
    if (!village) return assignedHome;
    if (phase == DayPhase::Day || phase == DayPhase::Dawn) return assignedHome;
    // Work and rest locations stay within the resident's own settlement.
    Vec2 offset;
    switch (role) {
    case NpcRole::Merchant: offset = {-550.0, 440.0}; break;
    case NpcRole::Guard: offset = {920.0, 140.0}; break;
    case NpcRole::Scholar: offset = {180.0, 730.0}; break;
    case NpcRole::Courier: offset = {60.0, -720.0}; break;
    case NpcRole::Wanderer: offset = {-700.0, -400.0}; break;
    }
    if (phase == DayPhase::Dusk) return {village->x + offset.x * .5, village->y + offset.y * .5};
    return {village->x + offset.x, village->y + offset.y};
}
inline Vec2 LocalInvestigationTarget(SettlementId home) noexcept {
    const auto* village = FindSettlement(home);
    return village ? Vec2{village->x, village->y + 440.0} : Vec2{};
}
inline Vec2 LocalRumorTarget(SettlementId home) noexcept {
    const auto* village = FindSettlement(home);
    return village ? Vec2{village->x - 340.0, village->y + 300.0} : Vec2{};
}
// Villagers do not know a global player choice unless it reaches them through their own memory.
// When it does, communities interpret the same witnessed act through different values.
inline int SettlementTrustDelta(SettlementId village, bool supportedShelter, bool witnessed) noexcept {
    const auto* settlement = FindSettlement(village);
    if (!settlement) return 0;
    const int weight = witnessed ? 15 : 5;
    if (settlement->leaning == SettlementLean::Shelter)
        return supportedShelter ? weight : -weight;
    if (settlement->leaning == SettlementLean::Research)
        return supportedShelter ? -weight : weight;
    return 0;
}
class SettlementVisits final {
public:
    bool Visit(SettlementId id) noexcept {
        const int value = static_cast<int>(id);
        if (value < 0 || value >= static_cast<int>(Settlements.size())) return false;
        const int bit = 1 << value;
        if (visited_ & bit) return false;
        visited_ |= bit; return true;
    }
    int Count() const noexcept {
        int count = 0;
        for(int i = 0; i < static_cast<int>(Settlements.size()); ++i)
            if(visited_ & (1 << i)) ++count;
        return count;
    }
    int Snapshot() const noexcept { return visited_; }
    bool Restore(int mask) noexcept {
        const int allowed = (1 << static_cast<int>(Settlements.size())) - 1;
        if(mask < 0 || (mask & ~allowed)) return false;
        visited_ = mask; return true;
    }
private:
    int visited_ = 0;
};
}
