// Exhaustive, engine-independent walkthrough of the authored Unanswered Road.
// This starts with NO relics, optional quests, player XP, or secret story toggles.
// First-story completion represents a real, externally witnessed quest result;
// it is not granted by the journal or by reading a marker.
#include "World/UnmadeCampaignSpineRules.h"
#include "Combat/UnmadeFinalEncounterRules.h"
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
namespace {
constexpr std::array<Realm,2> Frontier={{Realm::WidowedRain,Realm::HearthBeneath}};
constexpr std::array<Realm,3> Middle={{Realm::TidalLedger,Realm::SkyBelow,Realm::CinderSpine}};
constexpr std::array<Realm,2> Upper={{Realm::HundredUnlived,Realm::OrchardOfKings}};
bool HasLead(const RoadGuidance& g,Realm r) {
    for(int i=0;i<g.available;++i)if(g.alternatives[i]==r)return true;
    return false;
}
bool Connected(Realm a,Realm b,int attune) {
    const auto path=RoadRoute(a,b,attune);
    if(path.empty()||path.front()!=a||path.back()!=b)return false;
    for(std::size_t i=1;i<path.size();++i) {
        bool edge=false;
        for(const auto& e:AtlasPassages)
            if(((e.a==path[i-1]&&e.b==path[i])||
                (e.b==path[i-1]&&e.a==path[i]))&&e.attunement<=attune)
                edge=true;
        if(!edge)return false; // no unearned implicit teleport
    }
    return true;
}
void TestEnding(const RoadEvidence& story,Morning outcome) {
    assert(EvaluateUnansweredRoad(story).bossAccessible);
    FinalJourney game;
    assert(game.BreakMask(true,0)==FinalResult::MaskBroken);
    assert(game.BreakCore()==FinalResult::TrueVictory);
    assert(game.ChooseWorld(outcome)==FinalResult::NewMorning);
    assert(game.World()==outcome);
    assert(game.Act()==FinalAct::OtherMorning);
    const auto snapshot=game.Snapshot();
    FinalJourney restored;
    assert(restored.Restore(snapshot));
    assert(restored.World()==outcome);
    assert(restored.Snapshot().act==snapshot.act);
    for(int i=0;i<static_cast<int>(Realm::Count);++i) {
        const auto* line=FinalLocalRecord(outcome,static_cast<Realm>(i));
        assert(line && std::strlen(line)>30);
        assert(std::strcmp(line,FinalLocalRecord(
            outcome==Morning::Anchor?Morning::Many:Morning::Anchor,
            static_cast<Realm>(i)))!=0);
    }
    // The optional echo cannot prevent completion or change the chosen morning.
    assert(restored.WakeEcho()==FinalResult::EchoStarted);
    assert(restored.SettleEcho()==FinalResult::EchoDefeated);
    assert(restored.World()==outcome);
    FinalJourney invalidRestoration;
    assert(!invalidRestoration.Restore({5,0,0}));
    assert(invalidRestoration.Act()==FinalAct::Veil);
}
}
int main() {
    InventoryModel newPlayer;
    assert(AttunementFromRewards(newPlayer)==0);
    RoadEvidence start;
    assert(EvaluateUnansweredRoad(start).act==RoadAct::Opening);
    assert(RoadTravelAttunement(start,0)==0);
    assert(!EvaluateUnansweredRoad(start).bossAccessible);

    // All 512 combinations of first-story receipts, with and without actual
    // opening testimony. Extra optional/out-of-order work never grants a
    // fabricated earlier act or reduces legal travel access.
    int enumerated=0;
    for(int opening=0;opening<=1;++opening)
    for(int mask=0;mask<(1<<static_cast<int>(Realm::Count));++mask) {
        RoadEvidence e;
        e.openingWitnessed=(opening!=0);
        e.firstStories=static_cast<std::uint16_t>(mask);
        const auto g=EvaluateUnansweredRoad(e);
        const int a=RoadTravelAttunement(e,0);
        assert(a>=0&&a<=4);
        const bool frontier=RoadHas(e,Realm::WidowedRain)||RoadHas(e,Realm::HearthBeneath);
        const bool middle=RoadHas(e,Realm::TidalLedger)||RoadHas(e,Realm::SkyBelow)||
                          RoadHas(e,Realm::CinderSpine);
        const bool upper=RoadHas(e,Realm::HundredUnlived)||RoadHas(e,Realm::OrchardOfKings);
        const bool uvren=RoadHas(e,Realm::FirstAbsence);
        const int expected=!opening?0:!frontier?1:!middle?2:!upper?3:4;
        assert(a==expected);
        const bool earnedBoss=opening&&frontier&&middle&&upper&&uvren;
        assert(g.bossAccessible==earnedBoss);
        if(earnedBoss)assert(g.act==RoadAct::ConfrontTheAnswer);
        if(g.act==RoadAct::BeforePlace)assert(a==4);
        assert(g.available>=0&&g.available<=3);
        for(int i=0;i<g.available;++i) {
            const Realm dest=g.alternatives[i];
            assert(dest!=Realm::Count);
            // Every proposed lead is a real atlas route from the opening hub
            // with NO optional relics; never a false quest-journal shortcut.
            assert(Connected(Realm::ThreefoldReach,dest,a));
            for(int j=0;j<i;++j)assert(dest!=g.alternatives[j]);
        }
        for(int other=0;other<static_cast<int>(Realm::Count);++other) {
            RoadEvidence additional=e;
            additional.firstStories|=RoadMask(static_cast<Realm>(other));
            assert(RoadTravelAttunement(additional,0)>=a);
        }
        // Legacy relics assist navigation but never synthesize quest evidence.
        assert(RoadTravelAttunement(e,4)==4);
        assert(EvaluateUnansweredRoad(e).bossAccessible==earnedBoss);
        ++enumerated;
    }

    // Walk all offered frontier/middle/upper branch combinations in the
    // topology actually authored. Visit one first chapter at each stage.
    int completed=0;
    int frontierWays[2]={0,0};
    for(std::size_t f=0;f<Frontier.size();++f) {
        RoadEvidence e;
        e.openingWitnessed=true; // earned Crossings/Bellwold testimony
        const auto begin=EvaluateUnansweredRoad(e);
        assert(begin.act==RoadAct::MissingWorld);
        assert(HasLead(begin,Frontier[f]));
        assert(Connected(Realm::ThreefoldReach,Frontier[f],RoadTravelAttunement(e,0)));
        e.firstStories|=RoadMask(Frontier[f]);
        for(const auto middle:Middle) {
            const auto m=EvaluateUnansweredRoad(e);
            if(!HasLead(m,middle))continue;
            assert(m.act==RoadAct::CostOfAbsence);
            assert(Connected(Frontier[f],middle,RoadTravelAttunement(e,0)));
            RoadEvidence throughMiddle=e;
            throughMiddle.firstStories|=RoadMask(middle);
            for(const auto upper:Upper) {
                const auto g=EvaluateUnansweredRoad(throughMiddle);
                if(!HasLead(g,upper))continue;
                assert(g.act==RoadAct::RightToExist);
                assert(Connected(middle,upper,RoadTravelAttunement(throughMiddle,0)));
                RoadEvidence atUpper=throughMiddle;
                atUpper.firstStories|=RoadMask(upper);
                assert(EvaluateUnansweredRoad(atUpper).act==RoadAct::BeforePlace);
                assert(HasLead(EvaluateUnansweredRoad(atUpper),Realm::FirstAbsence));
                assert(Connected(upper,Realm::FirstAbsence,RoadTravelAttunement(atUpper,0)));
                assert(!EvaluateUnansweredRoad(atUpper).bossAccessible);
                atUpper.firstStories|=RoadMask(Realm::FirstAbsence);
                assert(EvaluateUnansweredRoad(atUpper).bossAccessible);
                TestEnding(atUpper,Morning::Anchor);
                TestEnding(atUpper,Morning::Many);
                ++frontierWays[f];++completed;
            }
        }
    }
    assert(frontierWays[0]>0&&frontierWays[1]>0&&completed>=4);
    // Strong negative cases: upper chapter alone, all optional rewards, and
    // an unvisited Auvren are never sufficient for the first final boss.
    RoadEvidence missingAuvren{true,static_cast<std::uint16_t>(
        RoadMask(Realm::WidowedRain)|RoadMask(Realm::TidalLedger)|
        RoadMask(Realm::HundredUnlived)),0};
    assert(!EvaluateUnansweredRoad(missingAuvren).bossAccessible);
    assert(RoadTravelAttunement(missingAuvren,0)==4);
    assert(!EvaluateUnansweredRoad(RoadEvidence{false,RoadMask(Realm::FirstAbsence),0}).bossAccessible);
    // Restored legitimate final-stage saves keep access without re-farming.
    for(int stage=1;stage<=5;++stage) {
        RoadEvidence oldSave{false,0,stage};
        assert(RoadTravelAttunement(oldSave,0)==4);
        assert(EvaluateUnansweredRoad(oldSave).bossAccessible);
    }
    std::cout<<"PASS: "<<enumerated
             <<" campaign evidence states, "<<completed
             <<" legitimate relic-free first-story routes, both persistent mornings, "
             <<"post-victory optional Echo and no fake unlocks\n";
}
