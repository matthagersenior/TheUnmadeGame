#include "World/UnmadeRealmJourneyRules.h"
#include "Items/UnmadeUnifiedLoadoutRules.h"
#include <cassert>
#include <limits>
#include <iostream>
using namespace UnmadeCore;
int main(){
 ExpeditionConfirmation confirmation;
 assert(!confirmation.Confirm(0,1,1,1));assert(!confirmation.Confirm(0,1,2,2));assert(confirmation.Confirm(0,1,2,3));assert(!confirmation.Confirm(0,1,2,4));assert(!confirmation.Confirm(0,1,2,25));assert(confirmation.Confirm(0,1,2,26));assert(!confirmation.Confirm(0,2,1,30));assert(!confirmation.Confirm(0,2,1,29));assert(!confirmation.Confirm(9,2,1,30));
 int paths=0,reloads=0;
 for(int a=0;a<9;++a)for(int branch=1;branch<=2;++branch)for(int accord=1;accord<=2;++accord)for(int late=0;late<2;++late){
  if(branch==1&&late)continue;
  Expeditions e;AdventureDepth d;PlayerExperience x;int day=1;
  auto check=[&](){Expeditions re;AdventureDepth rd;PlayerExperience rx;assert(re.Restore(e.Pack()));assert(rd.Restore(d.Pack(),re,day));assert(rx.Restore(x.Pack(),re,rd,day));assert(re.Pack()==e.Pack()&&rd.Pack()==d.Pack()&&rx.Pack()==x.Pack());++reloads;};
  auto goal=[&](){return NextRealmObjective(*e.Arc(a),*d.Arc(a),*x.Arc(a));};
  assert(goal().step==JourneyStep::Board);assert(!ExpeditionEncounterActive(*e.Arc(a),0));assert(!e.Kill(a,0));
  assert(e.Brief(a));check();assert(goal().step==JourneyStep::Frontier);assert(e.Visit(a,0));check();assert(e.Visit(a,1));
  assert(goal().step==JourneyStep::Chamber);
  for(int r=0;r<5;++r){assert(goal().index==r);auto at=DungeonRoom(a,r);assert(InsideRegisteredChamber(a,r,at.x,at.y,0));assert(!InsideRegisteredChamber(a,r,at.x+1200,at.y,0));assert(e.Enter(a,r));check();
   if(r<4){assert(ExpeditionEncounterActive(*e.Arc(a),r));assert(e.Kill(a,r));assert(!ExpeditionEncounterActive(*e.Arc(a),r));check();}
   if(r>0&&r<4){assert(e.Inspect(a,r-1));check();}
  }
  if(!late)for(int k=4;k<8;++k){assert(ExpeditionEncounterActive(*e.Arc(a),k));assert(e.Kill(a,k));check();}
  if(!late){assert(e.Rescue(a,0));assert(e.Rescue(a,1));}check();assert(goal().step==JourneyStep::Resolution);
  assert(!ExpeditionEncounterActive(*e.Arc(a),8));assert(e.Choose(a,branch));check();assert(goal().step==JourneyStep::Chamber&&goal().index==5);
  assert(!ExpeditionEncounterActive(*e.Arc(a),8));assert(e.Enter(a,5));assert(ExpeditionEncounterActive(*e.Arc(a),8));check();assert(goal().step==JourneyStep::Boss);
  assert(e.Boss(a,branch==1));check();for(int k=0;k<9;++k)assert(!ExpeditionEncounterActive(*e.Arc(a),k));
  if(late){assert(e.Rescue(a,0));assert(e.Rescue(a,1));check();}
  assert(goal().step==JourneyStep::Return);assert(e.Return(a));check();assert(goal().step==JourneyStep::Reward);assert(e.Claim(a,accord));assert(!e.Claim(a,3-accord));check();
  auto loadout=UnifiedLoadout::Migrate(e);assert(loadout.Valid(e));
  assert(goal().step==JourneyStep::PersonalOffer);assert(d.Offer(a,true));check();assert(goal().step==JourneyStep::Witness);assert(d.Witness(a));check();assert(goal().step==JourneyStep::PersonalEvidence);assert(d.Evidence(a,true));check();assert(goal().step==JourneyStep::Accord);assert(d.Choose(a,accord,true));check();
  assert(goal().step==JourneyStep::WingOffer);assert(x.OfferWing(a,e));assert(x.Visit(a,4,day));assert(x.Scene(a,0,e,d));check();
  for(int r=0;r<3;++r){assert(goal().step==JourneyStep::WingFieldwork&&goal().index==r);assert(x.WingStep(a,r,true));check();}
  assert(goal().step==JourneyStep::WingGuardian);if(a!=3)assert(!x.Guardian(a,true));assert(x.Guardian(a,a==3&&branch==2));check();assert(goal().step==JourneyStep::WingReward);assert(x.CompleteWing(a));check();assert(goal().step==JourneyStep::Home);
  assert(x.Visit(a,0,day));assert(x.Visit(a,3,day));++day;assert(x.Visit(a,0,day));assert(x.Visit(a,3,day));assert(x.Scene(a,1,e,d));assert(x.Scene(a,2,e,d));assert(x.Visit(a,1,day));assert(x.Visit(a,2,day));assert(x.Scene(a,3,e,d));check();assert(x.Arc(a)->insight==9);
  assert(x.Upgrade(a,1));assert(x.Upgrade(a,4));assert(x.Upgrade(a,5));assert(x.Upgrade(a,6));assert(x.Repair(a,d));if(accord==2)assert(x.Reconcile(a,d));assert(x.Specialty(a,day));assert(!x.Specialty(a,day));++day;assert(x.Specialty(a,day));++day;assert(!x.Specialty(a,day));check();
  auto old=x.Pack(),bad=old;if(a!=3){bad[6+a*25+2]=2;assert(!x.Restore(bad,e,d,day));bad=old;}bad[6+a*25+8]++;assert(!x.Restore(bad,e,d,day));assert(x.Pack()==old);assert(!e.Boss(a,true));assert(!e.Return(a));assert(!x.CompleteWing(a));check();++paths;
 }
 Expeditions campaign;UnifiedLoadout shared;
 for(int a=0;a<9;++a){assert(campaign.Brief(a));assert(campaign.Visit(a,0));for(int r=0;r<5;++r){assert(campaign.Enter(a,r));if(r>0&&r<4)assert(campaign.Inspect(a,r-1));}assert(campaign.Choose(a,2));assert(campaign.Enter(a,5));assert(campaign.Boss(a,false));assert(campaign.Rescue(a,0));assert(campaign.Rescue(a,1));assert(campaign.Return(a));assert(campaign.Claim(a,a%2+1));if(a%2)shared.armor=a;else shared.weapon=a;shared.technique=a;assert(shared.Valid(campaign));Expeditions loaded;assert(loaded.Restore(campaign.Pack()));for(int n=0;n<=a;++n)assert(loaded.Arc(n)->reward==n%2+1);assert(campaign.Equip(0));assert(shared.Valid(campaign));}
 assert(shared.weapon==8&&shared.armor==7&&shared.technique==8);
 assert(!InsideRegisteredChamber(9,0,0,0,0));assert(!InsideRegisteredChamber(0,0,std::numeric_limits<double>::quiet_NaN(),0,0));
 ExpeditionState forged;forged.rooms=63;forged.branch=1;assert(!ExpeditionEncounterActive(forged,8));assert(!ExpeditionEncounterActive(forged,9));
 std::cout<<"PASS: "<<paths<<" nine-realm branch/accord journeys; "<<reloads<<" composed reload checkpoints; finite rewards, encounters and corrupt-state protection\n";
}
