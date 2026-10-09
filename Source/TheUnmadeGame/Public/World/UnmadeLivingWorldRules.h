#pragma once
// Authored world state: offline, deterministic and independent of Unreal/LLMs.
#include "NPC/UnmadeNpcDecisionRules.h"
#include "NPC/UnmadeNpcMotionRules.h"
#include <cmath>
#include <cstdint>
#include <algorithm>
namespace UnmadeCore {
enum class DayPhase { Dawn, Day, Dusk, Night };
enum class District { EchoWell, PaperOrchard, SilentMile, BellGrave, MarketLedger, ShelterThreshold, Count };
enum class AmbientCue { EchoDay, EchoNight, PaperDay, PaperNight, SilentDay, SilentNight,
                       BellDay, BellNight, MarketDay, MarketNight,
                       ShelterUncertain, ShelterWelcoming, ShelterBarred };
class LivingWorldClock final {
public:
    static constexpr double SecondsPerDay = 1200.0;
    static constexpr double MaxRecordedSeconds = 1000000000.0;
    bool Restore(double seconds) noexcept {
        if (!std::isfinite(seconds) || seconds < 0 || seconds > MaxRecordedSeconds) return false;
        elapsed_ = seconds; return true;
    }
    bool Advance(double deltaSeconds) noexcept {
        if (!std::isfinite(deltaSeconds) || deltaSeconds <= 0 || elapsed_ >= MaxRecordedSeconds) return false;
        elapsed_ = std::min(MaxRecordedSeconds, elapsed_ + std::min(deltaSeconds, 1.0)); return true;
    }
    double ElapsedSeconds() const noexcept { return elapsed_; }
    int MinuteOfDay() const noexcept {
        const auto minutes = static_cast<std::int64_t>(std::floor(elapsed_ * 1440.0 / SecondsPerDay));
        return static_cast<int>((420 + minutes) % 1440);
    }
    int DayIndex() const noexcept {
        const auto minutes = static_cast<std::int64_t>(std::floor(elapsed_ * 1440.0 / SecondsPerDay));
        return static_cast<int>((420 + minutes) / 1440);
    }
    DayPhase Phase() const noexcept {
        const int minute = MinuteOfDay();
        if (minute >= 240 && minute < 420) return DayPhase::Dawn;
        if (minute >= 420 && minute < 1020) return DayPhase::Day;
        if (minute >= 1020 && minute < 1200) return DayPhase::Dusk;
        return DayPhase::Night;
    }
private:
    double elapsed_ = 0.0;
};
class DiscoveryLedger final {
public:
    bool Discover(District district) noexcept {
        const int id = static_cast<int>(district);
        if (id < 0 || id >= static_cast<int>(District::Count)) return false;
        const int bit = 1 << id;
        if (visited_ & bit) return false;
        visited_ |= bit; return true;
    }
    bool HasSeen(District district) const noexcept {
        const int id = static_cast<int>(district);
        return id >= 0 && id < static_cast<int>(District::Count) && (visited_ & (1 << id)) != 0;
    }
    int Count() const noexcept {
        int count = 0;
        for (int i = 0; i < static_cast<int>(District::Count); ++i) if (visited_ & (1 << i)) ++count;
        return count;
    }
    int Snapshot() const noexcept { return visited_; }
    bool Restore(int mask) noexcept {
        constexpr int validMask = (1 << static_cast<int>(District::Count)) - 1;
        if (mask < 0 || (mask & ~validMask)) return false;
        visited_ = mask; return true;
    }
private:
    int visited_ = 0;
};
inline Vec2 RoutineTarget(NpcRole role, DayPhase phase) noexcept {
    switch (role) {
    case NpcRole::Merchant: return phase == DayPhase::Night ? Vec2{-590,660} : Vec2{220,-260};
    case NpcRole::Guard: return phase == DayPhase::Night ? Vec2{-420,930}
        : phase == DayPhase::Dusk ? Vec2{0,500} : Vec2{-310,-230};
    case NpcRole::Scholar: return phase == DayPhase::Night ? Vec2{400,900}
        : phase == DayPhase::Dawn ? Vec2{-1500,800} : Vec2{-380,300};
    case NpcRole::Courier: return phase == DayPhase::Day ? Vec2{300,90}
        : phase == DayPhase::Dusk ? Vec2{210,-170} : Vec2{-500,620};
    case NpcRole::Wanderer: return phase == DayPhase::Night ? Vec2{-640,660}
        : phase == DayPhase::Dawn ? Vec2{-1500,800} : Vec2{170,340};
    }
    return {};
}
/** NPCs work near their own home during the day; at other hours they regroup by role. */
inline Vec2 RoutineTargetForHome(NpcRole role, DayPhase phase, Vec2 home) noexcept {
    if (phase == DayPhase::Day && std::isfinite(home.x) && std::isfinite(home.y))
        return home;
    return RoutineTarget(role, phase);
}
inline AmbientCue SelectAmbientCue(District district, DayPhase phase, int choice) noexcept {
    const bool night = phase == DayPhase::Night || phase == DayPhase::Dusk;
    switch (district) {
    case District::EchoWell: return night ? AmbientCue::EchoNight : AmbientCue::EchoDay;
    case District::PaperOrchard: return night ? AmbientCue::PaperNight : AmbientCue::PaperDay;
    case District::SilentMile: return night ? AmbientCue::SilentNight : AmbientCue::SilentDay;
    case District::BellGrave: return night ? AmbientCue::BellNight : AmbientCue::BellDay;
    case District::MarketLedger: return night ? AmbientCue::MarketNight : AmbientCue::MarketDay;
    case District::ShelterThreshold: return choice == 1 ? AmbientCue::ShelterWelcoming
        : choice == 2 ? AmbientCue::ShelterBarred : AmbientCue::ShelterUncertain;
    default: return AmbientCue::SilentDay;
    }
}
}
