#pragma once
#include "Save/UnmadeSessionRules.h"
namespace UnmadeCore {
struct ProfileSelection {int profile=0;bool backup=false;};
enum class ProfileArrival { FreshOwner, Waiting, TargetReady, Cancelled };
// Process-local ownership barrier. Settings remain on the original profile
// until a different world in the same game instance registers its player.
class ProfileHandoff final {
public:
 bool Pending()const{return pending_;}
 bool WritesBlocked()const{return blocked_;}
 ProfileSelection Original()const{return original_;}
 ProfileSelection Target()const{return target_;}
 ProfileSelection Effective(ProfileSelection settings)const{return pending_?target_:settings;}
 bool Begin(ProfileSelection original,ProfileSelection target){
  if(pending_||!ValidProfile(original.profile)||!ValidProfile(target.profile))return false;
  original_=original;target_=target;pending_=blocked_=true;return true;
 }
 void RecoverTarget(){if(pending_)target_.backup=true;}
 bool Fail(bool originalOwnerStillLive){
  if(!pending_)return false;
  pending_=false;blocked_=!originalOwnerStillLive;return true;
 }
 ProfileArrival RegisterOwner(bool originalWorld,bool sameGameInstance,bool freshWorld=true){
  if(!pending_){
   if(blocked_&&!freshWorld)return ProfileArrival::Waiting;
   blocked_=false;return ProfileArrival::FreshOwner;
  }
  if(!sameGameInstance){pending_=false;blocked_=true;return ProfileArrival::Cancelled;}
  if(originalWorld)return ProfileArrival::Waiting;
  pending_=blocked_=false;return ProfileArrival::TargetReady;
 }
private:
 ProfileSelection original_{},target_{};
 bool pending_=false,blocked_=false;
};
}
