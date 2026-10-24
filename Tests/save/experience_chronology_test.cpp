#include "combined_record_store.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;

namespace {
struct Session {
    Expeditions expeditions;
    AdventureDepth depth;
    PlayerExperience experience;
    int day=1;

    SaveFixture::Record Capture() const {
        SaveFixture::Record r;
        r.bHasLivingWorldSnapshot=true;r.LivingWorldSeconds=(day-1)*1200.;
        r.bHasExpeditions=true;r.ExpeditionState=expeditions.Pack();
        r.bHasAdventureDepth=true;r.AdventureDepthState=depth.Pack();
        r.bHasPlayerExperience=true;r.PlayerExperienceState=experience.Pack();
        return r;
    }
    void Restore(const SaveFixture::Record& r) {
        assert(expeditions.Restore(r.ExpeditionState));
        day=1+static_cast<int>(r.LivingWorldSeconds/1200.);
        assert(depth.Restore(r.AdventureDepthState,expeditions,day));
        assert(experience.Restore(r.PlayerExperienceState,expeditions,depth,day));
    }
    void Prepare() {
        for(int a=0;a<9;++a) {
            assert(expeditions.Brief(a));assert(expeditions.Visit(a,0));
            assert(expeditions.Enter(a,0));assert(experience.OfferWing(a,expeditions));
            for(int step=0;step<3;++step){assert(experience.WingStep(a,step,true));}
            assert(experience.Guardian(a,a==3));assert(experience.CompleteWing(a));
            assert(experience.Upgrade(a,6));
            for(int person=0;person<5;++person){assert(experience.Visit(a,person,1));}
        }
    }
};

bool SameProgress(const SaveFixture::Record& a,const SaveFixture::Record& b) {
    return a.LivingWorldSeconds==b.LivingWorldSeconds&&
        a.ExpeditionState==b.ExpeditionState&&
        a.AdventureDepthState==b.AdventureDepthState&&
        a.PlayerExperienceState==b.PlayerExperienceState;
}
}

