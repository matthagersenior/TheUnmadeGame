#include "UI/UnmadeRpgFeedback.h"
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
#include "World/UnmadeFieldSurveySave.h"
#include "Save/UnmadePrototypeSave.h"
#include "Save/UnmadeCaptureRules.h"
#include "Player/UnmadeCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"

namespace {
const FVector SurveyCenters[6]={FVector(-2250,1940,0),FVector(2350,1920,0),
    FVector(-20200,1700,0),FVector(20300,1900,0),
    FVector(-2100,-48000,0),FVector(-2100,52000,0)};
const TCHAR* SurveyReturns[6]={
    TEXT("Orren: The ribs hid a maintained footpath. I will mark it for travelers."),
    TEXT("Vesh: A glass shutter, not a miracle. Now we have another shelter from the watchers."),
    TEXT("Deryn: You found the service passage. We can tend the bell without crossing the hounds."),
    TEXT("Isti: The shelf latch was left for a keeper. I will record the route, not invent its missing owner."),
    TEXT("Halven: The wreck hatch still moves. That gives the net crews a covered way ashore."),
    TEXT("Varda: The kiln screen opens. Bes's hearth and his unpaid wages are a separate matter.")};
}

bool AUnmadePrototypeHub::TryFieldSurveyInteraction(AUnmadeCharacter* Player,
    double CompetingNpcDistanceSq)
{
    if(!GetWorld()||!IsValid(Player)||!IsValid(Player->GetCombat())||
       Player->GetCombat()->IsDefeated()||!bFieldSurveySaveUsable||FieldSurveyStages.Num()!=6)return false;
    for(int32 i=0;i<6;++i)
    {
        if(i==5&&FieldSurveyStages[i]==2)continue; // Varda remains available for authored dialogue.
        if(FieldSurveyStages[i]==2)continue; // All completed scouts regain normal conversations.
        const FVector Switch=SurveyCenters[i]+FVector(-780,-430,80);
        const FVector Report=SurveyCenters[i]+FVector(0,-870,100);
        const bool bReporting=FieldSurveyStages[i]>0;
        const FVector Target=bReporting?Report:Switch;
        const double Distance=FVector::DistSquared(Player->GetActorLocation(),Target);
        if(Distance>FMath::Square(260.f)||Distance>CompetingNpcDistanceSq)continue;
        FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeSurveySight),false);
        Params.AddIgnoredActor(Player);
        for(TActorIterator<AUnmadeNpcCharacter> Npc(GetWorld());Npc;++Npc)
            if(FVector::DistSquared(Npc->GetActorLocation(),Target)<FMath::Square(50.f))
                Params.AddIgnoredActor(*Npc);
        if(GetWorld()->LineTraceTestByChannel(Player->GetActorLocation()+FVector(0,0,40),
            Target+FVector(0,0,40),ECC_Visibility,Params))continue;
        const int32 Previous=FieldSurveyStages[i];
        if(Previous<2)
        {
            if(!UnmadeCore::CommitFieldSurveyStage(FieldSurveyStages,i,[&]{return WriteWorldSnapshot();}))
            {
                if(GEngine)UnmadeRpgFeedback(-1,4.f,FColor::Red,
                    TEXT("Fieldwork could not save. No route or objective changed; try again."));
                return true;
            }
            if(FieldSurveyGates.IsValidIndex(i)&&FieldSurveyGates[i].IsValid())
            {
                FieldSurveyGates[i]->SetActorEnableCollision(false);
                FieldSurveyGates[i]->SetActorHiddenInGame(true);
            }
        }
        if(FieldSurveySigns.IsValidIndex(i)&&FieldSurveySigns[i].IsValid())
            FieldSurveySigns[i]->GetTextRender()->SetText(FText::FromString(
                FieldSurveyStages[i]==1?TEXT("RETURN TO THE SCOUT - E TO REPORT"):
                TEXT("FIELD SURVEY COMPLETE - COVERED PASSAGE OPEN")));
        if(GEngine)UnmadeRpgFeedback(-1,7.f,FColor::Cyan,
            Previous==0?TEXT("Passage opened. Return to the nearby scout and press E to report."):
                SurveyReturns[i]);
        return true;
    }
    return false;
}


