#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeEchoQuestRules.h"
#include "World/UnmadeLaterRealmRules.h"
#include "Player/UnmadeCharacter.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include <string>

namespace {
FName EchoTag(int32 i,const TCHAR* Kind) {
    return FName(*FString::Printf(TEXT("Echo.%d.%s"),i,Kind));
}
FVector EchoRegion(UnmadeCore::Realm r) {
    if(const auto* Spec=UnmadeCore::FindLaterRealm(r))
        return FVector(Spec->centerX,Spec->centerY,0);
    if(const auto* Frontier=UnmadeCore::FindFrontier(r))
        return FVector(Frontier->centerX,Frontier->centerY,0);
    return FVector::ZeroVector;
}
}

void AUnmadePrototypeHub::BuildEchoQuests()
{
    if(!GetWorld())return;
    for(int32 i=0;i<static_cast<int32>(UnmadeCore::EchoQuests.size());++i)
    {
        const auto& Quest=UnmadeCore::EchoQuests[i];
        const FVector O=EchoRegion(Quest.realm);
        // Genuine third-visit detour: one clue off the old road, and one on
        // the far civic terrace, whose access depended on previous decisions.
        SpawnBlock(O+FVector(-1700,200,98),FVector(.6,.65,1.9),
            FName(UTF8_TO_TCHAR(Quest.evidenceA)));
        SpawnBlock(O+FVector(1500,1800,98),FVector(.6,.65,1.9),
            FName(UTF8_TO_TCHAR(Quest.evidenceB)));
        SpawnBlock(O+FVector(-850,1940,115),FVector(.7,.6,2.3),
            FName(UTF8_TO_TCHAR(Quest.ritualCare)));
        SpawnBlock(O+FVector(850,1940,115),FVector(.7,.6,2.3),
            FName(UTF8_TO_TCHAR(Quest.ritualTruth)));
        SpawnBlock(O+FVector(-700,2030,90),FVector(2.3,.8,1.8),
            EchoTag(i,TEXT("CivicCare")));
        SpawnBlock(O+FVector(700,2030,90),FVector(2.3,.8,1.8),
            EchoTag(i,TEXT("CivicTruth")));
        // The first adjacent destination receives a visible public *artifact*
        // of the decision, never instant psychic knowledge for distant NPCs.
        for(const auto& Edge:UnmadeCore::AtlasPassages)
        {
            UnmadeCore::Realm Other=UnmadeCore::Realm::Count;
            if(Edge.a==Quest.realm)Other=Edge.b;
            else if(Edge.b==Quest.realm)Other=Edge.a;
            if(Other==UnmadeCore::Realm::Count)continue;
            const FVector Destination=EchoRegion(Other);
            SpawnBlock(Destination+FVector(1100+i*130,-1300,115),
                FVector(.55,.5,2.2),EchoTag(i,TEXT("NeighborSignal")));
            break;
        }
    }
}

void AUnmadePrototypeHub::RefreshEchoQuestWorld()
{
    for(int32 i=0;i<static_cast<int32>(UnmadeCore::EchoQuests.size());++i)
    {
        const auto& Q=UnmadeCore::EchoQuests[i];
        const int stage=EchoQuest.Stage(Q.realm);
        const int ending=EchoQuest.Ending(Q.realm);
        SetRiteWorldActorState(FName(UTF8_TO_TCHAR(Q.evidenceA)),stage==1);
        SetRiteWorldActorState(FName(UTF8_TO_TCHAR(Q.evidenceB)),stage==1);
        SetRiteWorldActorState(FName(UTF8_TO_TCHAR(Q.ritualCare)),stage==2);
        SetRiteWorldActorState(FName(UTF8_TO_TCHAR(Q.ritualTruth)),stage==2);
        SetRiteWorldActorState(EchoTag(i,TEXT("CivicCare")),ending==1);
        SetRiteWorldActorState(EchoTag(i,TEXT("CivicTruth")),ending==2);
        SetRiteWorldActorState(EchoTag(i,TEXT("NeighborSignal")),ending!=0);
    }
}

bool AUnmadePrototypeHub::TryEchoQuestConversation(FName ResidentId)
{
    if(ResidentId.IsNone() || bEchoSaveRejected || bLaterRealmSaveRejected ||
       bRealmAftermathSaveRejected)return false;
    const FTCHARToUTF8 Identifier(*ResidentId.ToString());
    for(const auto& Q:UnmadeCore::EchoQuests)
    {
        const bool Opens=FCStringAnsi::Strcmp(Identifier.Get(),Q.openingWitness)==0;
        const bool Care=FCStringAnsi::Strcmp(Identifier.Get(),Q.witnessCare)==0;
        const bool Truth=FCStringAnsi::Strcmp(Identifier.Get(),Q.witnessTruth)==0;
        if(!Opens && !Care && !Truth)continue;
        const auto Before=EchoQuest.Snapshot();
        UnmadeCore::EchoResult Result=UnmadeCore::EchoResult::NoChange;
        if(EchoQuest.Stage(Q.realm)==0 && Opens)
            Result=EchoQuest.Begin(Q.realm,LaterRealm.IsComplete(Q.realm),
                                   RealmAftermath.Stage(Q.realm)==4,Identifier.Get());
        else if(EchoQuest.Stage(Q.realm)==1 && (Care || Truth))
            Result=EchoQuest.Testify(Q.realm,Identifier.Get());
        if(Result!=UnmadeCore::EchoResult::Started &&
           Result!=UnmadeCore::EchoResult::Testified)
        {
            if(Result==UnmadeCore::EchoResult::NeedEvidence && GEngine)
                GEngine->AddOnScreenDebugMessage(-1,7.f,FColor::Yellow,
                   TEXT("Two separate physical clues are required before testimony."));
            return false;
        }
        if(!WriteWorldSnapshot()) {
            EchoQuest.Restore(Before);return false;
        }
        RefreshEchoQuestWorld();
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1,12.f,FColor::Cyan,
                Result==UnmadeCore::EchoResult::Started
                ?FString::Printf(TEXT("NEW THIRD CHAPTER: %s — collect both material clues, not repeated errands."),
                    UTF8_TO_TCHAR(Q.title))
                :FString::Printf(TEXT("TESTIMONY RECEIVED: %s — inspect either civic intervention, then press again to confirm."),
                    UTF8_TO_TCHAR(Q.title)));
        return true;
    }
    return false;
}

