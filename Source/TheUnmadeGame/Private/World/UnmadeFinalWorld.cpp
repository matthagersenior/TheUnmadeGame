#include "World/UnmadePrototypeHub.h"
#include "Combat/UnmadeFinalBoss.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadeFrontierRealmRules.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include <string>
namespace {
FVector FinalOrigin(UnmadeCore::Realm realm)
{
    if(const auto* Later=UnmadeCore::FindLaterRealm(realm))
        return FVector(Later->centerX,Later->centerY,0);
    if(const auto* Frontier=UnmadeCore::FindFrontier(realm))
        return FVector(Frontier->centerX,Frontier->centerY,0);
    return FVector::ZeroVector;
}
FName FinalTag(int index,const TCHAR* Variant)
{
    return FName(*FString::Printf(TEXT("Final.World.%d.%s"),index,Variant));
}
}

void AUnmadePrototypeHub::BuildFinalWorld()
{
    if(!GetWorld())return;
    const FVector Before=FinalOrigin(UnmadeCore::Realm::FirstAbsence);
    // Both ending controls are on stable Auvren north ground.
    SpawnBlock(Before+FVector(-950,3080,112),FVector(.8,.8,2.2),
               FName(TEXT("Final.Control.Anchor")));
    SpawnBlock(Before+FVector(950,3080,112),FVector(.8,.8,2.2),
               FName(TEXT("Final.Control.Many")));
    SpawnBlock(Before+FVector(1600,3120,112),FVector(.8,.8,2.2),
               FName(TEXT("Final.Echo.Call")));
    // Nine distinct branch-conditioned public local markers; not nine copies
    // of another loaded map or a magical global NPC knowledge broadcast.
    for(int i=0;i<9;++i)
    {
        const auto Realm=static_cast<UnmadeCore::Realm>(i);
        const FVector Origin=FinalOrigin(Realm);
        SpawnBlock(Origin+FVector(-1050,-1250,95),FVector(.65,.65,2),
                   FinalTag(i,TEXT("Anchor")));
        SpawnBlock(Origin+FVector(1050,-1250,95),FVector(.65,.65,2),
                   FinalTag(i,TEXT("Many")));
    }
    if(AUnmadeFinalBoss* Boss=GetWorld()->SpawnActor<AUnmadeFinalBoss>(
        Before+FVector(0,3350,115),FRotator::ZeroRotator))
        Boss->Tags.AddUnique(FName(TEXT("Final.NhalVey")));
}
void AUnmadePrototypeHub::RefreshFinalWorld()
{
    if(!GetWorld())return;
    const auto Act=FinalStory.Act();
    const auto Choice=FinalStory.World();
    SetRiteWorldActorState(FName(TEXT("Final.Control.Anchor")),
        Act==UnmadeCore::FinalAct::AnswerPending);
    SetRiteWorldActorState(FName(TEXT("Final.Control.Many")),
        Act==UnmadeCore::FinalAct::AnswerPending);
    SetRiteWorldActorState(FName(TEXT("Final.Echo.Call")),
        Act==UnmadeCore::FinalAct::OtherMorning);
    for(int i=0;i<9;++i)
    {
        SetRiteWorldActorState(FinalTag(i,TEXT("Anchor")),
                              Choice==UnmadeCore::Morning::Anchor);
        SetRiteWorldActorState(FinalTag(i,TEXT("Many")),
                              Choice==UnmadeCore::Morning::Many);
        // World markers are readable signs, not added walls obstructing roads.
        const FName Current=Choice==UnmadeCore::Morning::Anchor
            ?FinalTag(i,TEXT("Anchor")):FinalTag(i,TEXT("Many"));
        for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
            if(It->ActorHasTag(Current))It->SetActorEnableCollision(false);
    }
    RefreshDistrictMood(); // final answer changes the visible sky immediately
}

