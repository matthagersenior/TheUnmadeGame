#include "UI/UnmadeRpgFeedback.h"
#include "World/UnmadePrototypeHub.h"
#include "Player/UnmadeCharacter.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
namespace {
using namespace UnmadeCore;
FVector Yard(int City,FVector Offset=FVector::ZeroVector){const auto& C=RoadCities[City];return FVector(C.x+3800,C.y+900,75)+Offset;}
FVector Clue(int City,int Index){return Yard(City,FVector(-150,300+Index*350,0));}
FVector Mechanism(int City){return Yard(City,FVector(300,1000,0));}
FVector Keeper(int City){return Yard(City,FVector(1000,1500,25));}
FVector Checkpoint(int City,int Route){return Yard(City,Route==3?FVector(100,1040,0):FVector(500,440,0));}
FVector Receiving(int City){return Yard(City,FVector(-300,-350,0));}
bool Able(AUnmadeCharacter* P){return IsValid(P)&&IsValid(P->GetCombat())&&!P->GetCombat()->IsDefeated();}
void Say(const FString& Line,FColor Color=FColor::Cyan){if(GEngine)UnmadeRpgFeedback(-1,9.f,Color,Line);}
bool Sight(UWorld* World,AUnmadeCharacter* P,FVector At,double Range,AActor* Ignore=nullptr){
 if(!World||!Able(P)||FVector::DistSquared(P->GetActorLocation(),At)>FMath::Square(Range))return false;
 FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeRoadSight),false);Params.AddIgnoredActor(P);if(Ignore)Params.AddIgnoredActor(Ignore);
 return !World->LineTraceTestByChannel(P->GetActorLocation()+FVector(0,0,35),At+FVector(0,0,85),ECC_Visibility,Params);
}
FString RouteText(int City,int Route){return Route==1?TEXT("hand-maintained daylight lane and open recess"):
 Route==2?FString(UTF8_TO_TCHAR(RoadCities[City].openLine))+TEXT("; recess shut"):
 Route==3?FString(UTF8_TO_TCHAR(RoadCities[City].sealedLine))+TEXT("; service lane shut"):
 Route==4&&City==4?TEXT("witnessed shared-kiln accord; kiln and archive open at night"):TEXT("undecided");}
