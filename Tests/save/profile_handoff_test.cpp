#include "Save/UnmadeProfileHandoffRules.h"
#include "combined_record_store.h"
#include <array>
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
using SaveFixture::Record;
using Slots=std::array<SaveFixture::Storage,4>;
static Slots Records(){
 Slots slots;
 for(int p=0;p<4;++p){Record r;r.bHasPlayerResume=true;r.ResumeHealth=30+p;assert(slots[p].Commit(r));r.ResumeHealth=40+p;assert(slots[p].Commit(r));}
 return slots;
}
static const Record* Read(Slots& slots,ProfileSelection selected){slots[selected.profile].recovery=selected.backup;return slots[selected.profile].Load();}
static bool Equal(ProfileSelection a,ProfileSelection b){return a.profile==b.profile&&a.backup==b.backup;}
int main(){
 int routes=0;
 for(int source=0;source<4;++source)for(bool sourceBackup:{false,true})for(int target=0;target<4;++target)for(bool targetBackup:{false,true}){
  const ProfileSelection original{source,sourceBackup},requested{target,targetBackup};
  ProfileSelection settings=original;ProfileHandoff h;auto slots=Records();
  assert(h.Begin(settings,requested));assert(h.Pending()&&h.WritesBlocked());assert(Equal(settings,original));assert(Equal(h.Effective(settings),requested));
  const auto* selected=Read(slots,h.Effective(settings));assert(selected&&selected->ResumeHealth==(targetBackup?30:40)+target);
  assert(!h.Begin(settings,{(target+1)%4,false})); // Reentrant Load/New cannot redirect ownership.
  assert(h.RegisterOwner(true,true)==ProfileArrival::Waiting);assert(h.WritesBlocked()&&h.Pending());
  std::array<int,4> writes;for(int p=0;p<4;++p)writes[p]=slots[p].writes;
  Record oldLive;oldLive.bHasPlayerResume=true;oldLive.ResumeHealth=88;
  auto saveOld=[&]{if(h.WritesBlocked())return false;const auto active=h.Effective(settings);Read(slots,active);return slots[active.profile].Commit(oldLive);};
  for(int repeat=0;repeat<20;++repeat)assert(!saveOld());
  for(int p=0;p<4;++p)assert(slots[p].writes==writes[p]);
  assert(h.Fail(true));settings=h.Original();assert(!h.Pending()&&!h.WritesBlocked());assert(Equal(settings,original));assert(!h.Fail(true));
  assert(saveOld());assert(slots[source].primary->ResumeHealth==88);
  if(sourceBackup)assert(slots[source].backup->ResumeHealth==30+source); // Recovery source is not rotated.
  else assert(slots[source].backup->ResumeHealth==40+source);
  for(int p=0;p<4;++p)if(p!=source)assert(slots[p].writes==writes[p]);
  // A failed handoff after losing the old owner restores selection but keeps
  // writes locked until a fresh player owns restored state.
  ProfileHandoff lost;assert(lost.Begin(original,requested));assert(lost.Fail(false));assert(lost.WritesBlocked());assert(Equal(lost.Effective(original),original));assert(lost.RegisterOwner(false,true,false)==ProfileArrival::Waiting);assert(lost.WritesBlocked());assert(lost.RegisterOwner(false,true,true)==ProfileArrival::FreshOwner);assert(!lost.WritesBlocked());
  // Destination reads may recover a valid backup before player registration.
  // Pin that recovery in the pending target, without changing saved settings.
  auto recovered=Records();ProfileHandoff arrival;assert(arrival.Begin(original,requested));
  if(!targetBackup){recovered[target].primary->bHasFieldSurveys=true;recovered[target].primary->FieldSurveyStages.clear();}
  const auto* backup=Read(recovered,arrival.Effective(settings));assert(backup&&backup->ResumeHealth==30+target);arrival.RecoverTarget();
  assert(Equal(settings,original)&&arrival.Target().backup);const auto destination=arrival.Target();
  assert(arrival.RegisterOwner(false,true)==ProfileArrival::TargetReady);settings=destination;assert(!arrival.WritesBlocked()&&!arrival.Pending());assert(Equal(arrival.Effective(settings),destination));
  const auto sourceWrites=recovered[source].writes;auto newLive=*Read(recovered,settings);newLive.ResumeHealth=91;assert(recovered[target].Commit(newLive));assert(recovered[target].backup->ResumeHealth==30+target);if(source!=target)assert(recovered[source].writes==sourceWrites);
  assert(!arrival.Fail(true)); // A late failure callback cannot revert an accepted target.
  ProfileHandoff otherSession;assert(otherSession.Begin(original,requested));assert(otherSession.RegisterOwner(false,false)==ProfileArrival::Cancelled);assert(otherSession.WritesBlocked()&&!otherSession.Pending());assert(Equal(otherSession.Effective(original),original));
  assert(otherSession.RegisterOwner(false,true,false)==ProfileArrival::Waiting);
  // An explicit new profile request remains possible after failure, without
  // allowing the partially initialized failed world to write or self-register.
  assert(otherSession.Begin(original,requested));assert(otherSession.RegisterOwner(true,true)==ProfileArrival::Waiting);assert(otherSession.RegisterOwner(false,true,true)==ProfileArrival::TargetReady);
  ++routes;
 }
 ProfileHandoff invalid;assert(!invalid.Begin({-1,false},{1,false}));assert(!invalid.Begin({0,false},{4,false}));assert(!invalid.WritesBlocked());
 std::cout<<"PASS: "<<routes<<" profile/recovery handoffs; failure/source isolation, deferred owner/settings, reentrant rejection and recovery pinning\n";
}
