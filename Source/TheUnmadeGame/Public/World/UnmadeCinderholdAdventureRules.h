#pragma once
// Optional local adventure. No wage, inventory, campaign or ending authority.
#include <cmath>
#include <cstdint>
namespace UnmadeCore {
enum class SlagResolution : int { None=0, Defeated=1, Quieted=2 };
enum class SlagRoute : int { Undecided=0, Maintained=1, VentOpen=2, VentSealed=3, SharedKiln=4 };
enum class SlagClue : int { Tracks=1, ServiceLog=2, Cadence=4 };
enum SlagMemory : int { BesHeard=1, VardaBriefed=2, HomeSeen=4, VardaReported=8,
    ReturnedHome=16, RhevaTold=32, OvennaTold=64, BesReported=128 };
struct CinderholdSnapshot {
    int version=1, memories=0, clues=0, resolution=0, route=0;
    int firstDay=0, returnDay=0, returnedHeat=0, returnedColdDays=0, returnedOwed=0;
};
class CinderholdAdventure {
public:
    CinderholdSnapshot Save() const { return state_; }
    bool Restore(const CinderholdSnapshot& s,int day) {
        if(day<1||s.version!=1||s.memories<0||s.memories>255||s.clues<0||s.clues>7||
           s.resolution<0||s.resolution>2||s.route<0||s.route>4||s.firstDay<0||s.firstDay>day||
           s.returnDay<0||s.returnDay>day||s.returnedHeat<0||s.returnedHeat>1000000||
           s.returnedColdDays<0||s.returnedColdDays>day||s.returnedOwed<0||s.returnedOwed>100000000)
            return false;
        const bool active=s.memories!=0||s.clues!=0||s.resolution!=0;
        if(active!=(s.firstDay>0)||(!active&&s.route!=0))return false;
        if(s.route!=0&&s.resolution==0)return false;
        // The shared-kiln pact is available only after the living elder was quieted
        // and both locally known people and every Blackslag clue were heard.
        if(s.route==4&&(s.resolution!=2||s.clues!=7||
           (s.memories&(BesHeard|VardaBriefed))!=(BesHeard|VardaBriefed)))return false;
        if(s.resolution==2&&s.clues!=7)return false;
        if((s.memories&VardaReported)&&(!(s.memories&VardaBriefed)||s.resolution==0||s.route==0))return false;
        if((s.memories&BesReported)&&(!(s.memories&BesHeard)||!(s.memories&VardaReported)))return false;
        if((s.memories&(RhevaTold|OvennaTold))&&!(s.memories&VardaReported))return false;
        const bool returned=(s.memories&ReturnedHome)!=0;
        if(returned&&(!(s.memories&HomeSeen)||!(s.memories&BesReported)||s.returnDay<s.firstDay))return false;
        if(!returned&&(s.returnDay||s.returnedHeat||s.returnedColdDays||s.returnedOwed))return false;
        state_=s; ventUntil_=foldUntil_=glimpseUntil_=-1; return true;
    }
    bool Remember(int memory,int day) {
        if(day<1||day<state_.firstDay||!(memory==BesHeard||memory==VardaBriefed||memory==HomeSeen||
           memory==VardaReported||memory==BesReported||memory==RhevaTold||memory==OvennaTold)||Has(memory))return false;
        if(memory==VardaReported&&(!Has(VardaBriefed)||!Resolved()||state_.route==0))return false;
        if(memory==BesReported&&(!Has(BesHeard)||!Has(VardaReported)))return false;
        if((memory==RhevaTold||memory==OvennaTold)&&!Has(VardaReported))return false;
        Start(day); state_.memories|=memory; return true;
    }
    bool Inspect(SlagClue clue,int day) {
        const int bit=static_cast<int>(clue);
        if(day<1||day<state_.firstDay||(bit!=1&&bit!=2&&bit!=4)||(state_.clues&bit))return false;
        Start(day);state_.clues|=bit;return true;
    }
    bool Defeat(int day) {
        if(day<1||day<state_.firstDay||Resolved())return false;
        Start(day);state_.resolution=static_cast<int>(SlagResolution::Defeated);return true;
    }
    bool Quiet(double now,int day) {
        if(day<1||day<state_.firstDay||Resolved()||state_.clues!=7||!VentActive(now))return false;
        Start(day);state_.resolution=static_cast<int>(SlagResolution::Quieted);return true;
    }
    bool Choose(SlagRoute route) {
        const int value=static_cast<int>(route);
        if(!Resolved()||state_.route!=0||value<1||value>3)return false;
        state_.route=value;return true;
    }
    bool ChooseSharedAccord(bool firsthandCinderholdFracture) {
        // Caller verifies the actual saved entry to Cinderhold's first chamber.
        // The world save rechecks that cross-system fact on load.
        if(!firsthandCinderholdFracture||state_.route!=0||state_.resolution!=2||
           state_.clues!=7||!Has(BesHeard)||!Has(VardaBriefed))return false;
        state_.route=static_cast<int>(SlagRoute::SharedKiln);return true;
    }
    bool ReturnHome(int day,int heat,int coldDays,int owed) {
        if(day<state_.firstDay||day<1||Has(ReturnedHome)||!Has(HomeSeen)||!Has(BesReported)||
           heat<0||heat>1000000||coldDays<0||coldDays>day||owed<0||owed>100000000)return false;
        state_.memories|=ReturnedHome;state_.returnDay=day;state_.returnedHeat=heat;
        state_.returnedColdDays=coldDays;state_.returnedOwed=owed;return true;
    }
    bool OpenVent(double now,bool folded=false) {
        if(!ValidTime(now)||(state_.clues&3)!=3||Resolved())return false;
        ventUntil_=now+(folded?18.0:12.0);return true;
    }
    bool Glimpse(double now) {
        if(!ValidTime(now)||now<glimpseUntil_)return false;
        glimpseUntil_=now+3.0;return true;
    }
    bool Fold(double now) {
        if(!ValidTime(now)||now<foldUntil_)return false;
        foldUntil_=now+6.0;
        if(VentActive(now))ventUntil_=now+18.0;
        return true;
    }
    bool VentActive(double now)const{return ValidTime(now)&&now<ventUntil_;}
    bool FoldActive(double now)const{return ValidTime(now)&&now<foldUntil_;}
    bool GlimpseActive(double now)const{return ValidTime(now)&&now<glimpseUntil_;}
    bool Has(int bit)const{return (state_.memories&bit)!=0;}
    bool Resolved()const{return state_.resolution!=0;}
    bool KilnPathOpen(bool daylight)const{return state_.route==2||state_.route==4||(state_.route==1&&daylight);}
    bool ArchivePathOpen()const{return state_.route==1||state_.route==3||state_.route==4;}
    const char* Objective()const {
        if(Has(ReturnedHome))return "Cinderhold adventure complete; residents remember your return";
        if(!Has(BesHeard))return "Speak with Bes; fieldwork cannot settle his unpaid wages";
        if(!Has(HomeSeen))return "Inspect Bes's household at the return shelf beside his hearth (E)";
        if(!Has(VardaBriefed))return "Ask Varda at the Blackslag entrance about the elder";
        if(!Resolved())return state_.clues==7
            ?"Cool the vent, then sound the learned cadence; or face the elder"
            :"Inspect tracks, service log and cadence plate along the covered west path (E or F)";
        if(state_.route==0)return "At vent E maintains, R/T rewrites; discover the Cinderhold fracture and inspect the shared-kiln accord stone (E) for another route";
        if(!Has(VardaReported))return "Return to Varda in person with the outcome and route decision";
        if(!Has(BesReported))return "Tell Bes what happened; he remembers which path you chose";
        return "Return to the shelf inside Bes's home (E); warmth and debt remain separate";
    }
private:
    static bool ValidTime(double t){return std::isfinite(t)&&t>=0;}
    void Start(int day){if(state_.firstDay==0)state_.firstDay=day;}
    CinderholdSnapshot state_{};
    double ventUntil_=-1,foldUntil_=-1,glimpseUntil_=-1;
};
// The elder defends a nest, never pursues civilians across the town.
// Alternating tells have a locked aim point; stepping aside defeats the strike.
enum class SlagAttack : int { None, GroundSlam, CinderSweep };
struct SlagElderStep { SlagAttack warning=SlagAttack::None; bool strike=false, recovering=false; };
class SlagElderPattern {
public:
    SlagElderStep Step(double now,bool halfHealth,bool inRange,bool clearSight,bool vent) {
        if(!std::isfinite(now)||now<0||now<last_)return {};
        last_=now;
        if(vent||!inRange||!clearSight){
            if(pending_!=SlagAttack::None){pending_=SlagAttack::None;ready_=now+1.5;}
            return {SlagAttack::None,false,now<ready_};
        }
        if(pending_!=SlagAttack::None){
            if(now<ends_)return {pending_,false,false};
            const auto strike=pending_;pending_=SlagAttack::None;
            ready_=now+(halfHealth?1.8:2.5);return {strike,true,true};
        }
        if(now<ready_)return {SlagAttack::None,false,true};
        const int attack=sequence_++;
        pending_=halfHealth&&(attack%2==1)?SlagAttack::CinderSweep:SlagAttack::GroundSlam;
        ends_=now+(pending_==SlagAttack::GroundSlam?1.6:2.0);
        return {pending_,false,false};
    }
    void Interrupt(double now){if(std::isfinite(now)&&now>=last_){pending_=SlagAttack::None;ready_=now+1.5;last_=now;}}
private:
    SlagAttack pending_=SlagAttack::None;int sequence_=0;
    double ends_=0,ready_=0,last_=-1;
};
}