AUnmadeEnemyCharacter* ActualKeeper(UWorld* World,int City){const FName Tag(*FString::Printf(TEXT("Road.%d.Elder"),City));for(TActorIterator<AUnmadeEnemyCharacter> E(World);E;++E)if(E->ActorHasTag(Tag))return *E;return nullptr;}
}
bool AUnmadePrototypeHub::CommitRoads(const UnmadeCore::RememberedRoads& Before,AUnmadeCharacter* StrainOwner){
 if(WriteWorldSnapshot(StrainOwner)){RefreshRememberedRoads();return true;}
 RememberedRoads=Before;Say(TEXT("The road record could not save. Cargo, routes and memories are unchanged; retry."),FColor::Red);return false;
}
void AUnmadePrototypeHub::BuildRememberedRoads(){
 if(!GetWorld())return;
 RoadLaneGates.SetNum(11);RoadRecessGates.SetNum(11);RoadSigns.SetNum(11);
 const auto Sign=[&](FVector At,const FString& Text){auto* A=GetWorld()->SpawnActor<ATextRenderActor>(At,FRotator(0,90,0));if(A){A->GetTextRender()->SetWorldSize(17);A->GetTextRender()->SetText(FText::FromString(Text));}return A;};
 const auto Gate=[&](FVector At){auto* A=GetWorld()->SpawnActor<AStaticMeshActor>(At,FRotator::ZeroRotator);if(A){auto* M=A->GetStaticMeshComponent();M->SetMobility(EComponentMobility::Movable);M->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));M->SetWorldScale3D(FVector(.3,2.7,2.4));}return A;};
 for(int i=0;i<11;++i){const auto& C=UnmadeCore::RoadCities[i];
  SpawnBlock(FVector(C.x+500,C.y,-30),FVector(120,110,.6),FName(*FString::Printf(TEXT("Road.%d.ConnectedGround"),i)));
  const auto Block=[&](FVector Offset,FVector Scale,const TCHAR* Part){return SpawnBlock(Yard(i,Offset),Scale,FName(*FString::Printf(TEXT("Road.%d.%s"),i,Part)));};
  Block(FVector(350,500,-92),FVector(20,26,.35),TEXT("YardFloor"));
  Block(FVector(1000,1500,-92),FVector(22,22,.35),TEXT("NestFloor"));
  Block(FVector(400,300,50),FVector(6,.3,2.5),TEXT("LaneSouth"));
  Block(FVector(400,580,50),FVector(6,.3,2.5),TEXT("LaneNorth"));
  Block(FVector(700,440,50),FVector(.3,3.1,2.5),TEXT("LaneBack"));
  Block(FVector(400,440,200),FVector(6,3.1,.3),TEXT("LaneRoof"));
  // Gate spans the entry of a walled covered lane, and the recess has a closed back.
  RoadLaneGates[i]=Gate(Yard(i,FVector(100,440,45)));
  if(RoadLaneGates[i].IsValid()){
    const FName Tag(*FString::Printf(TEXT("Road.%d.LaneGate"),i));
    RoadLaneGates[i]->Tags.AddUnique(Tag);
    StyleWorldProp(RoadLaneGates[i].Get(),Tag,FVector(.3,2.7,2.4));
  }
  Block(FVector(-100,900,50),FVector(6,.3,2.5),TEXT("RecessSouth"));
  Block(FVector(-100,1180,50),FVector(6,.3,2.5),TEXT("RecessNorth"));
  Block(FVector(-400,1040,50),FVector(.3,3.1,2.5),TEXT("RecessBack"));
  Block(FVector(-100,1040,200),FVector(6,3.1,.3),TEXT("RecessRoof"));
  RoadRecessGates[i]=Gate(Yard(i,FVector(200,1040,45)));
  if(RoadRecessGates[i].IsValid()){
    const FName Tag(*FString::Printf(TEXT("Road.%d.RecessGate"),i));
    RoadRecessGates[i]->Tags.AddUnique(Tag);
    StyleWorldProp(RoadRecessGates[i].Get(),Tag,FVector(.3,2.7,2.4));
  }
  RoadSigns[i]=Sign(Yard(i,FVector(0,-100,100)),FString(UTF8_TO_TCHAR(C.title))+TEXT(" / E - LOAD AFTER FIELDWORK"));
  for(int j=0;j<3;++j){Block(FVector(-150,300+j*350,-40),FVector(.6,.5,.5),TEXT("ClueStand"));Sign(Clue(i,j)+FVector(0,0,75),FString::Printf(TEXT("E / F - FIELD RECORD %d"),j+1));}
  Sign(Mechanism(i)+FVector(0,0,95),FString(UTF8_TO_TCHAR(C.mechanism))+TEXT(" / E calm or maintain / Q Fold / R-T review"));
  Sign(Receiving(i)+FVector(0,0,85),TEXT("E - RECEIVE A SEALED BUNDLE / DATED ARRIVAL"));
  Block(FVector(-300,-350,-40),FVector(1.3,.7,.5),TEXT("ReceivingShelf"));
  Sign(Yard(i,FVector(500,440,110)),TEXT("E - SERVICE-LANE CARGO CHECK"));
  Sign(Yard(i,FVector(100,1040,110)),TEXT("E - SHELTERED-RECESS CARGO CHECK"));
  if(auto* E=GetWorld()->SpawnActor<AUnmadeEnemyCharacter>(Keeper(i),FRotator::ZeroRotator)){
   E->ConfigureCreature(C.creature,true);E->ConfigureEncounter(FName(*FString::Printf(TEXT("Road.%d.Elder"),i)),UTF8_TO_TCHAR(C.elder),FLinearColor(.75f,.55f,.3f));E->ConfigureRoadElder(i);
  }
 }
 // Every stable person has a separate, open-front dwelling and inspectable threshold.
 for(int p=0;p<88;++p){const auto& P=UnmadeCore::RoadPeople[p];if(FName(UTF8_TO_TCHAR(P.id))==FName(TEXT("npc.cinderhold.courier.001")))continue;const FVector H(P.homeX,P.homeY,0);
  const auto HomeBlock=[&](FVector Offset,FVector Scale,const TCHAR* Part){SpawnBlock(H+Offset,Scale,FName(*FString::Printf(TEXT("Road.Home.%d.%s"),p,Part)));};
  HomeBlock(FVector(0,0,-17),FVector(6,5,.35),TEXT("Floor"));
  HomeBlock(FVector(-290,0,125),FVector(.2,5,2.5),TEXT("West"));HomeBlock(FVector(290,0,125),FVector(.2,5,2.5),TEXT("East"));
  HomeBlock(FVector(0,-240,125),FVector(6,.2,2.5),TEXT("Back"));HomeBlock(FVector(0,0,265),FVector(6,5,.3),TEXT("Roof"));
  HomeBlock(FVector(-140,-110,35),FVector(1.8,.8,.7),TEXT("Bench"));HomeBlock(FVector(140,-110,45),FVector(.8,.8,.9),TEXT("Table"));
  // Beds and hearths are geometric furnishings, never a claim of final asset production.
  HomeBlock(FVector(-160,85,25),FVector(1.8,.8,.5),TEXT("Bed"));HomeBlock(FVector(180,80,25),FVector(.65,.65,.5),TEXT("Hearth"));
  Sign(H+FVector(0,200,145),FString(UTF8_TO_TCHAR(P.name))+TEXT(" / E - remembered threshold"));
 }
 RefreshRememberedRoads();
}
void AUnmadePrototypeHub::RefreshRememberedRoads(){
 if(!GetWorld()||GetWorld()->GetTimeSeconds()<NextRoadRefreshAt)return;
 NextRoadRefreshAt=GetWorld()->GetTimeSeconds()+.5;
 const bool Day=Clock.Phase()!=UnmadeCore::DayPhase::Night;
 for(int i=0;i<11;++i){
  const auto Set=[](TWeakObjectPtr<AStaticMeshActor> G,bool Open){if(G.IsValid()){G->SetActorEnableCollision(!Open);G->SetActorHiddenInGame(Open);}};
  Set(RoadLaneGates[i],!bRoadSaveRejected&&RememberedRoads.LaneOpen(i,Day));Set(RoadRecessGates[i],!bRoadSaveRejected&&RememberedRoads.RecessOpen(i));
  if(RoadSigns[i].IsValid()){const auto* S=RememberedRoads.City(i);RoadSigns[i]->GetTextRender()->SetText(FText::FromString(FString::Printf(TEXT("%s / %d sealed units / %s\nE load; J journal. %s"),UTF8_TO_TCHAR(UnmadeCore::RoadCities[i].title),S->stock,*RouteText(i,S->route),Day?TEXT("Day work"):TEXT("Night: hand lane closed"))));}
 }
 const bool Carrying=RememberedRoads.Save().cargoCity>=0;
 auto* P=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
 if(Carrying&&Able(P)&&!RoadCargoVisual.IsValid()){
  auto* A=GetWorld()->SpawnActor<AStaticMeshActor>(P->GetActorLocation(),FRotator::ZeroRotator);
  if(A){auto* M=A->GetStaticMeshComponent();M->SetMobility(EComponentMobility::Movable);M->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));M->SetWorldScale3D(FVector(.45,.45,.35));A->SetActorEnableCollision(false);A->AttachToActor(P,FAttachmentTransformRules::KeepRelativeTransform);A->SetActorRelativeLocation(FVector(-45,0,15));
   const FName CargoTag(*FString::Printf(TEXT("Road.%d.Cargo"),RememberedRoads.Save().cargoCity));
   A->Tags.AddUnique(CargoTag);
   StyleWorldProp(A,CargoTag,FVector(.45,.45,.35));
   RoadCargoVisual=A;}
 }
 if(!Carrying&&RoadCargoVisual.IsValid()){RoadCargoVisual->Destroy();RoadCargoVisual.Reset();}
}
bool AUnmadePrototypeHub::TryRoadInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq){
 if(!GetWorld()||!Able(Player))return false;
 int City=-1,Kind=-1,Person=-1;double Best=FMath::Min(CompetingNpcDistanceSq,FMath::Square(240.0));
 const auto Candidate=[&](FVector At,int C,int K,int P=-1){const double D=FVector::DistSquared(Player->GetActorLocation(),At);if(D<=Best&&Sight(GetWorld(),Player,At,240)){Best=D;City=C;Kind=K;Person=P;}};
 for(int i=0;i<11;++i){for(int j=0;j<3;++j)Candidate(Clue(i,j),i,j);Candidate(Mechanism(i),i,3);Candidate(Yard(i),i,4);Candidate(Checkpoint(i,RememberedRoads.City(i)->route),i,5);Candidate(Receiving(i),i,6);}
 for(int p=0;p<88;++p){const auto& P=UnmadeCore::RoadPeople[p];Candidate(FVector(P.homeX,P.homeY+200,75),P.city,7,p);
  if(FName(UTF8_TO_TCHAR(P.id))!=FName(TEXT("npc.cinderhold.courier.001")))
  {
   Candidate(FVector(P.homeX-140,P.homeY-110,75),P.city,8,p);
   Candidate(FVector(P.homeX+140,P.homeY-110,75),P.city,9,p);
   Candidate(FVector(P.homeX-160,P.homeY+85,75),P.city,10,p);
   Candidate(FVector(P.homeX+180,P.homeY+80,75),P.city,11,p);
  }
 }
 if(Kind<0)return false;
 if(bRoadSaveRejected){Say(TEXT("Road save rejected; original file will not be overwritten."),FColor::Red);return true;}
 const auto Before=RememberedRoads;const int Day=GetGameDay();const double Now=GetWorld()->GetTimeSeconds();bool Changed=false;
 if(Kind<3){Changed=RememberedRoads.Inspect(City,Kind,Day);Say(UTF8_TO_TCHAR(UnmadeCore::RoadCities[City].clues[Kind]));}
 else if(Kind==3){
  if(!RememberedRoads.City(City)->resolution){
   auto* E=ActualKeeper(GetWorld(),City);
   if(RememberedRoads.Cooling(City,Now))Changed=RememberedRoads.Resolve(City,2,IsValid(E),IsValid(E)&&E->GetCombat()->IsDefeated(),Now);
   else if(RememberedRoads.Cool(City,Now))Say(TEXT("Mechanism quiet for 12 seconds (18 with Fold). E again to leave the actual elder alive."));
   else Say(TEXT("Inspect all three records first. You can retry the mechanism or fight the elder."));
  }else if(!RememberedRoads.City(City)->route){Changed=RememberedRoads.Maintain(City,Now);Say(FString::Printf(TEXT("Hand maintenance %d/3. Allow two seconds between turns; no Strain."),RememberedRoads.Turns(City)));}
  else Say(RouteText(City,RememberedRoads.City(City)->route));
 }
 else if(Kind==4){Changed=RememberedRoads.Pickup(City,Day);Say(Changed?TEXT("One sealed bundle loaded. Walk to your chosen lane's cargo check, then the named receiving shelf. Existing travel rules apply."):TEXT("Speak to the sender, inspect records, resolve the elder and choose a route. Only one cargo may be carried."));}
 else if(Kind==5){Changed=(RememberedRoads.City(City)->route==3?RememberedRoads.RecessOpen(City):RememberedRoads.LaneOpen(City,Clock.Phase()!=UnmadeCore::DayPhase::Night))&&RememberedRoads.Checkpoint(City,Day);Say(Changed?TEXT("Local route traversed; seal checked. Take this bundle to its named destination."):TEXT("This checkpoint needs the loaded bundle from this city."));}
 else if(Kind==6){Changed=RememberedRoads.Deliver(City,Day);Say(Changed?TEXT("The sealed bundle is received and dated. Return to the sender with its receiving account."):TEXT("No checked cargo for this destination. Consult J for its named receiving shelf."));}
 else if(Kind==7){
  AUnmadeNpcCharacter* Resident=nullptr;
  for(TActorIterator<AUnmadeNpcCharacter> N(GetWorld());N;++N)if(N->GetStableId()==FName(UTF8_TO_TCHAR(UnmadeCore::RoadPeople[Person].id))){Resident=*N;break;}
  const auto& Home=UnmadeCore::RoadPeople[Person];
  if(!Resident||!Sight(GetWorld(),Player,Resident->GetActorLocation(),450,Resident)||FVector::DistSquared2D(Resident->GetActorLocation(),FVector(Home.homeX,Home.homeY+160,100))>FMath::Square(300.f))
  {Say(TEXT("The resident is away. Their threshold remains; return when they are home, usually at night."));return true;}
  Changed=RememberedRoads.VisitHome(Person,Day);
  if(Person==UnmadeCore::RoadPersonIndex(UnmadeCore::RoadCities[City].sender))Changed=RememberedRoads.Return(City,Day)||Changed;
  Say(Changed?UTF8_TO_TCHAR(UnmadeCore::RoadPeople[Person].homeLine):TEXT("An ordinary threshold. A remembered home visit requires a personally told delivery account first."));
 }
 else {
  Changed=RememberedRoads.InspectFurniture(Person,Kind-8);
  const TCHAR* Detail=Kind==8?TEXT("The worn bench faces the household table."):
   Kind==9?TEXT("The work table holds a local concern: "):Kind==10?TEXT("The bed is set apart from the work table, ready for night."):
   TEXT("The hearth belongs to this household. Its supplies have a separate tally.");
  Say(FString(UTF8_TO_TCHAR(UnmadeCore::RoadPeople[Person].name))+TEXT(" - ")+Detail+(Kind==9?FString(UTF8_TO_TCHAR(UnmadeCore::RoadPeople[Person].concern)):FString()));
 }
 if(City==4&&RememberedRoads.City(City)->resolution&&!RememberedRoads.City(City)->route)Changed=(Cinderhold.Save().route==4?RememberedRoads.InheritCinderholdAccord(City,!bCinderholdSaveRejected):RememberedRoads.Choose(City,Cinderhold.Save().route))||Changed;
 if(Changed&&CommitRoads(Before)){
  if(Kind==7&&Person==UnmadeCore::RoadPersonIndex(UnmadeCore::RoadCities[City].sender)&&RememberedRoads.City(City)->returnDay)PlayRoadScene(City,true);
 }return true;
}
bool AUnmadePrototypeHub::TryRoadAbility(AUnmadeCharacter* Player,int32 Ability,int32 Choice){
 if(!GetWorld()||!Able(Player))return false;
 int City=-1,Record=-1;
 for(int i=0;i<11&&City<0;++i){if(Ability==0){for(int j=0;j<3;++j)if(Sight(GetWorld(),Player,Clue(i,j),240)){City=i;Record=j;break;}}else if(Sight(GetWorld(),Player,Mechanism(i),270))City=i;}
 if(City<0)return false;if(bRoadSaveRejected)return true;
 const auto Before=RememberedRoads;const double Now=GetWorld()->GetTimeSeconds();
 if(Ability==2){
  if(City==4){Say(TEXT("The First Cart Through keeps your saved Blackslag route. It cannot charge another Rewrite."));return true;}
  if(Choice<2||Choice>3||RememberedRoads.City(City)->route){Say(TEXT("A committed route cannot be rewritten again."));return true;}
  if(PendingRoadCity!=City||PendingRoadChoice!=Choice||Now>PendingRoadExpiresAt){PendingRoadCity=City;PendingRoadChoice=Choice;PendingRoadExpiresAt=Now+6;Say(TEXT("REWRITE costs 60 Strain permanently: ")+RouteText(City,Choice)+TEXT(". Repeat the same key here within six seconds. E maintenance is free."));return true;}
  PendingRoadCity=-1;PendingRoadChoice=0;
  if(!RememberedRoads.Choose(City,Choice)){Say(TEXT("Briefing, all field records and an elder outcome are needed first."));return true;}
  if(!Player->SpendRealityStrain(60)){RememberedRoads=Before;Say(TEXT("Not enough stability. E hand maintenance remains available."));return true;}
  if(!CommitRoads(Before,Player))Player->RefundRealityStrain(60);return true;
 }
 bool Applied=Ability==0?RememberedRoads.Glimpse(City,Now):RememberedRoads.Fold(City,Now);
 if(!Applied){Say(TEXT("Local ability window already active; no Strain charged."));return true;}
 const int Cost=Ability==0?8:24;if(!Player->SpendRealityStrain(Cost)){RememberedRoads=Before;Say(TEXT("Recover stability or use the physical alternative."));return true;}
 if(Ability==0)RememberedRoads.Inspect(City,Record,GetGameDay());
 if(City==4&&RememberedRoads.City(City)->resolution&&!RememberedRoads.City(City)->route)(Cinderhold.Save().route==4?RememberedRoads.InheritCinderholdAccord(City,!bCinderholdSaveRejected):RememberedRoads.Choose(City,Cinderhold.Save().route));
 if(!CommitRoads(Before,Player)){Player->RefundRealityStrain(Cost);return true;}
 if(Ability==0)Say(UTF8_TO_TCHAR(UnmadeCore::RoadCities[City].clues[Record]));
 else {if(auto* E=ActualKeeper(GetWorld(),City))E->ExposeToFold(Now,6);Say(TEXT("Fold interrupts this elder and extends active cooling to 18 seconds. E remains sufficient."));}
 return true;
}
bool AUnmadePrototypeHub::RecordRoadConversation(FName StableId){
 if(!GetWorld()||bRoadSaveRejected)return false;
 const FString Id=StableId.ToString();const int Person=UnmadeCore::RoadPersonIndex(TCHAR_TO_UTF8(*Id));if(Person<0)return false;
 auto* P=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));AUnmadeNpcCharacter* Actual=nullptr;
 for(TActorIterator<AUnmadeNpcCharacter> N(GetWorld());N;++N)if(N->GetStableId()==StableId){Actual=*N;break;}
 if(!Actual||!Sight(GetWorld(),P,Actual->GetActorLocation(),260,Actual))return false;
 const auto Before=RememberedRoads;const int City=UnmadeCore::RoadPeople[Person].city,Day=GetGameDay();bool Changed=false,Brief=false;
 if(StableId==FName(UTF8_TO_TCHAR(UnmadeCore::RoadCities[City].sender))){
  if(City==4&&!Cinderhold.Has(UnmadeCore::ReturnedHome)){Say(TEXT("Finish Bes's Blackslag return first. This new cargo is separate from his unpaid wages."));return false;}
  Brief=RememberedRoads.Brief(City,Day);Changed=Brief;
  Changed=RememberedRoads.Report(City,Day)||Changed;
 }
 if(City==4&&RememberedRoads.City(City)->resolution&&!RememberedRoads.City(City)->route)Changed=(Cinderhold.Save().route==4?RememberedRoads.InheritCinderholdAccord(City,!bCinderholdSaveRejected):RememberedRoads.Choose(City,Cinderhold.Save().route))||Changed;
 for(int i=0;i<11;++i)Changed=RememberedRoads.Tell(Person,i,Day)||Changed;
 if(Changed&&CommitRoads(Before)){if(Brief){Say(UTF8_TO_TCHAR(UnmadeCore::RoadCities[City].brief));PlayRoadScene(City,false);}return true;}return false;
}
FString AUnmadePrototypeHub::GetRoadResidentLine(FName StableId)const{
 if(bRoadSaveRejected)return TEXT("\nThe delivery record is unavailable; no replacement history is claimed.");
 const FString Id=StableId.ToString();const int P=UnmadeCore::RoadPersonIndex(TCHAR_TO_UTF8(*Id));if(P<0)return FString();
 const auto* M=RememberedRoads.Person(P);const auto& Person=UnmadeCore::RoadPeople[P];
 if(!M->knowledge)return FString(TEXT("\n"))+UTF8_TO_TCHAR(Person.concern);
 int Latest=-1;for(int i=0;i<11;++i)if(M->knowledge&(1<<i))if(Latest<0||RememberedRoads.City(i)->arrivalDay>RememberedRoads.City(Latest)->arrivalDay)Latest=i;
 const auto* S=RememberedRoads.City(Latest);FString Line=FString::Printf(TEXT("\nYou reported %s received at %s on day %d. "),UTF8_TO_TCHAR(UnmadeCore::RoadCities[Latest].cargo),UTF8_TO_TCHAR(UnmadeCore::RoadCities[UnmadeCore::RoadCities[Latest].destination].id),S->arrivalDay);
 Line+=UTF8_TO_TCHAR(Person.preference==S->route?Person.agrees:Person.disagrees);
 if(M->homeDay)Line+=FString::Printf(TEXT(" Our threshold visit on day %d remains remembered."),M->homeDay);
 return Line;
}
FString AUnmadePrototypeHub::GetRoadJournal()const{
 FString Line=TEXT("REMEMBERED ROADS - OPTIONAL / 11 LOCAL ADVENTURES\n");
 for(int i=0;i<11;++i){const auto* S=RememberedRoads.City(i);if(S->briefDay||S->clues||S->resolution)Line+=FString::Printf(TEXT("%s: %d/3 records; %s; %s; receipt day %d; home day %d\n"),UTF8_TO_TCHAR(UnmadeCore::RoadCities[i].id),((S->clues&1)?1:0)+((S->clues&2)?1:0)+((S->clues&4)?1:0),S->resolution==1?TEXT("elder defeated"):S->resolution==2?TEXT("elder alive, quieted"):TEXT("elder unresolved"),*RouteText(i,S->route),S->arrivalDay,S->returnDay);}
 const auto S=RememberedRoads.Save();if(S.cargoCity>=0)Line+=FString::Printf(TEXT("CARRYING: %s from %s to %s; local seal check %s.\n"),UTF8_TO_TCHAR(UnmadeCore::RoadCities[S.cargoCity].cargo),UTF8_TO_TCHAR(UnmadeCore::RoadCities[S.cargoCity].id),UTF8_TO_TCHAR(UnmadeCore::RoadCities[UnmadeCore::RoadCities[S.cargoCity].destination].id),S.cargoStep==2?TEXT("done"):TEXT("needed"));
 return Line+TEXT("E physical actions; F local records; Q Fold; R/T warned route choices. Existing realm travel remains authoritative.");
}
bool AUnmadePrototypeHub::RoadRoutineTarget(FName StableId,bool bNight,FVector& Target)const{
 const FString Id=StableId.ToString();const int P=UnmadeCore::RoadPersonIndex(TCHAR_TO_UTF8(*Id));if(P<0)return false;
 const auto& Person=UnmadeCore::RoadPeople[P];
 // Dedicated homes are actual local positions. Urgent material/hazard duties override at caller.
 if(bNight){Target=FVector(Person.homeX,Person.homeY+160,100);return true;}return false;
}
int32 AUnmadePrototypeHub::GetRoadElderResolution(int32 City)const{const auto* S=RememberedRoads.City(City);return !bRoadSaveRejected&&S?S->resolution:0;}
bool AUnmadePrototypeHub::IsRoadCooling(int32 City)const{return !bRoadSaveRejected&&GetWorld()&&RememberedRoads.Cooling(City,GetWorld()->GetTimeSeconds());}
bool AUnmadePrototypeHub::RecordRoadElderDefeat(AUnmadeEnemyCharacter* Elder,int32 City){
 if(!GetWorld()||bRoadSaveRejected||!RememberedRoads.City(City)||!IsValid(Elder)||!Elder->ActorHasTag(FName(*FString::Printf(TEXT("Road.%d.Elder"),City)))||!IsValid(Elder->GetCombat())||!Elder->GetCombat()->IsDefeated())return false;
 if(RememberedRoads.City(City)->resolution==1)return true;const auto Before=RememberedRoads;
 if(!RememberedRoads.Resolve(City,1,true,true,GetWorld()->GetTimeSeconds()))return false;
 if(City==4)(Cinderhold.Save().route==4?RememberedRoads.InheritCinderholdAccord(City,!bCinderholdSaveRejected):RememberedRoads.Choose(City,Cinderhold.Save().route));
 return CommitRoads(Before);
}
