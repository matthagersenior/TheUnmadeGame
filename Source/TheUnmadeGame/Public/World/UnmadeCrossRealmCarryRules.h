#pragma once
// Optional, durable physical custody of one local record per realm.
// The player carries it to the next realm's *actual* named expedition chief.
// There are no new favor grants, global witness flags, or shortcut unlocks.
#include <array>
#include <vector>
namespace UnmadeCore {
constexpr int CrossRealmDestination(int origin) noexcept{
 return origin>=0&&origin<9?(origin+1)%9:-1;
}
enum class CarryStage:int {Waiting=0,Carrying=1,Delivered=2,Reviewed=3};
enum class CarryEvent:int {
 InvalidRealm,InvalidClock,NotReady,WrongRecipient,Collected,Delivered,Reviewed,AlreadyDone
};
struct CarryRecord {int stage=0,collectDay=0,deliveredDay=0;};
class CrossRealmCarry {
 std::array<CarryRecord,9> records_{};
public:
 CarryStage Stage(int origin)const noexcept{
  return origin>=0&&origin<9?static_cast<CarryStage>(records_[origin].stage):CarryStage::Waiting;
 }
 CarryRecord Record(int origin)const noexcept{
  return origin>=0&&origin<9?records_[origin]:CarryRecord{};
 }
 CarryEvent Collect(int origin,int day,bool sourceReady)noexcept{
  if(CrossRealmDestination(origin)<0)return CarryEvent::InvalidRealm;
  if(day<1||day>1000000)return CarryEvent::InvalidClock;
  if(records_[origin].stage)return CarryEvent::AlreadyDone;
  if(!sourceReady)return CarryEvent::NotReady;
  records_[origin]={1,day,0};
  return CarryEvent::Collected;
 }
 CarryEvent Deliver(int origin,int destination,int day,bool destinationVisited)noexcept{
  const int next=CrossRealmDestination(origin);
  if(next<0)return CarryEvent::InvalidRealm;
  if(destination!=next)return CarryEvent::WrongRecipient;
  if(day<1||day>1000000||
    (records_[origin].collectDay&&day<records_[origin].collectDay))
    return CarryEvent::InvalidClock;
  if(records_[origin].stage>=2)return CarryEvent::AlreadyDone;
  if(records_[origin].stage!=1||!destinationVisited)return CarryEvent::NotReady;
  records_[origin].stage=2;
  records_[origin].deliveredDay=day;
  return CarryEvent::Delivered;
 }
 // The player must return with physically collected destination evidence
 // and personally present it beside the named chief. No third day is stored:
 // it is a later stage of the same already-dated hand-carried packet.
 CarryEvent Review(int origin,int destination,int day,
                   bool destinationEvidence,bool recipientPresent)noexcept{
  const int next=CrossRealmDestination(origin);
  if(next<0)return CarryEvent::InvalidRealm;
  if(destination!=next)return CarryEvent::WrongRecipient;
  if(day<1||day>1000000||
     (records_[origin].deliveredDay&&day<records_[origin].deliveredDay))
     return CarryEvent::InvalidClock;
  if(records_[origin].stage==3)return CarryEvent::AlreadyDone;
  if(records_[origin].stage!=2||!destinationEvidence||!recipientPresent)
     return CarryEvent::NotReady;
  records_[origin].stage=3;
  return CarryEvent::Reviewed;
 }
 std::vector<int> Pack()const{
  std::vector<int> p{1};
  for(const auto& r:records_){
   p.push_back(r.stage);p.push_back(r.collectDay);p.push_back(r.deliveredDay);
  }
  return p;
 }
 bool Restore(const std::vector<int>& p,int today,
  const std::array<bool,9>& sourceReady,
  const std::array<bool,9>& destinationVisited,
  const std::array<bool,9>& destinationEvidence={})noexcept{
  if(p.size()!=28||p[0]!=1||today<1||today>1000000)return false;
  std::array<CarryRecord,9> next{};
  for(int a=0;a<9;++a){
   const int i=1+a*3;
   auto& r=next[a];r={p[i],p[i+1],p[i+2]};
   if(r.stage<0||r.stage>3||r.collectDay<0||r.collectDay>today||
      r.deliveredDay<0||r.deliveredDay>today)return false;
   if(r.stage==0&&(r.collectDay||r.deliveredDay))return false;
   if(r.stage==1&&(r.collectDay==0||r.deliveredDay))return false;
   if(r.stage>=2&&(r.collectDay==0||r.deliveredDay<r.collectDay))return false;
   if(r.stage>0&&!sourceReady[a])return false;
   if(r.stage>=2&&!destinationVisited[CrossRealmDestination(a)])return false;
   if(r.stage==3&&!destinationEvidence[CrossRealmDestination(a)])return false;
   if(r.stage<records_[a].stage)return false;
   if(records_[a].stage>=2&&
       (r.stage!=2||r.collectDay!=records_[a].collectDay||
        r.deliveredDay!=records_[a].deliveredDay))return false;
  }
  records_=next;return true;
 }
};
template<class Mutate,class Save>
bool CommitCrossRealmCarry(CrossRealmCarry& carry,Mutate apply,Save save){
 const auto original=carry;
 const CarryEvent e=apply();
 if(e!=CarryEvent::Collected&&e!=CarryEvent::Delivered&&e!=CarryEvent::Reviewed)return false;
 if(!save()){carry=original;return false;}
 return true;
}
} // namespace UnmadeCore
