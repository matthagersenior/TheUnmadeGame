#include "Story/UnmadeCommitmentRules.h"
#include <cassert>
#include <iostream>
#include <limits>
using namespace UnmadeCore;
int main() {
    CommitmentGate gate;
    assert(gate.Attempt("Saltwake.Return",1,10)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Saltwake.Return",1,12)==CommitmentAttempt::Confirmed);
    assert(gate.Attempt("Saltwake.Return",1,12)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Cinderhold.Return",1,13)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Cinderhold.Return",2,14)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Saltwake.Return",2,15)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Cinderhold.Return",2,16)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Cinderhold.Return",2,23)==CommitmentAttempt::Preview); // timed out
    assert(gate.Attempt("Cinderhold.Return",2,28)==CommitmentAttempt::Confirmed);
    assert(gate.Attempt("Cinderhold.Return",1,30)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Cinderhold.Return",1,29)==CommitmentAttempt::Preview); // time reversal
    assert(gate.Attempt("",1,30)==CommitmentAttempt::Rejected);
    assert(gate.Attempt("Cinderhold.Return",1,31)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Cinderhold.Return",9,32)==CommitmentAttempt::Rejected);
    assert(gate.Attempt("Cinderhold.Return",1,33)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Cinderhold.Return",1,
        std::numeric_limits<double>::quiet_NaN())==CommitmentAttempt::Rejected);
    gate.Cancel();
    assert(gate.Attempt("Bellwold.SecondNight",2,36)==CommitmentAttempt::Preview);
    assert(gate.Attempt("Bellwold.SecondNight",2,38)==CommitmentAttempt::Confirmed);
    std::cout<<"PASS: targeted six-second warning, changed witnesses/paths, cancellation and expiry\n";
}
