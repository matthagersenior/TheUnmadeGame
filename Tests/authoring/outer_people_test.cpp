#include "Authoring/UnmadeOuterPeopleData.h"
#include "World/UnmadeEchoQuestRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
    assert(OuterPeople.size()==18);
    std::set<std::string> names,ids;
    for(const auto& p:OuterPeople){
        assert(p.id && p.displayName && p.realm);
        assert(std::strlen(p.displayName)>6);
        assert(std::strlen(p.dayLine)>48);
        assert(std::strlen(p.nightLine)>48);
        assert(std::strcmp(p.dayLine,p.nightLine)!=0);
        assert(std::strlen(p.realm)>6);
        assert(ids.insert(p.id).second);
        assert(names.insert(p.displayName).second);
        const auto* found=FindOuterPerson(p.id);
        assert(found && std::strcmp(found->displayName,p.displayName)==0);
    }
    assert(FindOuterPerson(nullptr)==nullptr);
    assert(FindOuterPerson("npc.unknown.000")==nullptr);
    for(const auto& quest:EchoQuests){
        assert(FindOuterPerson(quest.openingWitness));
        assert(FindOuterPerson(quest.witnessCare));
        assert(FindOuterPerson(quest.witnessTruth));
        assert(std::strcmp(FindOuterPerson(quest.openingWitness)->id,
                           FindOuterPerson(quest.witnessCare)->id)!=0);
    }
    std::cout<<"PASS: 18 stable named people, unique day/night voices, six witness triples\n";
}