// Optional combat grounds off the civilian streets; no waves or endless respawning.
void AUnmadePrototypeHub::BuildPlayableEncounters()
{
    if(!GetWorld())return;
    FieldSurveyStages.Init(0,6);
    auto* Save=Cast<UUnmadePrototypeSave>(UnmadeSave::LoadGameFromSlot());
    if(!Save&&UnmadeSave::HasRecord())bFieldSurveySaveUsable=false;
    {
        if(Save&&Save->bHasFieldSurveys){bFieldSurveySaveUsable=false;
            if(Save->FieldSurveyStages.Num()==6){bool valid=true;
                for(int32 stage:Save->FieldSurveyStages)if(stage<0||stage>2)valid=false;
                if(valid){FieldSurveyStages=Save->FieldSurveyStages;bFieldSurveySaveUsable=true;}}}
        else if(bFieldSurveySaveUsable&&UnmadeSave::SlotName()==TEXT("UnmadePrototypeNPC")&&UGameplayStatics::DoesSaveGameExist(TEXT("UnmadeOptionalFieldSurveys"),0))
        { // Read-only migration for the original profile; other profiles start independently.
            auto* legacy=Cast<UUnmadeFieldSurveySave>(UGameplayStatics::LoadGameFromSlot(TEXT("UnmadeOptionalFieldSurveys"),0));
            bFieldSurveySaveUsable=false;
            if(legacy&&legacy->Version==1&&legacy->Stages.Num()==6){bool valid=true;
                for(int32 stage:legacy->Stages)if(stage<0||stage>2)valid=false;
                if(valid){FieldSurveyStages=legacy->Stages;bFieldSurveySaveUsable=true;}}
        }
    }
    FieldSurveyGates.SetNum(6);
    FieldSurveySigns.SetNum(6);
    HabitatShutters.SetNum(6);
    HabitatInspectSigns.SetNum(6);
    HabitatResponseLights.SetNum(6);
    HabitatInspected.fill(false);
    HabitatLastCue.fill(-1);
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
    using Kind=UnmadeCore::CreatureKind;
    const Kind Species[6][3]={
        {Kind::Cinderjaw,Kind::PaleStrider,Kind::Cinderjaw},
        {Kind::Veilwatch,Kind::GleamMoth,Kind::Veilwatch},
        {Kind::HollowHound,Kind::GriefLantern,Kind::HollowHound},
        {Kind::Inkmaw,Kind::RedactionEye,Kind::Inkmaw},
        {Kind::SaltglassCrawler,Kind::DrownedWatch,Kind::SaltglassCrawler},
        {Kind::Ashmaw,Kind::KilnWisp,Kind::Ashmaw}};
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
        // A physical covered side passage. E operates its adjacent latch; the
        // optional quest requires returning to this site's scout in person.
        SpawnBlock(Site.Center+FVector(-900,130,0),FVector(3,9,.35),
            FName(*(Id+TEXT(".SurveyPath"))));
        SpawnBlock(Site.Center+FVector(-1030,130,120),FVector(.3,9,2.4),
            FName(*(Id+TEXT(".SurveyWall"))));
        if(AStaticMeshActor* Gate=GetWorld()->SpawnActor<AStaticMeshActor>(
            Site.Center+FVector(-900,-300,120),FRotator::ZeroRotator))
        {
            Gate->Tags.AddUnique(FName(*(Id+TEXT(".SurveyGate"))));
            auto* Mesh=Gate->GetStaticMeshComponent();
            Mesh->SetMobility(EComponentMobility::Movable);
            Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));
            Mesh->SetWorldScale3D(FVector(2.5,.3,2.4));
            FieldSurveyGates[GuideIndex]=Gate;
            Gate->SetActorEnableCollision(FieldSurveyStages[GuideIndex]==0);
            Gate->SetActorHiddenInGame(FieldSurveyStages[GuideIndex]>0);
        }
        if(ATextRenderActor* Latch=GetWorld()->SpawnActor<ATextRenderActor>(
            Site.Center+FVector(-780,-430,80),FRotator(0,90,0)))
        {
            FieldSurveySigns[GuideIndex]=Latch;
            Latch->GetTextRender()->SetWorldSize(17.f);
            Latch->GetTextRender()->SetText(FText::FromString(
                FieldSurveyStages[GuideIndex]==0?TEXT("E - OPEN COVERED PASSAGE / BEGIN SURVEY"):
                FieldSurveyStages[GuideIndex]==1?TEXT("RETURN TO THE SCOUT - E TO REPORT"):
                TEXT("FIELD SURVEY COMPLETE - COVERED PASSAGE OPEN")));
        }
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
        // Habitat placement uses the existing source-grounded creature roster.
        // Habitat silhouettes are explicitly PROVISIONAL world art, not a new
        // claim about sealed origins or a permanent spawning ecosystem.
        const auto& Habitat=UnmadeCore::HabitatSites[GuideIndex];
        const auto BuildEncounterHabitat=[&]()
        {
            const double N=Habitat.denOffsetY;
            if(GuideIndex==0) // JawDen: gnawed stone cavity, claw-scraped ribs.
            {
                AddLandmark(TEXT(".Habitat.JawDen.Hollow"),TEXT("Cylinder"),FVector(0,N+260,285),
                    FVector(3.6,2.3,3.1),FRotator(0,0,90),true);
                for(int j=0;j<5;++j)
                    AddLandmark(*FString::Printf(TEXT(".Habitat.JawDen.Rib%d"),j),
                        TEXT("Cylinder"),FVector(-260+j*130,N+40,165+j%2*25),
                        FVector(.20,.24,2.7),FRotator(0,12*j,(-24+j*10)),true);
                AddLandmark(TEXT(".Habitat.JawDen.Tracks"),TEXT("Cube"),
                    FVector(120,N-260,26),FVector(2.1,1.35,.2),FRotator::ZeroRotator,false);
            }
            else if(GuideIndex==1) // GlassRoost: prisms block watcher sightlines.
            {
                for(int j=0;j<5;++j)
                    AddLandmark(*FString::Printf(TEXT(".Habitat.GlassRoost.Spire%d"),j),
                        TEXT("Cone"),FVector((j-2)*140,N+45+j*18,180+j*18),
                        FVector(.95,1.25,3.8+j*.65),FRotator(0,10*j,(-12+j*7)),true);
                AddLandmark(TEXT(".Habitat.GlassRoost.Perch"),TEXT("Cylinder"),
                    FVector(0,N+130,280),FVector(3.6,.85,.48),FRotator(0,0,90),true);
                AddLandmark(TEXT(".Habitat.GlassRoost.SightBrace"),TEXT("Cube"),
                    FVector(0,N-170,185),FVector(.45,3,3.4),FRotator(0,23,0),true);
            }
            else if(GuideIndex==2) // HoundUndercroft, away from public refuge.
            {
                AddLandmark(TEXT(".Habitat.HoundUndercroft.Roof"),TEXT("Cylinder"),
                    FVector(0,N+235,295),FVector(4.5,1.4,2.1),FRotator(0,0,90),true);
                for(int side=-1;side<=1;side+=2)
                {
                    AddLandmark(*FString::Printf(TEXT(".Habitat.HoundUndercroft.Pillar%d"),side),
                        TEXT("Cylinder"),FVector(side*310,N-85,147),
                        FVector(.75,.75,3.0),FRotator(0,0,side*14),true);
                    AddLandmark(*FString::Printf(TEXT(".Habitat.HoundUndercroft.Trail%d"),side),
                        TEXT("Cube"),FVector(side*210,N-230,20),
                        FVector(.7,1.8,.16),FRotator(0,side*18,0),false);
                }
                AddLandmark(TEXT(".Habitat.HoundUndercroft.EmptyCradle"),TEXT("Cube"),
                    FVector(0,N+85,330),FVector(2.0,.45,.35),FRotator::ZeroRotator,true);
            }
            else if(GuideIndex==3) // ArchiveBurrow: paper-worn side galleries.
            {
                for(int j=0;j<6;++j)
                    AddLandmark(*FString::Printf(TEXT(".Habitat.ArchiveBurrow.Shelf%d"),j),
                        TEXT("Cube"),FVector((j%3-1)*210,N+90+(j/3)*220,160),
                        FVector(.45,1.8,3.2),FRotator(0,(-14+j*7),0),true);
                AddLandmark(TEXT(".Habitat.ArchiveBurrow.Rent"),TEXT("Cube"),
                    FVector(0,N+195,325),FVector(5.2,.4,.5),FRotator(0,19,16),true);
            }
            else if(GuideIndex==4) // TideCave: sheltered tide-cut stone and pilings.
            {
                AddLandmark(TEXT(".Habitat.TideCave.Shell"),TEXT("Cylinder"),
                    FVector(0,N+235,270),FVector(4.6,2.6,2.0),FRotator(0,0,90),true);
                for(int j=0;j<5;++j)
                    AddLandmark(*FString::Printf(TEXT(".Habitat.TideCave.Pile%d"),j),
                        TEXT("Cylinder"),FVector((j-2)*145,N-100+((j%2)*110),155),
                        FVector(.35,.4,3.1),FRotator(0,0,(j-2)*11),true);
                AddLandmark(TEXT(".Habitat.TideCave.WaterMark"),TEXT("Cube"),
                    FVector(0,N-280,15),FVector(6.2,1.0,.14),FRotator::ZeroRotator,false);
            }
            else // SlagMaw: blackslag chimney mouth with substantial cover.
            {
                AddLandmark(TEXT(".Habitat.SlagMaw.Mouth"),TEXT("Cylinder"),
                    FVector(0,N+240,300),FVector(3.3,2.1,2.5),FRotator(90,0,0),true);
                for(int j=0;j<4;++j)
                    AddLandmark(*FString::Printf(TEXT(".Habitat.SlagMaw.Vent%d"),j),
                        TEXT("Cone"),FVector((j-2)*190,N+90,285+j%2*90),
                        FVector(.75,.90,4.0+j*.60),FRotator(0,j*19,j*9),true);
                AddLandmark(TEXT(".Habitat.SlagMaw.Brace"),TEXT("Cube"),
                    FVector(0,N-215,140),FVector(5.3,.65,2.3),FRotator(0,16,0),true);
            }
            // Keep the shutter well ahead of the third creature's spawn (Y=600),
            // so the player-operated alcove is not accidentally a spawn cage.
            // Player can take physical cover on the approach. The reactive
            // enclosure is an inspectable movable obstruction: no quest gate.
            for(int side=-1;side<=1;side+=2)
                AddLandmark(*FString::Printf(TEXT(".Habitat.Cover%d"),side),
                    TEXT("Cube"),FVector(side*490,N-175,120),
                    FVector(1.2,1.8,2.2),FRotator(0,side*27,0),true);
            if(AStaticMeshActor* Shutter=GetWorld()->SpawnActor<AStaticMeshActor>(
                Site.Center+FVector(0,N-340,135),FRotator::ZeroRotator))
            {
                Shutter->Tags.AddUnique(FName(*(Id+TEXT(".Habitat.ShelterShutter"))));
                auto* Mesh=Shutter->GetStaticMeshComponent();
                Mesh->SetMobility(EComponentMobility::Movable);
                Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));
                Mesh->SetWorldScale3D(FVector(2.0,.28,2.6));
                Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                HabitatShutters[GuideIndex]=Shutter;
            }
            if(ATextRenderActor* Inspect=GetWorld()->SpawnActor<ATextRenderActor>(
                Site.Center+FVector(0,N-380,105),FRotator(0,90,0)))
            {
                Inspect->Tags.AddUnique(FName(*(Id+TEXT(".Habitat.Inspect"))));
                Inspect->GetTextRender()->SetWorldSize(17.f);
                Inspect->GetTextRender()->SetText(FText::FromString(
                    FString::Printf(TEXT("E - INSPECT %s TRACKS / SHELTER"),UTF8_TO_TCHAR(Habitat.kind))));
                HabitatInspectSigns[GuideIndex]=Inspect;
            }
            if(APointLight* Response=GetWorld()->SpawnActor<APointLight>(
                Site.Center+FVector(0,N,340),FRotator::ZeroRotator))
            {
                Response->Tags.AddUnique(FName(*(Id+TEXT(".Habitat.ResponseLight"))));
                Response->PointLightComponent->SetIntensity(90.f);
                Response->PointLightComponent->SetAttenuationRadius(650.f);
                Response->PointLightComponent->SetLightColor(FLinearColor(Site.Accent));
                HabitatResponseLights[GuideIndex]=Response;
            }
        };
        BuildEncounterHabitat();
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
                Enemy->ConfigureCreature(Species[GuideIndex][i],i==2);
                const bool bWatcher=Enemy->GetEnemyStyle()==UnmadeCore::EnemyStyle::Watcher;
                Enemy->ConfigureEncounter(
                    FName(*FString::Printf(TEXT("%s.Creature.%d"),Site.Id,i)),
                    GuideIndex==5&&i==2?TEXT("Sootbound Elder"):
                        i==2?FString(Site.Creatures[i])+TEXT(" - Elder"):FString(Site.Creatures[i]),
                    FLinearColor(Site.Accent));
                if(GuideIndex==5&&i==2)Enemy->ConfigureCinderholdElder();
                // Foraging/rest now returns toward the real placed habitat
                // forecourt, rather than the arbitrary original spawn point.
                Enemy->ConfigureHabitatHome(Site.Center+FVector((i-1)*280,
                    Habitat.denOffsetY-240+(i%2)*70,100));
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


