#include "UI/UnmadeRpgFeedback.h"
#include "World/UnmadePrototypeHub.h"
#include "Player/UnmadeCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
namespace {
const FVector SlagClues[3]={FVector(-3350,51550,75),FVector(-3350,52000,75),FVector(-3350,52450,75)};
const FVector SlagVent(-2900,52600,75),SlagCadence(-2900,52900,75),HomeShelf(1370,50520,75);
const TCHAR* ClueText[3]={
    TEXT("Tracks turn back toward the nest. The elder defends Blackslag; it does not hunt the town."),
    TEXT("Varda's service log: two turns cool the vent. The kiln lane draws the plume; the archive lane must stay sheltered."),
    TEXT("The cadence plate reads: two short knocks, a pause, one low knock. Cool the vent first, then sound it at the nest.")};
void Notice(const FString& Text,FColor Color=FColor::Cyan) {
    if(GEngine)UnmadeRpgFeedback(-1,8.f,Color,Text);
}
void SlagSound(UWorld* World,const TCHAR* Cue,FVector At){
    const FString Name=FString(TEXT("SFX_CH_"))+Cue;
    const FString Path=FString::Printf(TEXT("/Game/UnmadeProduction/Audio/Cinderhold/%s.%s"),*Name,*Name);
    if(USoundBase* Sound=LoadObject<USoundBase>(nullptr,*Path,nullptr,LOAD_NoWarn))
        UGameplayStatics::PlaySoundAtLocation(World,Sound,At,.65f);
}
bool Able(AUnmadeCharacter* P){return IsValid(P)&&IsValid(P->GetCombat())&&!P->GetCombat()->IsDefeated();}
bool LocalSight(UWorld* World,AUnmadeCharacter* Player,FVector Target,double Limit) {
    if(!World||!Able(Player)||FVector::DistSquared(Player->GetActorLocation(),Target)>FMath::Square(Limit))return false;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeSlagInteraction),false);
    Params.AddIgnoredActor(Player);
    return !World->LineTraceTestByChannel(Player->GetActorLocation()+FVector(0,0,35),Target+FVector(0,0,35),ECC_Visibility,Params);
}
const TCHAR* RouteName(int Route){return Route==1?TEXT("hand-maintained, daylight kiln path"):
    Route==2?TEXT("permanently open vent; kiln path open, archive sealed"):
    Route==3?TEXT("permanently sealed vent; archive open, kiln path shut"):TEXT("route undecided");}
}

bool AUnmadePrototypeHub::CommitCinderhold(const UnmadeCore::CinderholdAdventure& Before,AUnmadeCharacter* StrainOwner)
{
    if(WriteWorldSnapshot(StrainOwner)){RefreshCinderholdAdventure();return true;}
    Cinderhold=Before;
    Notice(TEXT("Cinderhold could not save. The adventure has not changed; retry this action."),FColor::Red);
    return false;
}