bool AUnmadePrototypeHub::BreakFinalMask()
{
    if(bFinalSaveRejected||bLaterRealmSaveRejected ||
       !GetUnansweredRoadGuidance().bossAccessible)return false;
    int held=0,published=0,spared=0;
    for(const auto& Region:UnmadeCore::LaterRealms)
    {
        if(LaterRealm.Outcome(Region.realm)==1)++held;
        if(LaterRealm.Outcome(Region.realm)==2)++published;
        if(Guardians.Outcome(Region.realm)==UnmadeCore::GuardianOutcome::Pacified)
            ++spared;
    }
    const int Memory=UnmadeCore::FinalMemorySignature(held,published,spared);
    const auto Before=FinalStory.Snapshot();
    if(FinalStory.BreakMask(
        LaterRealm.IsComplete(UnmadeCore::Realm::FirstAbsence),Memory)!=
       UnmadeCore::FinalResult::MaskBroken)return false;
    if(!WriteWorldSnapshot()){FinalStory.Restore(Before);return false;}
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,16.f,FColor::Orange,
        FString::Printf(TEXT("FIRST VICTORY WAS A LIE: %s"),
        UTF8_TO_TCHAR(UnmadeCore::LastAdversary.maskReveal)));
    return true;
}
bool AUnmadePrototypeHub::BreakFinalCore()
{
    if(bFinalSaveRejected)return false;
    const auto Before=FinalStory.Snapshot();
    if(FinalStory.BreakCore()!=UnmadeCore::FinalResult::TrueVictory)return false;
    if(!WriteWorldSnapshot()){FinalStory.Restore(Before);return false;}
    RefreshFinalWorld();
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,22.f,FColor::Yellow,
        FString::Printf(TEXT("NHAL-VEY FALLS: %s Choose an actual world control in Auvren."),
            UTF8_TO_TCHAR(UnmadeCore::LastAdversary.trueReveal)));
    return true;
}
bool AUnmadePrototypeHub::CommitFinalMorning(
    AUnmadeCharacter* Player,UnmadeCore::Morning Choice)
{
    if(!IsValid(Player)||bFinalSaveRejected)return false;
    const auto Before=FinalStory.Snapshot();
    if(FinalStory.ChooseWorld(Choice)!=UnmadeCore::FinalResult::NewMorning)
        return false;
    if(!WriteWorldSnapshot()){FinalStory.Restore(Before);return false;}
    RefreshFinalWorld();
    const int idx=static_cast<int>(Choice);
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,23.f,FColor::Cyan,
        FString::Printf(TEXT("THE SAME WORLD, ANOTHER MORNING: %s. %s Your gear, quests, and decisions remain."),
            UTF8_TO_TCHAR(UnmadeCore::FinalWorlds[idx].name),
            UTF8_TO_TCHAR(UnmadeCore::FinalWorlds[idx].changedWorld)));
    // The cinematic cut would fade out/in; this is a deterministic safe
    // pre-PC relocation in the existing level, NOT a second save slot.
    if(!Player->SetActorLocation(FVector(0,220,155),false,nullptr,
                                 ETeleportType::TeleportPhysics))
        UE_LOG(LogTemp,Warning,TEXT("Final-world saved; relocation failed, use a normal gate."));
    return true;
}
bool AUnmadePrototypeHub::WakeFinalEcho()
{
    if(bFinalSaveRejected)return false;
    const auto Before=FinalStory.Snapshot();
    if(FinalStory.WakeEcho()!=UnmadeCore::FinalResult::EchoStarted)return false;
    if(!WriteWorldSnapshot()){FinalStory.Restore(Before);return false;}
    RefreshFinalWorld();
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,13.f,FColor::Orange,
        FString::Printf(TEXT("OPTIONAL ECHO: %s. It remembers the first defeat but uses another rhythm."),
            UTF8_TO_TCHAR(UnmadeCore::FinalWorlds[static_cast<int>(FinalStory.World())].bossReturn)));
    return true;
}
bool AUnmadePrototypeHub::SettleFinalEcho()
{
    if(bFinalSaveRejected)return false;
    const auto Before=FinalStory.Snapshot();
    if(FinalStory.SettleEcho()!=UnmadeCore::FinalResult::EchoDefeated)return false;
    if(!WriteWorldSnapshot()){FinalStory.Restore(Before);return false;}
    RefreshFinalWorld();
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,12.f,FColor::Green,
        TEXT("THE ECHO HAS ENDED. Your other world remains alive, with every choice remembered."));
    return true;
}
bool AUnmadePrototypeHub::TryFinalInteraction(
    AUnmadeCharacter* Player,double CompetingNpcDistanceSq)
{
    if(!IsValid(Player)||!GetWorld())return false;
    const FVector At=Player->GetActorLocation();
    double best=FMath::Min(FMath::Square(330.0),CompetingNpcDistanceSq);
    int action=0,worldIndex=-1;
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        if(It->IsHidden())continue;
        const double dist=FVector::DistSquared(At,It->GetActorLocation());
        if(dist>=best)continue;
        int possible=0,idx=-1;
        if(It->ActorHasTag(FName(TEXT("Final.Control.Anchor"))))possible=1;
        else if(It->ActorHasTag(FName(TEXT("Final.Control.Many"))))possible=2;
        else if(It->ActorHasTag(FName(TEXT("Final.Echo.Call"))))possible=3;
        else for(int i=0;i<9;++i)
        {
            if(It->ActorHasTag(FinalTag(i,TEXT("Anchor"))) ||
               It->ActorHasTag(FinalTag(i,TEXT("Many"))))
            {possible=4;idx=i;break;}
        }
        if(!possible)continue;
        FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeFinalSight),false);
        Sight.AddIgnoredActor(Player);
        Sight.AddIgnoredActor(*It);
        if(GetWorld()->LineTraceTestByChannel(
            At+FVector(0,0,50),It->GetActorLocation()+FVector(0,0,50),
            ECC_Visibility,Sight))continue;
        action=possible;worldIndex=idx;best=dist;
    }
    if(!action)return false;
    if(action==4)
    {
        if(GEngine && worldIndex>=0)GEngine->AddOnScreenDebugMessage(-1,13.f,FColor::Cyan,
            FString(UTF8_TO_TCHAR(UnmadeCore::FinalLocalRecord(
                FinalStory.World(),static_cast<UnmadeCore::Realm>(worldIndex)))));
        return true;
    }
    if(action==3)
    {
        if(!WakeFinalEcho()&&GEngine)
            GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Yellow,
                TEXT("The boss's optional new form cannot be summoned yet."));
        return true;
    }
    if(FinalStory.Act()!=UnmadeCore::FinalAct::AnswerPending)return false;
    const auto Choice=action==1?UnmadeCore::Morning::Anchor:UnmadeCore::Morning::Many;
    const auto gate=FinalCommitGate.Attempt(
        "TheLastAnswer",action,GetWorld()->GetTimeSeconds());
    if(gate!=UnmadeCore::CommitmentAttempt::Confirmed)
    {
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,14.f,FColor::Yellow,
            FString::Printf(TEXT("PERMANENT NEW MORNING: %s. %s. Press the SAME control again within six seconds to accept."),
                UTF8_TO_TCHAR(UnmadeCore::FinalWorlds[action].name),
                UTF8_TO_TCHAR(UnmadeCore::FinalWorlds[action].changedWorld)));
        return true;
    }
    if(!CommitFinalMorning(Player,Choice)&&GEngine)
        GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Red,
            TEXT("The new morning was not saved. Your earlier world remains intact."));
    return true;
}
