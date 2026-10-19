#pragma once
// The Unmade - earned research, not a universal enemy-skill theft mechanic.
// Standalone C++17, deterministic, save-backed through the Unreal adapter.
#include <array>
#include <cstdint>

namespace UnmadeCore {
enum class StudyId : int { Cinderjaw=0, PaleStrider=1, RedactionEye=2, EchoWell=3, Count=4 };
enum class StudyStage : int { Unknown=0, Observed=1, Understood=2, Experimented=3, Mastered=4 };
enum class StudyResult { Advanced, EvidenceRecorded, AlreadyRecorded, NeedsEvidence,
                         NeedsUnderstanding, NeedsExperiments, AlreadyMastered, Invalid };
inline constexpr int StudyCount=static_cast<int>(StudyId::Count);
inline constexpr std::array<const char*,StudyCount> StudyNames={{
    "Kiln Ward / Cinderjaw", "Seam Step / Pale Strider",
    "Redaction Read / Redaction Eye", "Memory Lens / Echo Well"
}};
struct StudySnapshot {
    std::array<int,StudyCount> stages{};
    std::array<int,StudyCount> evidence{};
    std::array<int,StudyCount> trials{};
};
class AbilityStudy final {
public:
    const StudySnapshot& Snapshot()const noexcept{return state_;}
    bool Restore(const StudySnapshot& next) noexcept {
        for(int i=0;i<StudyCount;++i) {
            const int s=next.stages[i],e=next.evidence[i],t=next.trials[i];
            if(s<0||s>4||e<0||e>3||t<0||t>2)return false;
            if(s==0&&(e!=0||t!=0))return false;
            if(s>0&&!(e&1))return false; // physical firsthand encounter mandatory
            if(s>=2&&e!=3)return false; // a second independent physical signal
            if(s<2&&t!=0)return false;
            if(s==3&&t!=2)return false;
            if(s==4&&t!=2)return false;
        }
        state_=next;return true;
    }
    StudyStage Stage(StudyId id)const noexcept{
        int i=static_cast<int>(id);return Valid(i)?static_cast<StudyStage>(state_.stages[i]):StudyStage::Unknown;
    }
    int Evidence(StudyId id)const noexcept{const int i=static_cast<int>(id);return Valid(i)?state_.evidence[i]:0;}
    int Trials(StudyId id)const noexcept{const int i=static_cast<int>(id);return Valid(i)?state_.trials[i]:0;}
    // Only callers that verified the living opponent or firsthand world site may supply bits.
    StudyResult Observe(StudyId id,int sourceBit) noexcept {
        const int i=static_cast<int>(id);
        if(!Valid(i)||(sourceBit!=1&&sourceBit!=2))return StudyResult::Invalid;
        if(sourceBit==2 && !(state_.evidence[i]&1))return StudyResult::NeedsEvidence;
        if(state_.evidence[i]&sourceBit)return StudyResult::AlreadyRecorded;
        if(state_.stages[i]>=2)return StudyResult::AlreadyRecorded;
        state_.evidence[i]|=sourceBit;
        if(state_.stages[i]==0)state_.stages[i]=1;
        return StudyResult::EvidenceRecorded;
    }
    StudyResult Understand(StudyId id)noexcept{
        const int i=static_cast<int>(id);
        if(!Valid(i))return StudyResult::Invalid;
        if(state_.stages[i]!=1)return StudyResult::NeedsEvidence;
        if(state_.evidence[i]!=3)return StudyResult::NeedsEvidence;
        state_.stages[i]=2;return StudyResult::Advanced;
    }
    // Successful, paid, physically performed trials; no award for button spam.
    StudyResult Experiment(StudyId id)noexcept{
        const int i=static_cast<int>(id);
        if(!Valid(i))return StudyResult::Invalid;
        if(state_.stages[i]!=2)return StudyResult::NeedsUnderstanding;
        if(state_.trials[i]>=2)return StudyResult::Invalid;
        ++state_.trials[i];
        if(state_.trials[i]==2)state_.stages[i]=3;
        return StudyResult::Advanced;
    }
    StudyResult Master(StudyId id)noexcept{
        const int i=static_cast<int>(id);
        if(!Valid(i))return StudyResult::Invalid;
        if(state_.stages[i]==4)return StudyResult::AlreadyMastered;
        if(state_.stages[i]!=3||state_.trials[i]!=2)return StudyResult::NeedsExperiments;
        state_.stages[i]=4;return StudyResult::Advanced;
    }
    bool HasMastered(StudyId id)const noexcept{return Stage(id)==StudyStage::Mastered;}
    bool CanCombine(StudyId a,StudyId b)const noexcept{
        return a!=b&&HasMastered(a)&&HasMastered(b);
    }
    bool AtlasReady()const noexcept{
        int earned=0;for(int i=0;i<StudyCount;++i)earned+=state_.stages[i]==4;
        return earned>=3&&HasMastered(StudyId::EchoWell);
    }
private:
    static bool Valid(int id)noexcept{return id>=0&&id<StudyCount;}
    StudySnapshot state_{};
};
} // namespace UnmadeCore
