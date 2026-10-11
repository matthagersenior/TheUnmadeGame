#pragma once
#include "World/UnmadeRememberedRoadRegistry.h"
#include "World/UnmadeUnpaidShiftSceneRules.h"
namespace UnmadeCore {
inline UnpaidBeat RoadBeatAt(int city,bool returned,int beat) noexcept {
 if(city<0||city>=11||beat<0||beat>=3)return {UnpaidScene::Count,0,"stage","","","","","",0};
 const auto& C=RoadCities[city];
 if(!returned){
  if(beat==0)return {UnpaidScene::Count,0,C.sender,C.brief,"road_work_wide","turn toward the local loading yard","quiet_ledger",C.voice,7.f};
  if(beat==1)return {UnpaidScene::Count,1,C.sender,C.clues[1],"road_hand_insert","one open-hand gesture; do not summon cargo","pen_scratch",C.voice,6.f};
  return {UnpaidScene::Count,2,C.sender,"Read three field records. A physical release can leave the elder alive. Then choose the road and carry one sealed bundle.","road_portrait_close","face the visitor; yield control after instructions","wind_low",C.voice,8.f};
 }
 if(beat==0)return {UnpaidScene::Count,0,"stage",C.returned,"road_home_wide","hold at the actual visited threshold","room_tone_return",C.voice,7.f};
 if(beat==1)return {UnpaidScene::Count,1,"stage","ROAD_RECEIPT_DYNAMIC","road_home_detail","show the threshold and the saved delivery date","quiet_ledger",C.voice,6.f};
 return {UnpaidScene::Count,2,"stage","An arrival is remembered. The next day's work, wages and consent still have their own records.","road_home_wide","release the camera without moving an absent resident","wind_low",C.voice,6.f};
}
}
