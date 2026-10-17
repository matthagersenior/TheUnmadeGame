#pragma once
#include <string>
#include <vector>
#include <cmath>
namespace UnmadeCore {
// Unreal gamepad triggers report as analog keys, but can still act as buttons.
// Movement/look axes remain ineligible for action rebinding.
// The same controller buttons may serve different actions in gameplay and menus.
inline bool CanBindPlayerControl(bool valid,bool analog,bool supportedTrigger){return valid&&(!analog||supportedTrigger);}
struct ControlLabel {std::string key,display;};
inline bool LabelWord(unsigned char c){return c>=128||(c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_';}
inline std::string FormatControlLabels(const std::string& input,const std::vector<ControlLabel>& labels){
 std::string out;for(std::size_t i=0;i<input.size();){const ControlLabel* best=nullptr;
 for(const auto& label:labels){auto n=label.key.size();if(!n||i+n>input.size()||input.compare(i,n,label.key)!=0||(i&&LabelWord(input[i-1]))||(i+n<input.size()&&LabelWord(input[i+n])))continue;
 if(label.key=="I"){auto rest=input.substr(i+1);auto start=rest.find_first_not_of(" \t");rest=start==std::string::npos?"":rest.substr(start);bool hint=false;for(const auto* word:{"equipment","inventory","shows","opens","to view"})if(rest.rfind(word,0)==0)hint=true;if(!hint)continue;}
 if(!best||n>best->key.size())best=&label;}
 if(best){out+=best->display;i+=best->key.size();}else out+=input[i++];}return out;
}
enum class SessionIntent {None,Load,Recover,New};
class SessionConfirmation final {SessionIntent intent_=SessionIntent::None;int profile_=-1;public:
 void Clear(){intent_=SessionIntent::None;profile_=-1;}
 bool Request(SessionIntent intent,int profile,bool saved){if(intent==SessionIntent::None||profile<0||profile>3){Clear();return false;}if(saved&&intent!=SessionIntent::Recover){Clear();return true;}if(intent_==intent&&profile_==profile){Clear();return true;}intent_=intent;profile_=profile;return false;}
};
inline bool PhysicalTradeAllowed(bool alive,bool consenting,double distanceSq,bool unobstructed){return alive&&consenting&&std::isfinite(distanceSq)&&distanceSq>=0&&distanceSq<=340.0*340.0&&unobstructed;}
inline bool MotionMayReplace(bool defeated,bool deathPlaying,bool staggerHeld,bool actionHeld,bool incomingDeath,bool incomingStagger,bool incomingLocomotion){if(incomingDeath)return true;if(defeated||deathPlaying)return false;if(incomingStagger)return true;if(staggerHeld)return false;if(actionHeld&&incomingLocomotion)return false;return true;}
}
