#pragma once
// Source-authored nine-realm visual identity for native Unreal primitive meshes.
// No asset pack, save key, lore shortcut, interactable transform or collision change.
// Names and profiles intentionally remain deterministic for repeated reloads.
#include <array>
#include <cstring>

namespace UnmadeCore {
struct RealmRgb { int R,G,B; };
struct RealmSilhouettePart {
 int dx,dy,dz; float sx,sy,sz; // centimeters relative to the frontier site
};
struct RealmVisualDesign {
 const char* name;
 const char* landmark;
 const char* storyMotif;
 RealmRgb color,accent;
 std::array<RealmSilhouettePart,3> silhouette;
};
inline constexpr std::array<RealmVisualDesign,9> RealmVisualDesigns={{
 {"Crossings","The Three-Span Charter","An interrupted road of three unequal survey columns",
  {58,76,112},{228,181,108},
  {{{-2600,550,160,.65f,.8f,3.2f},{-2190,550,205,.35f,.7f,4.1f},{-1750,550,140,.8f,.8f,2.8f}}}},
 {"Saltwake","The Dry-Tide Gauge","A broken rain pole above a waterline that can no longer agree with itself",
  {39,105,120},{221,180,127},
  {{{-2580,620,205,.55f,.65f,4.1f},{-2130,620,140,1.6f,.42f,2.8f},{-1750,620,260,.45f,.7f,5.2f}}}},
 {"Cinderhold","The Borrowed Hearth Vent","Twin chimney throats connected by the commons, not a private crown",
  {122,66,55},{242,172,87},
  {{{-2590,580,205,.9f,.9f,4.1f},{-2120,580,135,1.45f,.8f,2.7f},{-1740,580,235,.65f,.8f,4.7f}}}},
 {"Drevlach","The Foreclosed Oath Pier","Three stacked debt slates balanced over an unowned crossing",
  {58,88,111},{213,164,109},
  {{{-2590,600,160,1.3f,.45f,3.2f},{-2180,600,230,.45f,.75f,4.6f},{-1750,600,175,1.15f,.5f,3.5f}}}},
 {"Orravane","The Choir Counterweight","An inverted conducting rest held between two unequal supports",
  {78,100,152},{232,212,163},
  {{{-2560,580,235,.65f,.65f,4.7f},{-2150,580,130,1.8f,.38f,2.6f},{-1740,580,290,.5f,.8f,5.8f}}}},
 {"Vathless","The Twin Ancestral Seam","Two incompatible quarry layers carrying the same family mark",
  {106,91,76},{190,174,127},
  {{{-2540,590,180,1.15f,.8f,3.6f},{-2160,590,100,.65f,.75f,2.0f},{-1750,590,225,1.3f,.5f,4.5f}}}},
 {"Eillun","The Occupied Blank Bed","A missing census entry framed by two standing shelter records",
  {101,81,131},{205,192,211},
  {{{-2560,600,215,.45f,.65f,4.3f},{-2140,600,115,1.65f,.7f,2.3f},{-1740,600,215,.45f,.65f,4.3f}}}},
 {"Tharniv","The Unclaimed Graft","A public root lattice refuses the profile of a royal throne",
  {65,110,76},{188,181,105},
  {{{-2580,580,160,.9f,1.1f,3.2f},{-2100,580,235,1.35f,.4f,4.7f},{-1740,580,185,.8f,1.1f,3.7f}}}},
 {"Auvren","The Double Honest Threshold","Two offset unfinished doors preserve incompatible but traversable approaches",
  {79,75,124},{172,220,216},
  {{{-2580,610,260,.45f,1.0f,5.2f},{-2140,610,185,1.55f,.35f,3.7f},{-1720,610,260,.45f,1.0f,5.2f}}}}
}};

enum class VisualRole { Foundation,Gate,Evidence,Mechanism,Landmark,Ordinary };
struct VisualProp {
 int realm=-1;VisualRole role=VisualRole::Ordinary;
 bool keepCubeCollision=true;
 bool allowDecoration=false;
};
inline bool VisualHas(const char* s,const char* part) noexcept {
 return s && part && std::strstr(s,part)!=nullptr;
}
inline int RealmForVisualTag(const char* label) noexcept {
 if(!label||!label[0])return -1;
 if(std::strncmp(label,"Expedition.",11)==0) {
   const char digit=label[11];
   return digit>='0'&&digit<='8'&&label[12]=='.'?digit-'0':-1;
 }
 constexpr const char* names[9]={
  "Crossings.","Saltwake.","Cinderhold.","Tidal.","Sky.",
  "Bones.","Unlived.","Orchard.","Absence."};
 for(int a=0;a<9;++a)
  if(std::strncmp(label,names[a],std::strlen(names[a]))==0)return a;
 if(std::strncmp(label,"Rain.",5)==0)return 1;
 if(std::strncmp(label,"Hearth.",7)==0)return 2;
 return -1;
}
inline VisualProp ClassifyRealmProp(const char* label,double sx,double sy,double sz) noexcept {
 VisualProp result;result.realm=RealmForVisualTag(label);
 if(!label||!label[0])return result;
 if(VisualHas(label,"Landmark.")){
  result.role=VisualRole::Landmark;result.allowDecoration=true;
 }else if(VisualHas(label,"Gate")||VisualHas(label,"Barrier")||
          VisualHas(label,"Door.Seal")){
  result.role=VisualRole::Gate;result.allowDecoration=sz>.8&&sx<15;
 }else if(VisualHas(label,"Structure")||VisualHas(label,"Ground")||
          VisualHas(label,"CivicWall")||VisualHas(label,"Floor")||
          VisualHas(label,"Route")||VisualHas(label,"Causeway")||
          (sx>=10&&sy>=10&&sz<=.8)){
  result.role=VisualRole::Foundation;
 }else if(VisualHas(label,"Vent")||VisualHas(label,"Sluice")||
          VisualHas(label,"Dial")||VisualHas(label,"Tuner")||
          VisualHas(label,"Lock")||VisualHas(label,"Valve")||
          VisualHas(label,"Accord")||VisualHas(label,"Switch")||
          VisualHas(label,"Control")||VisualHas(label,"Lever")){
  result.role=VisualRole::Mechanism;result.allowDecoration=true;
 }else if(VisualHas(label,"Evidence")||VisualHas(label,"Clue")||
          VisualHas(label,"Ledger")||VisualHas(label,"Archive")||
          VisualHas(label,"Record")||VisualHas(label,"Fracture")||
          VisualHas(label,"Deed")||VisualHas(label,"Registry")||
          VisualHas(label,"Board")||VisualHas(label,"Counterfoil")||
          VisualHas(label,"Common")||VisualHas(label,"Brace")||
          VisualHas(label,"Seam")||VisualHas(label,"Census")){
  result.role=VisualRole::Evidence;result.allowDecoration=true;
 }
 // The base cube remains the sole gameplay collision owner. Decorative
 // cylinder/sphere/cone children are visual-only and must never block E/LOS.
 return result;
}
} // namespace UnmadeCore
