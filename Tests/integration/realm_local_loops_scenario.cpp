#include "World/UnmadeRealmJourneyRules.h"
#include "Items/UnmadeUnifiedLoadoutRules.h"
#include "World/UnmadeIdentityRules.h"
#include "Items/UnmadeFieldCraftRules.h"
#include "Save/UnmadeRecoveryRules.h"
#include "Combat/UnmadeCombatRules.h"
#include "../save/combined_record_store.h"
#include <cassert>
#include <limits>
#include <iostream>
using namespace UnmadeCore;
// Runs existing authored local loops against the combined in-memory save boundary.
// Actor reachability, NPC movement, presentation and platform serialization need UE.
struct LocalPlayer {
 Combatant body;double stamina=19;
 double GetHealth()const{return body.Health();}
 double GetStamina()const{return stamina;}
 bool Heal(double n){return body.Heal(n);}
 bool RestoreSession(double health,double energy){stamina=energy;return body.RestoreHealth(health);}
 auto CaptureRecoveryState()const{return std::make_pair(body,stamina);}
 void RestoreRecoveryState(const std::pair<Combatant,double>& before){body=before.first;stamina=before.second;}
};
int main(){
 ExpeditionConfirmation confirmation;
 assert(!confirmation.Confirm(0,1,1,1));assert(!confirmation.Confirm(0,1,2,2));assert(confirmation.Confirm(0,1,2,3));assert(!confirmation.Confirm(0,1,2,4));assert(!confirmation.Confirm(0,1,2,25));assert(confirmation.Confirm(0,1,2,26));assert(!confirmation.Confirm(0,2,1,30));assert(!confirmation.Confirm(0,2,1,29));assert(!confirmation.Confirm(9,2,1,30));
 int paths=0,reloads=0,faults=0,services=0;
 for(int a=0;a<9;++a)for(int branch=1;branch<=2;++branch)for(int accord=1;accord<=2;++accord)for(int late=0;late<2;++late){
  if(branch==1&&late)continue;
  Expeditions e;AdventureDepth d;PlayerExperience x;WorldIdentity identity;
  UnifiedLoadout loadout;LocalPlayer player;InventoryModel bag;RegionalEconomy economy;
  assert(bag.Claim(Achievement::Starter)==RewardResult::Awarded);int day=1;SaveFixture::Storage disk;
  auto capture=[&](){SaveFixture::Record record;
   record.bHasLivingWorldSnapshot=true;record.LivingWorldSeconds=(day-1)*1200.;
   record.bHasPlayerResume=true;record.ResumeHealth=player.GetHealth();record.ResumeStamina=player.GetStamina();
   record.bHasExpeditions=true;record.ExpeditionState=e.Pack();
   record.bHasWorldIdentity=true;record.WorldIdentityState=identity.Pack();
   record.bHasAdventureDepth=true;record.AdventureDepthState=d.Pack();
   record.bHasPlayerExperience=true;record.PlayerExperienceState=x.Pack();
   record.bHasUnifiedLoadout=true;record.UnifiedRealmLoadout={loadout.weapon,loadout.armor,loadout.technique};
   const auto items=bag.Snapshot();record.bHasInventorySnapshot=true;
   record.ItemQuantities.assign(items.quantities.begin(),items.quantities.end());
   record.EquippedItems.assign(items.equipped.begin(),items.equipped.end());record.AwardedMilestoneBits=items.milestones;
   const auto money=economy.Snapshot();record.bHasEconomySnapshot=true;record.TradeMarks=money.marks;
   record.ProfessionSkills.assign(money.craftsmanship.begin(),money.craftsmanship.end());record.PaidContractMask=money.completedContracts;
   assert(SaveFixture::Valid(record));return record;
  };
  disk.primary=capture();disk.backup=disk.primary;
  auto check=[&](){
   const auto next=capture();const auto before=*disk.Load();
   for(int failed=1;failed<=2;++failed){auto trial=disk;trial.writes=0;trial.failAt=failed;trial.damageOnFailure=true;
    assert(!trial.Commit(next));const auto* recovered=trial.Load();assert(recovered);
    assert(recovered->ExpeditionState==before.ExpeditionState&&recovered->WorldIdentityState==before.WorldIdentityState);
    assert(recovered->AdventureDepthState==before.AdventureDepthState&&recovered->PlayerExperienceState==before.PlayerExperienceState);
    assert(recovered->UnifiedRealmLoadout==before.UnifiedRealmLoadout&&recovered->ResumeHealth==before.ResumeHealth);
    assert(recovered->ItemQuantities==before.ItemQuantities&&recovered->ProfessionSkills==before.ProfessionSkills&&recovered->TradeMarks==before.TradeMarks);++faults;
   }
   assert(disk.Commit(next));const auto* chosen=disk.Load();assert(chosen);
   Expeditions re;AdventureDepth rd;PlayerExperience rx;WorldIdentity ri;
   assert(re.Restore(chosen->ExpeditionState));assert(rd.Restore(chosen->AdventureDepthState,re,day));
   assert(rx.Restore(chosen->PlayerExperienceState,re,rd,day));assert(ri.Restore(chosen->WorldIdentityState,re,day));
   assert(re.Pack()==e.Pack()&&rd.Pack()==d.Pack()&&rx.Pack()==x.Pack()&&ri.Pack()==identity.Pack());
   InventorySnapshot stock;for(int item=0;item<chosen->ItemQuantities.Num();++item)stock.quantities[item]=chosen->ItemQuantities[item];
   for(int slot=0;slot<3;++slot)stock.equipped[slot]=chosen->EquippedItems[slot];
   stock.milestones=chosen->AwardedMilestoneBits;InventoryModel rb;assert(rb.Restore(stock));
   EconomySnapshot funds;funds.marks=chosen->TradeMarks;funds.completedContracts=chosen->PaidContractMask;
   for(int skill=0;skill<3;++skill)funds.craftsmanship[skill]=chosen->ProfessionSkills[skill];
   RegionalEconomy rm;assert(rm.Restore(funds));
   assert(rb.Snapshot().quantities==bag.Snapshot().quantities&&rm.Marks()==economy.Marks());
   UnifiedLoadout rl{chosen->UnifiedRealmLoadout[0],chosen->UnifiedRealmLoadout[1],chosen->UnifiedRealmLoadout[2]};assert(rl.Valid(re));
   e=re;d=rd;x=rx;identity=ri;bag=rb;economy=rm;loadout=rl;
   assert(player.RestoreSession(chosen->ResumeHealth,chosen->ResumeStamina));++reloads;
  };
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
  auto migrated=UnifiedLoadout::Migrate(e);assert(migrated.Valid(e));
  const auto previousLoadout=loadout;
  for(int fail=1;fail<=2;++fail){disk.writes=0;disk.failAt=fail;disk.damageOnFailure=true;
   assert(!CommitRealmSelection(loadout,e,a,[&]{return disk.Commit(capture());}));
   assert(loadout.weapon==previousLoadout.weapon&&loadout.armor==previousLoadout.armor&&loadout.technique==previousLoadout.technique);
   assert(e.Arc(a)->reward==accord);const auto* recovered=disk.Load();assert(recovered&&recovered->ExpeditionState==e.Pack());++faults;
  }
  disk.failAt=0;assert(CommitRealmSelection(loadout,e,a,[&]{return disk.Commit(capture());}));check();assert(loadout.Valid(e));
  assert(goal().step==JourneyStep::PersonalOffer);assert(d.Offer(a,true));check();assert(goal().step==JourneyStep::Witness);assert(d.Witness(a));check();assert(goal().step==JourneyStep::PersonalEvidence);assert(d.Evidence(a,true));check();assert(goal().step==JourneyStep::Accord);assert(d.Choose(a,accord,true));check();
  assert(goal().step==JourneyStep::WingOffer);assert(x.OfferWing(a,e));assert(x.Visit(a,4,day));assert(x.Scene(a,0,e,d));check();
  for(int r=0;r<3;++r){assert(goal().step==JourneyStep::WingFieldwork&&goal().index==r);assert(x.WingStep(a,r,true));check();}
  assert(goal().step==JourneyStep::WingGuardian);if(a!=3)assert(!x.Guardian(a,true));assert(x.Guardian(a,a==3&&branch==2));check();assert(goal().step==JourneyStep::WingReward);assert(x.CompleteWing(a));check();assert(goal().step==JourneyStep::Home);
  assert(x.Visit(a,0,day));assert(x.Visit(a,3,day));assert(!x.Visit(a,0,day));assert(!x.Visit(a,3,day));++day;assert(x.Visit(a,0,day));assert(x.Visit(a,3,day));assert(x.Scene(a,1,e,d));assert(x.Scene(a,2,e,d));assert(x.Visit(a,1,day));assert(x.Visit(a,2,day));assert(x.Scene(a,3,e,d));check();assert(x.Arc(a)->insight==9);
  assert(x.Upgrade(a,1));assert(x.Upgrade(a,4));assert(x.Upgrade(a,5));assert(x.Upgrade(a,6));assert(x.Repair(a,d));if(accord==2)assert(x.Reconcile(a,d));assert(x.Specialty(a,day));assert(!x.Specialty(a,day));++day;assert(x.Specialty(a,day));++day;assert(!x.Specialty(a,day));check();
  auto old=x.Pack(),bad=old;if(a!=3){bad[6+a*25+2]=2;assert(!x.Restore(bad,e,d,day));bad=old;}bad[6+a*25+8]++;assert(!x.Restore(bad,e,d,day));assert(x.Pack()==old);assert(!e.Boss(a,true));assert(!e.Return(a));assert(!x.CompleteWing(a));check();
  // While here: survey the actual mechanism, repair roads, and use finite
  // personal services. Progress comes from these original production models.
  int switches=-1;
  for(int mask=0;mask<8;++mask){int lights=0;for(int lever=0;lever<3;++lever)if(mask&(1<<lever))lights^=WorldIdentity::Coupling(a,lever);if(lights==WorldIdentity::Target(a)){switches=mask;break;}}
  assert(switches>=0);
  for(int lever=0;lever<3;++lever)if(switches&(1<<lever)){assert(identity.Operate(a,lever));check();}
  assert(identity.Chart(a,e.Arc(a)->clues));check();assert(identity.Arc(a)->chart==1);
  assert(!identity.Chart(a,7));
  for(int post=0;post<3;++post){assert(identity.Repair(a,post,e.Arc(a)->returned));check();}
  assert(identity.Arc(a)->repair==7);
  assert(d.Route(a,true));check();assert(d.Buy(a,1,day,true));check();
  assert(d.Secret(a,true));check();assert(d.Arc(a)->kits==1); // Caches do not consume field kits.
  for(int mastery=0;mastery<3;++mastery){assert(d.Buy(a,2,day,true));check();}
  assert(d.Arc(a)->training==3);assert(!d.Buy(a,2,day,true));
  // Realm-local recovery uses the exact helper wired to the hunt/rest adapter.
  auto recover=[&](int service){
   if(player.GetHealth()>=100)return false;
   const auto before=identity;const auto beforeDepth=d;
   int kills=0;for(int k=0;k<8;++k)kills+=(e.Arc(a)->kills>>k)&1;
   const bool changed=service==0?identity.Hunt(a,kills):service==1?identity.Rest(a,day,e.Arc(a)->returned):d.Buy(a,0,day,true);
   if(!changed)return false;
   if(!CommitHealing(player,35,[&]{return disk.Commit(capture());})){identity=before;d=beforeDepth;return false;}
   return true;
  };
  // Full health spends neither the one-time ration nor a daily rest receipt.
  assert(player.body.RestoreHealth(100));check();disk.writes=0;
  assert(!recover(0)&&!recover(1)&&!recover(2));assert(disk.writes==0&&identity.Arc(a)->hunt==0&&identity.Arc(a)->restDay==0);
  for(int service=0;service<3;++service){
   const double beforeHealth=service==1?95:37;
   assert(player.body.RestoreHealth(beforeHealth));check();
   const auto before=identity.Pack(),beforeDepth=d.Pack();
   for(int failure=1;failure<=2;++failure){disk.writes=0;disk.failAt=failure;disk.damageOnFailure=true;
    assert(!recover(service));assert(player.GetHealth()==beforeHealth&&player.GetStamina()==19);
    assert(identity.Pack()==before&&d.Pack()==beforeDepth);const auto* recovered=disk.Load();assert(recovered);
    assert(recovered->ResumeHealth==beforeHealth&&recovered->WorldIdentityState==before&&recovered->AdventureDepthState==beforeDepth);++faults;
   }
   disk.failAt=0;assert(recover(service));++services;
   const auto* saved=disk.Load();assert(saved->ResumeHealth==(service==1?100:72));
   assert(saved->WorldIdentityState==identity.Pack());
   check();assert(player.GetHealth()==(service==1?100:72));
   assert(player.body.RestoreHealth(37));check();disk.writes=0;
   assert(!recover(service));assert(disk.writes==0);
  }
  ++day;check();assert(recover(1));++services;check();
  assert(player.GetHealth()==72&&identity.Arc(a)->restDay==day);
  // Bring one recipe's ingredients in the fixture; no in-game free stock is added.
  const auto& recipe=FieldRecipes[a];assert(bag.Add(recipe.a,recipe.na));assert(bag.Add(recipe.b,recipe.nb));check();
  const auto beforeItems=bag.Snapshot();const auto beforeMoney=economy.Snapshot();
  for(int failure=1;failure<=2;++failure){
   assert(CraftField(bag,a,x.Arc(a)->complete)==EconomyResult::Completed);economy.Practice(recipe.profession);
   disk.writes=0;disk.failAt=failure;disk.damageOnFailure=true;
   assert(!disk.Commit(capture()));assert(bag.Restore(beforeItems));assert(economy.Restore(beforeMoney));
   const auto* recovered=disk.Load();assert(recovered&&recovered->ItemQuantities==capture().ItemQuantities);++faults;
  }
  disk.failAt=0;const int output=bag.Quantity(recipe.output);
  assert(CraftField(bag,a,x.Arc(a)->complete)==EconomyResult::Completed);economy.Practice(recipe.profession);
  assert(disk.Commit(capture()));check();assert(bag.Quantity(recipe.output)==output+recipe.count);
  assert(CraftField(bag,a,true)==EconomyResult::NotEnoughMaterials);
  assert(bag.Quantity(recipe.output)==output+recipe.count&&economy.Marks()==beforeMoney.marks);
  for(int h=0;h<24;++h)assert(ServiceOpen(a,h)==(a==2||(a==6?(h>=18||h<6):h>=6&&h<22)));
  ++paths;
 }
 Expeditions campaign;UnifiedLoadout shared;
 for(int a=0;a<9;++a){assert(campaign.Brief(a));assert(campaign.Visit(a,0));for(int r=0;r<5;++r){assert(campaign.Enter(a,r));if(r>0&&r<4)assert(campaign.Inspect(a,r-1));}assert(campaign.Choose(a,2));assert(campaign.Enter(a,5));assert(campaign.Boss(a,false));assert(campaign.Rescue(a,0));assert(campaign.Rescue(a,1));assert(campaign.Return(a));assert(campaign.Claim(a,a%2+1));if(a%2)shared.armor=a;else shared.weapon=a;shared.technique=a;assert(shared.Valid(campaign));Expeditions loaded;assert(loaded.Restore(campaign.Pack()));for(int n=0;n<=a;++n)assert(loaded.Arc(n)->reward==n%2+1);assert(campaign.Equip(0));assert(shared.Valid(campaign));}
 assert(shared.weapon==8&&shared.armor==7&&shared.technique==8);
 assert(!InsideRegisteredChamber(9,0,0,0,0));assert(!InsideRegisteredChamber(0,0,std::numeric_limits<double>::quiet_NaN(),0,0));
 ExpeditionState forged;forged.rooms=63;forged.branch=1;assert(!ExpeditionEncounterActive(forged,8));assert(!ExpeditionEncounterActive(forged,9));
 std::cout<<"PASS: "<<paths<<" realm-local branch/accord loops; "<<reloads<<" combined reload checkpoints; "<<faults<<" failed-write recoveries; "<<services<<" persisted hunt/sanctuary/provision uses\n";
}
