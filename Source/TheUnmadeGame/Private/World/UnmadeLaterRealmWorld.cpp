#include "World/UnmadePrototypeHub.h"
#include "Player/UnmadeCharacter.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"

// Six deliberately distinct region coordinates, not six labels at one portal.
namespace {
FVector RealmOrigin(UnmadeCore::Realm Realm)
{
    if(const auto* Later=UnmadeCore::FindLaterRealm(Realm))
        return FVector(Later->centerX,Later->centerY,0);
    if(const auto* Frontier=UnmadeCore::FindFrontier(Realm))
        return FVector(Frontier->centerX,Frontier->centerY,0);
    return FVector::ZeroVector;
}
FName LaterProp(int32 Index,const TCHAR* Kind)
{
    return FName(*FString::Printf(TEXT("Later.%d.%s"),Index,Kind));
}
FName AtlasGateway(int32 From,int32 To)
{
    return FName(*FString::Printf(TEXT("Atlas.Gateway.%d.%d"),From,To));
}
}

void AUnmadePrototypeHub::BuildAtlasGateways()
{
    std::array<int32,9> Count{};
    for(const auto& Edge:UnmadeCore::AtlasPassages)
    {
        // The two original Reach gates and their returns remain intact.
        if(Edge.a==UnmadeCore::Realm::ThreefoldReach &&
           (Edge.b==UnmadeCore::Realm::WidowedRain ||
            Edge.b==UnmadeCore::Realm::HearthBeneath))continue;
        for(int Side=0;Side<2;++Side)
        {
            const auto At=Side==0?Edge.a:Edge.b;
            const auto Destination=Side==0?Edge.b:Edge.a;
            const int32 Id=static_cast<int32>(At);
            const int32 N=Count[Id]++;
            const FVector Center=RealmOrigin(At);
            SpawnBlock(Center+FVector(-1650+N*425,-1580,125),
                FVector(.42,.42,2.15),
                AtlasGateway(Id,static_cast<int32>(Destination)));
        }
    }
}

bool AUnmadePrototypeHub::TryTravelAtlas(AUnmadeCharacter* Player)
{
    if(!IsValid(Player) || !GetWorld() || bLaterRealmSaveRejected)return false;
    const FVector Current=Player->GetActorLocation();
    for(const auto& Edge:UnmadeCore::AtlasPassages)
    {
        for(int Side=0;Side<2;++Side)
        {
            const auto At=Side==0?Edge.a:Edge.b;
            const auto Destination=Side==0?Edge.b:Edge.a;
            const FName Gate=AtlasGateway(static_cast<int32>(At),
                                          static_cast<int32>(Destination));
            bool bNearby=false;
            for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
            {
                if(It->ActorHasTag(Gate) &&
                   FVector::DistSquared(Current,It->GetActorLocation())<FMath::Square(390.f))
                {
                    bNearby=true;break;
                }
            }
            if(!bNearby)continue;
            // Gated by actually earned relics, not by a map cheat or hidden UI.
            const UUnmadeEquipmentComponent* Equipment=Player->GetEquipment();
            const int32 Attunement=IsValid(Equipment)?Equipment->GetAtlasAttunement():0;
            if(Attunement<Edge.attunement)
            {
                if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Yellow,
                    FString::Printf(TEXT("CROSSING SEALED: attunement %d required (you have %d)."),
                        Edge.attunement,Attunement));
                return false;
            }
            const auto PreviousLater=LaterRealm.Snapshot();
            const auto PreviousFrontier=Frontier.Snapshot();
            const bool bNewLater=LaterRealm.Visit(Destination)==UnmadeCore::LaterResult::Visited;
            const bool bNewFrontier=Frontier.Visit(Destination)==UnmadeCore::FrontierEvent::Advanced;
            if((bNewLater||bNewFrontier) && !WriteWorldSnapshot())
            {
                LaterRealm.Restore(PreviousLater);
                Frontier.Restore(PreviousFrontier);
                return false;
            }
            const FVector Arrival=RealmOrigin(Destination)+FVector(0,-1170,135);
            if(!Player->SetActorLocation(Arrival,false,nullptr,ETeleportType::TeleportPhysics))
            {
                LaterRealm.Restore(PreviousLater);
                Frontier.Restore(PreviousFrontier);
                if(bNewLater||bNewFrontier)WriteWorldSnapshot();
                return false;
            }
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Cyan,
                FString::Printf(TEXT("PASSAGE OPEN: %s. Listen to local witnesses before acting."),
                    *GetCurrentRealmName(Arrival)));
            return true;
        }
    }
    return false;
}

