#pragma once
// Offline character creation rules. Presentation/art will be authored in Unreal.
// Every appearance option is user-chosen; the impossible origin is shared canon.
#include <array>
#include <cstddef>
#include <string>
#include <utility>
namespace UnmadeCore {
enum class IdentityAspect : int {
    BodyFrame, Face, Hair, Voice, SkinPalette, RealityMark, Gait, Calling, Count
};
struct CharacterIdentitySnapshot {
    std::array<int,8> options{};
    std::string chosenName="The Unmade";
};
class CharacterIdentity final {
public:
    const CharacterIdentitySnapshot& Snapshot() const noexcept { return state_; }
    std::string SharedOrigin() const { return "Unmade.Origin.Impossible"; }
    static constexpr std::array<int,8> ChoiceCounts{{4,8,10,6,10,8,5,4}};
    static bool ValidName(const std::string& name) noexcept {
        if(name.empty() || name.size()>48)return false;
        bool meaningful=false;
        for(std::size_t i=0;i<name.size();) {
            const auto c=static_cast<unsigned char>(name[i]);
            if(c<128) {
                const bool letter=(c>='A'&&c<='Z')||(c>='a'&&c<='z');
                const bool numeral=(c>='0'&&c<='9');
                const bool punctuation=c==' '||c=='-'||c=='_'||c=='.'||c=='\'';
                if(!letter && !numeral && !punctuation)return false;
                meaningful|=letter||numeral;
                ++i;
                continue;
            }
            int trailing=0;
            if(c>=0xC2 && c<=0xDF)trailing=1;
            else if(c>=0xE0 && c<=0xEF)trailing=2;
            else if(c>=0xF0 && c<=0xF4)trailing=3;
            else return false;
            if(i+static_cast<std::size_t>(trailing)>=name.size())return false;
            const auto first=static_cast<unsigned char>(name[i+1]);
            if((c==0xE0 && first<0xA0) ||
               (c==0xED && first>=0xA0) ||
               (c==0xF0 && first<0x90) ||
               (c==0xF4 && first>=0x90) ||
               (c==0xC2 && first<0xA0))return false;
            for(int t=1;t<=trailing;++t) {
                const auto next=static_cast<unsigned char>(name[i+t]);
                if(next<0x80 || next>0xBF)return false;
            }
            meaningful=true;
            i+=static_cast<std::size_t>(trailing)+1;
        }
        return meaningful;
    }
    bool Select(IdentityAspect aspect,int value) noexcept {
        const int idx=static_cast<int>(aspect);
        if(idx<0 || idx>=8 || value<0 || value>=ChoiceCounts[idx])return false;
        state_.options[idx]=value;
        return true;
    }
    bool Rename(std::string name) {
        if(!ValidName(name))return false;
        state_.chosenName=std::move(name);
        return true;
    }
    bool Restore(const CharacterIdentitySnapshot& saved) {
        if(!ValidName(saved.chosenName))return false;
        for(int i=0;i<8;++i)
            if(saved.options[i]<0 || saved.options[i]>=ChoiceCounts[i])return false;
        state_=saved;
        return true;
    }
private:
    CharacterIdentitySnapshot state_{};
};
} // namespace UnmadeCore