bool AUnmadePrototypeHub::InspectEchoQuestSite(
    AUnmadeCharacter* Player,double CompetingNpcDistanceSq)
{
    if(!IsValid(Player) || !GetWorld() || bEchoSaveRejected ||
       bRealmAftermathSaveRejected || bLaterRealmSaveRejected)return false;
    int idx=-1,action=0,choice=0;
    double best=FMath::Min(FMath::Square(310.0),CompetingNpcDistanceSq);
    const FVector From=Player->GetActorLocation();
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        const double d=FVector::DistSquared(From,It->GetActorLocation());
        if(d>=best || It->IsHidden())continue;
        for(int i=0;i<static_cast<int>(UnmadeCore::EchoQuests.size());++i)
        {
            const auto& Q=UnmadeCore::EchoQuests[i];
            int A=0,C=0;
            if(It->ActorHasTag(FName(UTF8_TO_TCHAR(Q.evidenceA))))A=1;
            else if(It->ActorHasTag(FName(UTF8_TO_TCHAR(Q.evidenceB))))A=2;
            else if(It->ActorHasTag(FName(UTF8_TO_TCHAR(Q.ritualCare)))){A=3;C=1;}
            else if(It->ActorHasTag(FName(UTF8_TO_TCHAR(Q.ritualTruth)))){A=3;C=2;}
            else if(It->ActorHasTag(EchoTag(i,TEXT("NeighborSignal"))))A=4;
            if(!A)continue;
            FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeEchoSight),false);
            Sight.AddIgnoredActor(Player);
            Sight.AddIgnoredActor(*It);
            if(GetWorld()->LineTraceTestByChannel(
                From+FVector(0,0,50),
                It->GetActorLocation()+FVector(0,0,50),ECC_Visibility,Sight))continue;
            idx=i;action=A;choice=C;best=d;break;
        }
    }
    if(idx<0)return false;
    const auto& Q=UnmadeCore::EchoQuests[idx];
    const auto Before=EchoQuest.Snapshot();
    auto Result=UnmadeCore::EchoResult::NoChange;
    if(action==1)Result=EchoQuest.Inspect(Q.realm,Q.evidenceA);
    else if(action==2)Result=EchoQuest.Inspect(Q.realm,Q.evidenceB);
    else if(action==3 && EchoQuest.Stage(Q.realm)==2)
    {
        const auto Confirmation=EchoCommitGate.Attempt(
            std::string("Echo.")+std::to_string(idx),choice,GetWorld()->GetTimeSeconds());
        if(Confirmation!=UnmadeCore::CommitmentAttempt::Confirmed)
        {
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,9.f,FColor::Yellow,
                FString::Printf(TEXT("PROPOSED PERMANENT CHANGE: %s — press the SAME civic control again within six seconds."),
                    UTF8_TO_TCHAR(choice==1?Q.resultCare:Q.resultTruth)));
            return true;
        }
        Result=EchoQuest.Commit(Q.realm,choice,
                  choice==1?Q.ritualCare:Q.ritualTruth);
    }
    if(action==4)
    {
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,9.f,FColor::Cyan,
            FString::Printf(TEXT("THE NEIGHBOR'S RECORD: %s"),UTF8_TO_TCHAR(Q.farRealmMessage)));
        return true;
    }
    if(Result==UnmadeCore::EchoResult::Discovered ||
       Result==UnmadeCore::EchoResult::Committed)
    {
        if(!WriteWorldSnapshot())
        {
            EchoQuest.Restore(Before);
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Red,
                TEXT("Unable to persist the echo; the change was not applied."));
            return true;
        }
        RefreshEchoQuestWorld();
        RefreshRealmResonanceWorld();
    }
    if(GEngine)
    {
        FString Line=Result==UnmadeCore::EchoResult::Discovered
            ?FString(UTF8_TO_TCHAR(action==1?Q.evidenceALine:Q.evidenceBLine))
            :Result==UnmadeCore::EchoResult::Committed
            ?FString(UTF8_TO_TCHAR(choice==1?Q.resultCare:Q.resultTruth))
            :Result==UnmadeCore::EchoResult::WrongChoice
            ?TEXT("This account needs a matching local witness; changing it requires further testimony.")
            :TEXT("Read both unique clues, obtain testimony, then choose its matching civic control.");
        GEngine->AddOnScreenDebugMessage(-1,11.f,FColor::Cyan,Line);
    }
    return true;
}