void AUnmadePrototypeHub::BuildLaterRealms()
{
    if(!GetWorld())return;
    for(int32 Index=0;Index<static_cast<int32>(UnmadeCore::LaterRealms.size());++Index)
    {
        const auto& Spec=UnmadeCore::LaterRealms[Index];
        const auto& After=UnmadeCore::RealmAftermathSpecs[Index+3];
        const FVector Origin(Spec.centerX,Spec.centerY,0);
        SpawnBlock(Origin+FVector(0,0,-50),FVector(58,58,1),
                   LaterProp(Index,TEXT("Ground")));
        // Every settlement silhouette has two differently-proportioned landmarks.
        SpawnBlock(Origin+FVector(-1750,650,170+Index*22),
                   FVector(1.0+Index*.28,3.0,3.4+Index*.4),
                   LaterProp(Index,TEXT("Shelter")));
        SpawnBlock(Origin+FVector(1650,-350,220+Index*20),
                   FVector(2.0,.9+Index*.3,4.2+Index*.4),
                   LaterProp(Index,TEXT("CivicRecord")));
        SpawnBlock(Origin+FVector(-850,-800,105),FVector(.55,.55,2.1),
                   FName(UTF8_TO_TCHAR(Spec.trialEvidenceA)));
        SpawnBlock(Origin+FVector(850,-800,105),FVector(.55,.55,2.1),
                   FName(UTF8_TO_TCHAR(Spec.trialEvidenceB)));
        SpawnBlock(Origin+FVector(0,-180,115),FVector(.65,.65,2.3),
                   FName(UTF8_TO_TCHAR(Spec.mechanism)));
        SpawnBlock(Origin+FVector(0,610,140),FVector(50,.55,3),
                   LaterProp(Index,TEXT("Trial.Barrier")));
        SpawnBlock(Origin+FVector(-850,970,20),FVector(8,12,.35),
                   LaterProp(Index,TEXT("Trial.Route.Care")));
        SpawnBlock(Origin+FVector(850,970,20),FVector(8,12,.35),
                   LaterProp(Index,TEXT("Trial.Route.Truth")));
        SpawnBlock(Origin+FVector(0,1130,110),FVector(.55,.55,2.2),
                   FName(UTF8_TO_TCHAR(After.mechanismSite)));
        SpawnBlock(Origin+FVector(0,1480,150),FVector(52,.55,3),
                   LaterProp(Index,TEXT("Aftermath.Barrier")));
        SpawnBlock(Origin+FVector(-850,1810,105),FVector(.55,.55,2.1),
                   FName(UTF8_TO_TCHAR(After.evidenceForCare)));
        SpawnBlock(Origin+FVector(850,1810,105),FVector(.55,.55,2.1),
                   FName(UTF8_TO_TCHAR(After.evidenceForTruth)));
        SpawnBlock(Origin+FVector(-850,2130,160),FVector(8,.55,3.2),
                   LaterProp(Index,TEXT("Aftermath.Gate.Care")));
        SpawnBlock(Origin+FVector(850,2130,160),FVector(8,.55,3.2),
                   LaterProp(Index,TEXT("Aftermath.Gate.Truth")));
        SpawnBlock(Origin+FVector(-850,2460,20),FVector(8,6,.35),
                   LaterProp(Index,TEXT("Aftermath.Home.Care")));
        SpawnBlock(Origin+FVector(850,2460,20),FVector(8,6,.35),
                   LaterProp(Index,TEXT("Aftermath.Home.Truth")));
        // Three local people, not only quest markers. Their stable IDs are used
        // for witness gating, save/restore and observation-limited reactions.
        struct SpawnWitness {const char* Id;const TCHAR* RoleName;FVector Offset;
            UnmadeCore::NpcRole Role;UnmadeCore::NpcTemperament Temperament;const char* Line;};
        const SpawnWitness Residents[3]={
            {After.initiatingWitness,TEXT("Civic keeper"),FVector(0,-520,95),
             UnmadeCore::NpcRole::Guard,UnmadeCore::NpcTemperament::Steady,Spec.arrival},
            {After.careWitness,TEXT("Common witness"),FVector(-1050,1770,95),
             UnmadeCore::NpcRole::Scholar,UnmadeCore::NpcTemperament::Steady,Spec.trialChoiceA},
            {After.truthWitness,TEXT("Hidden witness"),FVector(1050,1770,95),
             UnmadeCore::NpcRole::Scholar,UnmadeCore::NpcTemperament::Curious,Spec.trialChoiceB}
        };
        for(const auto& Witness:Residents)
        {
            if(AUnmadeNpcCharacter* Npc=GetWorld()->SpawnActor<AUnmadeNpcCharacter>(
                 Origin+Witness.Offset,FRotator::ZeroRotator))
                Npc->ConfigureLaterRealm(Spec,Witness.Id,
                    FString::Printf(TEXT("%s of %s"),Witness.RoleName,
                                    UTF8_TO_TCHAR(Spec.name)),
                    Witness.Role,Witness.Temperament,Witness.Line);
        }
    }
}

