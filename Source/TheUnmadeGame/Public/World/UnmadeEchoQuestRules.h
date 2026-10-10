#pragma once
// THE UNMADE: six optional third-chapter echo quests.
// Offline C++17 narrative authority, independent of presentation/AI.
// Append-only index matches LaterRealms; all transitions are one-shot.
#include "World/UnmadeLaterRealmRules.h"
#include <array>
#include <cstring>
namespace UnmadeCore {
struct EchoQuestSpec {
    Realm realm;
    const char* title;
    const char* subtitle;
    const char* openingWitness;
    const char* evidenceA; const char* evidenceB;
    const char* evidenceALine; const char* evidenceBLine;
    const char* witnessCare; const char* witnessTruth;
    const char* testimonyCare; const char* testimonyTruth;
    const char* ritualCare; const char* ritualTruth;
    const char* resultCare; const char* resultTruth;
    const char* farRealmMessage; // reported *only* by public route markers
};
inline constexpr std::array<EchoQuestSpec,6> EchoQuests={{
 {Realm::TidalLedger,"The Child Who Repaid Tomorrow","Third chapter: an impossible debt with a living guarantor",
  "npc.tidal.ledgerkeeper.001","Tidal.Echo.CradleSeal","Tidal.Echo.LenderKeel",
  "The cradle seal bears a timestamp from an unborn winter; it was offered without its owner's consent.",
  "The lender's keel shows the debt was already collected from a vessel that never sailed.",
  "npc.tidal.ferryman.001","npc.tidal.witness.001",
  "The ferryman knows a sheltered child who has never crossed this water and refuses to use the name as collateral.",
  "The witness remembers signing a loan by lantern light, but the signature was written a century later.",
  "Tidal.Echo.RefugeBell","Tidal.Echo.OpenArchive",
  "A child-safe ferry begins running free; the names of absent borrowers remain in trust.",
  "A public archive records the fraudulent futures; every new borrower may inspect the ledger.",
  "The debts of an unborn child reached a shore without a sea."},
 {Realm::SkyBelow,"The Silence Between Two Notes","Third chapter: whose voice holds up a house?",
  "npc.sky.choirmaster.001","Sky.Echo.EmptyScore","Sky.Echo.CeilingNail",
  "The missing score was removed by order, but the rest of the choir still counts the beat.",
  "A nail beneath the roof holds the district steady only while one singer is denied a name.",
  "npc.sky.structuralist.001","npc.sky.dissenter.001",
  "The structuralist counted the houses that would fall if the choir stopped; she knows exactly which children sleep inside.",
  "The dissenter kept every erased note and can identify the singers ordered into silence.",
  "Sky.Echo.SharedMeter","Sky.Echo.VoiceStage",
  "The neighborhood shares its burden through a public supporting cadence; no one sings alone.",
  "An open stage restores the silenced names; the city accepts a harder harmony.",
  "An erased voice began answering from underneath the Orchard."},
 {Realm::CinderSpine,"The Mason's Borrowed Grandmother","Third chapter: when history itself is building material",
  "npc.bones.seamwarden.001","Bones.Echo.FamilyMortar","Bones.Echo.EmptyPortrait",
  "Mortar binds two family houses using a name erased from every register.",
  "The portrait contains a woman whose grandchildren remember her warmth, although she never existed.",
  "npc.bones.mason.001","npc.bones.descendant.001",
  "The mason can brace the houses without mining another forgotten generation.",
  "The descendant knows the face in the stone, though the census insists the family ended.",
  "Bones.Echo.HearthBrace","Bones.Echo.AncestorWall",
  "The homes receive safe stone supports; the quarry stops taking histories beneath them.",
  "An ancestor memorial exposes the erased workers and forces the quarry to rewrite its contracts.",
  "Someone from an impossible family remembered the hero by another name."},
 {Realm::HundredUnlived,"An Address For Nobody","Third chapter: a person without an official yesterday",
  "npc.unlived.censuskeeper.001","Unlived.Echo.BlankKey","Unlived.Echo.UnsentWelcome",
  "The blank key opens a home without recording the person who enters it.",
  "A welcome letter was sent to someone the city claims it cannot house.",
  "npc.unlived.caretaker.001","npc.unlived.challenger.001",
  "The caretaker knows the names only because people told her; she will not submit them to the census.",
  "The challenger demands a right to be counted without letting anyone revoke their existence.",
  "Unlived.Echo.QuietHome","Unlived.Echo.PublicThreshold",
  "A hidden row of homes gains stable doors for guests who choose anonymity.",
  "A voluntary civic threshold recognizes impossible citizens without coercive registry.",
  "An unclaimed address appeared in the Orchard's oldest title."},
 {Realm::OrchardOfKings,"The Coronation of the Soil","Third chapter: a crown no one has permission to wear",
  "npc.orchard.untitled.001","Orchard.Echo.RootVote","Orchard.Echo.CrownHollow",
  "The roots voted in rings long before any monarch could speak for the orchard.",
  "The hollow crown holds no head; its inscription asks who authorized the empty throne.",
  "npc.orchard.gardener.001","npc.orchard.historian.001",
  "The gardener knows which households rely on the public harvest and which roads roots still obstruct.",
  "The historian found the first coronation law, signed by people who refused to be ruled.",
  "Orchard.Echo.HarvestCommons","Orchard.Echo.CourtWithoutKing",
  "The harvest commons opens to all neighboring communities without requiring allegiance.",
  "An empty court records every disputed title without crowning a ruler.",
  "The crown's first law was written by those who declined to wear it."},
 {Realm::FirstAbsence,"The Witness Who Arrived Before You","Third chapter: an ending that refuses to end the world",
  "npc.absence.firstwitness.001","Absence.Echo.BeforeFootprint","Absence.Echo.SecondBeginning",
  "A footprint waits before the ground is made; it matches a traveler who has not arrived.",
  "Two mutually true beginnings are written together; neither can be removed without losing a witness.",
  "npc.absence.caretaker.001","npc.absence.otherwitness.001",
  "The caretaker can guide newcomers safely through one honest threshold without demanding a universal history.",
  "The other witness remembers the world both beginning and not beginning; neither account contradicts their life.",
  "Absence.Echo.HarborOfStarts","Absence.Echo.ManyMornings",
  "A welcome refuge preserves one reliable approach for future travelers, without overwriting earlier realms.",
  "A public hall holds contradictory histories side by side; nobody is required to choose a single origin.",
  "The ending remained open, and every old realm kept its own witnesses."}
}};
enum class EchoResult { Locked, Started, Discovered, Testified, Committed,
    WrongSite, WrongWitness, NeedEvidence, WrongChoice, NotReady, NoChange };
struct EchoSnapshot {
    std::array<int,6> stage{}; // 0 dormant, 1 investigate, 2 testified, 3 committed
    std::array<int,6> evidence{}; // 0 none, 1 A, 2 B, 3 both
    std::array<int,6> testimony{}; // 0 none, 1 care, 2 truth
    std::array<int,6> ending{}; // 0 none, 1 care, 2 truth
};
class EchoChronicle final {
public:
    const EchoSnapshot& Snapshot() const noexcept {return data_;}
    bool Restore(const EchoSnapshot& s) noexcept {
        for(int i=0;i<6;++i) {
            if(s.stage[i]<0||s.stage[i]>3 || s.evidence[i]<0||s.evidence[i]>3 ||
               s.testimony[i]<0||s.testimony[i]>2 || s.ending[i]<0||s.ending[i]>2 ||
               (s.stage[i]==0 && (s.evidence[i]||s.testimony[i]||s.ending[i])) ||
               (s.stage[i]==1 && (s.testimony[i]||s.ending[i])) ||
               (s.stage[i]>=2 && (s.evidence[i]!=3||s.testimony[i]==0)) ||
               (s.stage[i]==2 && s.ending[i]!=0) ||
               (s.stage[i]==3 && s.ending[i]!=s.testimony[i]))
                return false;
        }
        data_=s;return true;
    }
    int Stage(Realm realm)const noexcept {
        const int i=LaterIndex(realm);return i<0?0:data_.stage[i];
    }
    int Evidence(Realm realm)const noexcept {
        const int i=LaterIndex(realm);return i<0?0:data_.evidence[i];
    }
    int Ending(Realm realm)const noexcept {
        const int i=LaterIndex(realm);return i<0?0:data_.ending[i];
    }
    EchoResult Begin(Realm realm,bool firstArcDone,bool aftermathDone,
                     const char* witness) noexcept {
        const int i=LaterIndex(realm);
        if(i<0)return EchoResult::NoChange;
        if(data_.stage[i]!=0)return EchoResult::NoChange;
        if(!firstArcDone||!aftermathDone)return EchoResult::Locked;
        if(!witness||std::strcmp(witness,EchoQuests[i].openingWitness)!=0)
            return EchoResult::WrongWitness;
        data_.stage[i]=1;return EchoResult::Started;
    }
    EchoResult Inspect(Realm realm,const char* tag)noexcept {
        const int i=LaterIndex(realm);
        if(i<0||data_.stage[i]!=1)return EchoResult::NoChange;
        int bit=0;
        if(tag && std::strcmp(tag,EchoQuests[i].evidenceA)==0)bit=1;
        else if(tag && std::strcmp(tag,EchoQuests[i].evidenceB)==0)bit=2;
        else return EchoResult::WrongSite;
        if(data_.evidence[i]&bit)return EchoResult::NoChange;
        data_.evidence[i]|=bit;return EchoResult::Discovered;
    }
    EchoResult Testify(Realm realm,const char* witness)noexcept {
        const int i=LaterIndex(realm);
        if(i<0||data_.stage[i]!=1)return EchoResult::NoChange;
        int choice=0;
        if(witness && std::strcmp(witness,EchoQuests[i].witnessCare)==0)choice=1;
        else if(witness && std::strcmp(witness,EchoQuests[i].witnessTruth)==0)choice=2;
        else return EchoResult::WrongWitness;
        if(data_.evidence[i]!=3)return EchoResult::NeedEvidence;
        data_.testimony[i]=choice;data_.stage[i]=2;
        return EchoResult::Testified;
    }
    EchoResult Commit(Realm realm,int choice,const char* site)noexcept {
        const int i=LaterIndex(realm);
        if(i<0||data_.stage[i]!=2)return EchoResult::NoChange;
        if(choice!=data_.testimony[i])return EchoResult::WrongChoice;
        const char* expected=choice==1?EchoQuests[i].ritualCare:EchoQuests[i].ritualTruth;
        if(!site||std::strcmp(site,expected)!=0)return EchoResult::WrongSite;
        data_.ending[i]=choice;data_.stage[i]=3;
        return EchoResult::Committed;
    }
private: EchoSnapshot data_{};
};
} // namespace UnmadeCore
