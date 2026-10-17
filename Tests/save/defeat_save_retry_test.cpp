#include "Save/UnmadeDefeatSaveRetryRules.h"
#include <cassert>
#include <iostream>
#include <limits>
using namespace UnmadeCore;
int main(){
 DefeatSaveRetry retry;int calls=0;
 auto fail=[&]{++calls;return false;};auto succeed=[&]{++calls;return true;};
 assert(retry.Attempt(0,0,fail)==DefeatSaveResult::FirstFailure);
 for(int tick=0;tick<50;++tick)assert(retry.Attempt(0,tick/10.,fail)==DefeatSaveResult::Waiting);
 assert(calls==1);assert(retry.Attempt(0,5,fail)==DefeatSaveResult::FailedAgain);assert(calls==2);
 for(double invalid:{-1.,std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()})assert(retry.Attempt(0,invalid,fail)==DefeatSaveResult::Waiting);
 assert(retry.Attempt(-1,10,fail)==DefeatSaveResult::Waiting);
 assert(retry.Attempt(0,4,fail)==DefeatSaveResult::Waiting);assert(retry.Attempt(0,9.999,fail)==DefeatSaveResult::Waiting);assert(calls==2);
 assert(retry.Attempt(0,10,succeed)==DefeatSaveResult::Saved);assert(calls==3);
 assert(retry.Attempt(0,20,fail)==DefeatSaveResult::Waiting);assert(calls==3);
 assert(retry.Attempt(1,20,fail)==DefeatSaveResult::FirstFailure);assert(calls==4);
 retry.Reset();assert(retry.Attempt(1,0,succeed)==DefeatSaveResult::Saved);assert(calls==5);
 retry.Reset();const double huge=std::numeric_limits<double>::max();assert(retry.Attempt(0,huge,fail)==DefeatSaveResult::FirstFailure);assert(retry.Attempt(0,huge,fail)==DefeatSaveResult::Waiting);
 assert(calls==6);
 DefeatSaveRetry outage;int writes=0;
 for(int tick=0;tick<=600;++tick)outage.Attempt(0,tick/10.,[&]{++writes;return false;});
 assert(writes==13); // Immediate, then 5..60 seconds, instead of 601 writes.
 std::cout<<"PASS: immediate commit, five-second retries, settled suppression, phase/reset isolation and invalid clocks\n";
}
