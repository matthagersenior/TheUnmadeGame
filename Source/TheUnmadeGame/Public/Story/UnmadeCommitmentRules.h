#pragma once
#include <cmath>
#include <string>
namespace UnmadeCore {
enum class CommitmentAttempt { Preview, Confirmed, Rejected };
class CommitmentGate final {
public:
    CommitmentAttempt Attempt(const std::string& scope,int choice,double now) {
        if(scope.empty() || scope.size()>128 || choice<1 || choice>2 ||
           !std::isfinite(now) || now<0) {
            Cancel();return CommitmentAttempt::Rejected;
        }
        if(!pending_.empty() && scope==pending_ && choice==choice_ &&
           now>=openedAt_ && now<=expiresAt_) {
            Cancel();return CommitmentAttempt::Confirmed;
        }
        pending_=scope;choice_=choice;openedAt_=now;expiresAt_=now+6.0;
        return CommitmentAttempt::Preview;
    }
    void Cancel() {pending_.clear();choice_=0;openedAt_=expiresAt_=0.0;}
private:
    std::string pending_{};
    int choice_=0;
    double openedAt_=0.0,expiresAt_=0.0;
};
}
