#pragma once
// A source-only assignment policy for nine return-road civic routines.
// Local motion comes from the existing swept NPC controller; no new save bits.
namespace UnmadeCore {
enum class AccordDuty { None, Reserve, Hearing, Sanctuary, RoadWatch };
struct PublicAccordAssignment {
 bool active=false;
 AccordDuty duty=AccordDuty::None;
 int dx=0,dy=0;
};
// Offset from the existing provisioner near the realm's return-road homes.
// The offsets differ across nine settlements and across public resolutions.
inline PublicAccordAssignment PublicAccordDuty(
 int realm,int person,int choice,bool returned,int rescuedMask,
 bool atNight,bool acceptedSave,
 bool claimantResolved=false,bool postsRepaired=false) noexcept {
 if(realm<0||realm>=9||person<0||person>4||
    choice<1||choice>2||!returned||!acceptedSave||atNight)return {};
 if(person==1&&!(rescuedMask&1))return {};
 if(person==2&&!(rescuedMask&2))return {};
 // Reserve, hearing, earned sanctuary, and claimant-cleared road watch.
 constexpr int sites[9][4][2]={
  {{350,70},{540,-570},{540,350},{790,130}},  // Crossings
  {{390,180},{650,-510},{580,420},{830,220}},  // Saltwake
  {{420,0},{730,-425},{640,310},{760,90}},    // Cinderhold
  {{380,240},{690,-370},{560,470},{940,170}}, // Drevlach
  {{450,-90},{620,-620},{600,280},{870,10}},  // Orravane
  {{410,120},{785,-540},{720,390},{890,280}}, // Vathless
  {{480,260},{720,-300},{690,450},{810,330}}, // Eillun
  {{430,-170},{585,-460},{570,340},{970,110}},// Tharniv
  {{360,330},{810,-350},{650,500},{900,190}} // Auvren
 };
 const int branch=choice==1?(postsRepaired?2:0):(claimantResolved?3:1);
 const int spreadX=(person%3-1)*95;
 const int spreadY=(person/3)*95;
 const AccordDuty duty=branch==2?AccordDuty::Sanctuary:
    branch==3?AccordDuty::RoadWatch:
    choice==1?AccordDuty::Reserve:AccordDuty::Hearing;
 return {true,duty,
         sites[realm][branch][0]+spreadX,sites[realm][branch][1]+spreadY};
}
} // namespace UnmadeCore
