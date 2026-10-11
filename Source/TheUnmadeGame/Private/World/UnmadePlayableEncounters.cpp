#include "World/UnmadePrototypeHub.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

// Optional combat grounds off the civilian streets; no waves or endless respawning.
void AUnmadePrototypeHub::BuildPlayableEncounters()
{
    if(!GetWorld())return;
    struct FSite {
        const TCHAR* Id;
        const TCHAR* Label;
        FVector Center;
        FColor Accent;
        const TCHAR* Creatures[3];
        const TCHAR* Material;
    };
    const FSite Sites[]={
        {TEXT("Reach.AshRoad"),TEXT("ASH ROAD"),FVector(-2250,1940,0),
            FColor(222,144,85),{TEXT("Cinderjaw"),TEXT("Pale Strider"),TEXT("Cinderjaw")},TEXT("MI_AshBone")},
        {TEXT("Reach.GlassCut"),TEXT("GLASS CUT"),FVector(2350,1920,0),
            FColor(137,190,240),{TEXT("Veilwatch"),TEXT("Gleam Moth"),TEXT("Veilwatch")},TEXT("MI_PaleGlass")},
        {TEXT("Bellwold.Forgotten"),TEXT("FORGOTTEN BELLS"),FVector(-20200,1700,0),
            FColor(222,200,148),{TEXT("Hollow Hound"),TEXT("Grief Lantern"),TEXT("Hollow Hound")},TEXT("MI_BellPatina")},
        {TEXT("Paperhaven.Torn"),TEXT("TORN ARCHIVE"),FVector(20300,1900,0),
            FColor(187,161,230),{TEXT("Inkmaw"),TEXT("Redaction Eye"),TEXT("Inkmaw")},TEXT("MI_ArchiveInk")},
        {TEXT("Saltwake.Saltglass"),TEXT("SALTGLASS QUAY"),FVector(-2100,-48000,0),
            FColor(109,207,215),{TEXT("Saltglass Crawler"),TEXT("Drowned Watch"),TEXT("Saltglass Crawler")},TEXT("MI_SaltFoam")},
        {TEXT("Cinderhold.Slag"),TEXT("BLACKSLAG FIELD"),FVector(-2100,52000,0),
            FColor(246,126,69),{TEXT("Ashmaw"),TEXT("Kiln Wisp"),TEXT("Ashmaw")},TEXT("MI_KilnSlag")}
    };
    const char* GuideIds[]={
        "npc.reach.ashroad.scout.001","npc.reach.glasscut.scout.001",
        "npc.bellwold.forgotten.scout.001","npc.paperhaven.torn.scout.001",
        "npc.saltwake.saltglass.scout.001","npc.cinderhold.blackslag.scout.001"};
    const char* GuideNames[]={
        "Orren the ash-runner","Vesh the glass warden",
        "Deryn the bell seeker","Isti the torn-page keeper",
        "Halven the net seeker","Varda the slag scout"};
    const char* GuideLines[]={
        "Two jaws at the old arch. The third sound is the road taking a breath.",
        "The watchers answer any light. Stand near the old stone for cover.",
        "The hounds hear a bell even after the iron is taken away.",
        "I read the missing names here. Something read me back.",
        "The crawler comes ashore at dusk. The drowned watch does not sleep.",
        "The coal beast stalks the kiln road. Bes's house lies safely east."};
    int32 GuideIndex=0;
    for(const FSite& Site:Sites)
    {
        const FString Id(Site.Id);
        SpawnBlock(Site.Center+FVector(0,0,-17),FVector(11,8,.35),
            FName(*(Id+TEXT(".Floor"))));
        SpawnBlock(Site.Center+FVector(-560,250,190),FVector(.8,.8,3.8),
            FName(*(Id+TEXT(".WestPillar"))));
        SpawnBlock(Site.Center+FVector(560,250,220),FVector(.8,.8,4.4),
            FName(*(Id+TEXT(".EastPillar"))));
        SpawnBlock(Site.Center+FVector(0,410,365),FVector(12,.55,.6),
            FName(*(Id+TEXT(".FallenArch"))));
        SpawnBlock(Site.Center+FVector(-720,-100,55),FVector(1.5,.8,1.1),
            FName(*(Id+TEXT(".WestCover"))));
        SpawnBlock(Site.Center+FVector(720,-100,55),FVector(1.4,.9,1.1),
            FName(*(Id+TEXT(".EastCover"))));
        // Six unique, collision-bearing landmark silhouettes; recognizable even
        // without optional authored PBR material instances. No single generic box.
        const auto AddLandmark=[&](const TCHAR* Suffix,const TCHAR* EngineShape,
            FVector Offset,FVector Scale,FRotator Orientation,bool bSolid)
        {
            if(AStaticMeshActor* Shape=GetWorld()->SpawnActor<AStaticMeshActor>(
                Site.Center+Offset,Orientation))
            {
                Shape->Tags.AddUnique(FName(*(Id+Suffix)));
                if(UStaticMeshComponent* Geometry=Shape->GetStaticMeshComponent())
                {
                    Geometry->SetMobility(EComponentMobility::Movable);
                    const FString MeshPath=FString::Printf(
                        TEXT("/Engine/BasicShapes/%s.%s"),EngineShape,EngineShape);
                    if(UStaticMesh* Mesh=LoadObject<UStaticMesh>(nullptr,*MeshPath))
                        Geometry->SetStaticMesh(Mesh);
                    Geometry->SetWorldScale3D(Scale);
                    Geometry->SetCollisionEnabled(bSolid
                        ?ECollisionEnabled::QueryAndPhysics:ECollisionEnabled::NoCollision);
                    const FString MatName(Site.Material);
                    const FString MatPath=FString::Printf(
                        TEXT("/Game/UnmadeProduction/Materials/%s.%s"),
                        *MatName,*MatName);
                    if(UMaterialInterface* Mat=LoadObject<UMaterialInterface>(nullptr,*MatPath))
                        Geometry->SetMaterial(0,Mat);
                }
            }
        };
        if(GuideIndex==0) // Ash Road: fossil ribs and a fallen waystone.
        {
            AddLandmark(TEXT(".RibOne"),TEXT("Cylinder"),FVector(-330,300,165),
                FVector(.42,.42,3.3),FRotator(0,16,26),true);
            AddLandmark(TEXT(".RibTwo"),TEXT("Cylinder"),FVector(310,325,165),
                FVector(.38,.4,3.5),FRotator(0,-18,-24),true);
            AddLandmark(TEXT(".Waystone"),TEXT("Cone"),FVector(0,390,105),
                FVector(1.5,1.5,2.2),FRotator(0,0,80),true);
        }
        else if(GuideIndex==1) // Glass Cut: fractured prisms and luminous shard.
        {
            AddLandmark(TEXT(".PrismTall"),TEXT("Cone"),FVector(-260,300,245),
                FVector(1.45,1.0,5.0),FRotator(0,23,8),true);
            AddLandmark(TEXT(".PrismBroken"),TEXT("Cone"),FVector(310,350,135),
                FVector(1.3,1.3,2.8),FRotator(0,-20,45),true);
            AddLandmark(TEXT(".GlassEye"),TEXT("Sphere"),FVector(0,445,280),
                FVector(.65,.65,.85),FRotator::ZeroRotator,false);
        }
        else if(GuideIndex==2) // Forgotten Bells: hanging bell and hollow ring.
        {
            AddLandmark(TEXT(".BellVault"),TEXT("Cylinder"),FVector(0,360,185),
                FVector(2.0,2.0,3.6),FRotator(0,0,90),true);
            AddLandmark(TEXT(".SilentBell"),TEXT("Sphere"),FVector(0,360,285),
                FVector(1.1,1.1,1.5),FRotator::ZeroRotator,false);
            AddLandmark(TEXT(".Clapper"),TEXT("Cone"),FVector(0,360,142),
                FVector(.4,.4,1.2),FRotator::ZeroRotator,false);
        }
        else if(GuideIndex==3) // Torn Archive: shelves, fallen folios and redaction eye.
        {
            AddLandmark(TEXT(".ArchiveSpineA"),TEXT("Cube"),FVector(-280,270,178),
                FVector(.6,1.4,3.5),FRotator(0,14,0),true);
            AddLandmark(TEXT(".ArchiveSpineB"),TEXT("Cube"),FVector(300,330,136),
                FVector(.6,1.25,2.8),FRotator(0,-9,0),true);
            AddLandmark(TEXT(".OpenPage"),TEXT("Cube"),FVector(0,390,115),
                FVector(2.8,1.7,.11),FRotator(0,21,28),false);
        }
        else if(GuideIndex==4) // Saltglass: drowned pilings and overturned hull.
        {
            AddLandmark(TEXT(".SeaPileA"),TEXT("Cylinder"),FVector(-305,350,188),
                FVector(.6,.6,3.6),FRotator(0,0,12),true);
            AddLandmark(TEXT(".SeaPileB"),TEXT("Cylinder"),FVector(305,350,160),
                FVector(.6,.6,3.1),FRotator(0,0,-10),true);
            AddLandmark(TEXT(".WreckHull"),TEXT("Cylinder"),FVector(0,450,98),
                FVector(3.6,1.25,1.55),FRotator(90,0,0),true);
        }
        else // Blackslag: forked furnaces and a cooling slag sphere.
        {
            AddLandmark(TEXT(".Chimney"),TEXT("Cylinder"),FVector(-260,340,310),
                FVector(1.1,1.1,6.1),FRotator(0,0,-8),true);
            AddLandmark(TEXT(".BrokenKiln"),TEXT("Cylinder"),FVector(310,260,165),
                FVector(1.8,1.8,3.2),FRotator(0,0,40),true);
            AddLandmark(TEXT(".SlagHeart"),TEXT("Sphere"),FVector(0,440,124),
                FVector(1.5,1.5,1.65),FRotator::ZeroRotator,false);
        }
        if(ATextRenderActor* Sign=GetWorld()->SpawnActor<ATextRenderActor>(
            Site.Center+FVector(0,-650,185),FRotator(0,90,0)))
        {
            Sign->Tags.AddUnique(FName(*(Id+TEXT(".Sign"))));
            if(UTextRenderComponent* Text=Sign->GetTextRender())
            {
                Text->SetText(FText::FromString(Site.Label));
                Text->SetWorldSize(30.f);
                Text->SetTextRenderColor(Site.Accent);
                Text->SetHorizontalAlignment(EHTA_Center);
            }
        }
        if(APointLight* Lamp=GetWorld()->SpawnActor<APointLight>(
            Site.Center+FVector(0,300,345),FRotator::ZeroRotator))
        {
            Lamp->Tags.AddUnique(FName(*(Id+TEXT(".Lantern"))));
            Lamp->PointLightComponent->SetIntensity(1750.f);
            Lamp->PointLightComponent->SetAttenuationRadius(950.f);
            Lamp->PointLightComponent->SetLightColor(FLinearColor(Site.Accent));
        }
        const FVector Spots[3]={FVector(-350,170,100),FVector(350,170,100),
                                 FVector(0,600,100)};
        for(int32 i=0;i<3;++i)
        {
            if(AUnmadeEnemyCharacter* Enemy=GetWorld()->SpawnActor<AUnmadeEnemyCharacter>(
                Site.Center+Spots[i],FRotator::ZeroRotator))
            {
                const bool bWatcher=i==1;
                Enemy->ConfigureStyle(bWatcher?UnmadeCore::EnemyStyle::Watcher:
                    UnmadeCore::EnemyStyle::Stalker);
                Enemy->ConfigureEncounter(
                    FName(*FString::Printf(TEXT("%s.Creature.%d"),Site.Id,i)),
                    Site.Creatures[i],FLinearColor(Site.Accent));
                Enemy->SetVisualScale(bWatcher?FVector(.7,.7,.9):FVector(1.1,.85,1.05));
            }
        }
        // A genuinely talkable individual for each field trip, away from its beasts.
        if(AUnmadeNpcCharacter* Guide=GetWorld()->SpawnActor<AUnmadeNpcCharacter>(
            Site.Center+FVector(0,-870,100),FRotator::ZeroRotator))
        {
            if(GuideIndex>=4)
            {
                const UnmadeCore::FrontierResident FrontierGuide={
                    GuideIds[GuideIndex],GuideNames[GuideIndex],
                    GuideIndex==4?UnmadeCore::Realm::WidowedRain:
                                  UnmadeCore::Realm::HearthBeneath,
                    UnmadeCore::NpcRole::Guard,UnmadeCore::NpcTemperament::Steady,
                    0,0,GuideLines[GuideIndex]};
                Guide->ConfigureFrontier(FrontierGuide);
            }
            else
            {
                const auto Town=GuideIndex==2?UnmadeCore::SettlementId::Bellwold:
                    GuideIndex==3?UnmadeCore::SettlementId::Paperhaven:
                                  UnmadeCore::SettlementId::Crossings;
                Guide->ConfigureIdentity(FName(UTF8_TO_TCHAR(GuideIds[GuideIndex])),
                    FString(UTF8_TO_TCHAR(GuideNames[GuideIndex])),
                    UnmadeCore::NpcRole::Guard,UnmadeCore::NpcTemperament::Steady,Town,
                    FString(UTF8_TO_TCHAR(GuideLines[GuideIndex])));
            }
        }
        ++GuideIndex;
    }

    // Physical quest affordances: prompts point to existing E/LOS stations.
    // They never alter wage or consent saves, and consume no input themselves.
    struct FPrompt{FVector At;const TCHAR* Id;const TCHAR* Text;};
    const FPrompt Prompts[]={
        {FVector(1205,50540,245),TEXT("Bes.Home.Prompt"),TEXT("E  INSPECT HOME / DELIVER HEAT")},
        {FVector(1430,50790,350),TEXT("Bes.Walkout.Prompt"),TEXT("E  REVIEW / CONFIRM WALKOUT")},
        {FVector(650,50300,270),TEXT("Bes.Wages.Prompt"),TEXT("E  REVIEW / CONFIRM WAGES")},
        {FVector(1230,-49640,285),TEXT("Mera.Guarantee.Prompt"),TEXT("E  REVIEW / CONFIRM GUARANTEE")}
    };
    for(const FPrompt& Prompt:Prompts)
        if(ATextRenderActor* Sign=GetWorld()->SpawnActor<ATextRenderActor>(
            Prompt.At,FRotator(0,90,0)))
        {
            Sign->Tags.AddUnique(FName(Prompt.Id));
            if(UTextRenderComponent* Label=Sign->GetTextRender())
            {
                Label->SetText(FText::FromString(Prompt.Text));
                Label->SetWorldSize(19.f);
                Label->SetHorizontalAlignment(EHTA_Center);
                Label->SetTextRenderColor(FColor(242,199,117));
                Label->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
        }
}