void AUnmadePrototypeHub::BuildCinderholdAdventure()
{
    if(!GetWorld())return;
    SpawnBlock(FVector(-3350,52100,-17),FVector(4,20,.35),TEXT("Cinderhold.Adventure.WestWalk"));
    SpawnBlock(FVector(-3170,52120,125),FVector(.25,12,2.5),TEXT("Cinderhold.Adventure.WindScreen"));
    SpawnBlock(FVector(-3100,52780,-17),FVector(6,6,.35),TEXT("Cinderhold.Adventure.VentApron"));
    SpawnBlock(FVector(-2100,52800,-17),FVector(11,12,.35),TEXT("Cinderhold.Adventure.NestFloor"));
    SpawnBlock(FVector(-2550,52450,-17),FVector(9,2,.35),TEXT("Cinderhold.Adventure.KilnPath"));
    SpawnBlock(FVector(-3400,52900,-17),FVector(7,2,.35),TEXT("Cinderhold.Adventure.ArchivePath"));
    const auto MakeGate=[&](FVector At,FName Tag){
        auto* Gate=GetWorld()->SpawnActor<AStaticMeshActor>(At,FRotator::ZeroRotator);
        if(Gate){Gate->Tags.AddUnique(Tag);auto* Mesh=Gate->GetStaticMeshComponent();
            Mesh->SetMobility(EComponentMobility::Movable);
            Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));
            Mesh->SetWorldScale3D(FVector(.35,2.4,2.4));}
        return Gate;
    };
    // Narrow covered doorways have enclosing walls; these gates are actual
    // traversability changes, rather than a decorative block on an open plane.
    SpawnBlock(FVector(-3600,52780,125),FVector(3.4,.35,2.5),TEXT("Cinderhold.Adventure.ArchiveSouth"));
    SpawnBlock(FVector(-3600,53020,125),FVector(3.4,.35,2.5),TEXT("Cinderhold.Adventure.ArchiveNorth"));
    SpawnBlock(FVector(-3770,52900,125),FVector(.35,2.7,2.5),TEXT("Cinderhold.Adventure.ArchiveBack"));
    SpawnBlock(FVector(-3600,52900,265),FVector(3.7,2.7,.3),TEXT("Cinderhold.Adventure.ArchiveRoof"));
    SpawnBlock(FVector(-2640,52330,125),FVector(3.6,.35,2.5),TEXT("Cinderhold.Adventure.KilnSouth"));
    SpawnBlock(FVector(-2640,52570,125),FVector(3.6,.35,2.5),TEXT("Cinderhold.Adventure.KilnNorth"));
    SpawnBlock(FVector(-2640,52450,265),FVector(3.7,2.7,.3),TEXT("Cinderhold.Adventure.KilnRoof"));
    CinderholdKilnGate=MakeGate(FVector(-2800,52450,120),TEXT("Cinderhold.Adventure.KilnGate"));
    CinderholdArchiveGate=MakeGate(FVector(-3450,52900,120),TEXT("Cinderhold.Adventure.ArchiveGate"));
    const auto Sign=[&](FVector At,const TCHAR* Text){
        auto* S=GetWorld()->SpawnActor<ATextRenderActor>(At,FRotator(0,90,0));
        if(S){S->GetTextRender()->SetWorldSize(18.f);S->GetTextRender()->SetText(FText::FromString(Text));}
        return S;
    };
    const TCHAR* Labels[3]={TEXT("E / F - NEST TRACKS"),TEXT("E / F - VARDA'S SERVICE LOG"),TEXT("E / F - CADENCE PLATE")};
    for(int i=0;i<3;++i){
        SpawnBlock(SlagClues[i]-FVector(0,0,40),FVector(.6,.45,.5),FName(*FString::Printf(TEXT("Cinderhold.Adventure.Clue.%d"),i)));
        Sign(SlagClues[i]+FVector(0,0,55),Labels[i]);
    }
    SpawnBlock(SlagVent-FVector(0,0,40),FVector(.7,.7,.5),TEXT("Cinderhold.Adventure.VentStand"));
    if(auto* Wheel=GetWorld()->SpawnActor<AStaticMeshActor>(SlagVent+FVector(0,0,40),FRotator(90,0,0)))
    {
        Wheel->Tags.AddUnique(FName(TEXT("Cinderhold.Adventure.VentWheel")));
        auto* Mesh=Wheel->GetStaticMeshComponent();Mesh->SetMobility(EComponentMobility::Movable);
        Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cylinder.Cylinder")));
        Mesh->SetWorldScale3D(FVector(.5,.5,.12));Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        CinderholdVentWheel=Wheel;
    }
    CinderholdVentSign=Sign(SlagVent+FVector(0,0,100),TEXT("E - COOL VENT | Q - FOLD | R/T - REVIEW REWRITE"));
    Sign(SlagCadence+FVector(0,0,80),TEXT("E - SOUND THE LEARNED CADENCE WHILE VENT IS COOL"));
    Sign(FVector(-3300,51130,170),TEXT("BLACKSLAG FIELDWORK - COVERED WEST PATH / OPTIONAL"));
    Sign(FVector(-3680,52900,100),TEXT("ARCHIVE ALCOVE - OLD SERVICE COPIES KEPT DRY"));
    SpawnBlock(FVector(-3670,52900,45),FVector(.8,.6,.9),TEXT("Cinderhold.Adventure.ServiceCopies"));
    CinderholdHomeSign=Sign(HomeShelf+FVector(0,0,90),TEXT("E - INSPECT BES'S RETURN SHELF"));
}