bool AUnmadePrototypeHub::TryHabitatInteraction(AUnmadeCharacter* Player,
    double CompetingNpcDistanceSq)
{
    if(!GetWorld()||!IsValid(Player)||!IsValid(Player->GetCombat())||
       Player->GetCombat()->IsDefeated()||HabitatInspectSigns.Num()!=6)return false;
    for(int32 i=0;i<6;++i)
    {
        const auto& Site=UnmadeCore::HabitatSites[i];
        const FVector Marker(Site.centerX,Site.centerY+Site.denOffsetY-380.0,105);
        const double DistSq=FVector::DistSquared(Player->GetActorLocation(),Marker);
        if(DistSq>FMath::Square(Site.inspectionRadius)||DistSq>CompetingNpcDistanceSq)continue;
        FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeHabitatInspectSight),false);
        Sight.AddIgnoredActor(Player);
        if(HabitatInspectSigns.IsValidIndex(i)&&HabitatInspectSigns[i].IsValid())
            Sight.AddIgnoredActor(HabitatInspectSigns[i].Get());
        const bool bClear=!GetWorld()->LineTraceTestByChannel(
            Player->GetActorLocation()+FVector(0,0,50),Marker+FVector(0,0,60),
            ECC_Visibility,Sight);
        if(!UnmadeCore::HabitatCanInspect(Site,FMath::Sqrt(DistSq),bClear))continue;
        if(HabitatInspected[i])
        {
            if(GEngine)UnmadeRpgFeedback(-1,4.f,FColor::Cyan,
                TEXT("A previously inspected shelter alcove remains open. The den is still occupied."));
            return true;
        }
        // Physical world reaction, not a grant of combat skill or a quest
        // success. Inspecting makes the alcove cover available to use.
        HabitatInspected[i]=true;
        if(HabitatShutters.IsValidIndex(i)&&HabitatShutters[i].IsValid())
        {
            HabitatShutters[i]->SetActorEnableCollision(false);
            HabitatShutters[i]->SetActorHiddenInGame(true);
        }
        if(HabitatInspectSigns[i].IsValid())
            HabitatInspectSigns[i]->GetTextRender()->SetText(
                FText::FromString(TEXT("SHELTER ALCOVE OPEN - USE COVER / WATCH THE DEN")));
        if(GEngine)UnmadeRpgFeedback(-1,7.f,FColor::Cyan,
            FString::Printf(TEXT("HABITAT OBSERVED: %s | Side alcove opened. No power copied."),
                UTF8_TO_TCHAR(Site.inspection)));
        return true;
    }
    return false;
}
