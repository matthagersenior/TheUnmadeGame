#include "Lexicon/UnmadeLexiconRules.h"
#include <cassert>
#include <iostream>
#include <vector>
using namespace UnmadeCore;
int main() {
    LexiconModel journal;
    assert(journal.Interpretation("term.veyl")==InterpretationState::Unknown);
    assert(!journal.RecordEvidence("term.unsupported","evidence.glimpse"));
    assert(!journal.RecordEvidence("term.veyl","evidence.fake"));
    assert(journal.RecordEvidence("term.veyl","evidence.glimpse"));
    assert(journal.Interpretation("term.veyl")==InterpretationState::Partial);
    assert(!journal.RecordEvidence("term.veyl","evidence.glimpse"));
    assert(journal.RecordEvidence("term.veyl","evidence.archivist"));
    assert(journal.Interpretation("term.veyl")==InterpretationState::Understood);
    assert(!journal.RecordEvidence("term.veyl","evidence.archivist"));
    assert(journal.Interpretation("term.invalid")==InterpretationState::Unknown);
    auto snapshot=journal.Snapshot();
    LexiconModel reloaded;
    assert(reloaded.Restore(snapshot));
    assert(reloaded.Interpretation("term.veyl")==InterpretationState::Understood);
    assert(reloaded.Restore(snapshot) && reloaded.Snapshot()==snapshot);
    assert(!reloaded.Restore({"evidence.archivist","evidence.archivist"}));
    assert(reloaded.Interpretation("term.veyl")==InterpretationState::Understood);
    assert(!reloaded.Restore({"evidence.unknown"}));
    assert(reloaded.Interpretation("term.veyl")==InterpretationState::Understood);
    assert(reloaded.Restore({}));
    assert(reloaded.Interpretation("term.veyl")==InterpretationState::Unknown);
    std::cout<<"PASS: offline lexicon evidence, dedupe, partial/full understanding, atomic save restore\n";
}
