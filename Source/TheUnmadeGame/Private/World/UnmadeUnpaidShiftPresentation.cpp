#include "World/UnmadePrototypeHub.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "World/UnmadeUnpaidShiftSceneRules.h"

void AUnmadePrototypeHub::SkipBesDirectedScene()
{
    if(GetWorld())GetWorldTimerManager().ClearTimer(BesSubtitleTimer);
    BesActiveScene=-1;
    BesActiveBeat=0;
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
    OnBesDirectedBeat.Broadcast(
        Speaker,Subtitle,FName(UTF8_TO_TCHAR(Beat->camera)),
        FName(UTF8_TO_TCHAR(Beat->foley)),
        FString(UTF8_TO_TCHAR(Beat->blocking)),
        FString(UTF8_TO_TCHAR(Beat->voice)),Beat->duration);
    // Even without a bound cinematic UI or custom voice asset, each row is
    // readable one at a time and never joins 3 beats into an overfull card.
    if(GEngine)
    {
        const FString SpeakerName=Speaker==FName(TEXT("stage"))
            ?TEXT(""):Speaker==FName(TEXT("npc.cinderhold.courier.001"))
                ?TEXT("Bes: ")
                :Speaker==FName(TEXT("npc.cinderhold.emberwarden.001"))
                ?TEXT("Rheva: ")
                :Speaker==FName(TEXT("npc.saltwake.courier.001"))
                ?TEXT("Mera: ")
                :Speaker==FName(TEXT("npc.cinderhold.healer.001"))
                ?TEXT("Ovenna: "):TEXT("");
        GEngine->AddOnScreenDebugMessage(-1,Beat->duration,FColor::White,
            SpeakerName+Subtitle);
    }
    ++BesActiveBeat;
    GetWorldTimerManager().SetTimer(BesSubtitleTimer,this,
        &AUnmadePrototypeHub::AdvanceBesDirectedBeat,
        FMath::Max(0.5f,Beat->duration),false);
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
    return FString::Printf(TEXT("%s | %s | %s | %s"),
        *BesDecisionPreview(FName(TEXT("Bes.Home"))),
        *BesDecisionPreview(FName(TEXT("Bes.WalkoutChoice"))),
        *BesDecisionPreview(FName(TEXT("Bes.RestitutionChoice"))),
        *BesDecisionPreview(FName(TEXT("Bes.MeraGuarantee"))));
}