void AUnmadePrototypeHub::RefreshCinderholdAdventure()
{
    if(!GetWorld()||GetWorld()->GetTimeSeconds()<NextCinderholdRefreshAt)return;
    NextCinderholdRefreshAt=GetWorld()->GetTimeSeconds()+.25;
    if(CinderholdVentWheel.IsValid())CinderholdVentWheel->SetActorRotation(FRotator(
        90,0,Cinderhold.VentActive(GetWorld()->GetTimeSeconds())?95.f:
        Cinderhold.Save().route?Cinderhold.Save().route*45.f:CinderholdMaintenanceTurns*30.f));
    const bool Daylight=Clock.Phase()!=UnmadeCore::DayPhase::Night;
    const auto SetGate=[](TWeakObjectPtr<AStaticMeshActor> Gate,bool Open){
        if(Gate.IsValid()){Gate->SetActorEnableCollision(!Open);Gate->SetActorHiddenInGame(Open);}
    };
    SetGate(CinderholdKilnGate,!bCinderholdSaveRejected&&Cinderhold.KilnPathOpen(Daylight));
    SetGate(CinderholdArchiveGate,!bCinderholdSaveRejected&&Cinderhold.ArchivePathOpen());
    if(CinderholdVentSign.IsValid()){
        const auto S=Cinderhold.Save();
        FString Text=bCinderholdSaveRejected?TEXT("ADVENTURE SAVE UNAVAILABLE - ORIGINAL PRESERVED"):
            S.route?FString(RouteName(S.route)):
            Cinderhold.Resolved()?TEXT("E - HAND MAINTENANCE (3 TURNS) | R/T - PERMANENT ROUTE PREVIEW"):
            Cinderhold.VentActive(GetWorld()->GetTimeSeconds())?TEXT("VENT COOL - E AT CADENCE POST TO QUIET ELDER"):
            TEXT("E - COOL VENT (LEARN TRACKS + LOG) | Q - FOLD EXTENDS COOL WINDOW");
        CinderholdVentSign->GetTextRender()->SetText(FText::FromString(Text));
    }
    if(CinderholdHomeSign.IsValid())CinderholdHomeSign->GetTextRender()->SetText(FText::FromString(
        Cinderhold.Has(UnmadeCore::ReturnedHome)?TEXT("YOUR BLACKSLAG RETURN IS REMEMBERED - E FOR HOUSEHOLD"):
        TEXT("E - INSPECT RETURN SHELF / RETURN HOME AFTER TELLING BES")));
}

