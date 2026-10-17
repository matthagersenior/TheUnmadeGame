#pragma once
#include "World/UnmadeTenfoldChronicle.h"
#include <cstring>
namespace UnmadeCore {
enum class ConfirmedAction { None, Faction, Frontier, Afterlight, RealmAftermath, WitnessBraid, WitnessReturn };
inline ConfirmedAction ConfirmedActionFamily(const char* scope){
 if(!scope)return ConfirmedAction::None;
 const auto indexed=[&](const char* prefix,char low,char high){const auto n=std::strlen(prefix);return std::strncmp(scope,prefix,n)==0&&scope[n]>=low&&scope[n]<=high&&scope[n+1]=='\0';};
 if(indexed("Faction.",'0','2'))return ConfirmedAction::Faction;
 if(indexed("Frontier.",'0','1'))return ConfirmedAction::Frontier;
 if(indexed("RealmAftermath.",'1','8'))return ConfirmedAction::RealmAftermath;
 if(std::strcmp(scope,"Bellwold.Afterlight")==0)return ConfirmedAction::Afterlight;
 if(std::strcmp(scope,"WitnessBraid.Crossings")==0)return ConfirmedAction::WitnessBraid;
 if(std::strcmp(scope,"WitnessReturn.HessaReply")==0)return ConfirmedAction::WitnessReturn;
 return ConfirmedAction::None;
}
template<class Resolve> bool ResolveConfirmedAction(const char* confirmed,const char* current,Resolve resolve){
 if(!confirmed||!current||std::strcmp(confirmed,current)!=0)return false;
 const auto family=ConfirmedActionFamily(confirmed);
 return family!=ConfirmedAction::None&&resolve(family); // Exactly one owner, including failure.
}
// Snapshot only the participant owned by this boundary. Nested participants
// unwind in reverse order; the original world/cost owner keeps its own rollback.
template<class Capture,class Stage,class Writer,class Restore>
bool CommitActionConsequences(Capture capture,Stage stage,Writer write,Restore restore){
 const auto before=capture();stage();
 if(write())return true;
 restore(before);return false;
}
inline unsigned ActionDeedMask(const char* event){
 if(!event)return 0;
 if(std::strcmp(event,"Player.Helped")==0||std::strcmp(event,"Player.HelpedVillage")==0)
  return 1u<<static_cast<int>(Deed::Protected);
 if(std::strcmp(event,"World.FactionResolved")==0)return 1u<<static_cast<int>(Deed::Reconciled);
 return 0;
}
template<class Writer> bool CommitActionDeeds(TenfoldChronicle& chronicle,unsigned mask,Writer write){
 if(mask&~7u)return false;
 return CommitActionConsequences([&]{return chronicle;},[&]{
  for(int i=0;i<3;++i)if(mask&(1u<<i))chronicle.RecordDeed(static_cast<Deed>(i));
 },write,[&](const auto& before){chronicle=before;});
}
}
