#pragma once
// Offline-authored language discovery: no AI models, HTTP or probabilistic results.
#include <algorithm>
#include <string>
#include <vector>
namespace UnmadeCore {
enum class InterpretationState { Unknown, Partial, Understood };
class LexiconModel final {
public:
    bool RecordEvidence(const std::string& term, const std::string& evidenceId) {
        if (term != "term.veyl" || !IsKnownEvidence(evidenceId)) return false;
        if (std::find(known_.begin(),known_.end(),evidenceId)!=known_.end()) return false;
        known_.push_back(evidenceId);
        return true;
    }
    InterpretationState Interpretation(const std::string& term) const {
        if (term != "term.veyl" || known_.empty()) return InterpretationState::Unknown;
        return known_.size() == 2 ? InterpretationState::Understood : InterpretationState::Partial;
    }
    const std::vector<std::string>& Snapshot() const { return known_; }
    bool Restore(const std::vector<std::string>& saved) {
        if (saved.size() > 2) return false;
        for (std::size_t i=0; i<saved.size(); ++i) {
            if (!IsKnownEvidence(saved[i])) return false;
            if (std::find(saved.begin(),saved.begin()+i,saved[i]) != saved.begin()+i) return false;
        }
        known_ = saved;
        return true;
    }
private:
    static bool IsKnownEvidence(const std::string& id) {
        return id == "evidence.glimpse" || id == "evidence.archivist";
    }
    std::vector<std::string> known_;
};
}
