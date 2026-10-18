#include "World/UnmadeCivicReverberationRules.h"
#include <cassert>
#include <string>
using namespace UnmadeCore;
int main(){
 BesHouseholdSnapshot home; home.phase=static_cast<int>(BesPhase::Documented);
 PetitionRelaySnapshot none{}, saltPrivate{2,1,3,3,0,0}, crossingPrivate{4,1,3,3,4,4},
  crossingPublic{4,2,3,3,4,4};
 auto empty=EvaluateCivicReverberation(none,home);
 assert(!empty.saltwakeControversy&&!empty.protectedPassage&&!empty.publicAudit);
 assert(std::string(empty.nhalQuestion).empty());
 auto pending=EvaluateCivicReverberation(saltPrivate,home);
 assert(pending.saltwakeControversy&&!pending.protectedPassage&&!pending.publicAudit);
 auto protectedPath=EvaluateCivicReverberation(crossingPrivate,home);
 auto audit=EvaluateCivicReverberation(crossingPublic,home);
 assert(protectedPath.protectedPassage&&!protectedPath.publicAudit);
 assert(!audit.protectedPassage&&audit.publicAudit);
 assert(std::string(protectedPath.nhalQuestion).find("one history")!=std::string::npos);
 assert(std::string(audit.nhalQuestion).find("one history")!=std::string::npos);
 assert(std::string(protectedPath.crossingLine)!=std::string(audit.crossingLine));
 assert(std::string(protectedPath.nhalQuestion)!=std::string(audit.nhalQuestion));
 assert(protectedPath.wagesStillOwed && audit.wagesStillOwed);
 home.phase=static_cast<int>(BesPhase::Settled);
 auto settled=EvaluateCivicReverberation(crossingPublic,home);
 assert(!settled.wagesStillOwed);
 assert(std::string(settled.wageLine).find("repaid")!=std::string::npos);
 auto stillPrivate=EvaluateCivicReverberation(crossingPrivate,home);
 assert(stillPrivate.protectedPassage&&!stillPrivate.publicAudit);
 assert(!stillPrivate.wagesStillOwed);
 auto broken=EvaluateCivicReverberation({4,0,0,0,0,0},home);
 assert(!broken.protectedPassage&&!broken.publicAudit&&!broken.saltwakeControversy);
}
