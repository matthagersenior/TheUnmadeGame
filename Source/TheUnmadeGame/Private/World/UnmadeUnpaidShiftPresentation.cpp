#include "World/UnmadePrototypeHub.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Engine/StaticMeshActor.h"
#include "World/UnmadeUnpaidShiftSceneRules.h"
#include "UI/UnmadeUnpaidShiftSubtitleWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "UObject/UObjectGlobals.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "Player/UnmadeCharacter.h"
#include "EngineUtils.h"

void AUnmadePrototypeHub::AnimateBesWorkingCart()
{
    if(!GetWorld()||!BesWorkingCart.IsValid())return;
    // A small true runtime environmental movement, not a fake claim that
    // skeletal performance capture or bespoke delivery animation exists.
    const bool ShiftInProgress=
        !BesHousehold.OnWalkout() &&
        Clock.Phase()!=UnmadeCore::DayPhase::Night;
    const float Sweep=ShiftInProgress
        ?120.f*FMath::Sin(GetWorld()->GetTimeSeconds()*.48f):0.f;
    const FVector Target=BesCartRest+FVector(0,Sweep,0);
    BesWorkingCart->SetActorLocation(Target,false,nullptr,ETeleportType::TeleportPhysics);
    if(BesVisibleBasket.IsValid())
        BesVisibleBasket->SetActorLocation(Target+FVector(0,0,66),false,nullptr,
            ETeleportType::TeleportPhysics);
}
void AUnmadePrototypeHub::RestoreBesCamera()
{
    if(GetWorld())
    {
        if(APlayerController* PC=GetWorld()->GetFirstPlayerController())
        {
            AActor* Original=BesOriginalViewTarget.Get();
            if(!IsValid(Original))Original=PC->GetPawn();
            if(IsValid(Original)&&IsValid(BesShotCamera)&&
               PC->GetViewTarget()==BesShotCamera.Get())
                PC->SetViewTargetWithBlend(Original,.25f);
        }
    }
    if(IsValid(BesShotCamera))
    {
        // Allow time for the return blend to finish before removing the rig.
        BesShotCamera->SetLifeSpan(.40f);
        BesShotCamera=nullptr;
    }
    BesOriginalViewTarget.Reset();
    if(BesStagedSpeaker.IsValid())BesStagedSpeaker->EndDirectedBlocking();
    BesStagedSpeaker.Reset();
}

void AUnmadePrototypeHub::FrameBesDirectedBeat(FName SpeakerId,const FString& Cue,const FString& Blocking,float Seconds)
{
    if(!GetWorld() ||
       BesActiveScene>=static_cast<int32>(UnmadeCore::UnpaidScene::MaskPaid))return;
    APlayerController* PC=GetWorld()->GetFirstPlayerController();
    AUnmadeCharacter* Player=PC?Cast<AUnmadeCharacter>(PC->GetPawn()):nullptr;
    if(!IsValid(Player))return;

    if(BesStagedSpeaker.IsValid())BesStagedSpeaker->EndDirectedBlocking();
    BesStagedSpeaker.Reset();
    FVector Focus=Player->GetActorLocation()+Player->GetActorForwardVector()*240.f;
    if(SpeakerId!=FName(TEXT("stage")))
    {
        bool bSpeakerPresent=false;
        for(TActorIterator<AUnmadeNpcCharacter> It(GetWorld());It;++It)
        {
            if(It->GetStableId()!=SpeakerId)continue;
            if(FVector::DistSquared2D(It->GetActorLocation(),Player->GetActorLocation())
                >FMath::Square(1500.f))return; // Never magically cut to a remote actor.
            Focus=It->GetActorLocation();
            BesStagedSpeaker=*It;
            It->BeginDirectedBlocking(Player->GetActorLocation(),FMath::Max(.5f,Seconds),
                Blocking.Contains(TEXT("hand"))||Blocking.Contains(TEXT("ledger"))
                    ?FName(TEXT("Gesture")):FName(TEXT("Speak")));
            bSpeakerPresent=true;
            break;
        }
        if(!bSpeakerPresent)return;
    }
    Focus.Z+=100.f;

    if(!IsValid(BesShotCamera))
    {
        BesOriginalViewTarget=PC->GetViewTarget();
        BesShotCamera=GetWorld()->SpawnActor<ACameraActor>(
            Player->GetActorLocation()+FVector(-270,0,230),FRotator::ZeroRotator);
    }
    if(!IsValid(BesShotCamera))return;
    const bool bWide=Cue.Contains(TEXT("wide"))||Cue.Contains(TEXT("roof"));
    const bool bDetail=Cue.Contains(TEXT("hand"))||Cue.Contains(TEXT("insert"))||
        Cue.Contains(TEXT("detail"))||Cue.Contains(TEXT("basket"));
    const bool bClose=Cue.Contains(TEXT("close"))||Cue.Contains(TEXT("portrait"));
    const float Distance=bWide?630.f:bDetail?165.f:bClose?220.f:350.f;
    const float Height=bWide?230.f:bDetail?60.f:bClose?90.f:140.f;
    FVector Toward=(Focus-Player->GetActorLocation()).GetSafeNormal2D();
    if(Toward.IsNearlyZero())Toward=Player->GetActorForwardVector().GetSafeNormal2D();
    if(Toward.IsNearlyZero())Toward=FVector(1,0,0);
    const FVector Side=FVector::CrossProduct(FVector::UpVector,Toward);
    FVector CameraPosition=Focus-Toward*Distance+Side*85.f+FVector(0,0,Height);
    FHitResult Occlusion;
    FCollisionQueryParams CameraParams(SCENE_QUERY_STAT(UnmadeBesCameraObstruction),false);
    CameraParams.AddIgnoredActor(Player);
    if(BesStagedSpeaker.IsValid())CameraParams.AddIgnoredActor(BesStagedSpeaker.Get());
    if(GetWorld()->LineTraceSingleByChannel(
        Occlusion,Focus,CameraPosition,ECC_Visibility,CameraParams))
        CameraPosition=FMath::Lerp(Focus,Occlusion.Location,.83f);
    BesShotCamera->SetActorLocationAndRotation(
        CameraPosition,(Focus-CameraPosition).Rotation());
    if(UCameraComponent* Lens=BesShotCamera->GetCameraComponent())
        Lens->SetFieldOfView(bWide?67.f:bDetail?42.f:bClose?48.f:58.f);
    PC->SetViewTargetWithBlend(BesShotCamera,.3f);
}

