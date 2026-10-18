#include "World/UnmadePrototypeHub.h"
#include "Save/UnmadeRecoveryRules.h"
#include "World/UnmadeIdentityRegistry.h"
#include "World/UnmadeAdventureLayout.h"
#include "Player/UnmadeCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Combat/UnmadePresentation.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
namespace {
FVector Base(int a){auto& s=UnmadeCore::ExpeditionSpecs[a];return FVector(s.x,s.y,95);}
FVector Puzzle(int a,int n){auto p=UnmadeCore::DungeonRoom(a,2);return Base(a)+FVector(p.x+(n-1)*430,15000+p.y+700,0);}
FVector RepairPost(int a,int n){return Base(a)+FVector(3700,8500+n*3200,0);}
FVector Service(int a){return Base(a)+FVector(-900,8500,0);}
bool Near(AUnmadeCharacter* p,FVector at){if(!IsValid(p)||!p->GetCombat()||p->GetCombat()->IsDefeated()||FVector::DistSquared(p->GetActorLocation(),at)>FMath::Square(230.f))return false;FCollisionQueryParams q(SCENE_QUERY_STAT(IdentityInteraction),false);q.AddIgnoredActor(p);return !p->GetWorld()->LineTraceTestByChannel(p->GetActorLocation()+FVector(0,0,35),at+FVector(0,0,65),ECC_Visibility,q);}

}
void AUnmadePrototypeHub::BuildIdentity(){
 if(!GetWorld())return;IdentityPuzzleSigns.SetNum(9);IdentityRepairs.SetNum(27);IdentityPatrols.SetNum(9);
 auto Sign=[&](FVector at,const FString& line){auto* t=GetWorld()->SpawnActor<ATextRenderActor>(at+FVector(0,0,130),FRotator(0,90,0));if(t){t->GetTextRender()->SetWorldSize(18);t->GetTextRender()->SetText(FText::FromString(line));}return t;};
 for(int a=0;a<9;++a){auto& s=UnmadeCore::IdentitySpecs[a];IdentityPuzzleSigns[a]=Sign(Puzzle(a,1)+FVector(0,0,150),UTF8_TO_TCHAR(s.mechanism));
  for(int n=0;n<3;++n){Sign(Puzzle(a,n),FString::Printf(TEXT("E - VALVE %d / TOGGLES MASK %d (1,2,4 = LIGHTS)"),n+1,UnmadeCore::WorldIdentity::Coupling(a,n)));SpawnBlock(Puzzle(a,n)+FVector(0,0,-55),FVector(.8,.8,.7),FName(TEXT("IdentityValve")));
   auto* b=GetWorld()->SpawnActor<AStaticMeshActor>(RepairPost(a,n)+FVector(0,0,-35),FRotator::ZeroRotator);if(b){auto* m=b->GetStaticMeshComponent();m->SetMobility(EComponentMobility::Movable);m->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));m->SetWorldScale3D(FVector(.3,2,.4));m->SetCollisionEnabled(ECollisionEnabled::NoCollision);IdentityRepairs[a*3+n]=b;}Sign(RepairPost(a,n),TEXT("E - REPAIR WAYPOST AFTER WITNESSED RETURN"));
  }
  // The full morally grounded tasks are in interaction feedback and J;
  // long quest paragraphs on a world-space sign are not legible or helpful.
  Sign(Service(a),TEXT("LOCAL FIELDWORK\
E - CLAIM EARNED HUNT\
F - SURVEY RECORDS\
AFTER RETURN - 3 WAYPOSTS\
J - PURPOSE AND LOCAL CONSEQUENCES"));
  for(TActorIterator<AUnmadeNpcCharacter> npc(GetWorld());npc;++npc)if(npc->GetStableId()==FName(*FString::Printf(TEXT("npc.expedition.%d.3"),a)))IdentityPatrols[a]=*npc;

  // Different landmark silhouettes beyond the usable chamber edges; no invented nav shortcuts.
  for(int n=0;n<3+a%4;++n){FVector at=Base(a)+FVector(-2200-n*150,16000+n*600,0);auto* landmark=GetWorld()->SpawnActor<AStaticMeshActor>(at,FRotator(0,a*17,n*9));if(landmark){auto* m=landmark->GetStaticMeshComponent();m->SetMobility(EComponentMobility::Movable);m->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,a%3==0?TEXT("/Engine/BasicShapes/Cylinder.Cylinder"):a%3==1?TEXT("/Engine/BasicShapes/Cone.Cone"):TEXT("/Engine/BasicShapes/Cube.Cube")));m->SetWorldScale3D(FVector(1+a*.12,1+a*.12,3+n*.7));if(auto* mat=m->CreateAndSetMaterialInstanceDynamic(0))mat->SetVectorParameterValue(TEXT("Color"),FLinearColor(.18+a*.06,.35-a*.015,.6-a*.045));}}
  FString name=TEXT("SFX_ID_ambience_")+FString(UTF8_TO_TCHAR(s.ambience));FString path=TEXT("/Game/UnmadeProduction/Audio/Identity/")+name+TEXT(".")+name;
  if(auto* sound=LoadObject<USoundBase>(nullptr,*path,nullptr,LOAD_NoWarn)){auto* audio=NewObject<UAudioComponent>(this);audio->RegisterComponent();audio->SetWorldLocation(Service(a));audio->SetSound(sound);audio->bAllowSpatialization=true;audio->bOverrideAttenuation=true;audio->AttenuationOverrides.bAttenuate=true;audio->AttenuationOverrides.AttenuationShape=EAttenuationShape::Sphere;audio->AttenuationOverrides.AttenuationShapeExtents=FVector(2500,0,0);audio->AttenuationOverrides.FalloffDistance=5000;audio->SetVolumeMultiplier(.18f);audio->Play();}
 }
 RefreshIdentity();
}
void AUnmadePrototypeHub::RefreshIdentity(){
 if(IdentityRepairs.Num()!=27)return;
 for(int a=0;a<9;++a){auto* t=Identity.Arc(a);auto* e=Expeditions.Arc(a);
  if(IdentityPuzzleSigns[a].IsValid())IdentityPuzzleSigns[a]->GetTextRender()->SetText(FText::FromString(FString::Printf(TEXT("%s\n%s\nLIGHTS %d%d%d / TARGET %d%d%d\n%s"),UTF8_TO_TCHAR(UnmadeCore::IdentitySpecs[a].mechanism),UTF8_TO_TCHAR(UnmadeCore::IdentitySpecs[a].hint),t->puzzle&1?1:0,t->puzzle&2?1:0,t->puzzle&4?1:0,UnmadeCore::WorldIdentity::Target(a)&1?1:0,UnmadeCore::WorldIdentity::Target(a)&2?1:0,UnmadeCore::WorldIdentity::Target(a)&4?1:0,UTF8_TO_TCHAR(UnmadeCore::IdentitySpecs[a].survey_task)));
  for(int n=0;n<3;++n)if(IdentityRepairs[a*3+n].IsValid()){auto* p=IdentityRepairs[a*3+n].Get();p->SetActorRotation(FRotator(0,0,t->repair&(1<<n)?0:65));p->SetActorScale3D(t->repair&(1<<n)?FVector(.3,2,2):e->boss==1?FVector(.3,2,.2):FVector(.3,2,.4));}
  if(IdentityPatrols[a].IsValid()&&e->returned&&t->repair==7){auto* npc=IdentityPatrols[a].Get();FVector at=Base(a)+FVector(3700,10000,0);if(FVector::DistSquared2D(npc->GetActorLocation(),at)>FMath::Square(1600.f)){npc->SetActorLocation(at,false);npc->SetExpeditionHome(at);}}
 }
}
bool AUnmadePrototypeHub::TryIdentityInteraction(AUnmadeCharacter* p,double competing,bool Survey){
 int arc=-1,kind=-1,index=0;double best=FMath::Min(230.*230.,competing);
 auto Add=[&](int a,int k,int n,FVector at){double d=FVector::DistSquared(p->GetActorLocation(),at);if(d<best&&Near(p,at)){best=d;arc=a;kind=k;index=n;}};
 for(int a=0;a<9;++a){Add(a,2,0,Service(a));if(!Survey)for(int n=0;n<3;++n){Add(a,0,n,Puzzle(a,n));Add(a,1,n,RepairPost(a,n));}}
 if(arc<0)return false;if(bIdentitySaveRejected||bExpeditionSaveRejected){NotifyRpg(TEXT("Saved world identity rejected; original protected."));return true;}auto before=Identity;auto* e=Expeditions.Arc(arc);bool changed=false,heal=false;const TCHAR* cue=TEXT("inspect");FString line;
 if(kind==0){if(e->rooms&4)changed=Identity.Operate(arc,index);line=TEXT("Coupled valves change the displayed lights. Match the target; inspect the neighboring valves. A solved mechanism stays solved.");}
 if(kind==1){changed=Identity.Repair(arc,index,e->returned!=0);cue=TEXT("repair");line=changed?TEXT("Waypost repaired. Repair all three roadside posts to open sanctuary recovery and bring the roadkeeper back."):TEXT("Return must be witnessed before repairs. This post may already be sound.");}
 if(kind==2){int kills=0;for(int k=0;k<8;++k)kills+=(e->kills>>k)&1;
  if(Survey){changed=Identity.Chart(arc,e->clues);line=changed?TEXT("Survey map earned. Local dungeon danger grounded permanently."):TEXT("F survey requires three records and the solved valve mechanism. Map may already be owned.");cue=TEXT("reward");}
  else if(!Identity.Arc(arc)->hunt&&kills>=4){if(p->GetCombat()->GetMissingHealth()>0){changed=Identity.Hunt(arc,kills);heal=changed;}line=TEXT("Hunt: defeat four actual dungeon guards. One recovery ration restores up to 35 health; claim while injured.");cue=TEXT("reward");}
  else{if(p->GetCombat()->GetMissingHealth()>0){changed=Identity.Rest(arc,GetGameDay(),e->returned!=0);heal=changed;}line=TEXT("Sanctuary: all three roadposts repaired after return. Up to 35 health once per game day; no reward consumed at full health.");cue=TEXT("release");}
 }
 // Use the same earned-state activities but tell the traveler what this realm asks.
 if(kind==1)line=FString(UTF8_TO_TCHAR(UnmadeCore::IdentitySpecs[arc].repair_task))+TEXT("\n")+line;
 if(kind==2){
  int defeated=0;for(int k=0;k<8;++k)defeated+=(e->kills>>k)&1;
  const char* task=Survey?UnmadeCore::IdentitySpecs[arc].survey_task:
    (!before.Arc(arc)->hunt&&defeated>=4?UnmadeCore::IdentitySpecs[arc].hunt_task:UnmadeCore::IdentitySpecs[arc].repair_task);
  line=FString(UTF8_TO_TCHAR(task))+TEXT("\n")+line;
 }
 if(changed&&!UnmadeCore::CommitHealing(*p->GetCombat(),heal?35:0,[&]{return WriteWorldSnapshot(p);})){Identity=before;NotifyRpg(TEXT("Save failed. Quest, service and reward rolled back; retry."));return true;}
 if(changed){UnmadePresentation::Sound(p,cue);p->PlayIdentityMotion(FName(kind==1?TEXT("Repair"):TEXT("Inspect")));RefreshIdentity();RefreshExpeditions();}
 NotifyRpg(line);return true;
}