void AUnmadePrototypeHub::RefreshLaterRealmWorld()
{
    for(int32 i=0;i<static_cast<int32>(UnmadeCore::LaterRealms.size());++i)
    {
        const auto Realm=UnmadeCore::LaterRealms[i].realm;
        const int32 Trial=LaterRealm.Outcome(Realm);
        const int32 Ending=RealmAftermath.Ending(Realm);
        SetRiteWorldActorState(LaterProp(i,TEXT("Trial.Barrier")),
                               !LaterRealm.IsComplete(Realm));
        SetRiteWorldActorState(LaterProp(i,TEXT("Trial.Route.Care")),Trial==1);
        SetRiteWorldActorState(LaterProp(i,TEXT("Trial.Route.Truth")),Trial==2);
        SetRiteWorldActorState(LaterProp(i,TEXT("Aftermath.Barrier")),
                               !RealmAftermath.IsPrepared(Realm));
        SetRiteWorldActorState(LaterProp(i,TEXT("Aftermath.Gate.Care")),Ending!=1);
        SetRiteWorldActorState(LaterProp(i,TEXT("Aftermath.Gate.Truth")),Ending!=2);
        SetRiteWorldActorState(LaterProp(i,TEXT("Aftermath.Home.Care")),Ending==1);
        SetRiteWorldActorState(LaterProp(i,TEXT("Aftermath.Home.Truth")),Ending==2);
    }
}