void AUnmadePrototypeHub::SkipBesDirectedScene()
{
    if(GetWorld())GetWorldTimerManager().ClearTimer(BesSubtitleTimer);
    BesActiveScene=-1;
    BesActiveBeat=0;
    RestoreBesCamera();
    if(IsValid(BesSubtitleOverlay))BesSubtitleOverlay->ClearBeat();
}
void AUnmadePrototypeHub::PlayBesDirectedScene(int32 SceneIndex)
{
    if(!GetWorld()||SceneIndex<0||
       SceneIndex>=static_cast<int32>(UnmadeCore::UnpaidScene::Count))
        return;
    SkipBesDirectedScene();
    BesActiveScene=SceneIndex;
    AdvanceBesDirectedBeat();
}
void AUnmadePrototypeHub::AdvanceBesDirectedBeat()
{
    if(!GetWorld()||BesActiveScene<0)return;
    const auto Scene=static_cast<UnmadeCore::UnpaidScene>(BesActiveScene);
    const auto* Beat=UnmadeCore::UnpaidBeatAt(Scene,BesActiveBeat);
    if(!Beat){SkipBesDirectedScene();return;}
    const FName Speaker(UTF8_TO_TCHAR(Beat->speakerId));
    const FString Subtitle(UTF8_TO_TCHAR(Beat->subtitle));
    FrameBesDirectedBeat(Speaker,FString(UTF8_TO_TCHAR(Beat->camera)),
        FString(UTF8_TO_TCHAR(Beat->blocking)),Beat->duration);
    OnBesDirectedBeat.Broadcast(
        Speaker,Subtitle,FName(UTF8_TO_TCHAR(Beat->camera)),
        FName(UTF8_TO_TCHAR(Beat->foley)),
        FString(UTF8_TO_TCHAR(Beat->blocking)),
        FString(UTF8_TO_TCHAR(Beat->voice)),Beat->duration);
    // Always show complete sequential subtitles in a real lower-third widget.
    // A debug caption only remains if the local player cannot create a UI.
    const FString SpeakerName=Speaker==FName(TEXT("stage"))
        ?TEXT("THE UNMADE"):Speaker==FName(TEXT("npc.cinderhold.courier.001"))
            ?TEXT("BES"):Speaker==FName(TEXT("npc.cinderhold.emberwarden.001"))
            ?TEXT("RHEVA"):Speaker==FName(TEXT("npc.saltwake.courier.001"))
            ?TEXT("MERA"):Speaker==FName(TEXT("npc.cinderhold.healer.001"))
            ?TEXT("OVENNA"):Speaker.ToString();
    if(!IsValid(BesSubtitleOverlay))
    {
        if(APlayerController* PC=GetWorld()->GetFirstPlayerController())
        {
            BesSubtitleOverlay=CreateWidget<UUnmadeUnpaidShiftSubtitleWidget>(
                PC,UUnmadeUnpaidShiftSubtitleWidget::StaticClass());
            if(IsValid(BesSubtitleOverlay))BesSubtitleOverlay->AddToViewport(50);
        }
    }
    if(IsValid(BesSubtitleOverlay))
        BesSubtitleOverlay->DisplayBeat(SpeakerName,Subtitle);
    else if(GEngine)
        GEngine->AddOnScreenDebugMessage(-1,Beat->duration,FColor::White,
            SpeakerName+TEXT(": ")+Subtitle);

    // Original generated stereo Foley may be imported at this exact path by
    // the guarded first-PC Unreal editor helper. Missing sound is explicitly
    // tolerated, but it NEVER suppresses captions or fails the quest.
    // These files are texture-only effects; there are no fake spoken voices.
    const FString AssetName=FString::Printf(TEXT("SFX_US_%s"),
        UTF8_TO_TCHAR(Beat->foley));
    const FString AssetPath=FString::Printf(
        TEXT("/Game/UnmadeProduction/Audio/UnpaidShift/%s.%s"),
        *AssetName,*AssetName);
    if(USoundBase* Cue=Cast<USoundBase>(StaticLoadObject(
        USoundBase::StaticClass(),nullptr,*AssetPath,nullptr,LOAD_NoWarn)))
        UGameplayStatics::PlaySound2D(GetWorld(),Cue,0.68f);
    ++BesActiveBeat;
    GetWorldTimerManager().SetTimer(BesSubtitleTimer,this,
        &AUnmadePrototypeHub::AdvanceBesDirectedBeat,
        FMath::Max(0.5f,Beat->duration),false);
}
void AUnmadePrototypeHub::MaybePlayBesReturnScene(FName StableId)
{
    if(!GetWorld()||BesActiveScene>=0 ||
       BesHousehold.Phase()==UnmadeCore::BesPhase::Unheard)return;
    const double Now=GetWorld()->GetTimeSeconds();
    if(Now<LastBesReturningSceneAt+60.0)return;
    if(StableId==FName(TEXT("npc.cinderhold.courier.001")))
        PlayBesDirectedScene(static_cast<int32>(UnmadeCore::UnpaidScene::BesReturns));
    else if(StableId==FName(TEXT("npc.saltwake.courier.001")) &&
            BesHousehold.Save().meraConsented)
        PlayBesDirectedScene(static_cast<int32>(UnmadeCore::UnpaidScene::MeraReturns));
    else return;
    LastBesReturningSceneAt=Now;
}
FString AUnmadePrototypeHub::BesDecisionPreview(FName Tag) const
{
    const auto House=BesHousehold.Save();
    const auto* Worker=LivingTrades.Worker(UnmadeCore::Realm::HearthBeneath);
    const auto* Mera=LivingTrades.Worker(UnmadeCore::Realm::WidowedRain);
    if(!Worker||!Mera)return TEXT("The municipal wage ledger is unavailable.");
    if(!House.wardenInterviewed)
        return TEXT("Not ready. Hear Bes, examine the physical hearth and ask Rheva to confirm the pay ledger.");
    if(Tag==FName(TEXT("Bes.Home")))
    {
        return FString::Printf(TEXT("DELIVER FIVE REAL EMBER BASKETS? Bes's home reserve: %d; his unpaid wages remain %d marks. Requires one carried Cinderhold pack. Press E again to confirm."),
            House.heatBaskets,Worker->owedMarks);
    }
    if(Tag==FName(TEXT("Bes.WalkoutChoice")))
    {
        return FString::Printf(TEXT("SUPPORT BES'S RIGHT TO STOP? Unpaid: %d marks. One Cinderhold producer and Bes's future paid shifts pause while refusal persists. Press E again to confirm."),
            Worker->owedMarks);
    }
    if(Tag==FName(TEXT("Bes.RestitutionChoice")))
    {
        return FString::Printf(TEXT("PAY BOTH CLAIMANTS? Cinderhold treasury %d; Bes unpaid %d; Mera successor claim %d. Requires %d actual town marks; no partial or free settlement. Press E again to confirm."),
            Worker->employerMarks,Worker->owedMarks,House.guaranteeOwed,
            Worker->owedMarks+House.guaranteeOwed);
    }
    if(Tag==FName(TEXT("Bes.MeraGuarantee")))
    {
        const int32 Limit=FMath::Min(6,FMath::Min(Mera->earnedMarks,Worker->owedMarks));
        return FString::Printf(TEXT("MERA'S EXPLICIT RISK: direct consent %s, transfer up to %d of HER owned marks; city then owes Mera. No currency created. Press E again to confirm."),
            House.meraConsented?TEXT("recorded"):TEXT("NOT obtained"),
            Limit);
    }
    return TEXT("Unknown wage dispute station.");
}
FString AUnmadePrototypeHub::GetBesDecisionOptions() const
{
    return TEXT("Four physical choices: deliver Bes's ember pack; support his voluntary walkout; pay all municipal claims; or ask Mera in Saltwake to risk her own wages. Approach one station for the exact real cost.");
}
FString AUnmadePrototypeHub::GetBesDecisionPreviewForStation(FName StationTag) const
{
    return BesDecisionPreview(StationTag);
}
