#pragma once
// Readable status for six established dual-rite chambers. This never commits,
// awards, or fabricates witnessed testimony.
#include "World/UnmadeConfluenceRules.h"
#include <cmath>
namespace UnmadeCore {
inline const char* DescribeConfluenceNeed(int id,int stage,int casts,int site,
 double now,int witnesses,bool masterReady) noexcept {
 if(id<0||id>=6||stage<0||stage>2||casts<0||casts>3||
    !std::isfinite(now)||now<0)return "Chamber record unavailable.";
 if(stage==2)return "Both casts and the witnessed resolution have been recorded. Return to the affected people.";
 if(!masterReady)return "Master each of the two named disciplines through its authored quest before combining them.";
 if(site!=id)return "Travel to this chamber's physical altar. No remote cast or journal click will open it.";
 if(stage==0)return "Study the altar to begin the paired-rite challenge; no progress is granted by this hint.";
 if(casts==0)return "Cast the first mastered discipline here, then the second within the shared window.";
 if(casts==1)return "The first discipline is recorded. Cast the second at this altar before the window closes.";
 if(casts==2)return "The second discipline is recorded. Cast the first at this altar before the window closes.";
 if(witnesses<2)return "Both disciplines resonate. Find two verified witnesses before recording an irreversible outcome.";
 return "Both disciplines and witnesses are ready. Review and deliberately choose the outcome at this altar.";
}
} // namespace UnmadeCore
