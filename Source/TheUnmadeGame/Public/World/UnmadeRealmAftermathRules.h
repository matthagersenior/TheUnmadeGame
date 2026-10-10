#pragma once
// A nine-realm continuity layer: distinct authored aftermath investigations.
// Realm data do not claim unbuilt levels are playable. The realm atlas remains
// source of truth for names, geography, attunement and connections.
#include "World/UnmadeWorldAtlas.h"
#include <array>
#include <cstring>
namespace UnmadeCore {
enum class RealmAftermathResult {
    Locked, Started, EvidenceFound, Witnessed, Resolved,
    WrongSite, WrongWitness, WrongChoice, NoChange,
    NeedMechanism, WrongMechanism, Prepared
};
struct RealmAftermathSpec {
    Realm realm;
    const char* title;
    const char* initiatingWitness;
    const char* mechanismSite; // physical flood/vent control before either evidence route
    const char* evidenceForCare;
    const char* evidenceForTruth;
    const char* careWitness;
    const char* truthWitness;
    const char* careConsequence;
    const char* truthConsequence;
    const char* signatureAbility;
    const char* ethicalDilemma;
    const char* aftershock;
    bool initialLevelExists;
};
inline constexpr std::array<RealmAftermathSpec,9> RealmAftermathSpecs={{
    {Realm::ThreefoldReach,"The Road After the Rescue",
     "npc.roadwarden.001","Crossings.Aftermath.RoadSwitch","Crossings.Aftermath.SharedRoute",
     "Crossings.Aftermath.TollArchive","npc.bridgekeeper.001",
     "npc.archivist.001","The public crossing remains open after dark.",
     "Hidden toll histories become public records.","Living Roads + Witnesscraft",
     "Can a road serve refugees when evidence about its former owners is concealed?",
     "A changed road draws displaced residents from a neighboring settlement.",true},
    {Realm::WidowedRain,"The Storm's Second Invoice",
     "npc.saltwake.harborwarden.001","Saltwake.Aftermath.StormSluice","Saltwake.Aftermath.Cistern",
     "Saltwake.Aftermath.RainLedger","npc.saltwake.rainkeeper.001",
     "npc.saltwake.navigator.001","Rain is collected into a shared cistern.",
     "The rain broker's false tide prices become public.","Unreliable Map + Witnesscraft",
     "Does a drought emergency justify postponing the proof of who made it profitable?",
     "The storm changes which inland roads are passable.",true},
    {Realm::HearthBeneath,"The Ember That Chose No Heir",
     "npc.cinderhold.emberwarden.001","Cinderhold.Aftermath.HeatVent","Cinderhold.Aftermath.CommonKiln",
     "Cinderhold.Aftermath.EmberDeed","npc.cinderhold.healer.001",
     "npc.cinderhold.hearthreader.001","Households share a heat-safe communal kiln.",
     "An illegal ownership claim on the eternal ember is exposed.",
     "Oathbinding + Legacy Forging",
     "Can common warmth survive when revealing its stolen origin risks extinguishing trust?",
     "The kiln's heat awakens an old passage to the mountain.",true},
    {Realm::TidalLedger,"The Unpaid Person",
     "npc.tidal.ledgerkeeper.001","Tidal.Aftermath.DebtLock","Tidal.Aftermath.RefugeBonds",
     "Tidal.Aftermath.MissingDebtor","npc.tidal.ferryman.001",
     "npc.tidal.witness.001","Future debts are forgiven for the stranded.",
     "The unborn people used as collateral are named.","Tomorrow Debt + Oathbinding",
     "Forgive real debts or expose the identities of those who never agreed to repay?",
     "A named debtor appears alive in a different historical account.",false},
    {Realm::SkyBelow,"The Note That Held a Home",
     "npc.sky.choirmaster.001","Sky.Aftermath.HarmonicTuner","Sky.Aftermath.HarmonicBrace",
     "Sky.Aftermath.SilencedVerse","npc.sky.structuralist.001",
     "npc.sky.dissenter.001","A choir-supported neighborhood gains stable footing.",
     "Forbidden notes become audible to the public.","Unwrite Law + Paradox Convergence",
     "Restore a city's gravity, or uncover whose voices were forcibly removed?",
     "A saved district begins singing a melody from the next realm.",false},
    {Realm::CinderSpine,"The Quarry of Other Yesterdays",
     "npc.bones.seamwarden.001","Bones.Aftermath.SeamBrace","Bones.Aftermath.ProtectedSeam",
     "Bones.Aftermath.DescendantLedger","npc.bones.mason.001",
     "npc.bones.descendant.001","Timeline mining is halted near living homes.",
     "The erased lineage of quarry workers is documented.","Legacy Forging + Borrowed Lives",
     "Preserve today's livelihood or confront the lives erased to make its stone?",
     "A recovered ancestor remembers the hero's alternate name.",false},
    {Realm::HundredUnlived,"The Street of Unclaimed Birthdays",
     "npc.unlived.censuskeeper.001","Unlived.Aftermath.DoorOfNames","Unlived.Aftermath.ShelterRegistry",
     "Unlived.Aftermath.VoiceLedger","npc.unlived.caretaker.001",
     "npc.unlived.challenger.001","A shelter recognizes residents without birth documents.",
     "The city publicly acknowledges its impossible citizens.","Witnesscraft + Borrowed Lives",
     "Can proof of existence protect people without giving an authority power to erase them?",
     "A recognized resident inherits a claim to the Orchard's unwritten crown.",false},
    {Realm::OrchardOfKings,"The Crown No One Wanted",
     "npc.orchard.untitled.001","Orchard.Aftermath.RootValve","Orchard.Aftermath.CommonRoots",
     "Orchard.Aftermath.UnclaimedThrone","npc.orchard.gardener.001",
     "npc.orchard.historian.001","The untitled courts maintain a public orchard.",
     "The last concealed claim to kingship is revealed.","Oathbinding + Living Roads",
     "Can refusing a throne be an act of rule—and who consents to the refusal?",
     "The crown's law exposes a route into the Place Before Place.",false},
    {Realm::FirstAbsence,"The Place After the Ending",
     "npc.absence.firstwitness.001","Absence.Aftermath.OriginDial","Absence.Aftermath.SharedBeginning",
     "Absence.Aftermath.ManyOrigins","npc.absence.caretaker.001",
     "npc.absence.otherwitness.001","A stable passage remains open for other travelers.",
     "Multiple mutually true beginnings are preserved.","Unwrite Law + Unreliable Map",
     "Should one history be made safe, or must the universe learn to live with many?",
     "No state declares the game completed: older realms remember the chosen answer.",false}
}};
struct RealmAftermathSnapshot {
    std::array<int,9> stage{}; // 0 dormant, 1 evidence, 2 testimony, 3 choice, 4 resolved
    std::array<int,9> prepared{}; // 0 mechanism untouched, 1 route physically opened
    std::array<int,9> approach{}; // 1 care, 2 public truth
    std::array<int,9> ending{};
};
class RealmAftermathChronicle final {
public:
    const RealmAftermathSnapshot& Snapshot() const noexcept {return state_;}
    bool Restore(const RealmAftermathSnapshot& s) noexcept {
        for(int i=0;i<9;++i){
            if(s.stage[i]<0 || s.stage[i]>4 ||
               s.prepared[i]<0 || s.prepared[i]>1 ||
               s.approach[i]<0 || s.approach[i]>2 ||
               s.ending[i]<0 || s.ending[i]>2 ||
               (s.stage[i]==0 && s.prepared[i]!=0) ||
               (s.stage[i]>=2 && s.prepared[i]!=1) ||
               (s.stage[i]<=1 && (s.approach[i]!=0 || s.ending[i]!=0)) ||
               ((s.stage[i]==2 || s.stage[i]==3) &&
                 (s.approach[i]==0 || s.ending[i]!=0)) ||
               (s.stage[i]==4 &&
                 (s.approach[i]==0 || s.ending[i]!=s.approach[i])))
                return false;
        }
        state_=s;return true;
    }
    int Stage(Realm r) const noexcept {
        const int i=static_cast<int>(r);
        return i>=0 && i<9?state_.stage[i]:0;
    }
    bool IsPrepared(Realm r) const noexcept {
        const int i=static_cast<int>(r);
        return i>=0 && i<9 && state_.prepared[i]==1;
    }
    int Ending(Realm r) const noexcept {
        const int i=static_cast<int>(r);
        return i>=0 && i<9?state_.ending[i]:0;
    }
    RealmAftermathResult Begin(Realm r, bool firstArcResolved,
                              bool locationVisited, const char* witness) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9)return RealmAftermathResult::NoChange;
        if(state_.stage[i]!=0)return RealmAftermathResult::NoChange;
        if(!firstArcResolved || !locationVisited)return RealmAftermathResult::Locked;
        if(!witness || std::strcmp(witness,RealmAftermathSpecs[i].initiatingWitness)!=0)
            return RealmAftermathResult::WrongWitness;
        state_.stage[i]=1;return RealmAftermathResult::Started;
    }
    RealmAftermathResult Prepare(Realm r,const char* mechanismTag) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9 || state_.stage[i]!=1 || state_.prepared[i])return RealmAftermathResult::NoChange;
        if(!mechanismTag || std::strcmp(mechanismTag,RealmAftermathSpecs[i].mechanismSite)!=0)
            return RealmAftermathResult::WrongMechanism;
        state_.prepared[i]=1;
        return RealmAftermathResult::Prepared;
    }
    RealmAftermathResult Inspect(Realm r, int approach,
                                const char* site) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9 || (state_.stage[i]!=1 && state_.stage[i]!=2))
            return RealmAftermathResult::NoChange;
        if(!state_.prepared[i])return RealmAftermathResult::NeedMechanism;
        if(approach!=1 && approach!=2)return RealmAftermathResult::WrongChoice;
        const auto& spec=RealmAftermathSpecs[i];
        const char* expected=approach==1?spec.evidenceForCare:spec.evidenceForTruth;
        if(!site || std::strcmp(expected,site)!=0)return RealmAftermathResult::WrongSite;
        state_.stage[i]=2;state_.approach[i]=approach;
        return RealmAftermathResult::EvidenceFound;
    }
    RealmAftermathResult Testify(Realm r,const char* witness) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9 || state_.stage[i]!=2)return RealmAftermathResult::NoChange;
        const auto& spec=RealmAftermathSpecs[i];
        const char* expected=state_.approach[i]==1?spec.careWitness:spec.truthWitness;
        if(!witness || std::strcmp(witness,expected)!=0)
            return RealmAftermathResult::WrongWitness;
        state_.stage[i]=3;
        return RealmAftermathResult::Witnessed;
    }
    RealmAftermathResult Decide(Realm r,int ending) noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9 || state_.stage[i]!=3)return RealmAftermathResult::NoChange;
        if(ending!=state_.approach[i])return RealmAftermathResult::WrongChoice;
        state_.stage[i]=4;state_.ending[i]=ending;
        return RealmAftermathResult::Resolved;
    }
    const char* WorldConsequence(Realm r) const noexcept {
        const int i=static_cast<int>(r);
        if(i<0 || i>=9)return "";
        return state_.ending[i]==1?RealmAftermathSpecs[i].careConsequence:
               state_.ending[i]==2?RealmAftermathSpecs[i].truthConsequence:"";
    }
private:
    RealmAftermathSnapshot state_{};
};
} // namespace UnmadeCore
