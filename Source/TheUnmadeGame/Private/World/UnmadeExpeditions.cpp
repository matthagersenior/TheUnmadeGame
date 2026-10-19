#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeAdventureLayout.h"
#include "World/UnmadeRealmJourneyRules.h"
#include "World/UnmadeFirstFractureRules.h"
#include "World/UnmadeDiscoveryCivicRules.h"
#include "World/UnmadeEncounterHazardRules.h"
#include "World/UnmadeIdentityRules.h"
#include "World/UnmadeAdventureDepthRegistry.h"
#include "Save/UnmadeSaveSession.h"
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
FVector Room(int a,int room){auto p=UnmadeCore::DungeonRoom(a,room);auto& s=ExpeditionSpecs[a];return FVector(s.x+p.x,s.y+15000+p.y,95);}

FVector Board(int a){auto& s=ExpeditionSpecs[a];return FVector(s.x+700,s.y-700,95);}
FVector Record(int a,int c){return Room(a,c+1)+FVector(-650,450,0);}
FVector Captive(int a,int q){return Room(a,4)+FVector(-650,q?550:-550,0);}
FVector Council(int a){return Room(a,4)+FVector(650,0,0);}
FVector Truce(int a){return Room(a,5)+FVector(-650,650,0);}
FVector Kit(int a){return Site(a,0)+FVector(650,350,0);}
FVector HomeGate(int a){return Room(a,5)+FVector(0,1250,0);}
FVector ArenaCounterStations(int a,int station){
 return Room(a,5)+(station==0?FVector(-620,-620,0):FVector(620,80,0));
}
FVector SaltwakeWitnessBoard(){return Site(1,0)+FVector(480,1900,0);}
FVector EvidenceCounterStation(int a){
 const auto& c=UnmadeCore::EvidenceCounterplaySpecs[a];
 return Room(a,5)+FVector(c.offsetX,c.offsetY,0);
}
FVector Entrance(int a,int r){return Room(a,r)+FVector(0,-650,0);}
FName EnemyTag(int a,int slot){return FName(*FString::Printf(TEXT("Expedition.%d.Enemy.%d"),a,slot));}
AUnmadeEnemyCharacter* Enemy(UWorld* w,int a,int slot){for(TActorIterator<AUnmadeEnemyCharacter> e(w);e;++e)if(e->ActorHasTag(EnemyTag(a,slot)))return *e;return nullptr;}
bool Alive(AUnmadeCharacter* p){return IsValid(p)&&IsValid(p->GetCombat())&&!p->GetCombat()->IsDefeated();}
bool Local(UWorld* w,AUnmadeCharacter* p,FVector at,double range=230){if(!w||!Alive(p)||FVector::DistSquared(p->GetActorLocation(),at)>FMath::Square(range))return false;FCollisionQueryParams q(SCENE_QUERY_STAT(ExpeditionInspect),false);q.AddIgnoredActor(p);return !w->LineTraceTestByChannel(p->GetActorLocation()+FVector(0,0,35),at+FVector(0,0,65),ECC_Visibility,q);}

}
void AUnmadePrototypeHub::BuildExpeditions(){
 if(!GetWorld())return;ExpeditionSigns.SetNum(9);ExpeditionHazardSigns.SetNum(9);CrossRealmCombatSigns.SetNum(9);ExpeditionEvidenceSigns.SetNum(9);ExpeditionFractureActors.SetNum(9);ExpeditionFractureSigns.SetNum(9);ExpeditionReturnGates.SetNum(9);ExpeditionBossGates.SetNum(9);
 ArenaShelterVisible.fill(-1);ArenaAftermathVisible.fill(-1);CivicAftermathVisible.fill(-1);
 ArenaShelterUntil.fill(0);
 for(auto& Counter:ArenaCounters)Counter.Reset();
 for(auto& Counter:EvidenceCounters)Counter=UnmadeCore::EvidenceCounterplay{};
 const auto Sign=[&](FVector at,const FString& text){auto* a=GetWorld()->SpawnActor<ATextRenderActor>(at+FVector(0,0,95),FRotator(0,90,0));if(a){a->GetTextRender()->SetWorldSize(20);a->GetTextRender()->SetText(FText::FromString(text));}return a;};
 const auto Gate=[&](FVector at){auto* a=GetWorld()->SpawnActor<AStaticMeshActor>(at,FRotator::ZeroRotator);if(a){auto* m=a->GetStaticMeshComponent();m->SetMobility(EComponentMobility::Movable);m->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));m->SetWorldScale3D(FVector(6,.25,3));}return a;};
 for(int a=0;a<9;++a){const auto& s=UnmadeCore::ExpeditionSpecs[a];const auto Block=[&](FVector at,FVector scale){SpawnBlock(at,scale,FName(*FString::Printf(TEXT("Expedition.%d.Structure"),a)));};
  Block(FVector(s.x-500,s.y+7500,-25),FVector(100,170,.5)); // connected city/frontier/refuge approach
  // Canon: this clue is an actual visited-room investigation, not an earned reward.
  // A revisitable physical clue belongs to the room that was actually entered.
  // It is never a hidden quest gate, source of NPC omniscience, or free victory.
  const auto& fracture=UnmadeCore::FirstFracturePhysicals[a];
  const FVector at=Room(a,0)+FVector(fracture.dx,fracture.dy,-85);
  auto* relic=GetWorld()->SpawnActor<AStaticMeshActor>(at,FRotator::ZeroRotator);
  if(relic){
   auto* mesh=relic->GetStaticMeshComponent();
   mesh->SetMobility(EComponentMobility::Movable);
   mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));
   relic->SetActorScale3D(FVector(fracture.scaleX,fracture.scaleY,fracture.scaleZ));
   relic->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
   relic->SetActorHiddenInGame(true);
   ExpeditionFractureActors[a]=relic;
   ExpeditionFractureSigns[a]=Sign(at+FVector(0,0,175),TEXT("E - EXAMINE FIRSTHAND FRACTURE"));
   if(ExpeditionFractureSigns[a].IsValid())
    ExpeditionFractureSigns[a]->SetActorHiddenInGame(true);
  }

  int MinX=0,MaxX=0,MinY=0,MaxY=0;for(int r=0;r<6;++r){auto q=UnmadeCore::DungeonRoom(a,r);MinX=FMath::Min(MinX,q.x);MaxX=FMath::Max(MaxX,q.x);MinY=FMath::Min(MinY,q.y);MaxY=FMath::Max(MaxY,q.y);}Block(Room(a,0)+FVector((MinX+MaxX)/2.,(MinY+MaxY)/2.,-120),FVector((MaxX-MinX+2600)/100.,(MaxY-MinY+2600)/100.,.5));
  Sign(Board(a),FString(UTF8_TO_TCHAR(s.dungeon))+TEXT(" / E accept adventure; J journal"));
  for(int site=0;site<2;++site){FVector h=Site(a,site);Sign(h,UTF8_TO_TCHAR(site?s.refuge:s.frontier));
   for(int n=0;n<(site?2:3);++n){FVector home=h+FVector((n-1)*850,-700,0);Block(home+FVector(0,0,-105),FVector(7,6,.3));Block(home+FVector(-340,0,30),FVector(.2,6,2.6));Block(home+FVector(340,0,30),FVector(.2,6,2.6));Block(home+FVector(0,-290,30),FVector(7,.2,2.6));Block(home+FVector(0,0,175),FVector(7,6,.25));Block(home+FVector(-180,-120,-55),FVector(2,.8,.7));Block(home+FVector(170,-120,-55),FVector(1,1,.7));
    int person=site?n+3:n;if(!site&&(n==1||n==2))continue;FString id=FString::Printf(TEXT("npc.expedition.%d.%d"),a,person);
    if(auto* npc=GetWorld()->SpawnActor<AUnmadeNpcCharacter>(home+FVector(0,140,0),FRotator::ZeroRotator)){npc->ConfigureExpeditionResident(FName(*id),UTF8_TO_TCHAR(s.people[person]),s.realm,UTF8_TO_TCHAR(s.brief),a);}
   }
   Sign(h+FVector(0,500,0),TEXT("E - VISIT / LOCAL WITNESS"));
  }
  ExpeditionHazardSigns[a]=Sign(Room(a,3)+FVector(650,500,0),TEXT("DANGER FIELD - watch the warning; the chamber edges are safe"));
  // The report conveys observed battle facts, not a morality score.
  // Destination chart knowledge and the free vent still override this ward.
  // Local frontier watch responds only after a real receiving chief is handed
  // the previous realm's recorded battle testimony by the player.
  CrossRealmCombatSigns[a]=Sign(Room(a,3)+FVector(1050,850,0),TEXT("LOCAL BORDER WATCH"));
  if(CrossRealmCombatSigns[a].IsValid())CrossRealmCombatSigns[a]->SetActorHiddenInGame(true);
  Sign(Room(a,3)+FVector(-650,-650,0),TEXT("E - VENT THE DANGER FIELD FOR EIGHT SECONDS / FREE"));
  Block(Kit(a)+FVector(0,0,-75),FVector(2,1,.4));
  ExpeditionSigns[a]=Sign(Kit(a),TEXT("E weapon / F armor: choose one earned reward. Revisit to equip."));
  for(int r=0;r<6;++r){FVector c=Room(a,r);Sign(Entrance(a,r),FString::Printf(TEXT("E - %s / CHAMBER %d"),UTF8_TO_TCHAR(s.dungeon),r+1));
   // Distinct six-room switchback: doors only join authored neighboring chambers.
   bool west=UnmadeCore::DungeonDoor(a,r,-1,0),east=UnmadeCore::DungeonDoor(a,r,1,0)||r==UnmadeCore::WingAnchor(a),north=UnmadeCore::DungeonDoor(a,r,0,1),south=UnmadeCore::DungeonDoor(a,r,0,-1);
   for(int side=0;side<4;++side){bool open=side==0?west:side==1?east:side==2?north:south;bool vertical=side<2;float axis=side==0||side==3?-1200:1200;
    if(!open)Block(c+(vertical?FVector(axis,0,65):FVector(0,axis,65)),vertical?FVector(.25,24,3.4):FVector(24,.25,3.4));
    else for(int v:{-1,1})Block(c+(vertical?FVector(axis,v*750,65):FVector(v*750,axis,65)),vertical?FVector(.25,9,3.4):FVector(9,.25,3.4));
   }
   if(r<5){FVector next=(Room(a,r+1)-c).GetSafeNormal2D();const TCHAR* direction=next.X>0?TEXT("east"):next.X<0?TEXT("west"):next.Y>0?TEXT("north"):TEXT("south");Sign(c+next*850+FVector(-next.Y*220,next.X*220,0),FString::Printf(TEXT("CHAMBER %d: %s / FOLLOW THE ROOM NUMBERS"),r+2,direction));}
   for(int p:{-1,1})Block(c+FVector(p*400,-100,0),FVector(1.6,1.6,2)); // actual LOS cover
  }
  for(int q=0;q<2;++q){int person=q+1;auto* t=Expeditions.Arc(a);FVector pos=t->returned&&(t->rescued&(1<<q))?Site(a,0)+FVector((person-1)*850,-560,0):Captive(a,q)+FVector(-300,0,0);if(auto* npc=GetWorld()->SpawnActor<AUnmadeNpcCharacter>(pos,FRotator::ZeroRotator)){npc->ConfigureExpeditionResident(FName(*FString::Printf(TEXT("npc.expedition.%d.%d"),a,person)),UTF8_TO_TCHAR(s.people[person]),s.realm,TEXT("The sealed holding chamber is not a home."),a,q);}}
  // The arena's west opening is the only entry; its northern gate is the earned return.
  FVector Entry=(Room(a,4)-Room(a,5)).GetSafeNormal2D();ExpeditionBossGates[a]=Gate(Room(a,5)+Entry*1200+FVector(0,0,55));if(ExpeditionBossGates[a].IsValid())ExpeditionBossGates[a]->SetActorRotation(FRotator(0,Entry.X!=0?90:0,0));
  ExpeditionReturnGates[a]=Gate(HomeGate(a)+FVector(0,0,55));
  for(int c=0;c<3;++c)Sign(Record(a,c),FString::Printf(TEXT("E / F - EVIDENCE %d"),c+1));
  for(int q=0;q<2;++q){Sign(Captive(a,q),FString::Printf(TEXT("E - RELEASE %s / DEFEAT GUARD OR RESOLVE BOSS"),UTF8_TO_TCHAR(s.people[q+1])));Block(Captive(a,q)+FVector(-180,0,5),FVector(.2,5,2));}
  Sign(Council(a),TEXT("E: rescue resolution / F: expose evidence. Irreversible branch; inspect first."));Sign(Truce(a),TEXT("E: surrender evidence / F: read counterplay / Q: interrupt nearby attack"));Sign(HomeGate(a)+FVector(0,300,0),TEXT("RETURN PASS - opens after boss resolution"));
  Sign(Room(a,5)+FVector(650,-650,0),TEXT("E - GROUNDING LEVER / BRIEFLY INTERRUPTS THE BOSS; REUSABLE"));
  // Every realm has a different, timed TWO-position battlefield counter.
  // This is played while the existing signature boss remains dangerous:
  // no quest log interaction can complete the counter remotely.
  const auto& counter=UnmadeCore::Arenas[a];
  for(int station=0;station<2;++station){
    const FVector at=ArenaCounterStations(a,station);
    Sign(at,FString::Printf(TEXT("E - %s / %s %d / %s"),
       UTF8_TO_TCHAR(counter.name),
       counter.first==station?TEXT("FIRST"):TEXT("SECOND"),station+1,
       UTF8_TO_TCHAR(counter.counterObjective)));
    Block(at+FVector(0,0,-115),FVector(2.0,1.6,.28)); // navigable footing
  }
  // Canon: evidence must be personally obtained; no social or battle outcome is invented.
  // Each physical report offers a deliberate alternate tactical station.
  const auto& discovery=UnmadeCore::EvidenceCounterplaySpecs[a];
  const FVector discoveryAt=EvidenceCounterStation(a);
  ExpeditionEvidenceSigns[a]=Sign(discoveryAt,FString::Printf(TEXT("E - %s / EARN LOCAL PROOF FIRST"),
       UTF8_TO_TCHAR(discovery.name)));
  Block(discoveryAt+FVector(0,0,-115),FVector(1.8,1.6,.28));
  SpawnBlock(Room(a,5)+FVector(0,-70,50),FVector(1.4,1.9,2.8),
      FName(*FString::Printf(TEXT("Expedition.%d.ArenaShelter"),a)));

  // The seven formerly narrative-only aftermaths now have distinct reachable
  // physical investigation sites near their REAL expedition frontier homes.
  if(UnmadeCore::CivicUsesExpeditionAftermath(a)){
   const auto& civic=UnmadeCore::DiscoveryCivicSites[a];
   const auto& spec=UnmadeCore::RealmAftermathSpecs[a];
   const FVector home=Site(a,0);
   SpawnBlock(home+FVector(civic.mechanismX,900,25),FVector(.7,.8,1.4),FName(UTF8_TO_TCHAR(spec.mechanismSite)));
   SpawnBlock(home+FVector(civic.careX,1430,25),FVector(.8,.65,1.5),FName(UTF8_TO_TCHAR(spec.evidenceForCare)));
   SpawnBlock(home+FVector(civic.truthX,1430,25),FVector(.8,.65,1.5),FName(UTF8_TO_TCHAR(spec.evidenceForTruth)));
   Sign(home+FVector(civic.mechanismX,900,100),FString(TEXT("E - OPERATE / "))+UTF8_TO_TCHAR(civic.title));
   Sign(home+FVector(civic.careX,1430,100),TEXT("E - EXAMINE CARE EVIDENCE; TELL SUPPORT WITNESS"));
   Sign(home+FVector(civic.truthX,1430,100),TEXT("E - EXAMINE PUBLIC ACCOUNT; TELL TRUTH WITNESS"));
   for(int outcome=1;outcome<=2;++outcome){
    const double x=outcome==1?civic.careX:civic.truthX;
    // Enclosed branch doorway and visible platform, not a purely decorative line.
    SpawnBlock(home+FVector(x-360,1940,110),FVector(2.6,.4,2.5),
        FName(*FString::Printf(TEXT("Expedition.%d.CivicWall"),a)));
    SpawnBlock(home+FVector(x+360,1940,110),FVector(2.6,.4,2.5),
        FName(*FString::Printf(TEXT("Expedition.%d.CivicWall"),a)));
    SpawnBlock(home+FVector(x,1940,275),FVector(3.4,.4,.3),
        FName(*FString::Printf(TEXT("Expedition.%d.CivicWall"),a)));
    SpawnBlock(home+FVector(x,1940,110),FVector(2.6,.4,2.35),
        FName(*FString::Printf(TEXT("Expedition.%d.Civic%sGate"),a,outcome==1?TEXT("Care"):TEXT("Truth"))));
    SpawnBlock(home+FVector(x,2280,-40),FVector(7,8,.35),
        FName(*FString::Printf(TEXT("Expedition.%d.Civic%sRoute"),a,outcome==1?TEXT("Care"):TEXT("Truth"))));
    Sign(home+FVector(x,2370,95),UTF8_TO_TCHAR(outcome==1?civic.careWork:civic.truthWork));
   }
   Sign(home+FVector(-480,290,95),TEXT("CIVIC RETURN: EARN THREE RECORDS, DEFEAT OR SPARE THE BOSS, RETURN TO THE GUIDE"));
  }
  if(a==1){
    SpawnBlock(SaltwakeWitnessBoard()+FVector(0,0,-60),FVector(.95,.75,.65),
      TEXT("Saltwake.CivicWitnessBoard"));
    Sign(SaltwakeWitnessBoard()+FVector(0,0,90),
      TEXT("E - COMPARE RETURNED DRYWATER EVIDENCE WITH THE STORM INVOICE"));
  }
  for(int outcome=1;outcome<=3;++outcome)
    SpawnBlock(Site(a,0)+FVector(-550+outcome*260,1200,115),
      FVector(.65,.35,1.65),
      FName(*FString::Printf(TEXT("Expedition.%d.Aftermath.%d"),a,outcome)));
  for(int k=0;k<9;++k){int room=k<4?k:k<8?4:5;FVector pos=Room(a,room)+FVector(k%2?600:300,k%2?-350:350,0);
   auto role=k==8?UnmadeCore::ExpeditionRole::Boss:static_cast<UnmadeCore::ExpeditionRole>(UnmadeCore::EnemyRoleAt(a,k));
   if(auto* e=GetWorld()->SpawnActor<AUnmadeEnemyCharacter>(pos,FRotator::ZeroRotator)){e->ConfigureEncounter(EnemyTag(a,k),k==8?UTF8_TO_TCHAR(s.boss):role==UnmadeCore::ExpeditionRole::Sentinel?TEXT("Shield Sentinel"):role==UnmadeCore::ExpeditionRole::Skirmisher?TEXT("Flanking Skirmisher"):role==UnmadeCore::ExpeditionRole::Marksman?TEXT("Aimed Marksman"):TEXT("Fracture Disruptor"),FLinearColor(.7,.25+.04*a,.25));e->ConfigureExpedition(a,k,role);}
  }
  // Distinct battle priorities: a shield sentinel actively lends armor to
  // the boss while guarding within 500cm, and a marksman punishes stillness.
  // Both stand down when the boss yields or dies. They never write kill bits
  // and player attacks against them never mint renewable combat rewards.
  for(int add=0;add<2;++add){
    const auto role=add==0?UnmadeCore::ExpeditionRole::Sentinel:
        UnmadeCore::ExpeditionRole::Marksman;
    const FVector pos=Room(a,5)+(add==0?FVector(520,240,0):
        FVector(-520,280,0));
    if(auto* ArenaEscort=GetWorld()->SpawnActor<AUnmadeEnemyCharacter>(
         pos,FRotator::ZeroRotator)){
      ArenaEscort->ConfigureEncounter(EnemyTag(a,11+add),
          add==0?TEXT("Boss Ward Sentinel - BREAK THE SHIELD"):
                 TEXT("Boss Lane Marksman - USE COVER"),
          FLinearColor(.52,.32+.035*a,.64));
      ArenaEscort->ConfigureExpedition(a,11+add,role);
    }
  }
  // A separate return walkway outside the dungeon wall rejoins the refuge and frontier.
  double OuterX=MaxX+2200,OuterY=MaxY+17000;double BossX=Room(a,5).X-s.x,BossY=Room(a,5).Y-s.y;Block(FVector(s.x+OuterX,s.y+(8000+OuterY)/2.,-25),FVector(10,(OuterY-8000+1000)/100.,.5));Block(FVector(s.x+OuterX/2.,s.y+8000,-25),FVector(OuterX/100.+10,10,.5));Block(FVector(s.x+(BossX+OuterX)/2.,s.y+OuterY,-25),FVector((OuterX-BossX)/100.+10,10,.5));Block(FVector(s.x+BossX,s.y+(BossY+OuterY)/2.,-25),FVector(10,(OuterY-BossY)/100.+10,.5));Sign(FVector(s.x+BossX,s.y+OuterY,95),TEXT("RETURN WALK - EAST TO THE OUTER ROAD"));Sign(FVector(s.x+OuterX,s.y+OuterY,95),TEXT("RETURN WALK - SOUTH TO THE FRONTIER"));Sign(FVector(s.x+OuterX,s.y+8000,95),TEXT("RETURN WALK - WEST TO THE EQUIPMENT BENCH"));Sign(HomeGate(a)+FVector(0,450,0),TEXT("EARNED RETURN - NORTH TO THE MARKED OUTER WALK"));
 }
 RestoreCitizens(); // New stable residents now exist; restore their personal memory too.
 RefreshExpeditions();
}
bool AUnmadePrototypeHub::CommitExpeditions(const UnmadeCore::Expeditions& before,AUnmadeCharacter* player){
 if(WriteWorldSnapshot(player)){
  RefreshExpeditions();
  // Canon limit: this is one player's firsthand sense at the saved threshold,
  // not an NPC witness or proof of a preferred cosmological explanation.
  // This is a firsthand sound and observation, not a staged lore answer.
  // The original saved chamber bit makes it one-time across reloads,
  // including physical E entry as well as walking over the room boundary.
  if(IsValid(player))for(int a=0;a<9;++a)
   if(!(before.Arc(a)->rooms&1)&&(Expeditions.Arc(a)->rooms&1)){
    if(const char* cue=UnmadeCore::FirstFractureCue(a))
     UnmadePresentation::Sound(player,UTF8_TO_TCHAR(cue));
    if(const char* line=UnmadeCore::FirstFractureLine(a))
     NotifyRpg(FString::Printf(TEXT("FIRST FRACTURE / %s\n%s"),
       UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].city),
       UTF8_TO_TCHAR(line)));
   }
  return true;
 }
 Expeditions=before;
 NotifyRpg(TEXT("Adventure could not save. The choice and reward remain uncommitted; retry."));
 return false;
}
void AUnmadePrototypeHub::RefreshExpeditions(){
 if(!GetWorld()||GetWorld()->GetTimeSeconds()<NextExpeditionRefresh)return;NextExpeditionRefresh=GetWorld()->GetTimeSeconds()+.5;
 auto* p=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
 const bool playerReady=Alive(p)&&p->GetWorld()==GetWorld()&&UnmadeSave::IsSessionWritable(p);
 if(playerReady&&p->GetEquipment()){auto b=p->GetEquipment()->SelectedBenefit();p->GetEquipment()->SetExpeditionBonuses(b.attack,b.armor,b.glimpseDiscount,b.foldSeconds);}
 // Register actual chamber arrival before its actors can accept combat receipts.
 if(playerReady&&!bExpeditionSaveRejected){auto before=Expeditions;bool changed=false;for(int a=0;a<9;++a)for(int r=0;r<6;++r){auto& spec=UnmadeCore::ExpeditionSpecs[a];FVector at=p->GetActorLocation()-FVector(spec.x,spec.y+15000,95);if(UnmadeCore::InsideRegisteredChamber(a,r,at.X,at.Y,at.Z))changed=Expeditions.Enter(a,r)||changed;}if(changed)CommitExpeditions(before,p);}
 for(int a=0;a<9;++a){auto t=Expeditions.Arc(a);
  if(ExpeditionFractureActors[a].IsValid()){
   const bool observed=!bExpeditionSaveRejected&&(t->rooms&1)!=0;
   ExpeditionFractureActors[a]->SetActorHiddenInGame(!observed);
   if(ExpeditionFractureSigns[a].IsValid())
    ExpeditionFractureSigns[a]->SetActorHiddenInGame(!observed);
   if(observed){
    const auto& fracture=UnmadeCore::FirstFracturePhysicals[a];
    const double now=GetWorld()->GetTimeSeconds();
    ExpeditionFractureActors[a]->SetActorRotation(FRotator(
      9*FMath::Sin(now*fracture.spin),now*fracture.spin*18,0));
   }
  }
  if(t->boss!=0||bExpeditionSaveRejected){
    ArenaCounters[a].Reset();
    ArenaShelterUntil[a]=0;
  }
  const double Now=GetWorld()->GetTimeSeconds();
  // Read the original save-backed evidence, not temporary journal prose.
  if(ExpeditionEvidenceSigns[a].IsValid()){
    const auto& counter=UnmadeCore::EvidenceCounterplaySpecs[a];
    const bool hasProof=!bDepthSaveRejected&&!bExpeditionSaveRejected&&
      (Depth.Arc(a)->evidence!=0||t->clues==7);
    const TCHAR* state=bExpeditionSaveRejected||t->boss!=0
      ?TEXT("RESOLVED")
      :!hasProof?TEXT("PROOF NEEDED")
      :!t->branch||!(t->rooms&32)?TEXT("ARENA LOCKED")
      :Now<EvidenceCounters[a].ReadyAt()?TEXT("RECOVERING"):TEXT("READY");
    ExpeditionEvidenceSigns[a]->GetTextRender()->SetText(
      FText::FromString(FString::Printf(
      TEXT("%s / %s\nE - USE EARNED EVIDENCE\n%s"),
       UTF8_TO_TCHAR(counter.name),state,UTF8_TO_TCHAR(counter.intervention))));
  }
  const int shelter=(!bExpeditionSaveRejected&&t->boss==0&&
      (UnmadeCore::Arenas[a].shelterSeconds>0||UnmadeCore::EvidenceCounterplaySpecs[a].coverSeconds>0)&&Now<ArenaShelterUntil[a])?1:0;
  if(ArenaShelterVisible[a]!=shelter){
    SetRiteWorldActorState(FName(*FString::Printf(
        TEXT("Expedition.%d.ArenaShelter"),a)),shelter==1);
    ArenaShelterVisible[a]=shelter;
  }
  const auto aftermath=UnmadeCore::ArenaAftermathRecord(*t);
  const int outcomeValue=bExpeditionSaveRejected?0:static_cast<int>(aftermath);
  if(ArenaAftermathVisible[a]!=outcomeValue){
    for(int outcome=1;outcome<=3;++outcome)
      SetRiteWorldActorState(FName(*FString::Printf(
        TEXT("Expedition.%d.Aftermath.%d"),a,outcome)),outcome==outcomeValue);
    ArenaAftermathVisible[a]=outcomeValue;
  }
  const int source=UnmadeCore::CrossRealmCombatSource(a);
  const auto* origin=Expeditions.Arc(source);
  const auto stage=CrossRealmCarry.Stage(source);
  const auto response=UnmadeCore::CrossRealmCombatResponse(a,
     Expeditions.Arc(source)->boss,Expeditions.Arc(source)->kills,
     Expeditions.Arc(source)->rescued,
     Expeditions.Arc(source)->returned!=0,
     CrossRealmCarry.Stage(source)>=UnmadeCore::CarryStage::Delivered,
     stage==UnmadeCore::CarryStage::Reviewed,
     !bCrossRealmCarryRejected&&!bExpeditionSaveRejected&&!bDepthSaveRejected&&!bIdentitySaveRejected);
  if(CrossRealmCombatSigns[a].IsValid()){
    const bool witnessed=response.kind!=UnmadeCore::CrossRealmCombatKind::Unknown;
    CrossRealmCombatSigns[a]->SetActorHiddenInGame(!witnessed);
    if(witnessed){
      const bool fieldGrounded=Identity.Arc(a)->chart!=0||t->boss!=0;
      const FString account=FString::Printf(
        TEXT("RECEIVED FROM %s / %s\n%s\n%s: %d guards fell; %d people released\nLOCAL WATCH ADJUSTMENT %+d / %s / %s"),
        UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[source].city),
        origin->boss==1?TEXT("BOSS DEFEATED"):TEXT("BOSS YIELDED ALIVE"),
        UTF8_TO_TCHAR(UnmadeCore::CrossRealmCombatProfiles[a].frontier),
        UTF8_TO_TCHAR(UnmadeCore::CrossRealmCombatProfiles[a].warning),
        response.casualties,response.rescued,
        response.hazardDelta,
        stage==UnmadeCore::CarryStage::Reviewed?TEXT("LOCAL EVIDENCE REVIEWED"):TEXT("LOCAL EVIDENCE PENDING"),
        fieldGrounded?TEXT("FIELD GROUNDED / NO ACTIVE PULSE"):TEXT("ACTIVE FIELD / USE FREE VENT OR SURVEY MAP"));
      CrossRealmCombatSigns[a]->GetTextRender()->SetText(FText::FromString(account));
    }
  }
  auto hazard=UnmadeCore::SampleDungeonHazard(GetWorld()->GetTimeSeconds()+a,GetWorld()->GetTimeSeconds()<ExpeditionHazardQuiet[a]||t->boss!=0||bExpeditionSaveRejected||Identity.Arc(a)->chart!=0);
  // Safe-space instructions and damage footprint share the same authored realm shape.
  const FString realmTell=UTF8_TO_TCHAR(UnmadeCore::ExpeditionMove(UnmadeCore::ExpeditionRole::Boss,a,2,false).warning);
  if(ExpeditionHazardSigns[a].IsValid()){
   const FString notice=hazard.warning?TEXT("DANGER FIELD WARNING"):hazard.danger?TEXT("FIELD ACTIVE - TAKE THE SAFE SHAPE"):TEXT("FIELD QUIET - NEXT WARNING BEFORE DAMAGE");
   ExpeditionHazardSigns[a]->GetTextRender()->SetText(FText::FromString(notice+TEXT("\n")+realmTell+TEXT("\nFREE VENT AVAILABLE / SURVEY MAP GROUNDS THIS FIELD")));
  }
  const FVector hazardOffset=p?p->GetActorLocation()-(Room(a,3)+FVector(650,500,0)):FVector::ZeroVector;
  if(UnmadeCore::EncounterHazardInCylinder(playerReady,Alive(p),hazard,hazardOffset.X,hazardOffset.Y,hazardOffset.Z,280,150)
   &&UnmadeCore::RealmHazardFootprint(a,hazardOffset.X,hazardOffset.Y,280))
   p->GetCombat()->ReceiveHazardPulse(static_cast<uint64>(GetUniqueID())*32+a+1,hazard.pulse,
     FMath::Clamp(12+a+response.hazardDelta,6,25));
  const auto Set=[](TWeakObjectPtr<AStaticMeshActor> g,bool open){if(g.IsValid()){g->SetActorHiddenInGame(open);g->SetActorEnableCollision(!open);}};Set(ExpeditionBossGates[a],!bExpeditionSaveRejected&&t->branch!=0);Set(ExpeditionReturnGates[a],!bExpeditionSaveRejected&&t->boss!=0);
  if(t->returned)for(TActorIterator<AUnmadeNpcCharacter> npc(GetWorld());npc;++npc)for(int person=1;person<=2;++person)if(npc->GetStableId()==FName(*FString::Printf(TEXT("npc.expedition.%d.%d"),a,person))&&(t->rescued&(1<<(person-1)))&&FVector::DistSquared2D(npc->GetActorLocation(),Site(a,0))>FMath::Square(2000.f)){FVector At=Site(a,0)+FVector((person-1)*850,-560,0);npc->SetActorLocation(At,false);npc->SetExpeditionHome(At);}
  if(ExpeditionSigns[a].IsValid())ExpeditionSigns[a]->GetTextRender()->SetText(FText::FromString(t->returned?FString(UTF8_TO_TCHAR(t->boss==1?UnmadeCore::ExpeditionSpecs[a].kill_return:UnmadeCore::ExpeditionSpecs[a].mercy_return))+FString::Printf(TEXT("\nE %s (+%d attack) / F %s (+%d defense). ONE PERMANENT CHOICE; repeat to confirm.\nTechnique: %s. E re-equips an owned kit."),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].weapon),7+a*2,UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].armor),5+a,UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].technique)):TEXT("Return from the dungeon to earn equipment and a technique. E weapon / F armor.")));
 }
}
bool AUnmadePrototypeHub::ExpeditionEnemyInactive(int32 a,int32 slot)const{auto* t=Expeditions.Arc(a);return bExpeditionSaveRejected||!t||!UnmadeCore::ExpeditionEncounterActive(*t,slot);}
bool AUnmadePrototypeHub::RecordExpeditionDefeat(AUnmadeEnemyCharacter* e,int32 a,int32 slot){if(bExpeditionSaveRejected||!IsValid(e)||!e->IsExpeditionEnemy(a,slot)||!e->ActorHasTag(EnemyTag(a,slot))||!e->GetCombat()||!e->GetCombat()->IsDefeated())return false;auto* t=Expeditions.Arc(a);if(!t)return false;if(slot==8?t->boss==1:(t->kills&(1<<slot))!=0)return true;auto before=Expeditions;bool changed=slot==8?Expeditions.Boss(a,true):Expeditions.Kill(a,slot);return changed&&CommitExpeditions(before);}
bool AUnmadePrototypeHub::ConfirmExpeditionDecision(int a,int kind,int value){
 if(!GetWorld())return false;if(ExpeditionPendingDecision.Confirm(a,kind,value,GetWorld()->GetTimeSeconds()))return true;
 FString choice=kind==1?value==1?TEXT("RESCUE: the arena must be resolved through combat; the evidence surrender route will not be selected"):TEXT("EVIDENCE: present all three records to a living boss, or choose to fight"):FString::Printf(TEXT("%s: +%d %s, with %s. This realm grants ONE equipment choice; the other will remain unclaimed"),UTF8_TO_TCHAR(value==1?UnmadeCore::ExpeditionSpecs[a].weapon:UnmadeCore::ExpeditionSpecs[a].armor),value==1?7+a*2:5+a,value==1?TEXT("weapon attack"):TEXT("armor defense"),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].technique));
 NotifyRpg(choice+TEXT(". Repeat the same interaction within twenty seconds to confirm. Choosing the other option replaces this preview; waiting cancels it."));return false;
}
bool AUnmadePrototypeHub::TryExpeditionInteraction(AUnmadeCharacter* p,double competing){
 if(!Alive(p)||!GetWorld())return false;if(TryExperienceInteraction(p,competing))return true;if(TryDepthInteraction(p,0,competing))return true;if(TryIdentityInteraction(p,competing))return true;int arc=-1,kind=-1,index=-1;double best=FMath::Min(230.*230.,competing);FVector at;
 auto Candidate=[&](int a,int k,int n,FVector pos){double d=FVector::DistSquared(p->GetActorLocation(),pos);if(d<best&&Local(GetWorld(),p,pos)){best=d;arc=a;kind=k;index=n;at=pos;}};
 for(int a=0;a<9;++a){Candidate(a,0,0,Board(a));for(int site=0;site<2;++site)Candidate(a,1,site,Site(a,site)+FVector(0,500,0));for(int r=0;r<6;++r)Candidate(a,2,r,Entrance(a,r));for(int c=0;c<3;++c)Candidate(a,3,c,Record(a,c));for(int q=0;q<2;++q)Candidate(a,4,q,Captive(a,q));Candidate(a,5,0,Council(a));Candidate(a,6,0,Truce(a));Candidate(a,7,0,Kit(a));Candidate(a,8,0,Room(a,5)+FVector(650,-650,0));Candidate(a,9,0,Room(a,3)+FVector(-650,-650,0));
 if(a==1)Candidate(a,15,0,SaltwakeWitnessBoard());Candidate(a,10,0,ArenaCounterStations(a,0));Candidate(a,10,1,ArenaCounterStations(a,1));Candidate(a,13,0,EvidenceCounterStation(a));
  if(ExpeditionFractureActors.IsValidIndex(a)&&ExpeditionFractureActors[a].IsValid()
      &&!ExpeditionFractureActors[a]->IsHidden())
   Candidate(a,14,0,ExpeditionFractureActors[a]->GetActorLocation());
 }
 if(arc<0)return false;if(bExpeditionSaveRejected){NotifyRpg(TEXT("This adventure save is invalid; its original record is protected."));return true;}auto before=Expeditions;auto* t=Expeditions.Arc(arc);bool change=false;const auto& s=UnmadeCore::ExpeditionSpecs[arc];FString line;
 switch(kind){
 case 15:{
    const int ending=RealmAftermath.Ending(UnmadeCore::Realm::WidowedRain);
    if(UnmadeCore::CivicSaltwakeWitnessedDiscovery(t->rooms&1,t->clues,t->returned))
     line=FString::Printf(TEXT("FIRSTHAND BREAKWATER: three physically returned water records challenge the invoice. %s Other residents may examine these records locally. This does not publish anyone's private identity or force a new ending."),
       UTF8_TO_TCHAR(ending?RealmAftermath.WorldConsequence(UnmadeCore::Realm::WidowedRain):
                              UnmadeCore::RealmAftermathSpecs[1].ethicalDilemma));
    else line=TEXT("Examine the real first chamber, all three dungeon records and return to the frontier with a witnessed account before comparing the storm invoice. Saltwake's original sluice quest remains independently playable.");
    break;
 }
 case 0:change=Expeditions.Brief(arc);line=FString(UTF8_TO_TCHAR(s.brief))+TEXT(" Visit the frontier north of the city, then enter the six-chamber dungeon. J tracks progress.");break;
 case 1:change=Expeditions.Visit(arc,index);line=FString(UTF8_TO_TCHAR(index?s.refuge:s.frontier))+TEXT(": a real refuge, not a cargo stop. The dungeon lies farther north.");break;
 case 2:change=Expeditions.Enter(arc,index);line=change?TEXT("Chamber entered. Watch the enemy warnings; pillars break aimed attacks. Walking inside the next chamber registers entry; its doorway can also be inspected."):TEXT("Inspect earlier chambers and establish your objective branch before entering the boss arena.");break;
 case 3:change=Expeditions.Inspect(arc,index);line=UTF8_TO_TCHAR(s.clues[index]);break;
 case 4:{bool Present=false;for(TActorIterator<AUnmadeNpcCharacter> N(GetWorld());N;++N)if(N->GetStableId()==FName(*FString::Printf(TEXT("npc.expedition.%d.%d"),arc,index+1))&&FVector::DistSquared2D(N->GetActorLocation(),at)<FMath::Square(600.f))Present=true;change=Present&&Expeditions.Rescue(arc,index);line=change?FString(UTF8_TO_TCHAR(s.people[index+1]))+TEXT(": The cell is open. We will wait here until the passage is safe."):TEXT("Defeat this captive’s actual guard or resolve the boss, then return here to release them.");break;}
 case 5:if(t->branch){line=TEXT("Your objective branch is already committed.");break;}if(t->rescued==3){if(!ConfirmExpeditionDecision(arc,1,1))return true;change=Expeditions.Choose(arc,1);line=TEXT("Rescue branch committed. Defeat the boss to free the return passage. F instead commits the full evidence branch before this choice.");}else line=TEXT("E rescue branch requires both captives. F evidence branch requires all three physical records. You may complete both objectives, but select one resolution.");break;
 case 6:if(t->branch==2&&(t->rooms&32)){auto* boss=Enemy(GetWorld(),arc,8);if(IsValid(boss)&&boss->GetCombat()&&!boss->GetCombat()->IsDefeated()&&FVector::DistSquared2D(boss->GetActorLocation(),at)<FMath::Square(1400.f)){change=Expeditions.Boss(arc,false);line=UTF8_TO_TCHAR(s.mercy_return);}}if(!change)line=TEXT("A living boss must receive the complete evidence after the evidence branch. You may still fight; no surrender can revive a defeated boss.");break;
 case 7:if(!t->returned){bool Witness=false;for(TActorIterator<AUnmadeNpcCharacter> N(GetWorld());N;++N)if(N->GetStableId()==FName(*FString::Printf(TEXT("npc.expedition.%d.0"),arc))&&FVector::DistSquared(N->GetActorLocation(),p->GetActorLocation())<FMath::Square(1800.f)){FCollisionQueryParams sight(SCENE_QUERY_STAT(ExpeditionReturnWitness),false);sight.AddIgnoredActor(p);sight.AddIgnoredActor(*N);Witness=!GetWorld()->LineTraceTestByChannel(p->GetActorLocation()+FVector(0,0,45),N->GetActorLocation()+FVector(0,0,45),ECC_Visibility,sight);}change=Witness&&Expeditions.Return(arc);line=change?TEXT("Your return is witnessed. E again chooses the weapon; F chooses armor. Either includes the named technique."):TEXT("Resolve the boss and walk back to this frontier settlement first.");}else if(!t->reward){if(!ConfirmExpeditionDecision(arc,2,1))return true;change=Expeditions.Claim(arc,1);line=FString(UTF8_TO_TCHAR(s.weapon))+TEXT(" equipped. Technique: ")+UTF8_TO_TCHAR(s.technique);}else{change=Expeditions.Equip(arc);line=TEXT("Your chosen realm equipment and technique are active; previous gear remains owned.");}break;
 case 9:if(t->rooms&8){ExpeditionHazardQuiet[arc]=GetWorld()->GetTimeSeconds()+8;line=TEXT("Danger field vented for eight seconds. The edge route is always safe.");}else line=TEXT("Enter the chamber before operating its vent.");break;
 case 8:if((t->rooms&32)&&GetWorld()->GetTimeSeconds()>=ExpeditionReleaseReady[arc]){if(auto* boss=Enemy(GetWorld(),arc,8)){boss->ExposeToFold(GetWorld()->GetTimeSeconds(),3);ExpeditionReleaseReady[arc]=GetWorld()->GetTimeSeconds()+8;line=TEXT("Grounding lever interrupts the boss for three seconds. Eight-second recovery; no Strain required.");}}if(line.IsEmpty())line=TEXT("Enter the arena first. The grounding lever must recover between uses.");break;
 case 14:{
   if(!(t->rooms&1)||!ExpeditionFractureActors[arc].IsValid()
      ||ExpeditionFractureActors[arc]->IsHidden()){
    line=TEXT("This unexamined anomaly requires an actually registered chamber entry.");
    break;
   }
   const auto* fracture=UnmadeCore::FirstFracturePhysical(arc);
   const char* sensory=UnmadeCore::FirstFractureLine(arc);
   line=FString::Printf(TEXT("FIRSTHAND FRACTURE / %s\n%s\nWHAT TO INVESTIGATE: %s\nThis is a clue seen here, not permission to erase someone or proof of a universal answer."),
      UTF8_TO_TCHAR(s.city),UTF8_TO_TCHAR(sensory),UTF8_TO_TCHAR(fracture->investigation));
   UnmadePresentation::Sound(p,TEXT("inspect"));
   p->PlayIdentityMotion(FName(TEXT("Inspect")));
   break;
 }
 case 13:{
   const auto& counter=UnmadeCore::EvidenceCounterplaySpecs[arc];
   auto* boss=Enemy(GetWorld(),arc,8);
   const bool bBossReady=t->branch!=0&&(t->rooms&32)&&t->boss==0&&
       !bDepthSaveRejected&&!ExpeditionEnemyInactive(arc,8)&&
       IsValid(boss)&&IsValid(boss->GetCombat())&&
       !boss->GetCombat()->IsDefeated();
   const auto result=EvidenceCounters[arc].Use(arc,GetWorld()->GetTimeSeconds(),
       !bDepthSaveRejected&&Depth.Arc(arc)->evidence!=0,
       t->clues==7,t->boss!=0,bBossReady);
   switch(result){
    case UnmadeCore::EvidenceCounterplayResult::Active:
     boss->ExposeToFold(GetWorld()->GetTimeSeconds(),counter.exposureSeconds);
     if(counter.coverSeconds>0){
      ArenaShelterUntil[arc]=FMath::Max(ArenaShelterUntil[arc],
         GetWorld()->GetTimeSeconds()+counter.coverSeconds);
      SetRiteWorldActorState(FName(*FString::Printf(
          TEXT("Expedition.%d.ArenaShelter"),arc)),true);
     }
     line=FString::Printf(TEXT("%s / %s. Counterplay earned from your actual local proof. Boss interrupted for %.0f seconds.%s No boss outcome or rescue was automatically saved."),
       UTF8_TO_TCHAR(counter.name),UTF8_TO_TCHAR(counter.intervention),
       counter.exposureSeconds,counter.coverSeconds>0?
       TEXT(" Temporary physical cover blocks attack lines."):TEXT(" Exploit the opening while it lasts."));
     break;
    case UnmadeCore::EvidenceCounterplayResult::MissingProof:
     line=TEXT("This account lacks evidence. Examine local personal evidence with its witness OR inspect all three dungeon records, then return.");
     break;
    case UnmadeCore::EvidenceCounterplayResult::Recovering:
     line=FString::Printf(TEXT("The evidence station stabilizes for %.0f more seconds. The free lever and existing timed two-station counter still work."),
       FMath::Max(0.,EvidenceCounters[arc].ReadyAt()-GetWorld()->GetTimeSeconds()));
     break;
    default:
     line=TEXT("This station requires an entered objective branch and a living, active boss. The free grounding lever and two-station counter remain available.");
     break;
   }
   break;
 }
 case 10:{
   // A real positional battle objective: reach two stations in the realm's
   // prescribed order under active attacks. Partial progress never saves.
   auto* boss=Enemy(GetWorld(),arc,8);
   const bool bBossReady=t->branch!=0&&(t->rooms&32)&&t->boss==0&&
      !ExpeditionEnemyInactive(arc,8)&&IsValid(boss)&&
      IsValid(boss->GetCombat())&&!boss->GetCombat()->IsDefeated();
   if(!bBossReady)ArenaCounters[arc].Reset();
   const double now=GetWorld()->GetTimeSeconds();
   const auto result=ArenaCounters[arc].Use(arc,index,now,bBossReady);
   const auto& counter=UnmadeCore::Arenas[arc];
   switch(result){
     case UnmadeCore::CounterStep::Armed:
       line=FString::Printf(TEXT("%s: FIRST STATION ARMED. Cross the arena and use the opposite control within %.0f seconds, avoiding boss warnings."),
         UTF8_TO_TCHAR(counter.name),counter.openWindow);break;
     case UnmadeCore::CounterStep::Success:
       // The counter opens a genuine finite attack window by interrupting
       // the existing native boss attack. Cover is a physical LOS obstruction.
       boss->ExposeToFold(now,counter.exposureSeconds);
       if(counter.shelterSeconds>0){
          ArenaShelterUntil[arc]=now+counter.shelterSeconds;
          SetRiteWorldActorState(FName(*FString::Printf(
            TEXT("Expedition.%d.ArenaShelter"),arc)),true);
       }
       line=FString::Printf(TEXT("%s: COUNTER COMPLETED. Boss exposed for %.0f seconds. %s"),
         UTF8_TO_TCHAR(counter.name),counter.exposureSeconds,
         counter.shelterSeconds>0?TEXT("Temporary cover is active; fight around it."):
           TEXT("Strike during recovery or reposition for the next pattern."));break;
     case UnmadeCore::CounterStep::WrongOrder:
       line=FString::Printf(TEXT("Wrong control first. %s"),UTF8_TO_TCHAR(counter.counterObjective));break;
     case UnmadeCore::CounterStep::AlreadyArmed:
       line=TEXT("The first station is armed. Reach the OTHER station before the signal expires.");break;
     case UnmadeCore::CounterStep::Expired:
       line=TEXT("The counter window expired during the crossing. Re-arm the FIRST station and try a safer route.");break;
     case UnmadeCore::CounterStep::Cooldown:
       line=TEXT("The arena counter is recharging. Defend, evade or use existing cover before the next attempt.");break;
     default:
       line=TEXT("This arena counter requires the active, living boss and the real branch entry.");break;
   }
   break;
 }
 }
 if(change&&!CommitExpeditions(before,p))return true;if(change){if(kind==0)TrackExperienceRealm(arc,p);if(kind==2)LearnExperienceTutorial(1,p);if(kind==7&&Expeditions.Arc(arc)->reward&&(!p->GetEquipment()||!p->GetEquipment()->SelectRealmReward(arc))){NotifyRpg(TEXT("Reward ownership is saved, but equipment selection could not save. Press E at this reward marker to retry; your previous equipment remains active."));return true;}UnmadePresentation::Sound(p,kind==4?TEXT("release"):kind==6?TEXT("yield"):kind==7?TEXT("reward"):TEXT("inspect"));p->PlayIdentityMotion(FName(kind==4?TEXT("Release"):TEXT("Inspect")));}NotifyRpg(line);return true;
}
bool AUnmadePrototypeHub::TryExpeditionAbility(AUnmadeCharacter* p,int32 kind){
 if(!Alive(p)||!GetWorld())return false;
 if(kind==0&&TryDepthInteraction(p,3,230.*230.))return true;
 if(kind==0&&TryIdentityInteraction(p,230.*230.,true))return true;
 for(int a=0;a<9;++a){auto* t=Expeditions.Arc(a);if(kind==0&&Local(GetWorld(),p,Council(a))){auto before=Expeditions;if(!t->branch&&t->clues==7&&(t->rooms&16)&&!ConfirmExpeditionDecision(a,1,2))return true;if(Expeditions.Choose(a,2)){if(CommitExpeditions(before,p))NotifyRpg(TEXT("Evidence branch committed. Carry the witnessed records to the living boss’s surrender marker, or defeat it."));}else NotifyRpg(TEXT("Inspect all three records and enter the holding chamber; an existing branch cannot be changed."));return true;}
  if(kind==0&&Local(GetWorld(),p,Kit(a))){auto before=Expeditions;if(t->returned&&!t->reward&&!ConfirmExpeditionDecision(a,2,2))return true;if(Expeditions.Claim(a,2)){if(CommitExpeditions(before,p)){if(!p->GetEquipment()||!p->GetEquipment()->SelectRealmReward(a)){NotifyRpg(TEXT("Armor ownership is saved, but equipment selection could not save. Press E at this reward marker to retry; your previous equipment remains active."));return true;}NotifyRpg(FString(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].armor))+TEXT(" equipped with ")+UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].technique));}}else NotifyRpg(TEXT("Return must be witnessed first; equipment choice is permanent. E equips an already claimed reward."));return true;}
  if(kind==0)for(int c=0;c<3;++c)if(Local(GetWorld(),p,Record(a,c))){auto before=Expeditions;bool changed=Expeditions.Inspect(a,c);if(changed&&!CommitExpeditions(before,p))return true;NotifyRpg(UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].clues[c]));return true;}
  if(kind==0&&Local(GetWorld(),p,Truce(a))){NotifyRpg(TEXT("Boss: two opening attacks, then a realm signature footprint. Half-health warnings accelerate. Aim locks when its warning begins. Step out of the mark, take pillar cover, guard, or use the free grounding lever. Evidence can end this fight alive."));return true;}
 }
 return false; // Q retains the existing earned Fold ability and strain cost.
}
FString AUnmadePrototypeHub::GetExpeditionJournal()const{FString text=TEXT("COMBAT & EXPLORATION / E interact; F inspect or alternate objective; J journal\n");for(int a=0;a<9;++a){auto t=Expeditions.Arc(a);if(!t->brief)continue;int rooms=0,clues=0,kills=0;for(int i=0;i<8;++i){rooms+=(i<6&&(t->rooms&(1<<i)))?1:0;clues+=(i<3&&(t->clues&(1<<i)))?1:0;kills+=(t->kills&(1<<i))?1:0;}text+=FString::Printf(TEXT("%s > %s > %s: rooms %d/6, evidence %d/3, guards %d/8, captives %d/2, branch %s, boss %s, return %s, reward %s\n"),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].city),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].frontier),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].dungeon),rooms,clues,kills,(t->rescued&1?1:0)+(t->rescued&2?1:0),t->branch==1?TEXT("rescue"):t->branch==2?TEXT("evidence"):TEXT("undecided"),t->boss==1?TEXT("defeated"):t->boss==2?TEXT("yielded alive"):TEXT("unresolved"),t->returned?TEXT("witnessed"):TEXT("pending"),t->reward==1?TEXT("weapon"):t->reward==2?TEXT("armor"):TEXT("unclaimed"));}if(GetWorld())if(auto* player=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))if(auto* equipment=player->GetEquipment()){auto b=equipment->SelectedBenefit();text+=FString::Printf(TEXT("SELECTED REALM MODIFIERS: +%d attack, +%d defense, Glimpse cost -%d, Fold +%d seconds. I shows independent weapon, armor and techniques.\n"),b.attack,b.armor,b.glimpseDiscount,b.foldSeconds);}for(int a=0;a<9;++a){auto* i=Identity.Arc(a);auto* e=Expeditions.Arc(a);if(!e->brief)continue;text+=FString::Printf(TEXT("%s side quests: valve lights %d / target %d; hunt ration %s; map %s; repairs %d%d%d; sanctuary last day %d. E service / F survey.\n"),UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].frontier),i->puzzle,UnmadeCore::WorldIdentity::Target(a),i->hunt?TEXT("claimed"):TEXT("available"),i->chart?TEXT("owned"):TEXT("pending"),i->repair&1?1:0,i->repair&2?1:0,i->repair&4?1:0,i->restDay);}
 for(int a=0;a<9;++a){
   const auto* e=Expeditions.Arc(a);
   if(!e||!e->brief)continue;
      // Discovery-derived tactical information cannot invent a permanent boss result.
   const auto& proof=UnmadeCore::EvidenceCounterplaySpecs[a];
   const bool earned=(!bDepthSaveRejected&&Depth.Arc(a)->evidence!=0)||e->clues==7;
   text+=FString::Printf(TEXT("ARENA DISCOVERY / %s: %s. %s Counterplay earned: %s. E at the physical evidence station. Exposure %.0fs, cover %.0fs, cooldown %.0fs. Existing free grounding lever and two-station boss counters remain usable.\n"),
     UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].city),
     UTF8_TO_TCHAR(proof.name),UTF8_TO_TCHAR(proof.intervention),
     earned?TEXT("ready from your physical evidence"):TEXT("not yet; read local personal evidence or all three dungeon records"),
     proof.exposureSeconds,proof.coverSeconds,proof.cooldownSeconds);
   const auto& CounterObjective=UnmadeCore::Arenas[a];
   text+=FString::Printf(TEXT("ARENA STRATEGY / %s: %s. Use FIRST and SECOND stations within %.0f seconds. Shield sentinels, aimed marksmen and boss warnings remain real.\n"),
     UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].city),
     UTF8_TO_TCHAR(CounterObjective.counterObjective),
     CounterObjective.openWindow);
   const auto Outcome=UnmadeCore::ArenaAftermathRecord(*e);
   if(Outcome!=UnmadeCore::ArenaAftermath::None)
     text+=FString::Printf(TEXT("WITNESSED ARENA AFTERMATH / %s: %s\n"),
       UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].city),
       UTF8_TO_TCHAR(UnmadeCore::ArenaAftermathText(a,Outcome)));
 }
 for(int a=0;a<9;++a){
  const auto* e=Expeditions.Arc(a);
  if(!e||!e->brief)continue;
  if(UnmadeCore::CivicUsesExpeditionAftermath(a)){
   const auto* civic=UnmadeCore::CivicSite(a);
   const auto realm=static_cast<UnmadeCore::Realm>(a);
   const int ending=RealmAftermath.Ending(realm);
   text+=FString::Printf(TEXT("CIVIC RETURN / %s: %s | evidence %s | stage %d/4 | %s\\n"),
      UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[a].city),UTF8_TO_TCHAR(civic->title),
      UnmadeCore::CivicEvidenceReady(a,e)?TEXT("earned; speak to frontier guide"):TEXT("requires 3 records and witnessed return"),
      RealmAftermath.Stage(realm),UTF8_TO_TCHAR(ending?RealmAftermath.WorldConsequence(realm):
                               UnmadeCore::RealmAftermathSpecs[a].ethicalDilemma));
  }else if(a==1){
   text+=FString::Printf(TEXT("SALTWAKE / existing storm sluice and two civic resolutions. Returned breakwater proof: %s\\n"),
     UnmadeCore::CivicSaltwakeWitnessedDiscovery(e->rooms&1,e->clues,e->returned)?TEXT("personally earned"):TEXT("not complete"));
  }
 }
 return text;}
