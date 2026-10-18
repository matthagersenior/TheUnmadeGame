#pragma once
// A source-only assignment policy for nine return-road civic routines.
// Local motion comes from the existing swept NPC controller; no new save bits.
namespace UnmadeCore {
enum class AccordDuty { None, Reserve, Hearing };
struct PublicAccordAssignment {
 bool active=false;
 AccordDuty duty=AccordDuty::None;
 int dx=0,dy=0;
};
// Offset from the existing provisioner near the realm's return-road homes.
// The offsets differ across nine settlements and across public resolutions.
inline PublicAccordAssignment PublicAccordDuty(
 int realm,int person,int choice,bool returned,int rescuedMask,
 bool atNight,bool acceptedSave) noexcept {
 if(realm<0||realm>=9||person<0||person>4||
    choice<1||choice>2||!returned||!acceptedSave||atNight)return {};
 if(person==1&&!(rescuedMask&1))return {};
 if(person==2&&!(rescuedMask&2))return {};
 constexpr int sites[9][2][2]={
  {{-440,-280},{540,-570}},  // Crossings: protected lanterns / published names
  {{-390,-180},{650,-510}},  // Saltwake: water hour / withdrawal ledger
  {{-325,-325},{730,-425}},  // Cinderhold: warm homes / rescue reserve
  {{-470,-120},{690,-370}},  // Drevlach: private guarantor / public oath
  {{-285,-365},{620,-620}},  // Orravane: anchored span / load watch
  {{-400,-250},{785,-540}},  // Vathless: sealed trust / family wall
  {{-260,-155},{720,-300}},  // Eillun: protected registry / appeal witness
  {{-365,-415},{585,-460}},  // Tharniv: household share / common way
  {{-500,-320},{810,-350}}   // Auvren: rescue registry / warning map
 };
 const int branch=choice-1;
 const int spreadX=(person%3-1)*95;
 const int spreadY=(person/3)*95;
 return {true,choice==1?AccordDuty::Reserve:AccordDuty::Hearing,
         sites[realm][branch][0]+spreadX,sites[realm][branch][1]+spreadY};
}
} // namespace UnmadeCore
