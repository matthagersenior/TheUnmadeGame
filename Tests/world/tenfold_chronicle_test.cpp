#include "World/UnmadeTenfoldChronicle.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
static RiteContext context(RiteId id) {
    RiteContext c;
    c.site=static_cast<int>(id);
    c.witness=RiteSpecs[static_cast<int>(id)].witness;
    c.lineOfSight=true;
    c.villageVisits=7;
    c.landmarkVisits=63;
    c.frontierVisits=3;
    c.bossVictories=7;
    c.factionEndings=7;
    c.verifiedWitnesses=3;
    c.hasVeyl=true;
    c.bellQuestComplete=true;
    c.paperQuestComplete=true;
    c.day=12;
    c.now=1000;
    c.strain=10;
    return c;
}
int main() {
    assert(RiteSpecs.size()==10);
    for(int i=0;i<10;++i) {
        const auto id=static_cast<RiteId>(i);
        const auto& spec=RiteSpecs[i];
        assert(spec.id==id);
        assert(std::strlen(spec.name)>5 && std::strlen(spec.lore)>45);
        assert(std::strlen(spec.reward)>10);
        for(const char* chapter:spec.chapters)assert(std::strlen(chapter)>18);
        TenfoldChronicle q;
        auto c=context(id);
        c.site=-1;
        assert(q.Advance(id,RiteAction::Discover,c)==RiteResult::WrongLocation);
        c.site=i;
        assert(q.Advance(id,RiteAction::Discover,c)==RiteResult::Advanced);
        assert(q.Advance(id,RiteAction::Discover,c)==RiteResult::OutOfOrder);
        c.witness="npc.not.a.witness";
        assert(q.Advance(id,RiteAction::Testify,c)==RiteResult::WrongWitness);
        c.witness=spec.witness;c.lineOfSight=false;
        assert(q.Advance(id,RiteAction::Testify,c)==RiteResult::WrongWitness);
        c.lineOfSight=true;
        assert(q.Advance(id,RiteAction::Testify,c)==RiteResult::Advanced);
        assert(q.Advance(id,RiteAction::Trial,c)==RiteResult::NeedAbilityTrial);
        auto effect=q.Invoke(id,c);
        assert(effect.result==RiteResult::Applied);
        assert(effect.duration>0 || effect.impact>0);
        assert(q.Advance(id,RiteAction::Trial,c)==RiteResult::Advanced);
        c.verifiedWitnesses=0;
        assert(q.Advance(id,RiteAction::Decide,c,1)==RiteResult::NeedsEvidence);
        c.verifiedWitnesses=3;
        assert(q.Advance(id,RiteAction::Decide,c,1)==RiteResult::Advanced);
        assert(q.Stage(id)==4 && q.Choice(id)==1);
        if(id==RiteId::LegacyForging){
            q.RecordDeed(Deed::Protected);
            q.RecordDeed(Deed::Discovered);
            q.RecordDeed(Deed::Reconciled);
        }
        if(id==RiteId::Cartography)q.VerifyRoute(0,3,true);
        if(id==RiteId::TomorrowDebt){
            q.PassDay(13);
            c.day=15; // debt exhaustion must expire by natural time before future use
            c.now+=400;
        }
        assert(q.Advance(id,RiteAction::Master,c)==RiteResult::Completed);
        assert(q.Stage(id)==5 && q.IsMastered(id));
        assert(q.Advance(id,RiteAction::Master,c)==RiteResult::AlreadyCompleted);
        assert(q.Invoke(id,c).result==RiteResult::Applied ||
               q.Invoke(id,c).result==RiteResult::Cooldown);
        TenfoldChronicle restored;
        assert(restored.Restore(q.Snapshot()));
        assert(restored.IsMastered(id) && restored.Choice(id)==1);
        auto bad=q.Snapshot(); bad.stage[i]=6;
        assert(!restored.Restore(bad) && restored.IsMastered(id));
        bad=q.Snapshot(); bad.choice[i]=9;
        assert(!restored.Restore(bad));
    }
    // Witnesscraft and boss mercy must not be usable without *real* witnesses.
    for(const auto ritual : {RiteId::Witnesscraft,RiteId::UnderstandingBosses}) {
        TenfoldChronicle model;
        auto evidence=context(ritual);
        assert(model.Advance(ritual,RiteAction::Discover,evidence)==RiteResult::Advanced);
        assert(model.Advance(ritual,RiteAction::Testify,evidence)==RiteResult::Advanced);
        evidence.verifiedWitnesses=0;
        assert(model.Invoke(ritual,evidence).result==RiteResult::NeedsEvidence);
        evidence.verifiedWitnesses=2;
        assert(model.Invoke(ritual,evidence).result==RiteResult::Applied);
    }
    // Learning is gated by *actual* world evidence, not model text.
    TenfoldChronicle advanced;
    auto c=context(RiteId::Cartography);
    c.frontierVisits=0;
    assert(advanced.Advance(RiteId::Cartography,RiteAction::Discover,c)
           ==RiteResult::NeedsEvidence);
    c.frontierVisits=3;
    assert(advanced.Advance(RiteId::Cartography,RiteAction::Discover,c)
           ==RiteResult::Advanced);
    // A vow can be broken, but leaves a durable penalty.
    auto oath=context(RiteId::Oathbinding);
    TenfoldChronicle vows;
    vows.Advance(RiteId::Oathbinding,RiteAction::Discover,oath);
    vows.Advance(RiteId::Oathbinding,RiteAction::Testify,oath);
    assert(vows.Invoke(RiteId::Oathbinding,oath).result==RiteResult::Applied);
    assert(vows.BreakOath());
    assert(!vows.BreakOath());
    assert(vows.Snapshot().brokenOaths==1);
    // Borrowing tomorrow cannot be free or perpetual.
    TenfoldChronicle debts;
    auto debt=context(RiteId::TomorrowDebt);
    debts.Advance(RiteId::TomorrowDebt,RiteAction::Discover,debt);
    debts.Advance(RiteId::TomorrowDebt,RiteAction::Testify,debt);
    assert(debts.Invoke(RiteId::TomorrowDebt,debt).result==RiteResult::Applied);
    assert(debts.Snapshot().debtDueDay==13);
    assert(debts.Invoke(RiteId::TomorrowDebt,debt).result==RiteResult::DebtOutstanding);
    assert(debts.PassDay(13));
    assert(debts.Snapshot().exhaustedUntilDay==14);
    assert(debts.Invoke(RiteId::TomorrowDebt,debt).result==RiteResult::Exhausted);
    debt.day=15;debt.now+=400;
    assert(debts.Invoke(RiteId::TomorrowDebt,debt).result==RiteResult::Applied);
    // Different character histories cannot be farmed by repeating one deed.
    TenfoldChronicle legacy;
    assert(legacy.RecordDeed(Deed::Protected)==true);
    assert(legacy.RecordDeed(Deed::Protected)==false);
    assert(legacy.RecordDeed(Deed::Discovered)==true);
    assert(legacy.RecordDeed(Deed::Reconciled)==true);
    assert(legacy.Snapshot().deeds==7);
    // Unverified maps don't unlock permanent shortcuts.
    TenfoldChronicle maps;
    assert(!maps.VerifyRoute(0,1,false));
    assert(maps.VerifyRoute(0,2,true));
    assert(!maps.VerifyRoute(0,2,true));
    assert(maps.Snapshot().verifiedRoutes==1);
    // Unique, bounded strain and cooldown for a spell-like ability.
    TenfoldChronicle law;
    auto l=context(RiteId::UnwriteLaw);
    law.Advance(RiteId::UnwriteLaw,RiteAction::Discover,l);
    law.Advance(RiteId::UnwriteLaw,RiteAction::Testify,l);
    l.strain=95;
    assert(law.Invoke(RiteId::UnwriteLaw,l).result==RiteResult::InsufficientStability);
    l.strain=10;
    assert(law.Invoke(RiteId::UnwriteLaw,l).result==RiteResult::Applied);
    assert(law.Invoke(RiteId::UnwriteLaw,l).result==RiteResult::Cooldown);
    std::cout<<"PASS: ten authored five-stage quests, distinct ability effects, evidence, meaningful mastery, safe saves and consequences\n";
}
