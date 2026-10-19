#include "World/UnmadeCinderholdAdventureRules.h"
#include "World/UnmadeCinderholdSceneRules.h"
#include "Fracture/UnmadeFractureRules.h"
#include <cassert>
#include <limits>
using namespace UnmadeCore;
int main(){
 CinderholdAdventure a;
 assert(!a.ReturnHome(1,0,0,5));assert(!a.Choose(SlagRoute::VentOpen));
 assert(!a.Quiet(0,1));assert(!a.OpenVent(0));
 assert(a.Remember(BesHeard,1));assert(!a.Remember(BesHeard,1));
 assert(a.Remember(HomeSeen,1));assert(a.Remember(VardaBriefed,1));
 assert(a.Inspect(SlagClue::Tracks,1));assert(!a.Inspect(SlagClue::Tracks,1));
 assert(a.Inspect(SlagClue::ServiceLog,1));assert(a.OpenVent(10));
 assert(!a.Quiet(11,1));assert(a.Inspect(SlagClue::Cadence,1));
 assert(!a.Quiet(22,1)); // strict expiry; recover by turning the physical wheel.
 assert(a.Fold(24));assert(!a.Fold(25));assert(a.OpenVent(25,a.FoldActive(25)));
 assert(a.VentActive(42.99));assert(!a.VentActive(43));
 assert(a.OpenVent(44));assert(a.Quiet(45,2));assert(!a.Defeat(2));
 assert(a.Choose(SlagRoute::Maintained));assert(!a.Choose(SlagRoute::VentSealed));
 assert(a.KilnPathOpen(true));assert(!a.KilnPathOpen(false));assert(a.ArchivePathOpen());
 assert(!a.Remember(BesReported,2));assert(a.Remember(VardaReported,2));
 assert(a.Remember(OvennaTold,2));assert(a.Remember(RhevaTold,2));
 assert(a.Remember(BesReported,2));assert(a.ReturnHome(2,5,1,6));assert(!a.ReturnHome(3,0,2,8));
 auto saved=a.Save();assert(saved.returnedHeat==5&&saved.returnedOwed==6&&saved.returnDay==2);
 CinderholdAdventure restored;assert(restored.Restore(saved,4));
 assert(restored.Has(ReturnedHome));assert(!restored.FoldActive(25));assert(!restored.VentActive(45));
 assert(restored.Save().returnedOwed==6);
 // Restore fails atomically, including dependent/corrupt values and future days.
 for(int field=0;field<10;++field){auto bad=saved;
 switch(field){case 0:bad.version=2;break;case 1:bad.memories=256;break;case 2:bad.clues=8;break;
 case 3:bad.resolution=3;break;case 4:bad.route=5;break;case 5:bad.firstDay=5;break;
 case 6:bad.returnDay=5;break;case 7:bad.returnedHeat=-1;break;case 8:bad.returnedColdDays=5;break;
 case 9:bad.returnedOwed=-1;break;}
 assert(!restored.Restore(bad,4));assert(restored.Save().returnedOwed==6);}
 auto bad=saved;bad.clues=3;assert(!restored.Restore(bad,4));
 bad=saved;bad.memories&=~VardaReported;assert(!restored.Restore(bad,4));
 bad=saved;bad.memories&=~HomeSeen;assert(!restored.Restore(bad,4));
 bad=saved;bad.memories&=~ReturnedHome;assert(!restored.Restore(bad,4));
 for(auto route:{SlagRoute::VentOpen,SlagRoute::VentSealed}){
  CinderholdAdventure combat;assert(combat.Defeat(1));assert(combat.Choose(route));
  assert(combat.KilnPathOpen(false)==(route==SlagRoute::VentOpen));
  assert(combat.ArchivePathOpen()==(route==SlagRoute::VentSealed));
  assert(combat.Remember(VardaBriefed,2));assert(combat.Remember(VardaReported,2));
  assert(combat.Remember(BesHeard,2));assert(combat.Remember(BesReported,2));
  assert(combat.Remember(HomeSeen,2));assert(combat.ReturnHome(2,0,1,12));
 }
 CinderholdAdventure blank;assert(blank.Restore({},1));
 assert(!blank.Inspect(static_cast<SlagClue>(3),1));assert(!blank.Remember(ReturnedHome,1));
 assert(!blank.Glimpse(std::numeric_limits<double>::quiet_NaN()));assert(blank.Glimpse(0));
 assert(!blank.Glimpse(1));assert(blank.Glimpse(3));
 SlagElderPattern elder;
 auto step=elder.Step(0,false,true,true,false);assert(step.warning==SlagAttack::GroundSlam&&!step.strike);
 assert(!elder.Step(1.59,false,true,true,false).strike);
 assert(elder.Step(1.6,false,true,true,false).strike);
 assert(!elder.Step(1.6,false,true,true,false).strike); // exactly one hit window
 assert(elder.Step(2,false,true,true,false).recovering);
 assert(!elder.Step(4.0,false,true,true,false).strike);
 assert(elder.Step(4.1,true,true,true,false).warning==SlagAttack::CinderSweep);
 assert(elder.Step(5,true,true,true,true).recovering); // vent interrupts; no pending damage.
 assert(!elder.Step(6,true,true,true,false).strike);
 assert(elder.Step(6.5,true,true,true,false).warning==SlagAttack::GroundSlam);
 elder.Interrupt(7);assert(elder.Step(8,true,true,true,false).recovering);
 assert(elder.Step(8.5,true,true,true,false).warning==SlagAttack::CinderSweep);
 assert(!elder.Step(9,true,false,true,false).strike); // leaving nest cancels.
 assert(!elder.Step(8,true,true,true,false).strike); // backwards time rejected.
 for(int scene=0;scene<4;++scene)for(int beat=0;beat<3;++beat){
  const auto* b=CinderholdBeatAt(scene,beat);assert(b&&b->duration>=3&&b->subtitle[0]&&b->speakerId[0]);
 }
 assert(!CinderholdBeatAt(4,0));assert(!CinderholdBeatAt(0,3));
 // Runtime rollback uses these same value copies for quest and recoverable strain.
 CinderholdAdventure transaction;assert(transaction.Defeat(1));auto previous=transaction;
 FractureModel strain("region.prototype.hub",{"variant.open","variant.sealed"});auto strainBefore=strain.TakeSnapshot();assert(strain.SpendStrain(60));
 assert(transaction.Choose(SlagRoute::VentOpen));transaction=previous;assert(strain.Restore(strainBefore));
 assert(transaction.Save().route==0&&strain.CurrentStrain()==0);
}
