#pragma once
// Consequences derived from witnessed documents and independently accounted
// household wages. No new save fields, global NPC memories, rewards or gates.
#include "World/UnmadePetitionRelayRules.h"
namespace UnmadeCore {
struct CivicReverberation {
 bool saltwakeControversy=false;
 bool protectedPassage=false;
 bool publicAudit=false;
 bool wagesStillOwed=false;
 const char* crossingLine="";
 const char* nhalQuestion="";
 const char* wageLine="";
 const char* saltwakeLine="";
};
inline CivicReverberation EvaluateCivicReverberation(
    const PetitionRelaySnapshot& relay,const BesHouseholdSnapshot& home) noexcept {
 CivicReverberation out;
 if(relay.stage<2||relay.stage>4||(relay.route!=1&&relay.route!=2))return out;
 out.saltwakeControversy=true;
 out.wagesStillOwed=home.phase!=static_cast<int>(BesPhase::Settled);
 out.wageLine=out.wagesStillOwed
   ?"The wage ledger remains unsettled. A delivered petition is not repayment."
   :"The actual wages and any lawful guarantee were repaid; the archived dispute and its people still existed.";
 out.saltwakeLine=relay.route==1
   ?"Saltwake: Mera questions whether a private record can be audited without betraying the household."
   :"Saltwake: Mera challenges public accountability that may expose the people an institution was meant to shelter.";
 if(relay.stage!=4)return out;
 out.protectedPassage=relay.route==1;
 out.publicAudit=relay.route==2;
 out.crossingLine=out.protectedPassage
   ?"The bridge keeper has opened an optional screened walkway for vulnerable travelers. It grants neither exemption from the city's other duties nor victory."
   :"The bridge keeper has opened an optional evidence platform; visitors can examine the redacted civic question without refuge names or a verdict.";
 out.nhalQuestion=out.protectedPassage
   ?"If Nhal-Vey demands one history, can it explain how a sealed identity and an unpaid public obligation can both be true without forcing a person's exposure?"
   :"If Nhal-Vey demands one history, can it reconcile a public debt record and the private lives it must not reveal without erasing either witness?";
 return out;
}
} // namespace UnmadeCore
