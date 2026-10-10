#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeCampaignSpineRules.h"
#include "World/UnmadeFrontierRealmRules.h"
#include "World/UnmadeLaterRealmRules.h"
#include "Player/UnmadeCharacter.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"

namespace {
FVector UnansweredHome(UnmadeCore::Realm realm)
{
    if(const auto* Region=UnmadeCore::FindLaterRealm(realm))
        return FVector(Region->centerX,Region->centerY,0);
    if(const auto* Frontier=UnmadeCore::FindFrontier(realm))
        return FVector(Frontier->centerX,Frontier->centerY,0);
    return FVector::ZeroVector;
}
UnmadeCore::Realm WhereRoadBegins(FVector position)
{
    UnmadeCore::Realm nearest=UnmadeCore::Realm::ThreefoldReach;
    double best=FVector::DistSquared2D(position,FVector::ZeroVector);
    for(int i=1;i<static_cast<int>(UnmadeCore::Realm::Count);++i)
    {
        const auto place=static_cast<UnmadeCore::Realm>(i);
        const double dist=FVector::DistSquared2D(position,UnansweredHome(place));
        if(dist<best){best=dist;nearest=place;}
    }
    return nearest;
}
} // namespace

UnmadeCore::RoadEvidence AUnmadePrototypeHub::GetUnansweredRoadEvidence() const
{
    UnmadeCore::RoadEvidence Progress;
    // A change in the Crossings, two communities, or two genuine landmarks is
    // enough opening testimony. Migrated advanced saves remain recognized.
    Progress.openingWitnessed=ReadStoryChoice()!=0 ||
        VillagesVisited.Count()>=2 || Discoveries.Count()>=2;
    if(Frontier.IsResolved(UnmadeCore::Realm::WidowedRain))
        Progress.firstStories|=UnmadeCore::RoadMask(UnmadeCore::Realm::WidowedRain);
    if(Frontier.IsResolved(UnmadeCore::Realm::HearthBeneath))
        Progress.firstStories|=UnmadeCore::RoadMask(UnmadeCore::Realm::HearthBeneath);
    for(const auto& Region:UnmadeCore::LaterRealms)
        if(LaterRealm.IsComplete(Region.realm))
            Progress.firstStories|=UnmadeCore::RoadMask(Region.realm);
    // Do not retroactively undo a progressed pre-existing final save, even
    // if its old route did not have the newly authored five-act journal.
    if(Progress.firstStories!=0 || FinalStory.Act()!=UnmadeCore::FinalAct::Veil)
        Progress.openingWitnessed=true;
    Progress.finalAct=static_cast<int>(FinalStory.Act());
    return Progress;
}
UnmadeCore::RoadGuidance AUnmadePrototypeHub::GetUnansweredRoadGuidance() const
{
    return UnmadeCore::EvaluateUnansweredRoad(GetUnansweredRoadEvidence());
}
void AUnmadePrototypeHub::BuildUnansweredRoad()
{
    if(!GetWorld())return;
    for(int idx=0;idx<static_cast<int>(UnmadeCore::UnansweredRoadBeats.size());++idx)
    {
        const auto& Beat=UnmadeCore::UnansweredRoadBeats[idx];
        const FVector Place=UnansweredHome(Beat.realm);
        const FVector Offset=idx==0?FVector(-680,-1060,102):
                              FVector(-1200,-1380,102);
        // Existing reachable terrain near the arrival region. Reading E
        // never grants fake story completion or requires collecting nine glyphs.
        SpawnBlock(Place+Offset,FVector(.52,.6,2),
                   FName(UTF8_TO_TCHAR(Beat.publicTag)));
    }
}
FString AUnmadePrototypeHub::DescribeUnansweredRoad(AUnmadeCharacter* Player) const
{
    const auto Lead=GetUnansweredRoadGuidance();
    FString Message=FString::Printf(
        TEXT("THE UNANSWERED ROAD | ACT %d/6: %s | Question: %s | Next: %s"),
        static_cast<int>(Lead.act),UTF8_TO_TCHAR(Lead.title),
        UTF8_TO_TCHAR(Lead.question),UTF8_TO_TCHAR(Lead.objective));
    if(!IsValid(Player) || Lead.available==0)return Message;
    const auto From=WhereRoadBegins(Player->GetActorLocation());
    const UUnmadeEquipmentComponent* Gear=Player->GetEquipment();
    const int Attunement=IsValid(Gear)?Gear->GetAtlasAttunement():0;
    Message+=TEXT(" | Leads: ");
    for(int n=0;n<Lead.available;++n)
    {
        const auto Destination=Lead.alternatives[n];
        if(n>0)Message+=TEXT(" OR ");
        const auto Routes=UnmadeCore::RoadRoute(From,Destination,Attunement);
        Message+=FString(UTF8_TO_TCHAR(
            UnmadeCore::WorldAtlas[static_cast<int>(Destination)].name));
        if(Routes.empty())
        {
            Message+=TEXT(" [crossing not yet attuned; earn civic/relic progress]");
        }
        else
        {
            Message+=TEXT(" [");
            for(int k=0;k<static_cast<int>(Routes.size());++k)
            {
                if(k>0)Message+=TEXT(" -> ");
                Message+=FString(UTF8_TO_TCHAR(
                    UnmadeCore::WorldAtlas[static_cast<int>(Routes[k])].name));
            }
            Message+=TEXT("]");
        }
    }
    return Message;
}
bool AUnmadePrototypeHub::InspectUnansweredRoad(
    AUnmadeCharacter* Player,double CompetingNpcDistanceSq)
{
    if(!IsValid(Player)||!GetWorld())return false;
    const double radius=FMath::Min(FMath::Square(310.0),CompetingNpcDistanceSq);
    int Best=-1;
    double distance=radius;
    const FVector From=Player->GetActorLocation();
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        if(It->IsHidden())continue;
        const double Dist=FVector::DistSquared(From,It->GetActorLocation());
        if(Dist>=distance)continue;
        for(int i=0;i<static_cast<int>(UnmadeCore::UnansweredRoadBeats.size());++i)
        {
            if(!It->ActorHasTag(FName(
                UTF8_TO_TCHAR(UnmadeCore::UnansweredRoadBeats[i].publicTag))))continue;
            FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeRoadSight),false);
            Sight.AddIgnoredActor(Player);Sight.AddIgnoredActor(*It);
            if(GetWorld()->LineTraceTestByChannel(
                From+FVector(0,0,50),
                It->GetActorLocation()+FVector(0,0,50),
                ECC_Visibility,Sight))continue;
            Best=i;distance=Dist;break;
        }
    }
    if(Best<0)return false;
    const auto& Chapter=UnmadeCore::UnansweredRoadBeats[Best];
    const auto Evidence=GetUnansweredRoadEvidence();
    const bool IsKnown=Best==0?Evidence.openingWitnessed:
        UnmadeCore::RoadHas(Evidence,Chapter.realm);
    if(GEngine)
    {
        const FString Text=FString::Printf(
            TEXT("THE UNANSWERED ROAD — %s | %s | %s"),
            UTF8_TO_TCHAR(Chapter.chapter),
            UTF8_TO_TCHAR(IsKnown?Chapter.witnessedDiscovery:Chapter.localQuestion),
            UTF8_TO_TCHAR(IsKnown?Chapter.nextDirection:
                "The first local story must be genuinely resolved before its secret can be explained."));
        GEngine->AddOnScreenDebugMessage(-1,16.f,FColor::Cyan,Text);
        GEngine->AddOnScreenDebugMessage(-1,14.f,FColor::Yellow,
                                        DescribeUnansweredRoad(Player));
    }
    return true; // Story reading is freely revisitable and NEVER an item farm.
}
