#include "World/UnmadeUnpaidShiftSceneRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main(){
    assert(UnpaidShiftBeats.size()==39);
    double seconds=0;
    for(int s=0;s<static_cast<int>(UnpaidScene::Count);++s){
        const auto scene=static_cast<UnpaidScene>(s);
        assert(UnpaidBeatCount(scene)==3);
        double sceneSeconds=0;
        for(int i=0;i<3;++i){
            const auto* b=UnpaidBeatAt(scene,i);
            assert(b&&b->scene==scene&&b->beat==i);
            assert(std::strlen(b->subtitle)>10);
            assert(std::strlen(b->subtitle)<150); // accessible 1-beat caption
            assert(std::strlen(b->camera)>3);
            assert(std::strlen(b->blocking)>8);
            assert(std::strlen(b->foley)>2);
            assert(std::strlen(b->voice)>2);
            assert(b->duration>=2.0f&&b->duration<=8.0f);
            sceneSeconds+=b->duration;
        }
        assert(sceneSeconds>=7.0&&sceneSeconds<=25.0);
        seconds+=sceneSeconds;
        assert(UnpaidBeatAt(scene,3)==nullptr);
    }
    assert(UnpaidBeatAt(UnpaidScene::Count,0)==nullptr);
    assert(UnpaidBeatAt(UnpaidScene::FirstWage,-1)==nullptr);
    assert(BesMaskScene(BesPhase::Settled)==UnpaidScene::MaskPaid);
    assert(BesMaskScene(BesPhase::Underwritten)==UnpaidScene::MaskUnderwritten);
    assert(BesMaskScene(BesPhase::Walkout)==UnpaidScene::MaskUnpaid);
    assert(BesMaskScene(BesPhase::ReliefGiven)==UnpaidScene::MaskUnpaid);
    assert(BesMaskScene(BesPhase::WagesReported)==UnpaidScene::MaskUnpaid);
    BesHouseholdSnapshot h;
    assert(BesReturnScene(h)==UnpaidScene::BesReturns);
    assert(seconds>110.0);
    std::cout<<"PASS: 39 timed storyboard beats, 13 distinct scenes, caption bounds, camera+foley+blocking, truthful endgame selection\n";
}
