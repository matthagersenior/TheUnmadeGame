#include "World/UnmadePrototypeHub.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"
#include "Engine/World.h"

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
    };
    const FSite Sites[]={
        {TEXT("Reach.AshRoad"),TEXT("ASH ROAD"),FVector(-2250,1940,0),
            FColor(222,144,85),{TEXT("Cinderjaw"),TEXT("Pale Strider"),TEXT("Cinderjaw")}},
        {TEXT("Reach.GlassCut"),TEXT("GLASS CUT"),FVector(2350,1920,0),
            FColor(137,190,240),{TEXT("Veilwatch"),TEXT("Gleam Moth"),TEXT("Veilwatch")}},
        {TEXT("Bellwold.Forgotten"),TEXT("FORGOTTEN BELLS"),FVector(-20200,1700,0),
            FColor(222,200,148),{TEXT("Hollow Hound"),TEXT("Grief Lantern"),TEXT("Hollow Hound")}},
        {TEXT("Paperhaven.Torn"),TEXT("TORN ARCHIVE"),FVector(20300,1900,0),
            FColor(187,161,230),{TEXT("Inkmaw"),TEXT("Redaction Eye"),TEXT("Inkmaw")}},
        {TEXT("Saltwake.Saltglass"),TEXT("SALTGLASS QUAY"),FVector(-2100,-48000,0),
            FColor(109,207,215),{TEXT("Saltglass Crawler"),TEXT("Drowned Watch"),TEXT("Saltglass Crawler")}},
        {TEXT("Cinderhold.Slag"),TEXT("BLACKSLAG FIELD"),FVector(-2100,52000,0),
            FColor(246,126,69),{TEXT("Ashmaw"),TEXT("Kiln Wisp"),TEXT("Ashmaw")}}
    };
    for(const FSite& Site:Sites)
    {
        const FString Id(Site.Id);
        SpawnBlock(Site.Center+FVector(0,0,-40),FVector(11,8,.18),
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
    }
}
