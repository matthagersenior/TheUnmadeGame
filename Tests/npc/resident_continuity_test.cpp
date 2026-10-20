#include "NPC/UnmadeResidentContinuityRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
    static_assert(ResidentContinuityLegacyCount==82,"Old 48+16+18 save ordering must remain migratable");
    static_assert(ResidentContinuityCount==88,"82 residents plus six appended scouts");
    std::set<std::string> names;
    for(int i=0;i<ResidentContinuityCount;++i){
        const char* id=ResidentContinuityId(i);
        assert(id && std::strlen(id)>10);
        assert(names.insert(id).second);
        assert(ResidentContinuityIndex(id)==i);
    }
    assert(ResidentContinuityIndex(nullptr)==-1);
    assert(ResidentContinuityIndex("npc.fake.return.001")==-1);
    assert(ResidentContinuityId(82)!=nullptr);
    assert(ResidentContinuityId(88)==nullptr);
    int oldVisits[ResidentContinuityLegacyCount]{};
    int oldDays[ResidentContinuityLegacyCount]{};
    int oldAids[ResidentContinuityLegacyCount]{};
    oldVisits[0]=2;oldDays[0]=7;oldAids[0]=1;
    ResidentContinuitySnapshot migrated;
    assert(BuildResidentContinuitySnapshot(
        oldVisits,ResidentContinuityLegacyCount,
        oldDays,ResidentContinuityLegacyCount,
        oldAids,ResidentContinuityLegacyCount,migrated));
    assert(migrated.visits[0]==2&&migrated.lastDay[0]==7&&migrated.aids[0]==1);
    for(int i=ResidentContinuityLegacyCount;i<ResidentContinuityCount;++i)
        assert(migrated.visits[i]==0&&migrated.lastDay[i]==0&&migrated.aids[i]==0);
    assert(!BuildResidentContinuitySnapshot(
        oldVisits,ResidentContinuityLegacyCount,
        oldDays,ResidentContinuityLegacyCount-1,
        oldAids,ResidentContinuityLegacyCount,migrated));
    ResidentContinuity journal;
    assert(journal.Talk(Residents[0].id,1)==EncounterEvent::FirstHello);
    assert(journal.Level(Residents[0].id)==Familiarity::Recognized);
    assert(journal.Talk(Residents[0].id,1)==EncounterEvent::SameDay);
    assert(journal.Talk(Residents[0].id,0)==EncounterEvent::InvalidClock);
    assert(journal.Talk(Residents[0].id,2)==EncounterEvent::ReturnHello);
    assert(journal.Talk(Residents[0].id,1)==EncounterEvent::InvalidClock);
    assert(journal.Level(Residents[0].id)==Familiarity::Acquainted);
    assert(journal.Aid(Residents[0].id)==EncounterEvent::Aided);
    assert(journal.Aid(Residents[0].id)==EncounterEvent::AlreadyAided);
    assert(journal.Aided(Residents[0].id));
    assert(std::strstr(ResidentContinuityLine(journal.Level(Residents[0].id),true,
        false,false),"help")!=nullptr);
    assert(std::strstr(ResidentContinuityLine(Familiarity::Stranger,false,
        false,false),"begin")!=nullptr);
    for(int day=3;day<15;++day)
        journal.Talk(Residents[0].id,day);
    assert(journal.Visits(Residents[0].id)==5);
    const auto frontier=FrontierResidents[0].id,later=OuterPeople[0].id;
    assert(journal.Talk(frontier,3)==EncounterEvent::FirstHello);
    assert(journal.Talk(later,3)==EncounterEvent::FirstHello);
    assert(journal.Talk(frontier,4)==EncounterEvent::ReturnHello);
    assert(journal.Visits(later)==1 && journal.Visits(frontier)==2);
    assert(!journal.Aided(later));
    assert(std::strstr(ResidentContinuityLine(Familiarity::Acquainted,false,
        true,false),"records")!=nullptr);
    assert(std::strstr(ResidentContinuityLine(Familiarity::Trusted,false,
        true,true),"sky")!=nullptr);
    const auto snapshot=journal.Snapshot();
    ResidentContinuity restored;
    assert(restored.Restore(snapshot));
    assert(restored.Level(frontier)==Familiarity::Acquainted);
    assert(restored.Aided(Residents[0].id));
    auto bad=snapshot;bad.visits[2]=6;
    assert(!restored.Restore(bad));
    bad=snapshot;bad.lastDay[1]=10;
    assert(!restored.Restore(bad));
    bad=snapshot;bad.aids[1]=2;
    assert(!restored.Restore(bad));
    assert(restored.Visits(frontier)==2);
    ResidentContinuity oldSlot;
    assert(oldSlot.Restore({}));
    assert(oldSlot.Level(later)==Familiarity::Stranger);
    std::cout<<"PASS: 88 resident profiles, legacy 82-slot migration, visit/day limits, aid, local returns, corrupt-state refusal\n";
}