FString AUnmadePrototypeHub::GetExpeditionReaction(FName person)const{FString Personal=GetDepthReaction(person),Integration=ExperienceReaction(person);if(!Personal.IsEmpty())return Personal+TEXT(" ")+Integration;for(int a=0;a<9;++a)for(int n=0;n<5;++n)if(person==FName(*FString::Printf(TEXT("npc.expedition.%d.%d"),a,n))){auto t=Expeditions.Arc(a);const auto& s=UnmadeCore::ExpeditionSpecs[a];if((n==1||n==2)&&!(t->rescued&(1<<(n-1))))return FString(UTF8_TO_TCHAR(s.people[n]))+TEXT(": The door still needs your help. Release us at the holding chamber after the guard is defeated.");if(t->returned){auto* i=Identity.Arc(a);
    // Public outcome text is descriptive only: it has no action or save side effects.
    // Only a returned local companion observes the public consequences of the
    // saved accord; personal witnesses retain their separate authored lines.
    const int accord=Depth.Arc(a)->choice;
    const FString publicAftereffect=(n==1||n==2||n==4)&&accord
        ?FString(UTF8_TO_TCHAR(accord==1?UnmadeCore::DepthSpecs[a].public_return_a:UnmadeCore::DepthSpecs[a].public_return_b))
        :FString();
    FString memory=n==1||n==2?TEXT("You opened my cell. I remember waiting until the passage was safe. "):n==3&&i->repair==7?TEXT("You repaired the three roadposts. I can watch the road again. "):i->repair?TEXT("I saw your repairs on the return road. "):TEXT("The return road still needs repair. ");return FString(UTF8_TO_TCHAR(s.people[n]))+TEXT(": ")+memory+UTF8_TO_TCHAR(t->boss==1?s.kill_return:s.mercy_return)+TEXT(" ")+publicAftereffect+TEXT(" ")+Integration;}if(t->boss)return TEXT("The dungeon passage is open. Return to our equipment bench so we can witness what you chose.");return FString(UTF8_TO_TCHAR(s.people[n]))+TEXT(": ")+UTF8_TO_TCHAR(s.brief);}return FString();}