bool AUnmadePrototypeHub::TryCinderholdInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq)
{
    if(!Able(Player)||!GetWorld())return false;
    const FVector Targets[6]={SlagClues[0],SlagClues[1],SlagClues[2],SlagVent,SlagCadence,HomeShelf};
    int Closest=-1;double Distance=FMath::Min(CompetingNpcDistanceSq,FMath::Square(240.0));
    for(int i=0;i<6;++i){
        // Completed shelf no longer consumes household relief/pay interactions.
        if(i==5&&Cinderhold.Has(UnmadeCore::ReturnedHome))continue;
        const double D=FVector::DistSquared(Player->GetActorLocation(),Targets[i]);
        if(D<=Distance&&LocalSight(GetWorld(),Player,Targets[i],240)){Closest=i;Distance=D;}
    }
    if(Closest<0)return false;
    if(bCinderholdSaveRejected){Notice(TEXT("Adventure save rejected; it will not be overwritten."),FColor::Red);return true;}
    const auto Before=Cinderhold;
    bool Changed=false;
    const double Now=GetWorld()->GetTimeSeconds();
    if(Closest<3){
        Changed=Cinderhold.Inspect(static_cast<UnmadeCore::SlagClue>(1<<Closest),GetGameDay());
        if(!Changed||CommitCinderhold(Before))Notice(ClueText[Closest]);
    }else if(Closest==3){
        if(!Cinderhold.Resolved()){
            if(Cinderhold.OpenVent(Now,Cinderhold.FoldActive(Now))){SlagSound(GetWorld(),TEXT("vent_cool"),SlagVent);Notice(TEXT("Vent cooled. Sound the learned cadence at the nest post within 12 seconds (18 with Fold). The elder lowers its guard while the vent is cool."));}
            else Notice(TEXT("Read the nest tracks and Varda's service log before operating the vent."));
        }else if(Cinderhold.Save().route!=0)Notice(RouteName(Cinderhold.Save().route));
        else if(Now<CinderholdNextMaintenanceAt)Notice(TEXT("Let the wheel settle for two seconds before the next turn."));
        else {
            CinderholdNextMaintenanceAt=Now+2;
            CinderholdMaintenanceTurns=FMath::Min(3,CinderholdMaintenanceTurns+1);
            if(CinderholdMaintenanceTurns==3){
                Changed=Cinderhold.Choose(UnmadeCore::SlagRoute::Maintained);
                if(Changed&&CommitCinderhold(Before))Notice(TEXT("Hand maintenance recorded. Archive open; kiln path opens by daylight and shuts at night. No Strain spent, no wages paid."));
                else CinderholdMaintenanceTurns=2;
            }else Notice(FString::Printf(TEXT("Maintenance turn %d/3. This keeps both services intact, but the kiln path will still close at night."),CinderholdMaintenanceTurns));
        }
    }else if(Closest==4){
        if(Cinderhold.Resolved())Notice(TEXT("The nest is quiet. Its outcome is already recorded."));
        else {
            bool bLivingElder=false;
            for(TActorIterator<AUnmadeEnemyCharacter> Elder(GetWorld());Elder;++Elder)
                if(Elder->ActorHasTag(FName(TEXT("Cinderhold.Slag.Creature.2")))&&
                   IsValid(Elder->GetCombat())&&!Elder->GetCombat()->IsDefeated())bLivingElder=true;
            if(!bLivingElder){Notice(TEXT("No living elder is present to answer. A pending defeat receipt must save before fieldwork can continue."));return true;}
            if(Cinderhold.Quiet(Now,GetGameDay())){
                if(CommitCinderhold(Before)){SlagSound(GetWorld(),TEXT("cadence"),SlagCadence);Notice(TEXT("Two short knocks. A pause. One low knock. The elder settles beside the cooled vent; it is alive. Decide the route, then tell Varda."));}
            }else Notice(TEXT("The elder will not answer an invented signal. Read all three clues, cool the vent, then return here before it reheats."));
        }
    }else{
        const auto House=BesHousehold.Save();
        const auto* Worker=LivingTrades.Worker(UnmadeCore::Realm::HearthBeneath);
        if(bBesHouseholdRejected||bLivingEconomyRejected||!Worker){Notice(TEXT("Household evidence is unavailable; return cannot be recorded."),FColor::Red);return true;}
        Changed=Cinderhold.Remember(UnmadeCore::HomeSeen,GetGameDay());
        const bool Returned=Cinderhold.ReturnHome(GetGameDay(),House.heatBaskets,House.daysCold,Worker->owedMarks);
        if((Changed||Returned)&&!CommitCinderhold(Before))return true;
        if(Returned)PlayCinderholdScene(3);
        else Notice(FString::Printf(TEXT("Bes's return shelf: %d real heat baskets, %d remembered cold days, %d marks still owed. Hear Varda, resolve Blackslag, report to her, then tell Bes and return here."),House.heatBaskets,House.daysCold,Worker->owedMarks));
    }
    NextCinderholdRefreshAt=0;RefreshCinderholdAdventure();return true;
}

