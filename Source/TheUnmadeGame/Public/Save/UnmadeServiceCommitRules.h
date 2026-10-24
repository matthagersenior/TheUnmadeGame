#pragma once
#include <cmath>
namespace UnmadeCore {
// Runtime consent belongs to one physical service/choice, never to a save.
// Consume before staging consequences so a failed write needs a fresh preview.
class ScopedServiceConfirmation {
 int scope_=-1, choice_=-1;
 double preview_=-1, until_=-1;
public:
 bool Confirm(int scope,int choice,double now){
  if(scope<0||choice<0||!std::isfinite(now)||now<0){Cancel();return false;}
  if(scope_==scope&&choice_==choice&&now>=preview_&&now<=until_){Cancel();return true;}
  scope_=scope;choice_=choice;preview_=now;until_=now+20;return false;
 }
 void Cancel(){scope_=choice_=-1;preview_=until_=-1;}
};
// These stage persistent benefits before the owner's single combined write.
template<class Depth,class Experience>
bool StagePersonalAccord(Depth& depth,Experience& experience,int realm,int choice,bool boss){
 if(!depth.Choose(realm,choice,boss))return false;
 experience.Learn(4);return true;
}
template<class Experience,class Expeditions>
bool StageGuideService(Experience& experience,int realm,int day,const Expeditions& expeditions){
 if(experience.Tracked()==realm)return false;
 const auto before=experience;
 if(!experience.Specialty(realm,day))return false;
 if(experience.Track(realm,expeditions))return true;
 experience=before;return false;
}
}
