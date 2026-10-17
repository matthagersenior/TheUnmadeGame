#include "rite_cast_fixture.h"
#include "Save/UnmadeRealityTravelRules.h"
#include "Save/UnmadeDefeatSaveRetryRules.h"
#include "Combat/UnmadeCombatRules.h"
#include <iostream>
#include <limits>
using namespace UnmadeCore;
using SaveFixture::Record;
struct State {
 RiteFixture::State world;LexiconModel language;FrontierJourney frontier;SaveFixture::Position at;
 Combatant health{100,24,.75};SaveFixture::Array<SaveFixture::Npc> memories;
 Record Capture()const{
  auto r=world.Capture();r.LexiconEvidence.assign(language.Snapshot().begin(),language.Snapshot().end());r.NpcSnapshots=memories;
  r.bHasPlayerResume=true;r.ResumePosition=at;r.ResumeHealth=health.Health();r.ResumeStamina=80;
  auto f=frontier.Snapshot();r.bHasFrontierSnapshot=true;r.VisitedFrontierRealms=f.visits;r.DiscoveredFrontierClues=f.discoveries;r.FrontierStages.assign(f.stages.begin(),f.stages.end());r.FrontierEndings.assign(f.endings.begin(),f.endings.end());assert(SaveFixture::Valid(r));return r;
 }
 void Load(const Record& r){world.Load(r);assert(language.Restore({r.LexiconEvidence.begin(),r.LexiconEvidence.end()}));at=r.ResumePosition;assert(health.RestoreHealth(r.ResumeHealth));memories=r.NpcSnapshots;FrontierSnapshot f;f.visits=r.VisitedFrontierRealms;f.discoveries=r.DiscoveredFrontierClues;for(int i=0;i<2;++i){f.stages[i]=r.FrontierStages[i];f.endings[i]=r.FrontierEndings[i];}assert(frontier.Restore(f));}
};
static void Same(const Record& a,const Record& b){RiteFixture::Same(a,b);assert(a.LexiconEvidence==b.LexiconEvidence&&a.VerifiedRoadBits==b.VerifiedRoadBits&&a.VisitedFrontierRealms==b.VisitedFrontierRealms);assert(a.ResumePosition.X==b.ResumePosition.X&&a.ResumePosition.Y==b.ResumePosition.Y&&a.ResumeHealth==b.ResumeHealth);assert(a.NpcSnapshots.size()==b.NpcSnapshots.size());}
static CheckpointPoint Origin(Realm realm){if(const auto* l=FindLaterRealm(realm))return {l->centerX,l->centerY,135};if(const auto* f=FindFrontier(realm))return {f->centerX,f->centerY,135};return {};}
int main(){int casts=0,crossings=0,reloads=0,revivals=0;
 for(int ability=0;ability<4;++ability)for(int modifier=0;modifier<=5;++modifier)for(int clue=0;clue<2;++clue)for(int fault=1;fault<=2;++fault){
  State live;assert(live.world.fracture.Glimpse(true,100)==Result::Applied);if(clue)assert(live.language.RecordEvidence("term.veyl","evidence.archivist"));const auto initial=live.Capture();SaveFixture::Storage disk;assert(disk.Commit(initial));assert(disk.Commit(initial));disk.failAt=disk.writes+fault;disk.damageOnFailure=true;
  auto cast=[&]{return CommitFractureAction(live.world.fracture,[&]{if(ability==0)return live.world.fracture.Glimpse(true,101,modifier);if(ability==1)return live.world.fracture.Fold(true,101,modifier);return live.world.fracture.Rewrite("region.prototype.hub",ability==2?"variant.open":"variant.sealed",true);},[&]{auto write=[&]{return CommitActionConsequences([&]{return live.memories;},[&]{SaveFixture::Npc npc;npc.NpcId="npc.witness";npc.Observations.push_back({{77},"Reality.Anomaly","region.prototype.hub","",0});live.memories.push_back(npc);},[&]{return disk.Commit(live.Capture());},[&](const auto& before){live.memories=before;});};return ability==0?CommitLexiconEvidence(live.language,"evidence.glimpse",write):write();});};
  assert(cast()==FractureCommit::SaveFailed);Same(initial,live.Capture());assert(live.world.fracture.IsGlimpsing(101));assert(!live.world.fracture.IsFolded(101));assert(disk.Load());Same(initial,*disk.Load());
  assert(cast()==FractureCommit::Applied);const auto saved=live.Capture();if(ability==0)assert((live.language.Interpretation("term.veyl")==InterpretationState::Understood)==bool(clue));if(ability==1)assert(live.world.fracture.IsFolded(106+modifier));
  const int writes=disk.writes;for(int i=0;i<8;++i){State restored;restored.Load(*disk.Load());Same(saved,restored.Capture());assert(!restored.world.fracture.IsFolded(101)&&!restored.world.fracture.IsGlimpsing(101));++reloads;}assert(disk.writes==writes);++casts;
 }
 // Full model rollback preserves both temporary windows, including their exact
 // previous endpoints; invalid casts never invoke the writer.
 {FractureModel m{"region.prototype.hub",{"variant.open","variant.sealed"}};assert(m.Fold(true,10,5)==Result::Applied);assert(m.Glimpse(true,10,6)==Result::Applied);const auto before=m.TakeSnapshot();assert(CommitFractureAction(m,[&]{return m.Rewrite("region.prototype.hub","variant.open",true);},[]{return false;})==FractureCommit::SaveFailed);assert(m.TakeSnapshot().strain==before.strain&&m.IsFolded(20.9)&&!m.IsFolded(21)&&m.IsGlimpsing(12.9)&&!m.IsGlimpsing(13));assert(CommitFractureAction(m,[&]{return m.Fold(true,11);},[]{assert(false);return true;})==FractureCommit::Unavailable);assert(m.IsFolded(20.9));}
 // Both directions of every authored passage, plus repeated already-known visits.
 for(const auto& edge:AtlasPassages)for(int side=0;side<2;++side)for(int repeatVisit=0;repeatVisit<2;++repeatVisit)for(int fault=1;fault<=2;++fault){
  const auto from=side?edge.b:edge.a,to=side?edge.a:edge.b;const auto p=Origin(from),q=Origin(to);State live;RiteFixture::MasterAll(live.world);live.at={p.X,p.Y,p.Z};if(repeatVisit){live.world.chapters.first.Visit(to);live.frontier.Visit(to);}SaveFixture::Storage disk;const auto before=live.Capture();assert(disk.Commit(before));assert(disk.Commit(before));disk.failAt=disk.writes+fault;disk.damageOnFailure=true;
  auto travel=[&](bool failMove,bool failRollback){const auto later=live.world.chapters.first;const auto frontier=live.frontier;live.world.chapters.first.Visit(to);live.frontier.Visit(to);int moves=0,writes=0;const auto route=to==Realm::WidowedRain?0:to==Realm::HearthBeneath?1:-1;auto result=CommitCheckpointRelocation(live.at,SaveFixture::Position{q.X,q.Y-1170,q.Z},[&](auto at){++moves;if((moves==1&&failMove)||(moves==2&&failRollback))return false;live.at=at;return true;},[&]{++writes;return CommitExploredRoute(live.world.rites,route,[&]{return disk.Commit(live.Capture());});});if(result!=RelocationCommit::Applied){live.world.chapters.first=later;live.frontier=frontier;}if(failMove)assert(writes==0);return result;};
  assert(travel(true,false)==RelocationCommit::MoveFailed);Same(before,live.Capture());
  assert(travel(false,false)==RelocationCommit::SaveFailed);Same(before,live.Capture());assert(disk.Load());Same(before,*disk.Load());
  assert(travel(false,false)==RelocationCommit::Applied);const auto saved=live.Capture();assert(saved.ResumePosition.X==q.X&&saved.ResumePosition.Y==q.Y-1170);for(int i=0;i<8;++i){State loaded;loaded.Load(*disk.Load());Same(saved,loaded.Capture());++reloads;}++crossings;
 }
 // Failed rollback is explicit, and a move that fails cannot write a new record.
 {CheckpointPoint p{0,0,135};int moves=0,writes=0;assert(CommitCheckpointRelocation(p,CheckpointPoint{1,2,135},[&](auto at){++moves;if(moves==2)return false;p=at;return true;},[&]{++writes;return false;})==RelocationCommit::RollbackFailed);assert(writes==1&&moves==2);}
 {const CheckpointPoint p;const auto bad=CheckpointPoint{std::numeric_limits<double>::infinity(),0,135};assert(CommitCheckpointRelocation(p,bad,[](auto){assert(false);return true;},[]{assert(false);return true;})==RelocationCommit::MoveFailed);}
 // A known city elsewhere must not redirect a new realm's fallback checkpoint.
 for(int i=0;i<9;++i){const auto p=Origin(static_cast<Realm>(i));const auto entry=SafeRealmEntry(p.X,p.Y);assert(CheckpointRealm(entry.X,entry.Y)==static_cast<Realm>(i));for(int j=0;j<9;++j){const auto other=Origin(static_cast<Realm>(j));assert(SameCheckpointRealm(p.X,p.Y,other.X,other.Y)==(i==j));}}
 for(int realm=0;realm<9;++realm)for(int fault=1;fault<=2;++fault){
  State live;const auto p=Origin(static_cast<Realm>(realm));live.at={p.X+100,p.Y+300,135};assert(live.health.RestoreHealth(0));SaveFixture::Storage disk;const auto before=live.Capture();assert(disk.Commit(before));assert(disk.Commit(before));disk.failAt=disk.writes+fault;disk.damageOnFailure=true;DefeatSaveRetry retry;
  auto recover=[&]{const auto hp=live.health.Health();const auto safe=SafeRealmEntry(live.at.X,live.at.Y);const auto result=CommitCheckpointRelocation(live.at,SaveFixture::Position{safe.X,safe.Y,safe.Z},[&](auto at){live.at=at;return true;},[&]{assert(live.health.ReviveAtCheckpoint(.75));return disk.Commit(live.Capture());});if(result!=RelocationCommit::Applied)assert(live.health.RestoreHealth(hp));return result==RelocationCommit::Applied;};
  assert(retry.Attempt(0,4,recover)==DefeatSaveResult::FirstFailure);Same(before,live.Capture());assert(disk.Load());for(int i=41;i<90;++i)assert(retry.Attempt(0,i/10.,recover)==DefeatSaveResult::Waiting);assert(retry.Attempt(0,9,recover)==DefeatSaveResult::Saved);assert(live.health.Health()==75);const auto saved=live.Capture();for(int i=0;i<8;++i){State loaded;loaded.Load(*disk.Load());Same(saved,loaded.Capture());assert(loaded.health.Health()==75);++reloads;}++revivals;
 }
 // Full inventory/pending item grants cannot prevent the independent revival
 // health/position commit. Inventory and unclaimed receipt bits remain intact.
 {State dead;assert(dead.health.RestoreHealth(0));InventoryModel full;auto inv=full.Snapshot();for(const auto& item:ItemCatalog)inv.quantities[static_cast<int>(item.id)]=item.maxStack;assert(full.Restore(inv));
  const auto capture=[&]{auto r=dead.Capture();r.bHasInventorySnapshot=true;r.ItemQuantities.assign(inv.quantities.begin(),inv.quantities.end());r.EquippedItems.assign(inv.equipped.begin(),inv.equipped.end());r.AwardedMilestoneBits=inv.milestones;return r;};
  SaveFixture::Storage disk;assert(disk.Commit(capture()));assert(CommitCheckpointRelocation(dead.at,SaveFixture::Position{0,0,135},[&](auto at){dead.at=at;return true;},[&]{assert(dead.health.ReviveAtCheckpoint(.75));return disk.Commit(capture());})==RelocationCommit::Applied);assert(disk.Load()->ResumeHealth==75&&disk.Load()->ItemQuantities==capture().ItemQuantities&&static_cast<std::uint64_t>(disk.Load()->AwardedMilestoneBits)==inv.milestones);
 }
 // Malformed language/captured history cannot replace good storage; no free clue.
 {State live;SaveFixture::Storage disk;assert(disk.Commit(live.Capture()));assert(disk.Commit(live.Capture()));const int writes=disk.writes;assert(!CommitLexiconEvidence(live.language,"evidence.glimpse",[&]{auto bad=live.Capture();bad.RiteStages.clear();return disk.Commit(bad);}));assert(live.language.Snapshot().empty()&&disk.writes==writes);assert(!CommitLexiconEvidence(live.language,"invented",[]{assert(false);return true;}));}
 std::cout<<"PASS: "<<casts<<" reality faults, "<<crossings<<" passage faults, "<<revivals<<" saved revival faults and "<<reloads<<" reloads; rollback windows, language, arrival, road credit and all nine checkpoint realms\n";
}
