#pragma once
#include <cmath>
namespace UnmadeCore {
enum class DefeatSaveResult { Waiting, Saved, FirstFailure, FailedAgain };
// Session-only scheduling for an already defeated actor. A failed commit owns
// its rollback; this gate never advances progression or manufactures a reward.
class DefeatSaveRetry final {
public:
    static constexpr double RetrySeconds=5;
    void Reset(){scope_=-1;attempted_=settled_=false;lastAttempt_=0;}
    template<class Commit>
    DefeatSaveResult Attempt(int scope,double now,Commit commit){
        if(scope<0||!std::isfinite(now)||now<0)return DefeatSaveResult::Waiting;
        if(scope!=scope_){Reset();scope_=scope;}
        if(settled_||(attempted_&&(now<lastAttempt_||now-lastAttempt_<RetrySeconds)))
            return DefeatSaveResult::Waiting;
        const bool first=!attempted_;
        attempted_=true;lastAttempt_=now;
        if(commit()){settled_=true;return DefeatSaveResult::Saved;}
        return first?DefeatSaveResult::FirstFailure:DefeatSaveResult::FailedAgain;
    }
private:
    int scope_=-1;
    bool attempted_=false,settled_=false;
    double lastAttempt_=0;
};
}
