#pragma once
namespace UnmadeCore {
// The invoking component stages Tenfold, Confluence and Strain. Stage both world
// ledgers before that same writer, and restore every world model if it fails.
template<class Civic,class Outer,class Resonance,class Stage,class Writer>
bool CommitRiteWorld(Civic& civic,Outer& outer,Resonance& resonance,Stage stage,Writer write){
 const auto beforeCivic=civic;const auto beforeOuter=outer;const auto beforeResonance=resonance;
 stage();
 if(write())return true;
 civic=beforeCivic;outer=beforeOuter;resonance=beforeResonance;return false;
}
}