FString AUnmadePrototypeHub::GetDiscoveryCivicResidentLine(FName Id)const
{
 if(bRealmAftermathSaveRejected)return FString();
 for(int a=0;a<9;++a)if(UnmadeCore::CivicUsesExpeditionAftermath(a)){
  const auto* C=UnmadeCore::CivicSite(a);
  const auto realm=static_cast<UnmadeCore::Realm>(a);
  const int stage=RealmAftermath.Stage(realm);
  const int ending=RealmAftermath.Ending(realm);
  const int approach=RealmAftermath.Snapshot().approach[a];
  if(!stage||bExpeditionSaveRejected||
     !UnmadeCore::CivicEvidenceReady(a,Expeditions.Arc(a)))continue;
  if(Id==FName(UTF8_TO_TCHAR(C->initiator))){
   if(ending)return FString(TEXT(" The local proof was brought home. We now maintain the chosen route: "))+
      UTF8_TO_TCHAR(RealmAftermath.WorldConsequence(realm))+
      TEXT(" The alternate public decision remains closed.");
   return TEXT(" We spoke about the returned physical clues. Operate the worksite, find care or truth evidence, then speak to that actual witness.");
  }
  if(Id==FName(UTF8_TO_TCHAR(C->careWitness))&&approach==1&&stage>=3)
   return FString(TEXT(" I received the local shelter evidence. "))+UTF8_TO_TCHAR(C->careWork)+
     TEXT(". No public identity or innocent verdict is implied.");
  if(Id==FName(UTF8_TO_TCHAR(C->truthWitness))&&approach==2&&stage>=3)
   return FString(TEXT(" I personally heard your evidence for public scrutiny. "))+UTF8_TO_TCHAR(C->truthWork)+
     TEXT(". Records may be published only with the needed privacy and consent.");
 }
 if(!bExpeditionSaveRejected&&
    (Id==FName(TEXT("npc.saltwake.rainkeeper.001"))||
     Id==FName(TEXT("npc.saltwake.navigator.001")))){
  const auto* e=Expeditions.Arc(1);
  if(RealmAftermath.Ending(UnmadeCore::Realm::WidowedRain)!=0&&
     UnmadeCore::CivicSaltwakeWitnessedDiscovery(e->rooms&1,e->clues,e->returned))
     return TEXT(" The traveler has three returned breakwater records. I can compare those records against the local invoice, but cannot pretend to have seen their earlier journey.");
 }
 return FString();
}
bool AUnmadePrototypeHub::GetDiscoveryCivicRoutineTarget(
 FName Id,bool AtNight,FVector& Out)const
{
 if(AtNight||bRealmAftermathSaveRejected)return false;
 for(int a=0;a<9;++a)if(UnmadeCore::CivicUsesExpeditionAftermath(a)){
  const auto* C=UnmadeCore::CivicSite(a);
  const auto realm=static_cast<UnmadeCore::Realm>(a);
  const int stage=RealmAftermath.Stage(realm),ending=RealmAftermath.Ending(realm);
  const int approach=RealmAftermath.Snapshot().approach[a];
  if(!stage||bExpeditionSaveRejected||
     !UnmadeCore::CivicEvidenceReady(a,Expeditions.Arc(a)))continue;
  const auto& expedition=UnmadeCore::ExpeditionSpecs[a];
  const FVector home(expedition.x,expedition.y+8000,95);
  if(Id==FName(UTF8_TO_TCHAR(C->initiator))){
   Out=home+FVector(C->mechanismX,900,0);return true;
  }
  if(stage>=3&&approach==1&&Id==FName(UTF8_TO_TCHAR(C->careWitness))){
   Out=home+FVector(C->careX,ending==1?2280:1430,0);return true;
  }
  if(stage>=3&&approach==2&&Id==FName(UTF8_TO_TCHAR(C->truthWitness))){
   Out=home+FVector(C->truthX,ending==2?2280:1430,0);return true;
  }
 }
 if(!bExpeditionSaveRejected&&
    RealmAftermath.Stage(UnmadeCore::Realm::WidowedRain)==4){
  const auto* e=Expeditions.Arc(1);
  if(UnmadeCore::CivicSaltwakeWitnessedDiscovery(e->rooms&1,e->clues,e->returned)){
   if(Id==FName(TEXT("npc.saltwake.rainkeeper.001"))&&
      RealmAftermath.Ending(UnmadeCore::Realm::WidowedRain)==1){
    Out=FVector(0,-49000,95);return true;
   }
   if(Id==FName(TEXT("npc.saltwake.navigator.001"))&&
      RealmAftermath.Ending(UnmadeCore::Realm::WidowedRain)==2){
    Out=FVector(1200,-49000,95);return true;
   }
  }
 }
 return false;
}
