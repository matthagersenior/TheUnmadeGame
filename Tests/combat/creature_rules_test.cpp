#include "Combat/UnmadeCreatureRules.h"
#include <cassert>
#include <limits>
int main() {
    using namespace UnmadeCore;
    for(int i=0;i<12;++i) {
        const auto p=CreatureStats(static_cast<CreatureKind>(i));
        const auto elder=CreatureStats(static_cast<CreatureKind>(i),true);
        assert(elder.health>p.health&&elder.windup>p.windup);
        assert(CreatureIntent(p,p.reach,true,false)==EnemyIntent::Attack);
        assert(CreatureIntent(p,p.reach,true,true)==EnemyIntent::Hold);
        assert(CreatureIntent(p,p.reach,false,false)!=EnemyIntent::Attack);
        assert(CreatureIntent(p,std::numeric_limits<double>::quiet_NaN(),true,false)==EnemyIntent::Hold);
        if(p.retreat>0)assert(CreatureIntent(p,p.retreat/2,true,false)==EnemyIntent::Retreat);
    }
    assert(CreatureStats(CreatureKind::Ashmaw).damage>CreatureStats(CreatureKind::GleamMoth).damage);
    assert(CreatureStats(CreatureKind::PaleStrider).style==EnemyStyle::Stalker);
    assert(CreatureStats(CreatureKind::Veilwatch).style==EnemyStyle::Watcher);
}