bool AUnmadePrototypeHub::InspectLaterRealmSite(
    AUnmadeCharacter* Player,double CompetingNpcDistanceSq)
{
    if(!IsValid(Player) || !GetWorld() || bLaterRealmSaveRejected ||
       bRealmAftermathSaveRejected)return false;
    AStaticMeshActor* Best=nullptr;
    int32 Index=-1,Part=0,Choice=0;
    double Closest=FMath::Min(FMath::Square(310.0),CompetingNpcDistanceSq);
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        const double Dist=FVector::DistSquared(Player->GetActorLocation(),
                                               It->GetActorLocation());
        if(Dist>=Closest)continue;
        for(int32 i=0;i<static_cast<int32>(UnmadeCore::LaterRealms.size());++i)
        {
            const auto& S=UnmadeCore::LaterRealms[i];
            const auto& A=UnmadeCore::RealmAftermathSpecs[i+3];
            int P=0,C=0;
            if(It->ActorHasTag(FName(UTF8_TO_TCHAR(S.trialEvidenceA)))){P=1;C=1;}
            else if(It->ActorHasTag(FName(UTF8_TO_TCHAR(S.trialEvidenceB)))){P=1;C=2;}
            else if(It->ActorHasTag(FName(UTF8_TO_TCHAR(S.mechanism))))P=2;
            else if(It->ActorHasTag(FName(UTF8_TO_TCHAR(A.mechanismSite))))P=3;
            else if(It->ActorHasTag(FName(UTF8_TO_TCHAR(A.evidenceForCare)))){P=4;C=1;}
            else if(It->ActorHasTag(FName(UTF8_TO_TCHAR(A.evidenceForTruth)))){P=4;C=2;}
            if(P==0)continue;
            FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeLaterRealmSight),false);
            Sight.AddIgnoredActor(Player);
            Sight.AddIgnoredActor(*It);
            if(GetWorld()->LineTraceTestByChannel(
                Player->GetActorLocation()+FVector(0,0,50),
                It->GetActorLocation()+FVector(0,0,50),ECC_Visibility,Sight))continue;
            Best=*It;Index=i;Part=P;Choice=C;Closest=Dist;
            break;
        }
    }
    if(!Best || Index<0)return false;
    const auto& Spec=UnmadeCore::LaterRealms[Index];
    const auto& After=UnmadeCore::RealmAftermathSpecs[Index+3];
    const auto Before=LaterRealm.Snapshot();
    const auto BeforeAfter=RealmAftermath.Snapshot();
    UnmadeCore::LaterResult L=UnmadeCore::LaterResult::NoChange;
    UnmadeCore::RealmAftermathResult A=UnmadeCore::RealmAftermathResult::NoChange;
    if(Part==1)L=LaterRealm.Study(Spec.realm,Choice,
                        Choice==1?Spec.trialEvidenceA:Spec.trialEvidenceB);
    else if(Part==2)L=LaterRealm.Operate(Spec.realm,Spec.mechanism);
    else if(Part==3)A=RealmAftermath.Prepare(Spec.realm,After.mechanismSite);
    else if(Part==4)A=RealmAftermath.Inspect(Spec.realm,Choice,
                        Choice==1?After.evidenceForCare:After.evidenceForTruth);
    const bool Advanced=L==UnmadeCore::LaterResult::EvidenceFound ||
        L==UnmadeCore::LaterResult::Completed ||
        A==UnmadeCore::RealmAftermathResult::Prepared ||
        A==UnmadeCore::RealmAftermathResult::EvidenceFound;
    if(Advanced && !WriteWorldSnapshot())
    {
        LaterRealm.Restore(Before);
        RealmAftermath.Restore(BeforeAfter);
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Red,
             TEXT("Could not save this intervention; world state remains unchanged."));
        return true;
    }
    if(Advanced)RefreshLaterRealmWorld();
    if(GEngine)
    {
        FString Message;
        if(L==UnmadeCore::LaterResult::EvidenceFound)
            Message=FString::Printf(TEXT("TRIAL OBSERVED: %s | Return to the physical control."),
                   Choice==1?UTF8_TO_TCHAR(Spec.trialChoiceA):
                             UTF8_TO_TCHAR(Spec.trialChoiceB));
        else if(L==UnmadeCore::LaterResult::Completed)
            Message=FString::Printf(TEXT("NEW ROAD: %s | Speak to the local keeper again."),
                   Choice==1?UTF8_TO_TCHAR(Spec.trialChoiceA):
                             UTF8_TO_TCHAR(Spec.trialChoiceB));
        else if(L==UnmadeCore::LaterResult::NeedEvidence)
            Message=TEXT("FIRST INVESTIGATE one of the two evidence stones before operating the control.");
        else if(A==UnmadeCore::RealmAftermathResult::Prepared)
            Message=TEXT("THE SECOND PASSAGE OPENS: investigate the public relief or concealed record.");
        else if(A==UnmadeCore::RealmAftermathResult::NeedMechanism)
            Message=TEXT("The return chapter is sealed: first operate its mechanism.");
        else if(A==UnmadeCore::RealmAftermathResult::EvidenceFound)
            Message=TEXT("A matching local witness can now confirm the discovered record.");
        else if(LaterRealm.Stage(Spec.realm)==0)
            Message=TEXT("Speak with this realm's civic keeper first.");
        else if(RealmAftermath.Stage(Spec.realm)==4)
            Message=FString(UTF8_TO_TCHAR(RealmAftermath.WorldConsequence(Spec.realm)));
        else Message=TEXT("This evidence has not changed; consult the realm journal or its witnesses.");
        GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Cyan,Message);
    }
    return true;
}

