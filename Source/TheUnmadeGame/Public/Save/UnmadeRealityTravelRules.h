#pragma once
#include "Save/UnmadeActionCommitRules.h"
#include "Fracture/UnmadeFractureRules.h"
#include "Lexicon/UnmadeLexiconRules.h"
#include "World/UnmadeLaterRealmRules.h"
#include "World/UnmadeFrontierRealmRules.h"
namespace UnmadeCore {
enum class FractureCommit { Unavailable, Applied, SaveFailed };
template<class Apply,class Writer> FractureCommit CommitFractureAction(FractureModel& model,Apply apply,Writer write){
 const auto before=model;
 if(apply()!=Result::Applied)return FractureCommit::Unavailable;
 if(write())return FractureCommit::Applied;
 model=before;return FractureCommit::SaveFailed;
}
template<class Writer> bool CommitLexiconEvidence(LexiconModel& lexicon,const std::string& id,Writer write){
 if(id!="evidence.glimpse"&&id!="evidence.archivist")return false;
 return CommitActionConsequences([&]{return lexicon;},[&]{lexicon.RecordEvidence("term.veyl",id);},write,[&](const auto& before){lexicon=before;});
}
enum class RelocationCommit { Applied, MoveFailed, SaveFailed, RollbackFailed };
template<class Position,class Move,class Writer>
RelocationCommit CommitCheckpointRelocation(const Position origin,const Position destination,Move move,Writer write){
 const auto finite=[](const auto& p){return std::isfinite(p.X)&&std::isfinite(p.Y)&&std::isfinite(p.Z);};
 if(!finite(origin)||!finite(destination))return RelocationCommit::MoveFailed;
 if(!move(destination))return move(origin)?RelocationCommit::MoveFailed:RelocationCommit::RollbackFailed;
 if(write())return RelocationCommit::Applied;
 return move(origin)?RelocationCommit::SaveFailed:RelocationCommit::RollbackFailed;
}
struct CheckpointPoint {double X=0,Y=0,Z=135;};
inline Realm CheckpointRealm(double x,double y){
 if(!std::isfinite(x)||!std::isfinite(y))return Realm::Count;
 Realm nearest=Realm::ThreefoldReach;double distance=std::hypot(x,y);
 const auto consider=[&](Realm r,double cx,double cy){const double d=std::hypot(x-cx,y-cy);if(d<distance){distance=d;nearest=r;}};
 for(const auto& r:FrontierOutposts)consider(r.realm,r.centerX,r.centerY);
 for(const auto& r:LaterRealms)consider(r.realm,r.centerX,r.centerY);
 return nearest;
}
inline CheckpointPoint SafeRealmEntry(double x,double y){
 const auto realm=CheckpointRealm(x,y);
 for(const auto& r:FrontierOutposts)if(r.realm==realm)return {r.centerX,r.centerY,135};
 for(const auto& r:LaterRealms)if(r.realm==realm)return {r.centerX,r.centerY-1170,135};
 return {};
}
inline bool SameCheckpointRealm(double x,double y,double candidateX,double candidateY){
 const auto realm=CheckpointRealm(x,y);return realm!=Realm::Count&&realm==CheckpointRealm(candidateX,candidateY);
}
template<class Writer> bool CommitExploredRoute(TenfoldChronicle& chronicle,int route,Writer write){
 return CommitActionConsequences([&]{return chronicle;},[&]{if(chronicle.Stage(RiteId::Cartography)>=2)chronicle.VerifyRoute(route,2,true);},write,[&](const auto& before){chronicle=before;});
}
}
