#include "Fracture/UnmadeFractureRules.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
using namespace UnmadeCore;
int main() {
    FractureModel m("region.prototype.hub", {"variant.open","variant.sealed"});
    assert(m.Glimpse(false,1) == Result::UnsupportedTarget && m.CurrentStrain()==0);
    assert(m.Fold(false,1) == Result::UnsupportedTarget);
    assert(m.Glimpse(true,2) == Result::Applied && m.CurrentStrain()==8);
    assert(m.IsGlimpsing(3) && !m.IsGlimpsing(5.1) && m.WorldVariant().empty());
    assert(m.Fold(true,6)==Result::Applied && m.IsFolded(10));
    assert(m.Fold(true,7)==Result::AlreadyActive && m.CurrentStrain()==32);
    assert(!m.IsFolded(12.1));
    assert(m.Rewrite("region.invalid","variant.open",true)==Result::UnsupportedTarget);
    assert(m.Rewrite("region.prototype.hub","variant.invalid",true)==Result::UnsupportedChoice);
    assert(m.Rewrite("region.prototype.hub","variant.open",false)==Result::NeedsConfirmation);
    assert(m.CurrentStrain()==32 && m.WorldVariant().empty());
    assert(m.Rewrite("region.prototype.hub","variant.open",true)==Result::Applied);
    assert(m.WorldVariant()=="variant.open" && m.CurrentStrain()==92);
    assert(m.Rewrite("region.prototype.hub","variant.open",true)==Result::AlreadyCommitted);
    assert(m.Rewrite("region.prototype.hub","variant.sealed",true)==Result::AlreadyCommitted);
    assert(m.CurrentStrain()==92);
    assert(!m.Recover(-2) && !m.Recover(std::numeric_limits<double>::infinity()));
    assert(!m.Recover(std::numeric_limits<double>::quiet_NaN()));
    assert(m.Recover(10) && std::abs(m.CurrentStrain()-62.0)<0.001);
    assert(m.Recover(999) && m.CurrentStrain()==0);
    FractureModel gearEnabled("region.prototype.hub", {"variant.open","variant.sealed"});
    assert(gearEnabled.Glimpse(true,1.0,3.0)==Result::Applied);
    assert(gearEnabled.CurrentStrain()==5.0);
    assert(gearEnabled.Glimpse(true,2.0,-1.0)==Result::InvalidModifier);
    assert(gearEnabled.CurrentStrain()==5.0);
    assert(gearEnabled.Fold(true,3.0,3.0)==Result::Applied);
    assert(gearEnabled.IsFolded(11.99) && !gearEnabled.IsFolded(12.0));
    assert(gearEnabled.Fold(true,13.0,999.0)==Result::InvalidModifier);
    assert(gearEnabled.CurrentStrain()==29.0);
    FractureModel fresh("region.prototype.hub", {"variant.open","variant.sealed"});
    assert(fresh.Restore(m.TakeSnapshot()) && fresh.WorldVariant()=="variant.open");
    assert(!fresh.IsFolded(0) && !fresh.IsGlimpsing(0));
    FractureSnapshot bad = m.TakeSnapshot(); bad.strain=120;
    assert(!fresh.Restore(bad) && fresh.WorldVariant()=="variant.open");
    bad = m.TakeSnapshot(); bad.variant="variant.injected";
    assert(!fresh.Restore(bad) && fresh.WorldVariant()=="variant.open");
    FractureModel exhausted("region.prototype.hub", {"variant.open","variant.sealed"});
    assert(exhausted.Restore({"",95.0}));
    assert(exhausted.Fold(true,0)==Result::NotEnoughStability);
    assert(exhausted.Rewrite("region.prototype.hub","variant.open",true)==Result::NotEnoughStability);
    assert(exhausted.WorldVariant().empty() && exhausted.CurrentStrain()==95);
    std::cout<<"PASS fracture domain tests: Glimpse/Fold/Rewrite, costs, expiry, recovery and save validation\n";
}
