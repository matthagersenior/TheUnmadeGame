#pragma once
// All mutable player choices remain in their existing game systems.
// These authoring decisions are static and never allocate new quest/save IDs.
// Parent cube and its original collision are preserved on every art pass.
#include "World/UnmadeProceduralArtRules.h"
#include "World/UnmadeRememberedRoadRegistry.h"
#include <array>
#include <cstring>
#include <string_view>

namespace UnmadeCore {
enum class GrayboxKit { Ground, Causeway, Commons, Market, Watch, BellTower, Sanctuary, ArchiveTower, Records, Scriptorium, Forge, Home, Bed, Hearth, Table, Bench, Gate, Portal, Civic, Mechanism, Evidence, Cargo, Ruin, Arena, Cover, Hazard, Ritual, Signal, FinalChoice, Landmark, Bridge, Garden, Workshop, Generic };
struct GrayboxProfile {
 GrayboxKit kit;
 const char* label;
 const char* surface;
 const char* material;
 const char* detail;
 const char* replacementMesh;
 bool detailEnabled;
};
inline constexpr std::array<GrayboxProfile,34> GrayboxProfiles={{
 {GrayboxKit::Ground,"Ground plane","Compacted gravel with broken paving joints","M_WeatheredEarth","No vertical trim; travelable surface is the canonical cube","SM_Common_Ground01",false},
 {GrayboxKit::Causeway,"Road and bridges","Layered stone, rope ties and wagon-scored wheel ruts","M_WornCauseway","Two edge bollards only on short pieces, never close travel lanes","SM_Common_Causeway01",false},
 {GrayboxKit::Commons,"Town commons","Shared stone pavilion, market cloth and message niche","M_SmokedStone","Shallow roof ridge and common-use lintel; no new doorway","SM_Settlement_Commons01",true},
 {GrayboxKit::Market,"Trading court","Timber sales shelf and rolled fabric canopy","M_OiledTimber","Counter lip and tied awning on facade","SM_Settlement_TradeStall01",true},
 {GrayboxKit::Watch,"Watchpost","Pitted iron and stitched duty placard","M_BlackIron","Top lantern, two spare bell brackets and sight slits","SM_Settlement_Watchtower01",true},
 {GrayboxKit::BellTower,"Refuge bell tower","Chalk limestone with salvaged lead","M_Bellwold_Chalk","An empty bell frame and suspended double hanger","SM_Bellwold_UncastBelltower01",true},
 {GrayboxKit::Sanctuary,"Shelter and refuge","Limewashed fieldstone with repaired cloth","M_RefugeStone","Roof hood, shared shelter lintel and lantern hearth","SM_Sanctuary_ShelteredThreshold01",true},
 {GrayboxKit::ArchiveTower,"Archive tower","Oxidized paper stone and bound timber shelving","M_Paperhaven_Paperstone","Serrated scroll crown and reading-gallery bracket","SM_Paperhaven_ArchiveTower01",true},
 {GrayboxKit::Records,"Registers and ledgers","Inkstone slab, paper creases and wax imprints","M_ArchiveInk","One horizontal witness shelf and offset folio disc","SM_Records_WitnessLedger01",true},
 {GrayboxKit::Scriptorium,"Scribes' workshop","Wood desks with laminated paper niches","M_InkStainedWood","Sloped reading surface and blind emboss seal","SM_Paperhaven_Scriptorium01",true},
 {GrayboxKit::Forge,"Forge and kiln","Cinderbrick, heat-checked metal and fire glazing","M_KilnAsh","A ventilated chimney hood and furnace rim","SM_Cinderhold_CommuneForge01",true},
 {GrayboxKit::Home,"Domestic walls","Locally repaired plaster and dark timber","M_LivedInPlaster","Low structural beam without added wall collision","SM_Home_ModularWall01",false},
 {GrayboxKit::Bed,"Domestic bed","Worn linen with simple bedframe","M_SpunLinen","Pillow head and simple cylinder bedroll","SM_Home_RefugeBed01",true},
 {GrayboxKit::Hearth,"Shared hearth","Blackened charcoal ring and clay mantle","M_CharcoalClay","Wide low brazier with a visible central ember stone","SM_Home_CommonHearth01",true},
 {GrayboxKit::Table,"Domestic table","Cross-braced wood with handmade pottery","M_RoughTimber","Flat circular vessel and a shallow ledger saucer","SM_Home_Worktable01",true},
 {GrayboxKit::Bench,"Domestic bench","Rough timber and patched supports","M_RoughTimber","A wooden back rail; still usable as an inspectable fixture","SM_Home_RepairBench01",true},
 {GrayboxKit::Gate,"Stateful doors","Clamped forged iron and handled hinge loops","M_AgedLatch","Two hinge barrels mounted on original door collision cube","SM_Access_StatefulGate01",true},
 {GrayboxKit::Portal,"Atlas gateway","Joined nonmatching masonry and suspended witness ring","M_AtlasThreshold","Concentric crown loop outside the player's collision channel","SM_Atlas_WitnessGateway01",true},
 {GrayboxKit::Civic,"Hearing and accord site","Carved legal stone, felt desks and witness cord","M_CivicWitnessStone","An asymmetrical public hearing shelf and non-identifying badge","SM_Civic_ChoiceDocket01",true},
 {GrayboxKit::Mechanism,"Interactable control","Weathered bronze drum with hand-operated spindle","M_ServiceBronze","Vertical shaft and gear wheel, with one E contact point","SM_Mechanism_WorkingCrank01",true},
 {GrayboxKit::Evidence,"Clue and observed relic","Split shale face, evidence folio and mica","M_SignatureEvidence","A framed artifact head and small visual-only evidence disc","SM_Clue_FirsthandSpecimen01",true},
 {GrayboxKit::Cargo,"Shelves and packages","Strapped wicker, timber crates and rope seals","M_CargoCanvas","Two banded parcels atop the existing cargo cube","SM_Cargo_DatedReceipt01",true},
 {GrayboxKit::Ruin,"Ancient remains","Fractured stone strata and eroded chisels","M_OldStone","Offset fallen lintel and broken column ends","SM_Ruin_StratifiedMasonry01",true},
 {GrayboxKit::Arena,"Combat ring","Deep scored masonry and tactical sight lines","M_ScarredArena","Silent markers only; never add unintended boss cover","SM_Arena_ReadableBoundary01",false},
 {GrayboxKit::Cover,"Combat cover","Chipped blocks with directional dents","M_BattleStone","A modest top cap, no raised projectile obstruction","SM_Combat_CoverBlock01",false},
 {GrayboxKit::Hazard,"Warning apparatus","Oxidized warning copper and chipped hazard enamel","M_DangerMetal","A conical warning crown; damage footprint unchanged","SM_Hazard_TelegraphMarker01",true},
 {GrayboxKit::Ritual,"Attunement plinth","Inlaid lode stone and paired resonance fractures","M_ResonanceStone","Two concentric rings without an extra cast position","SM_Resonance_AttunementPlinth01",true},
 {GrayboxKit::Signal,"Public relay signal","Stamped tin, woven fabric and cord post","M_CourierSignal","Small leaning lantern and delivery knot","SM_Echo_SignalMarker01",true},
 {GrayboxKit::FinalChoice,"Final threshold decision","Unfinished volcanic glass and undecided silver","M_FirstAbsenceGlass","Paired offset crowns; no automatic truth, boss or ending","SM_Final_ConditionalAnswer01",true},
 {GrayboxKit::Landmark,"Realm skyline","Dedicated local stone, visible from the frontier","M_RealmSilhouette","Architectural crown determined uniquely by the realm","SM_Realm_AuthoredSkyline01",true},
 {GrayboxKit::Bridge,"Passages and spans","Walk-worn flagged decking and anchor stone","M_BridgeSurface","Edge lighting only: cannot affect traversal collision","SM_Route_TraverseBridge01",false},
 {GrayboxKit::Garden,"Orchard and roots","Gnarled common roots and moss stone","M_RootedCommons","A paired branch motif above the public threshold","SM_Tharniv_PublicRoot01",true},
 {GrayboxKit::Workshop,"Maintenance worksite","Grease paper, tools and bronze work plate","M_RepairBronze","Accessible rivet/anvil shape, not a second action key","SM_Workshop_FieldRepair01",true},
 {GrayboxKit::Generic,"Undesignated structural prop","Weathered gray granite baseline with no invented history","M_NeutralBlock","Leave safe and plain until it receives an authored tag","SM_Generic_Blockout01",false}
}};
inline const GrayboxProfile& GrayboxDetail(GrayboxKit kit) noexcept {
 for(const auto& profile:GrayboxProfiles)if(profile.kit==kit)return profile;
 return GrayboxProfiles.back();
}
inline int GrayboxIndexAfter(const char* text,const char* prefix,int maximum) noexcept {
 if(!text||!prefix)return -1;
 const auto n=std::strlen(prefix);
 if(std::strncmp(text,prefix,n)!=0)return -1;
 const char* cursor=text+n;
 if(*cursor<'0'||*cursor>'9')return -1;
 int value=0,digits=0;
 while(*cursor>='0'&&*cursor<='9'&&digits<3){
  value=value*10+(*cursor++-'0');++digits;
 }
 return digits>0&&(*cursor=='.'||*cursor=='\0')&&value<=maximum?value:-1;
}
struct GrayboxArt {
 int realm=0;
 int paletteSlot=11;
 GrayboxKit kit=GrayboxKit::Generic;
 const char* detail="";
 const char* replacementMesh="";
 bool footprintUnchanged=true;
 bool decorate=false;
};
inline bool GrayboxHas(const char* text,const char* part)noexcept {
 return text&&part&&std::strstr(text,part);
}
inline GrayboxArt ClassifyGrayboxArt(const char* label,double sx,double sy,double sz)noexcept {
 GrayboxArt out;
 if(!label||!label[0])return out;
 const int r=GrayboxIndexAfter(label,"Road.",RoadCityCount-1);
 const int h=GrayboxIndexAfter(label,"Road.Home.",RoadPersonCount-1);
 const int l=GrayboxIndexAfter(label,"Later.",5);
 const int echo=GrayboxIndexAfter(label,"Echo.",5);
 const int resonance=GrayboxIndexAfter(label,"Resonance.",5);
 const int finale=GrayboxIndexAfter(label,"Final.World.",8);
 const int expedition=GrayboxIndexAfter(label,"Expedition.",8);
 const int atlas=GrayboxIndexAfter(label,"Atlas.Gateway.",8);
 const int depth=GrayboxIndexAfter(label,"Depth.CivicWork.",8);
 const int artifact=GrayboxIndexAfter(label,"Depth.Evidence.",8);
 const int cache=GrayboxIndexAfter(label,"Depth.Cache.",8);
 const int cross=GrayboxIndexAfter(label,"CrossRealm.Review.",8);
 const auto malformed=GrayboxHas(label,"Road.")&&(std::strncmp(label,"Road.",5)==0)&&r<0&&h<0;
 if(malformed||((GrayboxHas(label,"Later.")&&std::strncmp(label,"Later.",6)==0)&&l<0)||
    (std::strncmp(label,"Expedition.",11)==0&&expedition<0)) {out.realm=-1;return out;}
 if(r>=0){out.realm=static_cast<int>(RoadCities[r].realm);out.paletteSlot=r==1?9:r==2?10:out.realm;}
 else if(h>=0){const auto city=RoadPeople[h].city;
  out.realm=static_cast<int>(RoadCities[city].realm);
  out.paletteSlot=city==1?9:city==2?10:out.realm;
 }
 else if(l>=0){out.realm=l+3;out.paletteSlot=out.realm;}
 else if(echo>=0){out.realm=echo+3;out.paletteSlot=out.realm;}
 else if(resonance>=0){out.realm=resonance+3;out.paletteSlot=out.realm;}
 else if(finale>=0){out.realm=finale;out.paletteSlot=out.realm;}
 else if(expedition>=0){out.realm=expedition;out.paletteSlot=out.realm;}
 else if(atlas>=0){out.realm=atlas;out.paletteSlot=out.realm;}
 else if(depth>=0){out.realm=depth;out.paletteSlot=out.realm;}
 else if(artifact>=0){out.realm=artifact;out.paletteSlot=out.realm;}
 else if(cache>=0){out.realm=cache;out.paletteSlot=out.realm;}
 else if(cross>=0){out.realm=cross;out.paletteSlot=out.realm;}
 else {
  out.realm=RealmForVisualTag(label);
  if(out.realm<0)out.realm=0;
  out.paletteSlot=out.realm;
 }
 if(GrayboxHas(label,"Bellwold.")||GrayboxHas(label,"Village.Bellwold"))out.paletteSlot=9;
 if(GrayboxHas(label,"Paperhaven.")||GrayboxHas(label,"Village.Paperhaven"))out.paletteSlot=10;
 // Decision branches and dynamic combat gates take precedence over ordinary props.
 GrayboxKit kit=GrayboxKit::Generic;
 if(GrayboxHas(label,"Final.Control")||GrayboxHas(label,"Final.Echo")||
    GrayboxHas(label,"Final.World."))kit=GrayboxKit::FinalChoice;
 else if(GrayboxHas(label,"Gateway")||GrayboxHas(label,"Portal"))kit=GrayboxKit::Portal;
 else if(GrayboxHas(label,"Gate")||GrayboxHas(label,"Barrier")||
         GrayboxHas(label,"Door.Seal"))kit=GrayboxKit::Gate;
 else if(std::strcmp(label,"Village.Bellwold")==0||
         std::strcmp(label,"Village.Paperhaven")==0||
         GrayboxHas(label,"Ground")||GrayboxHas(label,"Floor")||
         GrayboxHas(label,"ConnectedGround")||
         (sx>=10&&sy>=10&&sz<=.8))kit=GrayboxKit::Ground;
 else if(GrayboxHas(label,"Causeway")||GrayboxHas(label,"Walkway")||
         GrayboxHas(label,"Lane")||GrayboxHas(label,"Path")||
         GrayboxHas(label,"Route"))kit=GrayboxKit::Causeway;
 else if(GrayboxHas(label,"Bridge")||GrayboxHas(label,"Span"))kit=GrayboxKit::Bridge;
 else if(GrayboxHas(label,"Landmark."))kit=GrayboxKit::Landmark;
 else if(GrayboxHas(label,"Belltower"))kit=GrayboxKit::BellTower;
 else if(GrayboxHas(label,"ArchiveTower"))kit=GrayboxKit::ArchiveTower;
 else if(GrayboxHas(label,"Refuge")||GrayboxHas(label,"Shelter")||
         GrayboxHas(label,"Sanctuary"))kit=GrayboxKit::Sanctuary;
 else if(GrayboxHas(label,"Civic")||GrayboxHas(label,"WitnessBraid")||
         GrayboxHas(label,"AccordWork"))kit=GrayboxKit::Civic;
 else if(GrayboxHas(label,"Registry")||GrayboxHas(label,"ServiceCopies")||
         GrayboxHas(label,"Ledger")||GrayboxHas(label,"Relay.")||
         GrayboxHas(label,"Archive")||GrayboxHas(label,"Deed"))kit=GrayboxKit::Records;
 else if(GrayboxHas(label,"Scriptorium"))kit=GrayboxKit::Scriptorium;
 else if(GrayboxHas(label,"SharedKilnAccord"))kit=GrayboxKit::Mechanism;
 else if(GrayboxHas(label,"Forge")||GrayboxHas(label,"Kiln")||
         GrayboxHas(label,"Furnace"))kit=GrayboxKit::Forge;
 else if(GrayboxHas(label,"Hearth"))kit=GrayboxKit::Hearth;
 else if(GrayboxHas(label,"Bed"))kit=GrayboxKit::Bed;
 else if(GrayboxHas(label,"Table"))kit=GrayboxKit::Table;
 else if(GrayboxHas(label,"Bench"))kit=GrayboxKit::Bench;
 else if(GrayboxHas(label,"Watch")||GrayboxHas(label,"Guard"))kit=GrayboxKit::Watch;
 else if(GrayboxHas(label,"Village.Commons")||GrayboxHas(label,"WarmCommons"))kit=GrayboxKit::Commons;
 else if(GrayboxHas(label,"Trades")||GrayboxHas(label,"Market"))kit=GrayboxKit::Market;
 else if(GrayboxHas(label,"Cache")||GrayboxHas(label,"Cargo")||
         GrayboxHas(label,"Receiving")||GrayboxHas(label,"Parcel")||
         GrayboxHas(label,"Cart")||GrayboxHas(label,"Shelf"))kit=GrayboxKit::Cargo;
 else if(GrayboxHas(label,"Signal"))kit=GrayboxKit::Signal;
 else if(GrayboxHas(label,"Resonance.")||GrayboxHas(label,"Plinth")||
         GrayboxHas(label,"Echo.")||GrayboxHas(label,"Ritual"))kit=GrayboxKit::Ritual;
 else if(GrayboxHas(label,"Crank")||GrayboxHas(label,"Valve")||
         GrayboxHas(label,"Vent")||GrayboxHas(label,"Accord")||
         GrayboxHas(label,"Control")||GrayboxHas(label,"Mechanism")||
         GrayboxHas(label,"Wheel")||GrayboxHas(label,"Sluice")||
         GrayboxHas(label,"Tuner")||GrayboxHas(label,"Dial"))kit=GrayboxKit::Mechanism;
 else if(GrayboxHas(label,"Clue")||GrayboxHas(label,"Evidence")||
         GrayboxHas(label,"Fracture")||GrayboxHas(label,"Record")||
         GrayboxHas(label,"Board")||GrayboxHas(label,"Review"))kit=GrayboxKit::Evidence;
 else if(GrayboxHas(label,"Commons"))kit=GrayboxKit::Commons;
 else if(GrayboxHas(label,"Workshop"))kit=GrayboxKit::Workshop;
 else if(GrayboxHas(label,"Gard")||GrayboxHas(label,"Root")||
         GrayboxHas(label,"Orchard"))kit=GrayboxKit::Garden;
 else if(GrayboxHas(label,"Arena")||GrayboxHas(label,"Duel"))kit=GrayboxKit::Arena;
 else if(GrayboxHas(label,"Cover"))kit=GrayboxKit::Cover;
 else if(GrayboxHas(label,"Hazard")||GrayboxHas(label,"Warning"))kit=GrayboxKit::Hazard;
 else if(GrayboxHas(label,"Ruin")||GrayboxHas(label,"Stone")||
         GrayboxHas(label,"Pillar")||GrayboxHas(label,"Arch"))kit=GrayboxKit::Ruin;
 else if(GrayboxHas(label,"Village.Housing")||GrayboxHas(label,"Road.Home."))kit=GrayboxKit::Home;
 const auto& detail=GrayboxDetail(kit);
 out.kit=kit;out.detail=detail.detail;out.replacementMesh=detail.replacementMesh;
 out.decorate=detail.detailEnabled&&sx<15&&sy<15&&sz<12;
 return out;
}
} // namespace UnmadeCore
