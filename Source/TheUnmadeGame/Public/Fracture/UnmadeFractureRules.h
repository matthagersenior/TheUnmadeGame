#pragma once
// Deterministic rules shared by the actual Unreal component and native C++ tests.
#include <algorithm>
#include <cmath>
#include <string>
#include <utility>
#include <vector>
namespace UnmadeCore {
enum class Result { Applied, UnsupportedTarget, UnsupportedChoice, InvalidModifier, NeedsConfirmation, AlreadyActive, AlreadyCommitted, NotEnoughStability };
struct FractureSnapshot { std::string variant; double strain = 0.0; };
class FractureModel final {
public:
    FractureModel(std::string regionId, std::vector<std::string> choices)
        : region_(std::move(regionId)), choices_(std::move(choices)) {}
    static constexpr double MaxStrain = 100.0, GlimpseCost = 8.0, FoldCost = 24.0,
        RewriteCost = 60.0, RecoveryPerSecond = 3.0, GlimpseSeconds = 3.0, FoldSeconds = 6.0;
    double CurrentStrain() const { return strain_; }
    const std::string& WorldVariant() const { return committedVariant_; }
    Result Glimpse(bool validTarget, double now, double discount=0.0) {
        if (!validTarget || !ValidTime(now)) return Result::UnsupportedTarget;
        if (!std::isfinite(discount) || discount<0.0 || discount>6.0) return Result::InvalidModifier;
        const double cost=std::max(1.0,GlimpseCost-discount);
        if (strain_ + cost > MaxStrain) return Result::NotEnoughStability;
        strain_ += cost; glimpseUntil_ = now + GlimpseSeconds; return Result::Applied;
    }
    Result Fold(bool validTarget, double now, double extraSeconds=0.0) {
        if (!validTarget || !ValidTime(now)) return Result::UnsupportedTarget;
        if (!std::isfinite(extraSeconds) || extraSeconds<0.0 || extraSeconds>5.0)
            return Result::InvalidModifier;
        if (now < foldUntil_) return Result::AlreadyActive;
        if (strain_ + FoldCost > MaxStrain) return Result::NotEnoughStability;
        strain_ += FoldCost; foldUntil_ = now + FoldSeconds + extraSeconds; return Result::Applied;
    }
    Result Rewrite(const std::string& region, const std::string& choice, bool confirmed) {
        if (region != region_ || region_.empty()) return Result::UnsupportedTarget;
        if (!Supports(choice)) return Result::UnsupportedChoice;
        if (!committedVariant_.empty()) return Result::AlreadyCommitted;
        if (!confirmed) return Result::NeedsConfirmation;
        if (strain_ + RewriteCost > MaxStrain) return Result::NotEnoughStability;
        committedVariant_ = choice; strain_ += RewriteCost; return Result::Applied;
    }
    bool IsGlimpsing(double now) const { return ValidTime(now) && now < glimpseUntil_; }
    bool IsFolded(double now) const { return ValidTime(now) && now < foldUntil_; }
    bool SpendStrain(double amount) {
        if(!std::isfinite(amount) || amount<=0 || strain_+amount>MaxStrain)return false;
        strain_+=amount;return true;
    }
    bool Recover(double deltaSeconds) {
        if (!std::isfinite(deltaSeconds) || deltaSeconds <= 0.0 || strain_ <= 0.0) return false;
        strain_ = std::max(0.0, strain_ - deltaSeconds * RecoveryPerSecond); return true;
    }
    FractureSnapshot TakeSnapshot() const { return {committedVariant_, strain_}; }
    bool Restore(const FractureSnapshot& saved) {
        if (!std::isfinite(saved.strain) || saved.strain < 0 || saved.strain > MaxStrain) return false;
        if (!saved.variant.empty() && !Supports(saved.variant)) return false;
        committedVariant_ = saved.variant; strain_ = saved.strain;
        glimpseUntil_ = 0.0; foldUntil_ = 0.0; return true;
    }
private:
    static bool ValidTime(double time) { return std::isfinite(time) && time >= 0.0; }
    bool Supports(const std::string& choice) const {
        return !choice.empty() && std::find(choices_.begin(), choices_.end(), choice) != choices_.end();
    }
    std::string region_, committedVariant_;
    std::vector<std::string> choices_;
    double strain_ = 0.0, glimpseUntil_ = 0.0, foldUntil_ = 0.0;
};
}
