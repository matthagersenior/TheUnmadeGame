#include "World/UnmadePrototypeHub.h"
#include "UI/UnmadeBesQuestTrackerWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"

void AUnmadePrototypeHub::UpdateBesQuestTracker()
{
    if(!GetWorld())return;
    const double Now=GetWorld()->GetTimeSeconds();
    if(Now<NextBesTrackerRefreshAt)return;
    NextBesTrackerRefreshAt=Now+0.75;

    const APlayerController* PC=GetWorld()->GetFirstPlayerController();
    const APawn* Player=PC?PC->GetPawn():nullptr;
    if(!IsValid(Player)||bBesHouseholdRejected||
       bLivingEconomyRejected||bMaterialSaveRejected||BesActiveScene>=0)
    {
        if(IsValid(BesQuestTracker))BesQuestTracker->HideObjective();
        return;
    }
    const FVector At=Player->GetActorLocation();
    const bool NearCinder=FVector::DistSquared2D(At,FVector(0,50000,0))<
                          FMath::Square(3400.f)||
        FVector::DistSquared2D(At,FVector(-2100,52000,0))<FMath::Square(2100.f);
    const bool NearSaltwake=FVector::DistSquared2D(At,FVector(0,-50000,0))<
                          FMath::Square(3400.f);
    if(!NearCinder&&!NearSaltwake)
    {
        if(IsValid(BesQuestTracker))BesQuestTracker->HideObjective();
        return;
    }

    const auto Record=BesHousehold.Save();
    const auto Phase=BesHousehold.Phase();
    if(NearSaltwake&&Phase==UnmadeCore::BesPhase::Unheard)
    {
        if(IsValid(BesQuestTracker))BesQuestTracker->HideObjective();
        return;
    }
    FString Goal;
    if(Phase==UnmadeCore::BesPhase::Unheard)
        Goal=TEXT("Speak with Bes, Cinderhold's coal runner");
    else if(!Record.homeVisited)
        Goal=TEXT("Enter Bes's home; inspect his cold hearth with E");
    else if(!Record.wardenInterviewed)
        Goal=TEXT("Ask Rheva to corroborate the missing wage entry");
    else if(Phase==UnmadeCore::BesPhase::Settled)
        Goal=TEXT("Wages settled. Return to Bes to hear what changed");
    else if(Phase==UnmadeCore::BesPhase::Underwritten)
        Goal=TEXT("Mera risked her wages; the city must repay her claim");
    else if(NearSaltwake)
        Goal=Record.meraConsented
            ?TEXT("At Mera's station, review her real transfer; E again to confirm")
            :TEXT("Speak with Mera and ask permission before any guarantee");
    else if(Record.meraConsented&&!Record.meraGuaranteeUsed)
        Goal=TEXT("Return to Saltwake and enact Mera's consented guarantee");
    else
        Goal=TEXT("Choose real relief, a walkout, full pay, or Mera's consent");

    if(NearCinder&&!bCinderholdSaveRejected&&Cinderhold.Save().firstDay>0)
        Goal=FString(UTF8_TO_TCHAR(Cinderhold.Objective()))+TEXT("\nHousehold: ")+Goal;
    const auto* Worker=LivingTrades.Worker(UnmadeCore::Realm::HearthBeneath);
    const int32 Owed=Worker?Worker->owedMarks:0;
    const FString Evidence=FString::Printf(
        TEXT("Bes owed %d marks  |  Hearth %d baskets  |  Cold %d days  |  Day %d"),
        Owed,Record.heatBaskets,Record.daysCold,GetGameDay());
    if(!IsValid(BesQuestTracker))
    {
        APlayerController* MutablePC=GetWorld()->GetFirstPlayerController();
        if(!IsValid(MutablePC))return;
        BesQuestTracker=CreateWidget<UUnmadeBesQuestTrackerWidget>(
            MutablePC,UUnmadeBesQuestTrackerWidget::StaticClass());
        if(IsValid(BesQuestTracker))BesQuestTracker->AddToViewport(25);
    }
    if(IsValid(BesQuestTracker))BesQuestTracker->ShowObjective(Goal,Evidence);
}
