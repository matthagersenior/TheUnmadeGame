#pragma once
// THE UNMADE: ten authored, deterministic reality disciplines and 50 quest beats.
// The source model never invokes an LLM, HTTP service, or random quest generator.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace UnmadeCore {
enum class RiteId : int {
    UnwriteLaw, Witnesscraft, BorrowedLives, LegacyForging, LivingRoads,
    TomorrowDebt, UnderstandingBosses, ParadoxConvergence, Oathbinding,
    Cartography, Count
};
enum class Discipline : int { Unmaker, Witness, Elseborn, Relicwright, Oathkeeper, Pathweaver, Count };
enum class RiteAction { Discover, Testify, Trial, Decide, Master };
enum class RiteResult {
    Applied, Advanced, Completed, WrongLocation, WrongWitness, NeedsEvidence,
    NeedAbilityTrial, OutOfOrder, AlreadyCompleted, InsufficientStability,
    Cooldown, DebtOutstanding, Exhausted, BrokenOath, Invalid
};
enum class Deed : int { Protected, Discovered, Reconciled };
enum class RealityEffect {
    UnwriteLaw, Corroborate, BorrowIdentity, TemperLegacy, OpenRoad,
    FutureCredit, PacifyBoss, DualHistory, GuardOath, ChartTruth
};
struct RiteSpec {
    RiteId id;
    Discipline discipline;
    const char* name;
    const char* siteName;
    const char* witness;
    const char* lore;
    const char* reward;
    std::array<const char*,5> chapters;
    int strainCost;
    double duration;
    double cooldown;
};
// Quest chapters are separately authored, with explicit objectives and stakes.
// Later content can use the same IDs without renumbering prior saved rites.
inline constexpr std::array<RiteSpec,10> RiteSpecs = {{
    {RiteId::UnwriteLaw,Discipline::Unmaker,"The Law Beneath the Last Bell",
     "Bellgrave","npc.bellmaker.001",
     "Before the first city had walls, someone wrote the rules of falling, sound and distance into the world's stone. The hollow bell contains a syllable erased from that decree.",
     "The Bellmaker's Unwritten Verse",
     {{"Examine the bell's upward shadow and the forbidden mark beneath it.",
       "Ask the bellmaker why the town buried a bell without its keeper.",
       "Silence a small circle of the world without silencing its history.",
       "Choose whose voices survive when one law must give way.",
       "After mastering the bell's danger, return to bind one temporary law safely."}},18,12,80},
    {RiteId::Witnesscraft,Discipline::Witness,"The Bridge of Living Testimony",
     "EchoWell","npc.welllistener.001",
     "The first bridges were promises rather than stones. A crossing vanished when its last witnesses died. Three independent memories may bring its missing span into focus.",
     "The Bridge That Remembers",
     {{"Recover the lost bridge's reflection from the Well of Returned Voices.",
       "Question the well listener, separating firsthand sight from convenient rumor.",
       "Give shape to a fragile crossing using living witnesses.",
       "Decide whether the crossing serves shelter or scholarship.",
       "Return with independent witnesses and make a truthful account endure."}},14,20,75},
    {RiteId::BorrowedLives,Discipline::Elseborn,"The Life Not Taken",
     "PaperOrchard","npc.orchardexile.001",
     "Each leaf of the Orchard of Unwritten Names bears the profession of someone who was never born. One leaf carries your own handwriting, but you learned that craft in no known life.",
     "The Second Name",
     {{"Find your own impossible handwriting in the neverborn orchard.",
       "Ask the displaced keeper who remembers your unlived childhood.",
       "Borrow a possible craft for a short, dangerous interval.",
       "Choose whether to reveal your other self to those who recognize them.",
       "Return with foreign testimony and learn to relinquish a borrowed life."}},20,40,120},
    {RiteId::LegacyForging,Discipline::Relicwright,"A Blade That Was There",
     "BellwoldForge","npc.bellwold.stonewright.001",
     "Bellwold's forge will accept no anonymous steel. Its flames can read whether a blade has protected, discovered or reconciled; every deed changes which history the metal remembers.",
     "The Three-Deed Temper",
     {{"Inspect the cold forge and hear the blade's unfinished name.",
       "Ask the stonewright why relics remember how they were used.",
       "Awaken a weapon's provisional memory through a real deed.",
       "Choose what the newly awakened steel must never forget.",
       "Offer three distinct deeds, not three repeated kills, to finish the temper."}},12,55,110},
    {RiteId::LivingRoads,Discipline::Pathweaver,"No Road Belongs to One",
     "SilentMile","npc.roadwarden.001",
     "Roads once appeared whenever two communities agreed to meet. The Silent Mile was severed by a dispute over its ownership; the path now ends differently for every traveler.",
     "The Common Causeway",
     {{"Map the silent road's missing first footprint.",
       "Ask the road warden why two villages claim the same destination.",
       "Momentarily open a path that belongs to neither side.",
       "Choose whether the road serves travelers or officials.",
       "Reconcile multiple communities before stabilizing the true passage."}},16,30,90},
    {RiteId::TomorrowDebt,Discipline::Unmaker,"The Ledger of Borrowed Dawn",
     "DebtMarket","npc.tollbroker.001",
     "The market can lend someone tomorrow's strength before tomorrow exists. The debt comes due regardless of victories, and no signature can transfer that fatigue to an innocent.",
     "The Unpaid Sunrise",
     {{"Find your name dated tomorrow in the deserted market ledger.",
       "Ask the keeper of future debts who collects a day that never arrived.",
       "Borrow one short burst of strength and accept its recorded due date.",
       "Decide which promises are worth tomorrow's weakness.",
       "Survive repayment and demonstrate you can wield debt without exploiting others."}},8,17,125},
    {RiteId::UnderstandingBosses,Discipline::Witness,"The Name Beneath the Monster",
     "BellOssuary","npc.bellwold.matron.001",
     "The buried bell's keeper was condemned under an erased name. Its furious shape protects a testimony nobody was willing to hear. Learning its grief may offer another end to the encounter.",
     "Mercy of the Hollow Keeper",
     {{"Read the original keeper's epitaph behind Bellwold's ossuary.",
       "Ask the refuge matron whom the bell was supposed to warn.",
       "Use a fragment of remembered truth to interrupt a violent cycle.",
       "Choose mercy or force without erasing the keeper's existence.",
       "Confront the full encounter and return with evidence of your choice."}},24,16,140},
    {RiteId::ParadoxConvergence,Discipline::Elseborn,"The Tower That Fell and Stood",
     "PaperhavenScribe","npc.paperhaven.mapmaker.001",
     "An archive tower was destroyed during a century that the people deny occurred. Its intact shadow still contains rooms. Neither history needs to erase the other.",
     "The Room Between Histories",
     {{"Discover an intact doorway in the ruined mapmaker's archive.",
       "Ask the mapmaker which tower stands on the official chart.",
       "Hold both versions of the threshold in one brief interval.",
       "Decide which forgotten residents can pass between histories.",
       "Prove the overlap is safe by carrying real testimony through it."}},26,19,150},
    {RiteId::Oathbinding,Discipline::Oathkeeper,"The Lantern You Promised",
     "BellwoldRefuge","npc.bellwold.guard.001",
     "The guardians of Bellwold do not swear loyalty to rulers. A promise made before the communal flame becomes weight in armor and expectation in every witness.",
     "The Unbroken Shelter Oath",
     {{"Read the cracked oaths engraved beneath the refuge threshold.",
       "Ask the gate captain what protecting strangers has cost this village.",
       "Bind protection to an actual promise instead of claiming free power.",
       "Choose to honor the people or speak a dangerous public truth.",
       "Return when the oath has been tested without betraying its witnesses."}},12,50,135},
    {RiteId::Cartography,Discipline::Pathweaver,"The Map of Disagreeing Shores",
     "SaltwakeHarbor","npc.saltwake.navigator.001",
     "The sea disappeared, but the travelers' maps still disagree about its edge. The most detailed map is not always the most truthful; a safe route must survive direct inspection.",
     "Atlas of the Missing Coast",
     {{"Compare contradictory routes in Saltwake's ruined port.",
       "Ask the drowned navigator which channel she personally crossed.",
       "Chart a provisional road while marking testimony as uncertain.",
       "Choose whether to publish an unproven crossing.",
       "Verify a real route in person before granting it permanent trust."}},15,35,95}
}};
struct RiteContext {
    int site=-1;                    // physically verified ritual site (0..9)
    const char* witness=nullptr;   // nearby stable NPC ID, not text generation
    bool lineOfSight=false;
    int villageVisits=0, landmarkVisits=0, frontierVisits=0;
    int bossVictories=0, factionEndings=0, verifiedWitnesses=0;
    bool hasVeyl=false, bellQuestComplete=false, paperQuestComplete=false;
    int day=0;
    double now=0, strain=0;
    int selectedLaw=0;             // gravity, sound, or momentum
    int selectedLife=0;            // fighter, artisan, archivist
};
struct RiteEffect {
    RiteResult result=RiteResult::Invalid;
    RealityEffect kind=RealityEffect::UnwriteLaw;
    int cost=0, impact=0;
    double duration=0;
};
struct TenfoldSnapshot {
    std::array<int,10> stage{};
    std::array<int,10> choice{};
    std::array<double,10> readyAt{};
    std::array<int,6> disciplineMastery{};
    std::uint16_t trialUsed=0;
    std::uint8_t deeds=0;
    std::uint8_t verifiedRoutes=0;
    int debtDueDay=0, exhaustedUntilDay=0, brokenOaths=0;
    bool oathActive=false;
    bool oathRedeemed=false;
};
class TenfoldChronicle final {
public:
    const TenfoldSnapshot& Snapshot() const noexcept { return data_; }
    bool Restore(const TenfoldSnapshot& value) noexcept {
        for(int i=0;i<10;++i) {
            if(value.stage[i]<0 || value.stage[i]>5 ||
               value.choice[i]<0 || value.choice[i]>2 ||
               !std::isfinite(value.readyAt[i]) || value.readyAt[i]<0 ||
               (value.stage[i]>=3 && !(value.trialUsed & (1u<<i))) ||
               (value.stage[i]>=4 && value.choice[i]==0) ||
               (value.stage[i]<4 && value.choice[i]!=0)) return false;
        }
        for(int mastery:value.disciplineMastery)
            if(mastery<0 || mastery>10)return false;
        if((value.trialUsed & ~std::uint16_t{1023}) ||
           (value.deeds & ~std::uint8_t{7}) ||
           (value.verifiedRoutes & ~std::uint8_t{3}) ||
           value.debtDueDay<0 || value.exhaustedUntilDay<0 ||
           value.brokenOaths<0 || value.brokenOaths>1 ||
           (value.oathActive && value.brokenOaths!=0 && !value.oathRedeemed) ||
           (value.oathRedeemed && value.brokenOaths!=1))return false;
        data_=value;return true;
    }
    int Stage(RiteId id) const noexcept {
        const int i=static_cast<int>(id);
        return Valid(i)?data_.stage[i]:0;
    }
    int Choice(RiteId id) const noexcept {
        const int i=static_cast<int>(id);
        return Valid(i)?data_.choice[i]:0;
    }
    bool IsMastered(RiteId id) const noexcept {return Stage(id)==5;}
    int Mastery(Discipline d) const noexcept {
        const int i=static_cast<int>(d);
        return i>=0 && i<6?data_.disciplineMastery[i]:0;
    }
    RiteResult Advance(RiteId id,RiteAction step,const RiteContext& context,int decision=1) noexcept {
        const int i=static_cast<int>(id);
        if(!Valid(i) || !ValidContext(context))return RiteResult::Invalid;
        const int stage=data_.stage[i];
        if(stage==5)return RiteResult::AlreadyCompleted;
        if(static_cast<int>(step)!=stage)return RiteResult::OutOfOrder;
        const auto& spec=RiteSpecs[i];
        if(stage==0) {
            if(context.site!=i)return RiteResult::WrongLocation;
            if(!Prerequisite(id,context))return RiteResult::NeedsEvidence;
        } else if(stage==1) {
            if(!context.lineOfSight || !context.witness ||
               std::strcmp(context.witness,spec.witness)!=0)return RiteResult::WrongWitness;
        } else if(stage==2) {
            if(context.site!=i)return RiteResult::WrongLocation;
            if(!(data_.trialUsed & (1u<<i)))return RiteResult::NeedAbilityTrial;
        } else if(stage==3) {
            if(decision!=1 && decision!=2)return RiteResult::Invalid;
            if(context.verifiedWitnesses<2 ||
               !context.lineOfSight || !context.witness ||
               std::strcmp(context.witness,spec.witness)!=0)
                return RiteResult::NeedsEvidence;
            data_.choice[i]=decision;
        } else if(stage==4) {
            if(context.site!=i)return RiteResult::WrongLocation;
            if(!MasteryGate(id,context))return RiteResult::NeedsEvidence;
        }
        ++data_.stage[i];
        if(stage==4) {
            ++data_.disciplineMastery[static_cast<int>(spec.discipline)];
            return RiteResult::Completed;
        }
        return RiteResult::Advanced;
    }
    RiteEffect Invoke(RiteId id,const RiteContext& context) noexcept {
        const int i=static_cast<int>(id);
        if(!Valid(i) || !ValidContext(context))return {};
        const auto& spec=RiteSpecs[i];
        RiteEffect out{RiteResult::OutOfOrder,static_cast<RealityEffect>(i),0,0,0};
        if(data_.stage[i]<2)return out;
        if(data_.stage[i]<5 && context.site!=i) {
            out.result=RiteResult::WrongLocation;return out;
        }
        if((id==RiteId::Witnesscraft || id==RiteId::UnderstandingBosses) &&
           context.verifiedWitnesses<2) {
            out.result=RiteResult::NeedsEvidence;
            return out;
        }
        const bool bMastered=data_.stage[i]==5;
        const int cost=spec.strainCost -
            (bMastered && data_.choice[i]==1 &&
             (id==RiteId::UnwriteLaw || id==RiteId::TomorrowDebt) ? 2:0);
        if(context.strain>100.0-cost){
            out.result=RiteResult::InsufficientStability;return out;
        }
        if(id==RiteId::TomorrowDebt) {
            if(data_.debtDueDay>0){out.result=RiteResult::DebtOutstanding;return out;}
            if(context.day<data_.exhaustedUntilDay){out.result=RiteResult::Exhausted;return out;}
        }
        if(id==RiteId::Oathbinding && data_.brokenOaths>0 && !data_.oathRedeemed) {
            out.result=RiteResult::BrokenOath;return out;
        }
        if(context.now<data_.readyAt[i]){out.result=RiteResult::Cooldown;return out;}
        if(id==RiteId::UnwriteLaw && (context.selectedLaw<0 || context.selectedLaw>2))return out;
        if(id==RiteId::BorrowedLives && (context.selectedLife<0 || context.selectedLife>2))return out;
        // Different effects and strengths; no generic "press once, win" action.
        out.result=RiteResult::Applied;
        out.cost=cost;
        out.duration=spec.duration+
            (bMastered && data_.choice[i]==2 &&
             (id==RiteId::UnwriteLaw || id==RiteId::TomorrowDebt ||
              id==RiteId::ParadoxConvergence) ? 7.0:0.0);
        switch(id) {
        case RiteId::UnwriteLaw: out.impact=context.selectedLaw+1;break;
        case RiteId::Witnesscraft:out.impact=context.verifiedWitnesses;break;
        case RiteId::BorrowedLives:out.impact=context.selectedLife==0 ? 9 :
            context.selectedLife==1 ? 2 : 7;break;
        case RiteId::LegacyForging:out.impact=3+CountDeeds()*3;break;
        case RiteId::LivingRoads:out.impact=3;break;
        case RiteId::TomorrowDebt:out.impact=18;data_.debtDueDay=context.day+1;break;
        case RiteId::UnderstandingBosses:out.impact=1;break;
        case RiteId::ParadoxConvergence:out.impact=2;break;
        case RiteId::Oathbinding:out.impact=14;data_.oathActive=true;break;
        case RiteId::Cartography:out.impact=static_cast<int>(data_.verifiedRoutes)+1;break;
        default: return {};
        }
        data_.trialUsed|=1u<<i;
        data_.readyAt[i]=context.now+spec.cooldown;
        return out;
    }
    bool PassDay(int day) noexcept {
        if(day<0 || data_.debtDueDay==0 || day<data_.debtDueDay)return false;
        data_.debtDueDay=0;data_.exhaustedUntilDay=day+1;return true;
    }
    bool BreakOath() noexcept {
        if(!data_.oathActive || data_.brokenOaths!=0)return false;
        data_.oathActive=false;++data_.brokenOaths;return true;
    }
    // Redemption is difficult but a broken promise can never hard-lock the
    // entire game. The betrayal remains recorded permanently.
    bool RedeemOath(const RiteContext& c) noexcept {
        if(data_.brokenOaths!=1 || data_.oathRedeemed ||
           (c.factionEndings&1)==0 || !c.bellQuestComplete ||
           c.verifiedWitnesses<3 || CountDeeds()<3)return false;
        data_.oathRedeemed=true;
        data_.oathActive=true;
        return true;
    }
    bool RecordDeed(Deed deed) noexcept {
        const int i=static_cast<int>(deed);
        if(i<0 || i>2)return false;
        const auto bit=std::uint8_t{1}<<i;
        if(data_.deeds & bit)return false;
        data_.deeds|=bit;return true;
    }
    bool VerifyRoute(int route,int confidence,bool directlyExplored) noexcept {
        if(route<0 || route>1 || confidence<2 || confidence>3 || !directlyExplored)return false;
        const int bit=1<<route;
        if(data_.verifiedRoutes & bit)return false;
        data_.verifiedRoutes|=bit;return true;
    }
private:
    static bool Valid(int i) noexcept {return i>=0 && i<10;}
    static bool ValidContext(const RiteContext& c) noexcept {
        return c.day>=0 && std::isfinite(c.now) && c.now>=0 &&
            std::isfinite(c.strain) && c.strain>=0 && c.strain<=100;
    }
    bool Prerequisite(RiteId id,const RiteContext& c) const noexcept {
        switch(id) {
        case RiteId::UnwriteLaw:return (c.landmarkVisits&8)!=0;
        case RiteId::Witnesscraft:return (c.landmarkVisits&1)!=0 && c.hasVeyl;
        case RiteId::BorrowedLives:return (c.landmarkVisits&2)!=0 && c.paperQuestComplete;
        case RiteId::LegacyForging:return c.bellQuestComplete && (c.villageVisits&2)!=0;
        case RiteId::LivingRoads:return (c.villageVisits&7)==7;
        case RiteId::TomorrowDebt:return (c.landmarkVisits&16)!=0;
        case RiteId::UnderstandingBosses:return c.bellQuestComplete && (c.landmarkVisits&8)!=0;
        case RiteId::ParadoxConvergence:return c.paperQuestComplete && c.hasVeyl;
        case RiteId::Oathbinding:return c.bellQuestComplete;
        case RiteId::Cartography:return (c.frontierVisits&1)!=0;
        default:return false;
        }
    }
    bool MasteryGate(RiteId id,const RiteContext& c) const noexcept {
        switch(id) {
        case RiteId::UnwriteLaw:return c.bossVictories!=0;
        case RiteId::Witnesscraft:return c.verifiedWitnesses>=3;
        case RiteId::BorrowedLives:return c.frontierVisits!=0;
        case RiteId::LegacyForging:return CountDeeds()>=3;
        case RiteId::LivingRoads:return (c.factionEndings&3)==3 && c.villageVisits==7;
        case RiteId::TomorrowDebt:return data_.debtDueDay==0;
        case RiteId::UnderstandingBosses:return c.bossVictories!=0;
        case RiteId::ParadoxConvergence:return c.hasVeyl && c.factionEndings!=0;
        case RiteId::Oathbinding:return data_.oathActive && 
            (data_.brokenOaths==0 || data_.oathRedeemed);
        case RiteId::Cartography:return data_.verifiedRoutes!=0;
        default:return false;
        }
    }
    int CountDeeds() const noexcept {
        return (data_.deeds&1)+((data_.deeds>>1)&1)+((data_.deeds>>2)&1);
    }
    TenfoldSnapshot data_{};
};
} // namespace UnmadeCore
