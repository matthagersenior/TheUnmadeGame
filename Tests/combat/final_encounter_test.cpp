#include "Combat/UnmadeFinalEncounterRules.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
    assert(static_cast<int>(Realm::Count)==9);
    for(int i=0;i<9;++i){
        const auto realm=static_cast<Realm>(i);
        assert(std::string(FinalLocalRecord(Morning::Anchor,realm)).size()>42);
        assert(std::string(FinalLocalRecord(Morning::Many,realm)).size()>42);
        assert(std::string(FinalLocalRecord(Morning::Anchor,realm))!=
               std::string(FinalLocalRecord(Morning::Many,realm)));
        assert(std::string(FinalLocalRecord(Morning::Unchosen,realm)).empty());
    }
    assert(FinalMemorySignature(0,0,0)==0);
    assert(FinalMemorySignature(3,4,2)==(3+8+6)%6);
    FinalJourney story;
    assert(story.BreakMask(false,0)==FinalResult::Locked);
    assert(story.BreakCore()==FinalResult::WrongStage);
    assert(story.ChooseWorld(Morning::Anchor)==FinalResult::WrongStage);
    assert(story.WakeEcho()==FinalResult::WrongStage);
    assert(story.BreakMask(true,9)==FinalResult::Invalid);
    assert(story.BreakMask(true,4)==FinalResult::MaskBroken);
    assert(story.MemorySeed()==4);
    assert(story.BreakMask(true,3)==FinalResult::WrongStage);
    assert(story.BreakCore()==FinalResult::TrueVictory);
    assert(story.BreakCore()==FinalResult::WrongStage);
    const auto pending=story.Snapshot();
    FinalJourney pendingReload;assert(pendingReload.Restore(pending));
    assert(pendingReload.Act()==FinalAct::AnswerPending);
    assert(story.ChooseWorld(Morning::Unchosen)==FinalResult::Invalid);
    assert(story.ChooseWorld(Morning::Many)==FinalResult::NewMorning);
    assert(story.Act()==FinalAct::OtherMorning);
    assert(story.World()==Morning::Many);
    assert(story.ChooseWorld(Morning::Anchor)==FinalResult::AlreadyDone);
    auto worldSaved=story.Snapshot();
    FinalJourney reload;assert(reload.Restore(worldSaved));
    assert(reload.World()==Morning::Many);
    assert(reload.WakeEcho()==FinalResult::EchoStarted);
    assert(reload.SettleEcho()==FinalResult::EchoDefeated);
    assert(reload.SettleEcho()==FinalResult::AlreadyDone);
    assert(reload.Snapshot().morning==static_cast<int>(Morning::Many));
    assert(reload.Snapshot().memorySeed==4);
    auto bad=reload.Snapshot();
    bad.act=9;assert(!reload.Restore(bad));
    bad=reload.Snapshot();bad.act=1;assert(!reload.Restore(bad));
    bad=reload.Snapshot();bad.morning=0;assert(!reload.Restore(bad));
    bad=reload.Snapshot();bad.memorySeed=7;assert(!reload.Restore(bad));
    assert(reload.Act()==FinalAct::EchoSettled);
    // All three acts have an explicit windup; even the echo cannot strike
    // before a visible warning, and a fold interrupts the pending strike.
    for(auto phase:{FinalAct::Veil,FinalAct::Unmasked,FinalAct::EchoAwake}){
        for(int sig=0;sig<6;++sig){
            FinalBattleRhythm battle;
            const auto start=battle.Advance(5,120,false,phase,sig,Morning::Many);
            assert(start.move==FinalMove::Telegraph);
            assert(start.warningSeconds>=1.8 && start.damage==0);
            assert(std::string(start.warning).size()>60);
            const auto tooSoon=battle.Advance(5.9,120,false,phase,sig,Morning::Many);
            assert(tooSoon.move==FinalMove::Telegraph && tooSoon.damage==0);
            const auto strike=battle.Advance(5+start.warningSeconds+.001,
                                            120,false,phase,sig,Morning::Many);
            assert(strike.move==FinalMove::Strike && strike.damage>0);
            assert(battle.Advance(5+start.warningSeconds+.1,120,false,
                   phase,sig,Morning::Many).move==FinalMove::Still);
            battle.Reset();
            assert(battle.Advance(5,120,false,phase,sig,Morning::Anchor).move==FinalMove::Telegraph);
            assert(battle.Advance(5.1,120,true,phase,sig,Morning::Anchor).move==FinalMove::Stagger);
            assert(battle.Advance(5.2,120,false,phase,sig,Morning::Anchor).move==FinalMove::Still);
        }
    }
    // Returned boss uses an exclusive mechanic family in each saved reality,
    // rather than the same fight with a different title screen.
    for(int memory=0;memory<6;++memory){
        FinalBattleRhythm held,many;
        const auto A=held.Advance(20,100,false,FinalAct::EchoAwake,
                                  memory,Morning::Anchor);
        const auto B=many.Advance(20,100,false,FinalAct::EchoAwake,
                                  memory,Morning::Many);
        assert(A.attack==FinalAttack::Bell||A.attack==FinalAttack::Names);
        assert(B.attack==FinalAttack::Horizon||
               B.attack==FinalAttack::Counterfactual);
        assert(A.attack!=B.attack);
        assert(A.warningSeconds==2.8);
        assert(B.warningSeconds==1.95);
    }
    FinalBattleRhythm away;
    assert(away.Advance(2,3000,false,FinalAct::Veil,0,Morning::Unchosen).move==FinalMove::Disengage);
    assert(away.Advance(std::numeric_limits<double>::quiet_NaN(),100,false,FinalAct::Veil,0,Morning::Unchosen).move==FinalMove::Still);
    assert(away.Advance(10,100,false,FinalAct::OtherMorning,0,Morning::Many).move==FinalMove::Still);
    // Both endings are independently reachable and do not reset the story.
    FinalJourney anchor;assert(anchor.BreakMask(true,1)==FinalResult::MaskBroken);
    assert(anchor.BreakCore()==FinalResult::TrueVictory);
    assert(anchor.ChooseWorld(Morning::Anchor)==FinalResult::NewMorning);
    assert(anchor.World()!=story.World());
    std::cout<<"PASS: double defeat, non-reset alternate nine worlds, two choices, optional rematch, all three fair attack phases\n";
}
