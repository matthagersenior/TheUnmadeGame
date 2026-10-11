#include "World/UnmadePrototypeHub.h"
#include "Combat/UnmadePresentation.h"
#include "Player/UnmadeCharacter.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
namespace {
using namespace UnmadeCore;
FVector Site(int a,int site){auto& s=ExpeditionSpecs[a];return FVector(s.x+(site? -3500:0),s.y+(site?11500:8000),95);}
FVector Room(int a,int room){static constexpr int X[]={0,2600,2600,0,0,2600},Y[]={0,0,2600,2600,5200,5200};auto& s=ExpeditionSpecs[a];return FVector(s.x+X[room],s.y+15000+Y[room],95);}
FVector Board(int a){auto& s=ExpeditionSpecs[a];return FVector(s.x+700,s.y-700,95);}
FVector Record(int a,int c){return Room(a,c+1)+FVector(-650,450,0);}
FVector Captive(int a,int q){return Room(a,4)+FVector(-650,q?550:-550,0);}
FVector Council(int a){return Room(a,4)+FVector(650,0,0);}
FVector Truce(int a){return Room(a,5)+FVector(-650,650,0);}
FVector Kit(int a){return Site(a,0)+FVector(650,350,0);}
FVector HomeGate(int a){return Room(a,5)+FVector(0,1250,0);}
FVector Entrance(int a,int r){return Room(a,r)+FVector(0,-650,0);}
FName EnemyTag(int a,int slot){return FName(*FString::Printf(TEXT("Expedition.%d.Enemy.%d"),a,slot));}
AUnmadeEnemyCharacter* Enemy(UWorld* w,int a,int slot){for(TActorIterator<AUnmadeEnemyCharacter> e(w);e;++e)if(e->ActorHasTag(EnemyTag(a,slot)))return *e;return nullptr;}
bool Alive(AUnmadeCharacter* p){return IsValid(p)&&IsValid(p->GetCombat())&&!p->GetCombat()->IsDefeated();}
bool Local(UWorld* w,AUnmadeCharacter* p,FVector at,double range=230){if(!w||!Alive(p)||FVector::DistSquared(p->GetActorLocation(),at)>FMath::Square(range))return false;FCollisionQueryParams q(SCENE_QUERY_STAT(ExpeditionInspect),false);q.AddIgnoredActor(p);return !w->LineTraceTestByChannel(p->GetActorLocation()+FVector(0,0,35),at+FVector(0,0,65),ECC_Visibility,q);}
void Say(const FString& s){if(GEngine)GEngine->AddOnScreenDebugMessage(-1,12,FColor::Cyan,s);}
}
void AUnmadePrototypeHub::BuildExpeditions(){
 if(!GetWorld())return;ExpeditionSigns.SetNum(9);ExpeditionHazardSigns.SetNum(9);ExpeditionReturnGates.SetNum(9);ExpeditionBossGates.SetNum(9);
 const auto Sign=[&](FVector at,const FString& text){auto* a=GetWorld()->SpawnActor<ATextRenderActor>(at+FVector(0,0,95),FRotator(0,90,0));if(a){a->GetTextRender()->SetWorldSize(20);a->GetTextRender()->SetText(FText::FromString(text));}return a;};
 const auto Gate=[&](FVector at){auto* a=GetWorld()->SpawnActor<AStaticMeshActor>(at,FRotator::ZeroRotator);if(a){auto* m=a->GetStaticMeshComponent();m->SetMobility(EComponentMobility::Movable);m->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));m->SetWorldScale3D(FVector(6,.25,3));}return a;};
 for(int a=0;a<9;++a){const auto& s=UnmadeCore::ExpeditionSpecs[a];const auto Block=[&](FVector at,FVector scale){SpawnBlock(at,scale,FName(*FString::Printf(TEXT("Expedition.%d.Structure"),a)));};
  Block(FVector(s.x-500,s.y+7500,-25),FVector(100,170,.5)); // connected city/frontier/refuge approach
  Block(Room(a,2)+FVector(-1300,0,-120),FVector(65,90,.5));
  Sign(Board(a),FString(UTF8_TO_TCHAR(s.dungeon))+TEXT(" / E accept adventure; J journal"));
  for(int site=0;site<2;++site){FVector h=Site(a,site);Sign(h,UTF8_TO_TCHAR(site?s.refuge:s.frontier));
   for(int n=0;n<(site?2:3);++n){FVector home=h+FVector((n-1)*850,-700,0);Block(home+FVector(0,0,-105),FVector(7,6,.3));Block(home+FVector(-340,0,30),FVector(.2,6,2.6));Block(home+FVector(340,0,30),FVector(.2,6,2.6));Block(home+FVector(0,-290,30),FVector(7,.2,2.6));Block(home+FVector(0,0,175),FVector(7,6,.25));Block(home+FVector(-180,-120,-55),FVector(2,.8,.7));Block(home+FVector(170,-120,-55),FVector(1,1,.7));
    int person=site?n+3:n;if(!site&&(n==1||n==2))continue;FString id=FString::Printf(TEXT("npc.expedition.%d.%d"),a,person);
    if(auto* npc=GetWorld()->SpawnActor<AUnmadeNpcCharacter>(home+FVector(0,140,0),FRotator::ZeroRotator)){npc->ConfigureExpeditionResident(FName(*id),UTF8_TO_TCHAR(s.people[person]),s.realm,UTF8_TO_TCHAR(s.brief),a);}
   }
   Sign(h+FVector(0,500,0),TEXT("E - VISIT / LOCAL WITNESS"));
  }
  ExpeditionHazardSigns[a]=Sign(Room(a,3)+FVector(650,500,0),TEXT("DANGER FIELD - watch the warning; the chamber edges are safe"));
  Sign(Room(a,3)+FVector(-650,-650,0),TEXT("E - VENT THE DANGER FIELD FOR EIGHT SECONDS / FREE"));
  Block(Kit(a)+FVector(0,0,-75),FVector(2,1,.4));
  ExpeditionSigns[a]=Sign(Kit(a),TEXT("E weapon / F armor: choose one earned reward. Revisit to equip."));
  for(int r=0;r<6;++r){FVector c=Room(a,r);Sign(Entrance(a,r),FString::Printf(TEXT("E - %s / CHAMBER %d"),UTF8_TO_TCHAR(s.dungeon),r+1));
   // Distinct six-room switchback: doors only join authored neighboring chambers.
   bool west=r==1||r==2||r==5,east=r==0||r==3||r==4,north=r==1||r==3||r==5,south=r==0||r==2||r==4;
   for(int side=0;side<4;++side){bool open=side==0?west:side==1?east:side==2?north:south;bool vertical=side<2;float axis=side==0||side==3?-1200:1200;
    if(!open)Block(c+(vertical?FVector(axis,0,65):FVector(0,axis,65)),vertical?FVector(.25,24,3.4):FVector(24,.25,3.4));
    else for(int v:{-1,1})Block(c+(vertical?FVector(axis,v*750,65):FVector(v*750,axis,65)),vertical?FVector(.25,9,3.4):FVector(9,.25,3.4));
   }
   for(int p:{-1,1})Block(c+FVector(p*400,-100,0),FVector(1.6,1.6,2)); // actual LOS cover
  }
  for(int q=0;q<2;++q){int person=q+1;auto* t=Expeditions.Arc(a);FVector pos=t->returned&&(t->rescued&(1<<q))?Site(a,0)+FVector((person-1)*850,-560,0):Captive(a,q)+FVector(-300,0,0);if(auto* npc=GetWorld()->SpawnActor<AUnmadeNpcCharacter>(pos,FRotator::ZeroRotator)){npc->ConfigureExpeditionResident(FName(*FString::Printf(TEXT("npc.expedition.%d.%d"),a,person)),UTF8_TO_TCHAR(s.people[person]),s.realm,TEXT("The sealed holding chamber is not a home."),a,q);}}
  // The arena's west opening is the only entry; its northern gate is the earned return.
  ExpeditionBossGates[a]=Gate(Room(a,5)+FVector(-1200,0,55));if(ExpeditionBossGates[a].IsValid())ExpeditionBossGates[a]->SetActorRotation(FRotator(0,90,0));
  ExpeditionReturnGates[a]=Gate(HomeGate(a)+FVector(0,0,55));
  for(int c=0;c<3;++c)Sign(Record(a,c),FString::Printf(TEXT("E / F - EVIDENCE %d"),c+1));
  for(int q=0;q<2;++q){Sign(Captive(a,q),FString::Printf(TEXT("E - RELEASE %s / DEFEAT GUARD OR RESOLVE BOSS"),UTF8_TO_TCHAR(s.people[q+1])));Block(Captive(a,q)+FVector(-180,0,5),FVector(.2,5,2));}
  Sign(Council(a),TEXT("E: rescue resolution / F: expose evidence. Irreversible branch; inspect first."));Sign(Truce(a),TEXT("E: surrender evidence / F: read counterplay / Q: interrupt nearby attack"));Sign(HomeGate(a)+FVector(0,300,0),TEXT("RETURN PASS - opens after boss resolution"));
  Sign(Room(a,5)+FVector(650,-650,0),TEXT("E - GROUNDING LEVER / BRIEFLY INTERRUPTS THE BOSS; REUSABLE"));
  for(int k=0;k<9;++k){int room=k<4?k:k<8?4:5;FVector pos=Room(a,room)+FVector(k%2?600:300,k%2?-350:350,0);
   auto role=k==8?UnmadeCore::ExpeditionRole::Boss:static_cast<UnmadeCore::ExpeditionRole>(k%4);
   if(auto* e=GetWorld()->SpawnActor<AUnmadeEnemyCharacter>(pos,FRotator::ZeroRotator)){e->ConfigureEncounter(EnemyTag(a,k),k==8?UTF8_TO_TCHAR(s.boss):k%4==0?TEXT("Shield Sentinel"):k%4==1?TEXT("Flanking Skirmisher"):k%4==2?TEXT("Aimed Marksman"):TEXT("Fracture Disruptor"),FLinearColor(.7,.25+.04*a,.25));e->ConfigureExpedition(a,k,role);}
  }
  // A separate return walkway outside the dungeon wall rejoins the refuge and frontier.
  Block(Room(a,5)+FVector(1500,-2600,-120),FVector(13,105,.5));Sign(Room(a,5)+FVector(1500,1000,0),TEXT("OPEN RETURN WALK - west toward refuge; south toward frontier"));
 }
 RestoreCitizens(); // New stable residents now exist; restore their personal memory too.
 RefreshExpeditions();
}
bool AUnmadePrototypeHub::CommitExpeditions(const UnmadeCore::Expeditions& before,AUnmadeCharacter* player){if(WriteWorldSnapshot(player)){RefreshExpeditions();return true;}Expeditions=before;Say(TEXT("Adventure could not save. The choice and reward remain uncommitted; retry."));return false;}
void AUnmadePrototypeHub::RefreshExpeditions(){
 if(!GetWorld()||GetWorld()->GetTimeSeconds()<NextExpeditionRefresh)return;NextExpeditionRefresh=GetWorld()->GetTimeSeconds()+.5;
 auto* p=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));if(Alive(p)&&p->GetEquipment()){auto b=ExpeditionBenefit();p->GetEquipment()->SetExpeditionBonuses(b.attack,b.armor,b.glimpseDiscount,b.foldSeconds);}
 for(int a=0;a<9;++a){auto t=Expeditions.Arc(a);
  auto hazard=UnmadeCore::SampleDungeonHazard(GetWorld()->GetTimeSeconds()+a,GetWorld()->GetTimeSeconds()<ExpeditionHazardQuiet[a]||t->boss!=0||bExpeditionSaveRejected||Identity.Arc(a)->chart!=0);
  if(ExpeditionHazardSigns[a].IsValid())ExpeditionHazardSigns[a]->GetTextRender()->SetText(FText::FromString(hazard.warning?TEXT("FLOOR WARNING - STEP OUT OF THE FIELD"):hazard.danger?TEXT("DANGER PULSE - SAFE EDGES / FREE VENT"):TEXT("FLOOR QUIET - NEXT WARNING BEFORE DAMAGE")));
  if(Alive(p)&&hazard.danger&&FVector::DistSquared2D(p->GetActorLocation(),Room(a,3)+FVector(650,500,0))<FMath::Square(280.f)&&FMath::Abs(p->GetActorLocation().Z-95)<150)p->GetCombat()->ReceiveHazardPulse(static_cast<uint64>(GetUniqueID())*32+a+1,hazard.pulse,12+a);
  const auto Set=[](TWeakObjectPtr<AStaticMeshActor> g,bool open){if(g.IsValid()){g->SetActorHiddenInGame(open);g->SetActorEnableCollision(!open);}};Set(ExpeditionBossGates[a],!bExpeditionSaveRejected&&t->branch!=0);Set(ExpeditionReturnGates[a],!bExpeditionSaveRejected&&t->boss!=0);
  if(t->returned)for(TActorIterator<AUnmadeNpcCharacter> npc(GetWorld());npc;++npc)for(int person=1;person<=2;++person)if(npc->GetStableId()==FName(*FString::Printf(TEXT("npc.expedition.%d.%d"),a,person))&&(t->rescued&(1<<(person-1)))&&FVector::DistSquared2D(npc->GetActorLocation(),Site(a,0))>FMath::Square(2000.f)){FVector At=Site(a,0)+FVector((person-1)*850,-560,0);npc->SetActorLocation(At,false);npc->SetExpeditionHome(At);}
  if(ExpeditionSigns[a].IsValid())ExpeditionSigns[a]->GetTextRender()->SetText(FText::FromString(t->returned?FString(UTF8_TO_TCHAR(t->boss==1?UnmadeCore::ExpeditionSpecs[a].kill_return:UnmadeCore::ExpeditionSpecs[a].mercy_return))+FString::Printf(TEXT("\nE %s (+%d attack) / F %s (+%d defense). ONE PERMANENT CHOICE.\nTechnique: %s. E re-equips an owned kit."),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].weapon),7+a*2,UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].armor),5+a,UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].technique)):TEXT("Return from the dungeon to earn equipment and a technique. E weapon / F armor.")));
 }
}
bool AUnmadePrototypeHub::ExpeditionEnemyInactive(int32 a,int32 slot)const{auto* t=Expeditions.Arc(a);return bExpeditionSaveRejected||!t||slot<0||slot>8||(slot==8?(!t->branch||t->boss!=0):(t->boss!=0||(t->kills&(1<<slot))!=0));}
bool AUnmadePrototypeHub::RecordExpeditionDefeat(AUnmadeEnemyCharacter* e,int32 a,int32 slot){if(bExpeditionSaveRejected||!IsValid(e)||!e->IsExpeditionEnemy(a,slot)||!e->ActorHasTag(EnemyTag(a,slot))||!e->GetCombat()||!e->GetCombat()->IsDefeated())return false;auto* t=Expeditions.Arc(a);if(!t)return false;if(slot==8?t->boss==1:(t->kills&(1<<slot))!=0)return true;auto before=Expeditions;bool changed=slot==8?Expeditions.Boss(a,true):Expeditions.Kill(a,slot);return changed&&CommitExpeditions(before);}
bool AUnmadePrototypeHub::TryExpeditionInteraction(AUnmadeCharacter* p,double competing){
 if(!Alive(p)||!GetWorld())return false;if(TryIdentityInteraction(p,competing))return true;int arc=-1,kind=-1,index=-1;double best=FMath::Min(230.*230.,competing);FVector at;
 auto Candidate=[&](int a,int k,int n,FVector pos){double d=FVector::DistSquared(p->GetActorLocation(),pos);if(d<best&&Local(GetWorld(),p,pos)){best=d;arc=a;kind=k;index=n;at=pos;}};
 for(int a=0;a<9;++a){Candidate(a,0,0,Board(a));for(int site=0;site<2;++site)Candidate(a,1,site,Site(a,site)+FVector(0,500,0));for(int r=0;r<6;++r)Candidate(a,2,r,Entrance(a,r));for(int c=0;c<3;++c)Candidate(a,3,c,Record(a,c));for(int q=0;q<2;++q)Candidate(a,4,q,Captive(a,q));Candidate(a,5,0,Council(a));Candidate(a,6,0,Truce(a));Candidate(a,7,0,Kit(a));Candidate(a,8,0,Room(a,5)+FVector(650,-650,0));Candidate(a,9,0,Room(a,3)+FVector(-650,-650,0));}
 if(arc<0)return false;if(bExpeditionSaveRejected){Say(TEXT("This adventure save is invalid; its original record is protected."));return true;}auto before=Expeditions;auto* t=Expeditions.Arc(arc);bool change=false;const auto& s=UnmadeCore::ExpeditionSpecs[arc];FString line;
 switch(kind){case 0:change=Expeditions.Brief(arc);line=FString(UTF8_TO_TCHAR(s.brief))+TEXT(" Visit the frontier north of the city, then enter the six-chamber dungeon. J tracks progress.");break;
 case 1:change=Expeditions.Visit(arc,index);line=FString(UTF8_TO_TCHAR(index?s.refuge:s.frontier))+TEXT(": a real refuge, not a cargo stop. The dungeon lies farther north.");break;
 case 2:change=Expeditions.Enter(arc,index);line=change?TEXT("Chamber entered. Watch the enemy warnings; pillars break aimed attacks. Inspect the next doorway to register entry."):TEXT("Inspect earlier chambers and establish your objective branch before entering the boss arena.");break;
 case 3:change=Expeditions.Inspect(arc,index);line=UTF8_TO_TCHAR(s.clues[index]);break;
 case 4:{bool Present=false;for(TActorIterator<AUnmadeNpcCharacter> N(GetWorld());N;++N)if(N->GetStableId()==FName(*FString::Printf(TEXT("npc.expedition.%d.%d"),arc,index+1))&&FVector::DistSquared2D(N->GetActorLocation(),at)<FMath::Square(600.f))Present=true;change=Present&&Expeditions.Rescue(arc,index);line=change?FString(UTF8_TO_TCHAR(s.people[index+1]))+TEXT(": The cell is open. We will wait here until the passage is safe."):TEXT("Defeat this captive’s actual guard or resolve the boss, then return here to release them.");break;}
 case 5:if(t->branch){line=TEXT("Your objective branch is already committed.");break;}if(t->rescued==3){change=Expeditions.Choose(arc,1);line=TEXT("Rescue branch committed. Defeat the boss to free the return passage. F instead commits the full evidence branch before this choice.");}else line=TEXT("E rescue branch requires both captives. F evidence branch requires all three physical records. You may complete both objectives, but select one resolution.");break;
 case 6:if(t->branch==2&&(t->rooms&32)){auto* boss=Enemy(GetWorld(),arc,8);if(IsValid(boss)&&boss->GetCombat()&&!boss->GetCombat()->IsDefeated()&&FVector::DistSquared2D(boss->GetActorLocation(),at)<FMath::Square(1400.f)){change=Expeditions.Boss(arc,false);line=UTF8_TO_TCHAR(s.mercy_return);}}if(!change)line=TEXT("A living boss must receive the complete evidence after the evidence branch. You may still fight; no surrender can revive a defeated boss.");break;
 case 7:if(!t->returned){bool Witness=false;for(TActorIterator<AUnmadeNpcCharacter> N(GetWorld());N;++N)if(N->GetStableId()==FName(*FString::Printf(TEXT("npc.expedition.%d.0"),arc))&&FVector::DistSquared2D(N->GetActorLocation(),p->GetActorLocation())<FMath::Square(1800.f))Witness=true;change=Witness&&Expeditions.Return(arc);line=change?TEXT("Your return is witnessed. E again chooses the weapon; F chooses armor. Either includes the named technique."):TEXT("Resolve the boss and walk back to this frontier settlement first.");}else if(!t->reward){change=Expeditions.Claim(arc,1);line=FString(UTF8_TO_TCHAR(s.weapon))+TEXT(" equipped. Technique: ")+UTF8_TO_TCHAR(s.technique);}else{change=Expeditions.Equip(arc);line=TEXT("Your chosen realm equipment and technique are active; previous gear remains owned.");}break;
 case 9:if(t->rooms&8){ExpeditionHazardQuiet[arc]=GetWorld()->GetTimeSeconds()+8;line=TEXT("Danger field vented for eight seconds. The edge route is always safe.");}else line=TEXT("Enter the chamber before operating its vent.");break;
 case 8:if((t->rooms&32)&&GetWorld()->GetTimeSeconds()>=ExpeditionReleaseReady[arc]){if(auto* boss=Enemy(GetWorld(),arc,8)){boss->ExposeToFold(GetWorld()->GetTimeSeconds(),3);ExpeditionReleaseReady[arc]=GetWorld()->GetTimeSeconds()+8;line=TEXT("Grounding lever interrupts the boss for three seconds. Eight-second recovery; no Strain required.");}}if(line.IsEmpty())line=TEXT("Enter the arena first. The grounding lever must recover between uses.");break;
 }
 if(change&&!CommitExpeditions(before,p))return true;if(change){UnmadePresentation::Sound(p,kind==4?TEXT("release"):kind==6?TEXT("yield"):kind==7?TEXT("reward"):TEXT("inspect"));p->PlayIdentityMotion(FName(kind==4?TEXT("Release"):TEXT("Inspect")));}Say(line);return true;
}
bool AUnmadePrototypeHub::TryExpeditionAbility(AUnmadeCharacter* p,int32 kind){
 if(!Alive(p)||!GetWorld())return false;
 if(kind==0&&TryIdentityInteraction(p,230.*230.,true))return true;
 for(int a=0;a<9;++a){auto* t=Expeditions.Arc(a);if(kind==0&&Local(GetWorld(),p,Council(a))){auto before=Expeditions;if(Expeditions.Choose(a,2)){if(CommitExpeditions(before,p))Say(TEXT("Evidence branch committed. Carry the witnessed records to the living boss’s surrender marker, or defeat it."));}else Say(TEXT("Inspect all three records and enter the holding chamber; an existing branch cannot be changed."));return true;}
  if(kind==0&&Local(GetWorld(),p,Kit(a))){auto before=Expeditions;if(Expeditions.Claim(a,2)){if(CommitExpeditions(before,p))Say(FString(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].armor))+TEXT(" equipped with ")+UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].technique));}else Say(TEXT("Return must be witnessed first; equipment choice is permanent. E equips an already claimed reward."));return true;}
  if(kind==0)for(int c=0;c<3;++c)if(Local(GetWorld(),p,Record(a,c))){auto before=Expeditions;bool changed=Expeditions.Inspect(a,c);if(changed&&!CommitExpeditions(before,p))return true;Say(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].clues[c]));return true;}
  if(kind==0&&Local(GetWorld(),p,Truce(a))){Say(TEXT("Boss: two opening attacks, then a realm signature footprint. Half-health warnings accelerate. Aim locks when its warning begins. Step out of the mark, take pillar cover, guard, or use the free grounding lever. Evidence can end this fight alive."));return true;}
 }
 return false; // Q retains the existing earned Fold ability and strain cost.
}
FString AUnmadePrototypeHub::GetExpeditionJournal()const{FString text=TEXT("COMBAT & EXPLORATION / E interact; F inspect or alternate objective; J journal\n");for(int a=0;a<9;++a){auto t=Expeditions.Arc(a);if(!t->brief)continue;int rooms=0,clues=0,kills=0;for(int i=0;i<8;++i){rooms+=(i<6&&(t->rooms&(1<<i)))?1:0;clues+=(i<3&&(t->clues&(1<<i)))?1:0;kills+=(t->kills&(1<<i))?1:0;}text+=FString::Printf(TEXT("%s > %s > %s: rooms %d/6, evidence %d/3, guards %d/8, captives %d/2, branch %s, boss %s, return %s, reward %s\n"),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].city),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].frontier),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].dungeon),rooms,clues,kills,(t->rescued&1?1:0)+(t->rescued&2?1:0),t->branch==1?TEXT("rescue"):t->branch==2?TEXT("evidence"):TEXT("undecided"),t->boss==1?TEXT("defeated"):t->boss==2?TEXT("yielded alive"):TEXT("unresolved"),t->returned?TEXT("witnessed"):TEXT("pending"),t->reward==1?TEXT("weapon"):t->reward==2?TEXT("armor"):TEXT("unclaimed"));}if(Expeditions.Save().gear){int a=(Expeditions.Save().gear-1)/2;auto b=ExpeditionBenefit();text+=FString::Printf(TEXT("ACTIVE KIT: %s / %s; +%d attack, +%d defense, Glimpse cost -%d, Fold +%d seconds.\n"),UTF8_TO_TCHAR(Expeditions.Save().gear%2?UnmadeCore::ExpeditionSpecs[a].weapon:UnmadeCore::ExpeditionSpecs[a].armor),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].technique),b.attack,b.armor,b.glimpseDiscount,b.foldSeconds);}for(int a=0;a<9;++a){auto* i=Identity.Arc(a);auto* e=Expeditions.Arc(a);if(!e->brief)continue;text+=FString::Printf(TEXT("%s side quests: valve lights %d / target %d; hunt ration %s; map %s; repairs %d%d%d; sanctuary last day %d. E service / F survey.\n"),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].frontier),i->puzzle,UnmadeCore::WorldIdentity::Target(a),i->hunt?TEXT("claimed"):TEXT("available"),i->chart?TEXT("owned"):TEXT("pending"),i->repair&1?1:0,i->repair&2?1:0,i->repair&4?1:0,i->restDay);}return text;}
FString AUnmadePrototypeHub::GetExpeditionReaction(FName person)const{for(int a=0;a<9;++a)for(int n=0;n<5;++n)if(person==FName(*FString::Printf(TEXT("npc.expedition.%d.%d"),a,n))){auto t=Expeditions.Arc(a);const auto& s=UnmadeCore::ExpeditionSpecs[a];if((n==1||n==2)&&!(t->rescued&(1<<(n-1))))return FString(UTF8_TO_TCHAR(s.people[n]))+TEXT(": The door still needs your help. Release us at the holding chamber after the guard is defeated.");if(t->returned){auto* i=Identity.Arc(a);FString memory=n==1||n==2?TEXT("You opened my cell. I remember waiting until the passage was safe. "):n==3&&i->repair==7?TEXT("You repaired the three roadposts. I can watch the road again. "):i->repair?TEXT("I saw your repairs on the return road. "):TEXT("The return road still needs repair. ");return FString(UTF8_TO_TCHAR(s.people[n]))+TEXT(": ")+memory+UTF8_TO_TCHAR(t->boss==1?s.kill_return:s.mercy_return);}if(t->boss)return TEXT("The dungeon passage is open. Return to our equipment bench so we can witness what you chose.");return FString(UTF8_TO_TCHAR(s.people[n]))+TEXT(": ")+UTF8_TO_TCHAR(s.brief);}return FString();}
