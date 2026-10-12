#include "UI/UnmadePlayerFacingRules.h"
#include "Player/UnmadeCharacterIdentityRules.h"
#include <cassert>
#include <limits>
using namespace UnmadeCore;
int main(){
 // Replacements never become new input tokens, even with legal rebound keys.
 std::vector<ControlLabel> labels={{"Left mouse","Left Shift"},{"Shift","Z"},{"F1","One"},{"F11","Eleven"},{"E","X"},{"I","Inventory"}};
 assert(FormatControlLabels("Left mouse attack / Shift dodge",labels)=="Left Shift attack / Z dodge");
 assert(FormatControlLabels("F11 or F1; E enters; Eillun; I remember",labels)=="Eleven or One; X enters; Eillun; I remember");
 assert(FormatControlLabels("I equipment / I inventory",labels)=="Inventory equipment / Inventory inventory");
 assert(FormatControlLabels("éE E_1 E",labels)=="éE E_1 X");
 assert(FormatControlLabels("Left mouse",{{"Left","wrong"},{"Left mouse","correct"}})=="correct");
 SessionConfirmation c;
 assert(!c.Request(SessionIntent::Recover,0,false));assert(!c.Request(SessionIntent::Recover,1,false));assert(c.Request(SessionIntent::Recover,1,false));
 assert(!c.Request(SessionIntent::Recover,1,true)); // Recovery never saves first.
 assert(!c.Request(SessionIntent::Load,1,false));assert(c.Request(SessionIntent::Load,1,false));
 assert(!c.Request(SessionIntent::Recover,1,false));c.Clear();assert(!c.Request(SessionIntent::Recover,1,false));
 assert(c.Request(SessionIntent::New,2,true));assert(!c.Request(SessionIntent::New,2,false));assert(!c.Request(SessionIntent::New,3,false));assert(c.Request(SessionIntent::New,3,false));
 assert(!c.Request(SessionIntent::None,0,true));assert(!c.Request(SessionIntent::Load,9,true));
 assert(PhysicalTradeAllowed(true,true,340*340,true));
 assert(!PhysicalTradeAllowed(true,true,1,false));assert(!PhysicalTradeAllowed(false,true,1,true));assert(!PhysicalTradeAllowed(true,false,1,true));assert(!PhysicalTradeAllowed(true,true,341*341,true));assert(!PhysicalTradeAllowed(true,true,std::numeric_limits<double>::quiet_NaN(),true));
 // During stagger every lower priority cue, including guard/attack, is refused.
 assert(!MotionMayReplace(false,false,true,true,false,false,false));assert(!MotionMayReplace(false,false,true,true,false,false,true));
 assert(MotionMayReplace(false,false,true,true,false,true,false));assert(MotionMayReplace(true,true,true,true,true,false,false));
 assert(!MotionMayReplace(true,true,false,false,false,true,false));assert(!MotionMayReplace(false,false,false,true,false,false,true));assert(MotionMayReplace(false,false,false,false,false,false,true));
 CharacterIdentity identity;
 for(int a=0;a<8;++a)for(int value=0;value<CharacterIdentity::ChoiceCounts[a];++value){assert(identity.Select(static_cast<IdentityAspect>(a),value));auto saved=identity.Snapshot();CharacterIdentity restored;assert(restored.Restore(saved));assert(restored.Snapshot().options==saved.options);}
 auto old=identity.Snapshot();assert(!identity.Select(IdentityAspect::Hair,10));assert(!identity.Rename("\n"));assert(identity.Snapshot().options==old.options&&identity.Snapshot().chosenName==old.chosenName);assert(identity.Rename("Éva"));
}