int main() {
    Session initial;initial.Prepare();const auto good=initial.Capture();
    assert(SaveFixture::Valid(good));
    int malformed=0,reloads=0,writeFaults=0;
    auto Reload=[&](SaveFixture::Storage& disk,const SaveFixture::Record& expected) {
        for(int n=0;n<12;++n) {
            const auto* loaded=disk.Load();assert(loaded);assert(SameProgress(*loaded,expected));
            Session restored;restored.Restore(*loaded);
            assert(SameProgress(restored.Capture(),expected));++reloads;
        }
    };
    auto Reject=[&](const SaveFixture::Record& bad) {
        PlayerExperience receiver=initial.experience;const auto before=receiver.Pack();
        assert(!receiver.Restore(bad.PlayerExperienceState,initial.expeditions,initial.depth,1));
        assert(receiver.Pack()==before);assert(!SaveFixture::Valid(bad));++malformed;

        // Invalid outgoing history cannot rotate either good stored record.
        SaveFixture::Storage disk;disk.primary=good;disk.backup=good;
        assert(!disk.Commit(bad));assert(disk.writes==0);
        assert(SameProgress(*disk.primary,good)&&SameProgress(*disk.backup,good));
        Reload(disk,good);

        // Availability and recovery selection use the same complete preflight.
        disk.primary=bad;disk.backup=good;Reload(disk,good);assert(disk.recovery);
        assert(!disk.Commit(bad));assert(disk.writes==0);
        assert(SameProgress(*disk.backup,good));
        disk.primary=good;disk.backup=bad;disk.recovery=false;Reload(disk,good);
        disk.primary=bad;assert(!disk.Load());

        // A failed recovery write may damage primary; its good source stays pinned.
        Session next=initial;assert(next.experience.Learn(5));const auto outgoing=next.Capture();
        disk.primary=bad;disk.backup=good;disk.recovery=false;
        assert(disk.Load()==&*disk.backup);disk.failAt=1;disk.damageOnFailure=true;
        assert(!disk.Commit(outgoing));++writeFaults;
        assert(disk.recovery&&SameProgress(*disk.backup,good));Reload(disk,good);
        disk.failAt=0;assert(disk.Commit(outgoing));assert(!disk.recovery);
        assert(SameProgress(*disk.backup,good));Reload(disk,outgoing);
    };

    for(int a=0;a<9;++a) {
        const int base=6+a*25;
        for(int person=0;person<5;++person) {
            // Visit records at most one distinct day per person per world day.
            auto bad=good;bad.PlayerExperienceState[base+15+person]=2;
            Reject(bad);
        }
        auto bad=good;bad.PlayerExperienceState[base+13]=1;
        bad.PlayerExperienceState[base+14]=2;Reject(bad);

        // Even an internally balanced insight budget cannot launder forged visits.
        bad=good;bad.PlayerExperienceState[base+15+3]=2;
        bad.PlayerExperienceState[base+12]|=2;++bad.PlayerExperienceState[base+8];
        Reject(bad);

        // Without a selected build there is no repeatable spender: the permanent
        // purchases must explain every point, even when the total budget balances.
        bad=good;--bad.PlayerExperienceState[base+8];++bad.PlayerExperienceState[base+9];
        Reject(bad);
    }

    // Every emitted older history remains valid: gaps are allowed, counts cap at
    // five while their last day advances, and the final supported day is legal.
    Session capped=initial;
    for(int day=2;day<=6;++day)for(int a=0;a<9;++a)for(int person=0;person<5;++person) {
        assert(capped.experience.Visit(a,person,day));
        assert(capped.experience.Arc(a)->visits[person]==std::min(5,day));
    }
    capped.day=6;assert(SaveFixture::Valid(capped.Capture()));
    for(int a=0;a<9;++a) {
        assert(capped.experience.Specialty(a,1));assert(!capped.experience.Specialty(a,1));
        assert(capped.experience.Specialty(a,2));assert(!capped.experience.Specialty(a,3));
        assert(capped.experience.Scene(a,1,capped.expeditions,capped.depth));
        assert(!capped.experience.Scene(a,1,capped.expeditions,capped.depth));
        assert(capped.experience.Arc(a)->insight==5);
        assert(capped.experience.Upgrade(a,1));assert(capped.experience.Upgrade(a,2));
        assert(capped.experience.Upgrade(a,3));assert(capped.experience.Upgrade(a,1));
        assert(capped.experience.Arc(a)->insight==0&&capped.experience.Arc(a)->spent==6);
    }
    SaveFixture::Storage disk;disk.primary=capped.Capture();disk.backup=good;
    Reload(disk,*disk.primary);
    capped.day=1+static_cast<int>(LivingWorldClock::MaxRecordedSeconds/1200.);
    for(int a=0;a<9;++a)for(int person=0;person<5;++person) {
        assert(capped.experience.Visit(a,person,capped.day));
    }
    disk.primary=capped.Capture();assert(SaveFixture::Valid(*disk.primary));Reload(disk,*disk.primary);
    Session late;
    for(int a=0;a<9;++a)for(int person=0;person<5;++person) {
        assert(late.experience.Visit(a,person,capped.day));
    }
    late.day=capped.day;assert(SaveFixture::Valid(late.Capture()));
    // The component's wider day range remains supported independently of the clock.
    for(int a=0;a<9;++a)for(int person=0;person<5;++person) {
        assert(late.experience.Visit(a,person,1000000));
    }
    PlayerExperience latest;
    assert(latest.Restore(late.experience.Pack(),late.expeditions,late.depth,1000000));

    // Schema-1 records without this optional subrecord keep their existing path.
    auto legacy=good;legacy.bHasPlayerExperience=false;legacy.PlayerExperienceState.clear();
    assert(SaveFixture::Valid(legacy));assert(SaveFixture::Valid(SaveFixture::Record{}));
    std::cout<<"experience chronology: "<<malformed<<" malformed records, "
        <<writeFaults<<" write faults, "<<reloads<<" reloads\n";
}
