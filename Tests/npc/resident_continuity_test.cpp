#include "NPC/UnmadeResidentContinuityRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
    static_assert(ResidentContinuityCount==82,"48+16+18 stable residents required");
    std::set<std::string> names;
    for(int i=0;i<ResidentContinuityCount;++i){
        const char* id=ResidentContinuityId(i);
        assert(id && std::strlen(id)>10);
        assert(names.insert(id).second);
        assert(ResidentContinuityIndex(id)==i);
    }
    assert(ResidentContinuityIndex(nullptr)==-1);
    assert(ResidentContinuityIndex("npc.fake.return.001")==-1);
    assert(ResidentContinuityId(82)==nullptr);
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
        false,false),"first")!=nullptr);
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
    std::cout<<"PASS: 82 individually saved identities, visit/day limits, aid, local returns, corrupt-state refusal\n";
}