bool AUnmadePrototypeHub::TryLaterRealmConversation(FName ResidentId)
{
    if(ResidentId.IsNone() || bLaterRealmSaveRejected ||
       bRealmAftermathSaveRejected)return false;
    const FString Name=ResidentId.ToString();
    const FTCHARToUTF8 NameUtf8(*Name);
    for(const auto& Spec:UnmadeCore::LaterRealms)
    {
        const int idx=static_cast<int>(Spec.realm);
        const auto& After=UnmadeCore::RealmAftermathSpecs[idx];
        const bool Initiating=FCStringAnsi::Strcmp(NameUtf8.Get(),Spec.initiator)==0;
        const bool Care=FCStringAnsi::Strcmp(NameUtf8.Get(),After.careWitness)==0;
        const bool Truth=FCStringAnsi::Strcmp(NameUtf8.Get(),After.truthWitness)==0;
        if(!Initiating && !Care && !Truth)continue;
        const auto Before=LaterRealm.Snapshot();
        const auto BeforeAfter=RealmAftermath.Snapshot();
        const auto L=Initiating?LaterRealm.Begin(Spec.realm,Spec.initiator):
            UnmadeCore::LaterResult::NoChange;
        auto A=UnmadeCore::RealmAftermathResult::NoChange;
        if(LaterRealm.IsComplete(Spec.realm))
        {
            if(Initiating && RealmAftermath.Stage(Spec.realm)==0)
                A=RealmAftermath.Begin(Spec.realm,LaterRealm.IsComplete(Spec.realm),
                                       LaterRealm.Visited(Spec.realm),After.initiatingWitness);
            else if(RealmAftermath.Stage(Spec.realm)==2)
                A=RealmAftermath.Testify(Spec.realm,NameUtf8.Get());
        }
        const bool Changed=L==UnmadeCore::LaterResult::Started ||
            A==UnmadeCore::RealmAftermathResult::Started ||
            A==UnmadeCore::RealmAftermathResult::Witnessed;
        if(!Changed)return false;
        if(!WriteWorldSnapshot())
        {
            LaterRealm.Restore(Before);
            RealmAftermath.Restore(BeforeAfter);
            return false;
        }
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1,12.f,FColor::Cyan,
                L==UnmadeCore::LaterResult::Started
                ?TEXT("FIRST ARRIVAL: inspect two distinct evidence stones, select one approach and operate its control.")
                :A==UnmadeCore::RealmAftermathResult::Started
                ?TEXT("RETURN CHAPTER: operate the second mechanism, inspect the chosen record and find its witness.")
                :TEXT("TESTIMONY RECORDED: return to the keeper. F7 protects neighbors; F8 exposes hidden history."));
        return true;
    }
    return false;
}