bool AUnmadePrototypeHub::TryCinderholdAbility(AUnmadeCharacter* Player,int32 Ability,int32 Choice)
{
    if(!Able(Player)||!GetWorld()||Ability<0||Ability>2)return false;
    int Clue=-1;double Best=FMath::Square(240.f);
    if(Ability==0)for(int i=0;i<3;++i){
        const double D=FVector::DistSquared(Player->GetActorLocation(),SlagClues[i]);
        if(D<=Best&&LocalSight(GetWorld(),Player,SlagClues[i],240)){Best=D;Clue=i;}
    }
    if(Ability==0&&Clue<0)return false;
    if(Ability!=0&&!LocalSight(GetWorld(),Player,SlagVent,280))return false;
    if(bCinderholdSaveRejected){Notice(TEXT("Adventure save unavailable. No Strain spent."),FColor::Red);return true;}
    const double Now=GetWorld()->GetTimeSeconds();
    const auto Before=Cinderhold;
    const int Cost=Ability==0?8:Ability==1?24:60;
    if(Ability==2){
        if((Choice!=2&&Choice!=3)||!Cinderhold.Resolved()||Cinderhold.Save().route!=0){
            CinderholdPendingRoute=0;
            Notice(TEXT("Resolve the elder first. An already chosen route cannot be rewritten again; hand maintenance remains available before any choice."));return true;
        }
        if(CinderholdPendingRoute!=Choice||Now>CinderholdConfirmUntil){
            CinderholdPendingRoute=Choice;CinderholdConfirmUntil=Now+6;
            Notice(Choice==2?
                TEXT("PERMANENT +60 STRAIN: R opens the kiln path at all hours but seals the archive alcove against the plume. No home fuel or pay is granted. Press R again here within 6 seconds. E offers hand maintenance instead."):
                TEXT("PERMANENT +60 STRAIN: T seals the kiln vent/path, preserving the archive alcove. No home fuel or pay is granted. Press T again here within 6 seconds. E offers hand maintenance instead."),FColor::Orange);return true;
        }
        CinderholdPendingRoute=0;
    }
    if((Ability==0&&Cinderhold.GlimpseActive(Now))||(Ability==1&&Cinderhold.FoldActive(Now))){
        Notice(TEXT("That local ability is still active; no additional Strain spent."));return true;
    }
    if(!Player->SpendRealityStrain(Cost)){Notice(TEXT("Recover Strain before trying this ability. E provides a physical alternative."),FColor::Yellow);return true;}
    bool Applied=false;
    if(Ability==0){Applied=Cinderhold.Glimpse(Now);Cinderhold.Inspect(static_cast<UnmadeCore::SlagClue>(1<<Clue),GetGameDay());}
    else if(Ability==1)Applied=Cinderhold.Fold(Now);
    else Applied=Cinderhold.Choose(static_cast<UnmadeCore::SlagRoute>(Choice));
    if(!Applied||!CommitCinderhold(Before,Player)){
        Cinderhold=Before;Player->RefundRealityStrain(Cost);return true;
    }
    if(Ability==0)Notice(FString(TEXT("GLIMPSE (+8 Strain): "))+ClueText[Clue]);
    else if(Ability==1){
        for(TActorIterator<AUnmadeEnemyCharacter> Enemy(GetWorld());Enemy;++Enemy)
            if(FVector::DistSquared2D(Enemy->GetActorLocation(),SlagVent)<FMath::Square(950.f))
                Enemy->ExposeToFold(Now,6);
        Notice(TEXT("FOLD (+24 Strain): six seconds of fracture exposure. Cooling the learned vent now lasts eighteen seconds; any pending elder strike is interrupted."));
    }else Notice(FString(TEXT("Permanent local route recorded: "))+RouteName(Choice));
    NextCinderholdRefreshAt=0;RefreshCinderholdAdventure();return true;
}

bool AUnmadePrototypeHub::RecordCinderholdConversation(FName StableId)
{
    if(bCinderholdSaveRejected||!GetWorld())return false;
    auto* Player=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    if(!Able(Player))return false;
    // Public caller cannot manufacture a distant witness.
    AUnmadeNpcCharacter* Witness=nullptr;
    for(TActorIterator<AUnmadeNpcCharacter> Npc(GetWorld());Npc;++Npc)
        if(Npc->GetStableId()==StableId&&FVector::DistSquared(Npc->GetActorLocation(),Player->GetActorLocation())<=FMath::Square(260.f)){
            Witness=*Npc;break;
        }
    if(!Witness)return false;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeCinderholdWitness),false);
    Params.AddIgnoredActor(Player);Params.AddIgnoredActor(Witness);
    if(GetWorld()->LineTraceTestByChannel(Player->GetActorLocation()+FVector(0,0,40),Witness->GetActorLocation()+FVector(0,0,40),ECC_Visibility,Params))return false;
    int Memory=0,Scene=-1;
    if(StableId==FName(TEXT("npc.cinderhold.courier.001"))){
        Memory=Cinderhold.Has(UnmadeCore::BesHeard)?UnmadeCore::BesReported:UnmadeCore::BesHeard;
        if(Memory==UnmadeCore::BesReported)Scene=2;
    }else if(StableId==FName(TEXT("npc.cinderhold.blackslag.scout.001"))){
        Memory=Cinderhold.Has(UnmadeCore::VardaBriefed)?UnmadeCore::VardaReported:UnmadeCore::VardaBriefed;
        Scene=Memory==UnmadeCore::VardaBriefed?0:1;
    }else if(StableId==FName(TEXT("npc.cinderhold.emberwarden.001")))Memory=UnmadeCore::RhevaTold;
    else if(StableId==FName(TEXT("npc.cinderhold.healer.001")))Memory=UnmadeCore::OvennaTold;
    if(Memory==0)return false;
    const auto Before=Cinderhold;
    if(!Cinderhold.Remember(Memory,GetGameDay())||!CommitCinderhold(Before))return false;
    if(Scene>=0)PlayCinderholdScene(Scene);
    return true;
}

