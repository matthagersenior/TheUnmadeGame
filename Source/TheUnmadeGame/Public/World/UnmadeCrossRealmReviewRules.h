#pragma once
// A received report changes the local *question*, not the facts: the
// destination still needs its own physically examined source of evidence.
// All decisions are projections of existing durable custody and evidence.
#include "World/UnmadeCrossRealmCarryRules.h"
namespace UnmadeCore {
constexpr int CrossRealmSource(int destination)noexcept {
 return destination>=0&&destination<9?(destination+8)%9:-1;
}
enum class ReviewState:int {
 Unavailable=0,Absent=1,NeedEvidence=2,Compared=3
};
constexpr ReviewState CrossRealmReview(int destination,CarryStage incoming,
 bool visited,bool evidence,bool validSave)noexcept {
 if(CrossRealmSource(destination)<0||!validSave||!visited)
  return ReviewState::Unavailable;
 if(incoming!=CarryStage::Delivered)return ReviewState::Absent;
 return evidence?ReviewState::Compared:ReviewState::NeedEvidence;
}
} // namespace UnmadeCore
