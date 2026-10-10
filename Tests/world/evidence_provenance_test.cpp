#include "World/UnmadeEvidenceProvenanceRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main() {
    WitnessEchoLedger ledger;
    assert(EvidenceNotebook.size()==3);
    assert(NotebookStage(ledger,"CALL.01")==EvidenceStage::Hidden);
    assert(KnownNotebookNode(ledger,"CALL.01")==nullptr);
    assert(KnownNotebookNode(ledger,"CALL.03")==nullptr);
    assert(!NotebookComparisonUnlocked(ledger));
    assert(ledger.Read("site.callback.uncounted_cup",0)==WitnessEchoEvent::FirstReading);
    const auto* hessa=KnownNotebookNode(ledger,"CALL.01");
    assert(hessa!=nullptr && std::strcmp(hessa->namedSpeaker,"Hessa")==0);
    assert(std::strstr(hessa->theory,"suspects")!=nullptr);
    assert(KnownNotebookNode(ledger,"CALL.03")==nullptr);
    assert(NotebookStage(ledger,"CALL.01")==EvidenceStage::PersonallySeen);
    ledger.Read("site.callback.inkless_nail",0);
    ledger.Read("site.callback.two_faced_press",0);
    assert(!NotebookComparisonUnlocked(ledger));
    ledger.Read("site.callback.two_faced_press",1);
    assert(NotebookStage(ledger,"CALL.03")==EvidenceStage::SeenAgain);
    assert(NotebookComparisonUnlocked(ledger));
    const auto snap=ledger.Snapshot();
    WitnessEchoLedger restored;
    assert(restored.Restore(snap));
    assert(KnownNotebookNode(restored,"CALL.03")!=nullptr);
    assert(KnownNotebookNode(restored,"CALL.XX")==nullptr);
    assert(BraidAccountFromEvent("World.WitnessBraidShelter")==WitnessBraidAccount::Shelter);
    assert(BraidAccountFromEvent("World.WitnessBraidDocket")==WitnessBraidAccount::Docket);
    assert(BraidAccountFromEvent("World.WitnessBraidResolved")==WitnessBraidAccount::None);
    assert(IsBraidEvent("World.WitnessBraidShelter"));
    assert(!IsBraidEvent("World.Random"));
    assert(std::strlen(BraidAccountLine(WitnessBraidAccount::Shelter,WitnessProvenance::NoEvidence))==0);
    assert(std::strlen(BraidAccountLine(WitnessBraidAccount::Docket,WitnessProvenance::HeardFromPerson))==0);
    const char* direct=BraidAccountLine(WitnessBraidAccount::Docket,WitnessProvenance::PersonallyWitnessed);
    const char* rumor=BraidAccountLine(WitnessBraidAccount::Docket,WitnessProvenance::HeardFromPerson,"npc.bridgekeeper.001");
    assert(std::strstr(direct,"I saw") && !std::strstr(rumor,"I saw"));
    assert(std::strstr(rumor,"I have not seen")!=nullptr);
    assert(std::strstr(BraidAccountLine(WitnessBraidAccount::Shelter,
        WitnessProvenance::PersonallyWitnessed),"cannot call the case closed")!=nullptr);
    std::cout<<"PASS: provenance-limited 3-city notebook, source-attributed and rumor-safe NPC dialogue\n";
}