FString AUnmadePrototypeHub::GetCinderholdResidentLine(FName Id) const
{
    if(bCinderholdSaveRejected)return FString();
    const auto S=Cinderhold.Save();
    if(Id==FName(TEXT("npc.cinderhold.blackslag.scout.001"))){
        if(Cinderhold.Has(UnmadeCore::VardaReported))return FString::Printf(
            TEXT("You reported the elder %s and chose %s. I recorded your fieldwork; I did not witness Bes's wages."),S.resolution==2?TEXT("quieted alive"):TEXT("defeated"),RouteName(S.route));
        if(Cinderhold.Has(UnmadeCore::VardaBriefed))return TEXT("You heard my warning. Read the west-path tracks, service log and cadence plate. The elder will settle only while its vent is cool; you can also face it. Bring the result to me yourself.");
    }
    if(Id==FName(TEXT("npc.cinderhold.courier.001"))){
        if(Cinderhold.Has(UnmadeCore::ReturnedHome))return FString::Printf(
            TEXT("You came home again on day %d, after telling me what happened at Blackslag. We counted %d baskets, %d cold days and %d unpaid marks that day. I remember the visit; the current ledger may have changed since."),S.returnDay,S.returnedHeat,S.returnedColdDays,S.returnedOwed);
        if(Cinderhold.Has(UnmadeCore::BesReported))return FString::Printf(TEXT("You told me the elder was %s. I remember the %s. Come back to the shelf at home; the fuel and the wage ledger still tell their own truth."),S.resolution==2?TEXT("left alive"):TEXT("killed"),RouteName(S.route));
        if(Cinderhold.Has(UnmadeCore::BesHeard))return TEXT("You stopped to hear me. Varda keeps a service log west of Blackslag. A safer road matters, but it cannot sign my pay mark.");
    }
    if(Id==FName(TEXT("npc.cinderhold.emberwarden.001"))&&Cinderhold.Has(UnmadeCore::RhevaTold))return
        TEXT("You brought Varda's field report to me. I can put that route on the work board; the unpaid entry remains a separate obligation.");
    if(Id==FName(TEXT("npc.cinderhold.healer.001"))&&Cinderhold.Has(UnmadeCore::OvennaTold))return S.resolution==2?
        TEXT("You told me you quieted the elder alive. I prefer a road that leaves room for another living thing. Tell me too if the houses stay cold."):
        TEXT("You told me you killed the elder. I will tend whoever walks that road, but I would have wanted you to try the cadence.");
    return FString();
}
FString AUnmadePrototypeHub::GetCinderholdJournal()const
{
    if(bCinderholdSaveRejected)return TEXT("Cinderhold adventure snapshot rejected; original data protected.");
    const auto S=Cinderhold.Save();
    FString Text=FString(UTF8_TO_TCHAR(Cinderhold.Objective()))+TEXT(" | ")+RouteName(S.route);
    if(Cinderhold.Has(UnmadeCore::ReturnedHome))Text+=FString::Printf(
        TEXT(" | Returned day %d: hearth %d baskets, remembered cold %d days, Bes owed %d marks at that visit."),S.returnDay,S.returnedHeat,S.returnedColdDays,S.returnedOwed);
    return Text;
}
bool AUnmadePrototypeHub::RecordCinderholdElderDefeat(AUnmadeEnemyCharacter* Elder)
{
    if(bCinderholdSaveRejected||!IsValid(Elder)||
       !Elder->ActorHasTag(FName(TEXT("Cinderhold.Slag.Creature.2")))||
       !IsValid(Elder->GetCombat())||!Elder->GetCombat()->IsDefeated())return false;
    if(Cinderhold.Resolved())return true;
    const auto Before=Cinderhold;
    if(!Cinderhold.Defeat(GetGameDay()))return false;
    if(!CommitCinderhold(Before))return false;
    Notice(TEXT("Sootbound Elder defeated. This optional outcome is saved. Choose the Blackslag route and report to Varda; no wages or campaign victory were granted."));return true;
}
bool AUnmadePrototypeHub::IsCinderholdVentActive()const
{
    return GetWorld()&&!bCinderholdSaveRejected&&Cinderhold.VentActive(GetWorld()->GetTimeSeconds());
}
