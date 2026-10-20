#include "World/UnmadeRealmAftermathRules.h"
#include <array>
#include <cassert>
#include <cstring>

using namespace UnmadeCore;

struct Expected {
    Realm realm;
    const char* speaker;
    const char* care;
    const char* truth;
};

int main() {
    const std::array<Expected,9> expected={{
        {Realm::ThreefoldReach,"npc.archivist.001",
         "The refuge cord now guides unnamed visitors toward shelter. The archive still lacks a public lead.",
         "The public docket lets people compare all three objects without naming protected guests. What questions will they bring to it?"},
        {Realm::WidowedRain,"npc.saltwake.navigator.001",
         "The ferry passage shelters families without asking for old ownership papers. The missing sea still has no name on the map.",
         "The ledger records the missing water and its collateral; survivors are not declared debtors."},
        {Realm::HearthBeneath,"npc.cinderhold.hearthreader.001",
         "The unregistered homes have heat without signing first. The kiln’s fuel still carries its old cost.",
         "The ember’s source is public, with a repair plan the city can answer for."},
        {Realm::TidalLedger,"npc.tidal.ledgerkeeper.001",
         "Tomorrow’s children can cross without carrying yesterday’s payments.",
         "The fraudulent contracts are open to public proof; borrowers’ addresses remain private."},
        {Realm::SkyBelow,"npc.sky.choirmaster.001",
         "The houses stay aloft to a refrain people chose to sing.",
         "The forbidden verse and its names are public; the choice to sing remains theirs."},
        {Realm::CinderSpine,"npc.bones.seamwarden.001",
         "The passage is braced without quarrying another remembered ancestor.",
         "The carved lineages are open for descendants to contest."},
        {Realm::HundredUnlived,"npc.unlived.censuskeeper.001",
         "The refuge stays open without asking anyone’s name.",
         "The recognition board waits for each resident to choose whether to speak."},
        {Realm::OrchardOfKings,"npc.orchard.untitled.001",
         "The harvest paths are open to those who tend them; the crown remains empty.",
         "The unsigned law is public, with room for neighbors to challenge it."},
        {Realm::FirstAbsence,"npc.absence.firstwitness.001",
         "Every traveler has a reliable way back, whatever their origin.",
         "Both maps remain open to inspection; neither is the required history."}
    }};
    assert(FirstStoryReturnDialogue.size()==expected.size());
    for(const auto& row:expected) {
        assert(std::strcmp(FirstStoryReturnLine(row.realm,row.speaker,1),row.care)==0);
        assert(std::strcmp(FirstStoryReturnLine(row.realm,row.speaker,2),row.truth)==0);
        assert(std::strcmp(FirstStoryReturnLine(row.realm,row.speaker,0),"")==0);
        assert(std::strcmp(FirstStoryReturnLine(row.realm,row.speaker,3),"")==0);
        assert(std::strcmp(FirstStoryReturnLine(row.realm,"npc.unrelated.person.001",1),"")==0);
        assert(std::strcmp(row.care,row.truth)!=0);
    }
}
