#include "World/UnmadeSettlementRegistry.h"
#include <cassert>
#include <cstring>
#include <set>
#include <string>
#include <iostream>
using namespace UnmadeCore;
int main() {
    assert(Settlements.size() >= 3);
    assert(Residents.size() >= 48);
    assert(SettlementAt(0,0) == SettlementId::Crossings);
    assert(SettlementAt(-18000,0) == SettlementId::Bellwold);
    assert(SettlementAt(18000,0) == SettlementId::Paperhaven);
    assert(SettlementAt(9000,0) == SettlementId::None);
    assert(SettlementAt(0,0,-1) == SettlementId::None);
    assert(SettlementAt(1e99,1e99) == SettlementId::None);
    assert(FindSettlement(SettlementId::None) == nullptr);
    std::set<std::string> ids;
    int counts[3] = {};
    int merchants[3] = {};
    int guards[3] = {};
    int scholars[3] = {};
    for(const auto& r : Residents) {
        assert(ids.emplace(r.id).second);
        assert(std::strlen(r.name) > 3);
        assert(std::strlen(r.authoredLine) > 12);
        const int idx = static_cast<int>(r.home);
        assert(idx >= 0 && idx < 3);
        ++counts[idx];
        merchants[idx] += r.role == NpcRole::Merchant;
        guards[idx] += r.role == NpcRole::Guard;
        scholars[idx] += r.role == NpcRole::Scholar;
        const Vec2 position = ResidentWorldPosition(r);
        assert(SettlementAt(position.x, position.y) == r.home);
        const Vec2 night = LocalRoutineTarget(r.home, r.role, DayPhase::Night,position);
        assert(SettlementAt(night.x,night.y)==r.home);
        const Vec2 rumor = LocalRumorTarget(r.home);
        const Vec2 investigate = LocalInvestigationTarget(r.home);
        assert(SettlementAt(rumor.x,rumor.y)==r.home);
        assert(SettlementAt(investigate.x,investigate.y)==r.home);
    }
    for (int i = 0; i < 3; ++i) {
        assert(counts[i] >= 12);
        assert(merchants[i] > 0 && guards[i] > 0 && scholars[i] > 0);
    }
    for(const char* oldId : {"npc.merchant.001", "npc.guard.001", "npc.archivist.001",
                              "npc.welllistener.001","npc.orchardexile.001","npc.nightcourier.001"})
        assert(FindResident(oldId) != nullptr);
    assert(FindResident("npc.nonexistent") == nullptr);
    assert(FindResident(nullptr) == nullptr);
    assert(SettlementTrustDelta(SettlementId::Bellwold,true,true)>0);
    assert(SettlementTrustDelta(SettlementId::Paperhaven,true,true)<0);
    assert(SettlementTrustDelta(SettlementId::Crossings,true,true)==0);
    assert(SettlementTrustDelta(SettlementId::Paperhaven,false,false)>0);
    assert(SettlementTrustDelta(SettlementId::Bellwold,false,false)<0);
    SettlementVisits visits;
    assert(visits.Visit(SettlementId::Crossings));
    assert(visits.Visit(SettlementId::Bellwold));
    assert(!visits.Visit(SettlementId::Crossings));
    assert(visits.Count()==2);
    const int saved=visits.Snapshot();
    SettlementVisits loaded;
    assert(loaded.Restore(saved) && loaded.Count()==2);
    assert(!loaded.Restore(1<<9) && loaded.Count()==2);
    assert(!loaded.Visit(SettlementId::None));
    assert(loaded.Visit(SettlementId::Paperhaven) && loaded.Count()==3);
    std::cout << "PASS: 3 villages, 48 original residents, homebound schedules, local belief, visit saves\n";
}
