#include "Player/UnmadeCharacterIdentityRules.h"
#include <cassert>
#include <string>
#include <iostream>
using namespace UnmadeCore;
int main() {
    CharacterIdentity profile;
    assert(profile.SharedOrigin()=="Unmade.Origin.Impossible");
    assert(profile.Snapshot().chosenName=="The Unmade");
    assert(profile.Select(IdentityAspect::BodyFrame,3));
    assert(profile.Select(IdentityAspect::Face,7));
    assert(profile.Select(IdentityAspect::Hair,9));
    assert(profile.Select(IdentityAspect::Voice,5));
    assert(profile.Select(IdentityAspect::SkinPalette,9));
    assert(profile.Select(IdentityAspect::RealityMark,7));
    assert(profile.Select(IdentityAspect::Gait,4));
    assert(profile.Select(IdentityAspect::Calling,3));
    assert(!profile.Select(IdentityAspect::Hair,10));
    assert(!profile.Select(IdentityAspect::BodyFrame,-1));
    assert(profile.Rename(u8"Vēyl-No One"));
    assert(profile.Snapshot().chosenName==u8"Vēyl-No One");
    assert(!profile.Rename("    "));
    assert(!profile.Rename("Name\nInjection"));
    assert(!profile.Rename(std::string("\xC0\xAF",2)));
    assert(!profile.Rename(std::string(49,'A')));
    const auto preserved=profile.Snapshot();
    CharacterIdentity restored;
    assert(restored.Restore(preserved));
    assert(restored.SharedOrigin()==profile.SharedOrigin());
    auto invalid=preserved;
    invalid.options[3]=99;
    assert(!restored.Restore(invalid));
    assert(restored.Snapshot().options==preserved.options);
    assert(restored.Snapshot().chosenName==preserved.chosenName);
    invalid=preserved; invalid.chosenName=std::string(1,'\x01');
    assert(!restored.Restore(invalid));
    assert(restored.Snapshot().chosenName==preserved.chosenName);
    assert(restored.Select(IdentityAspect::Voice,1));
    assert(profile.Snapshot().options[3]==5); // no cross-character shared state
    std::cout<<"PASS: eight authored customization dimensions, name, immutable origin and atomic saves\n";
}
