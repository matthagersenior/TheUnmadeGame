#pragma once
// V6 authored, deterministic cinematic timeline. Portable C++17 by design.
// These are actual spoken/subtitled screenplay rows and virtual production
// cues; missing VO/animation/Sequencer assets must NOT be called finished.
#include "World/UnmadeBesHouseholdRules.h"
#include <array>
#include <cstddef>
#include <cstring>

namespace UnmadeCore {
enum class UnpaidScene : int {
    FirstWage=0, HomeCold=1, HearthWarm=2, Ledger=3,
    Refusal=4, Restitution=5, MeraOffer=6, MeraSacrifice=7,
    BesReturns=8, MeraReturns=9, MaskPaid=10, MaskUnderwritten=11,
    MaskUnpaid=12, Count=13
};
struct UnpaidBeat {
    UnpaidScene scene;
    int beat;
    const char* speakerId; // exact stable NPC ID or "stage" for narration
    const char* subtitle;  // full caption, never covered by choice buttons
    const char* camera;    // authored camera rig cue, not a fabricated shot
    const char* blocking;  // what a real actor/prop must physically do
    const char* foley;     // event identifier; silence is explicit
    const char* voice;     // performance direction; VO still to record
    float duration;
};
// Keep subtitles brief and display sequentially. Never cram several lines
// into a poster card, nor substitute synthetic speech for final actors.
inline constexpr std::array<UnpaidBeat,39> UnpaidShiftBeats={{
 {UnpaidScene::FirstWage,0,"stage","The communal flame wakes before the people who carry it.","cinderhold_roof_wide","smoke and cart enter dawn","wind_low","diegetic silence",4.0f},
 {UnpaidScene::FirstWage,1,"npc.cinderhold.courier.001","The baskets arrived. My wages didn't.","bes_hand_close","Bes sets empty coal basket down","basket_wood_drop","restrained, not pleading",3.5f},
 {UnpaidScene::FirstWage,2,"npc.cinderhold.courier.001","Before they write me as absent, come see my home.","bes_mid_eye","Bes holds gaze then returns home","ash_steps","hard-earned hesitation",4.0f},
 {UnpaidScene::HomeCold,0,"stage","A place can be occupied and still be cold.","hearth_interior_wide","camera crosses open doorway, bed at right","door_draft","quiet room tone",4.2f},
 {UnpaidScene::HomeCold,1,"npc.cinderhold.courier.001","One basket yesterday. Nothing for tonight.","hearth_basket_close","Bes lifts the final empty basket","basket_rattle","plain fact",3.6f},
 {UnpaidScene::HomeCold,2,"stage","Look at the hearth. The unpaid ledger is elsewhere.","hearth_to_ledger_rack","pan to two separate props","fire_dies","hold without musical rescue",3.8f},
 {UnpaidScene::HearthWarm,0,"stage","Five real baskets cross the threshold.","parcel_tracking","player's carrying prop lowers into bin","coal_stack","no triumph sting",3.2f},
 {UnpaidScene::HearthWarm,1,"npc.cinderhold.courier.001","That gives us tomorrow. It does not pay for yesterday.","bes_hearth_two_shot","Bes lights a measured coal ember","ember_catch","gratitude with boundary",4.2f},
 {UnpaidScene::HearthWarm,2,"stage","The wage claim remains on the public ledger.","ledger_cutaway","camera reveals still-unpaid board","quiet_ledger","restrained",2.8f},
 {UnpaidScene::Ledger,0,"npc.cinderhold.emberwarden.001","He worked. The pay mark never moved.","rheva_receipt_insert","Rheva reads a specific municipal entry","ledger_page_turn","responsible, ashamed",4.0f},
 {UnpaidScene::Ledger,1,"npc.cinderhold.emberwarden.001","I counted the flame. I forgot whose hands held it.","rheva_half_profile","hand pauses above empty wage column","iron_mark_on_table","no excuses",4.5f},
 {UnpaidScene::Ledger,2,"stage","The source is documented. The choices now cost something.","public_ledger_wide","reveal work notice and marked routes","distant_kiln","allow player control",3.2f},
 {UnpaidScene::Refusal,0,"npc.cinderhold.courier.001","Then the cart can wait.","bes_tracking_departure","Bes unhooks harness and walks toward home","cart_harness_unfasten","resolution, not anger",3.3f},
 {UnpaidScene::Refusal,1,"stage","One fewer worker will tend Cinderhold tomorrow.","kiln_empty_space","empty station remains unfilled","kiln_missing_rhythm","absence in music",4.0f},
 {UnpaidScene::Refusal,2,"npc.cinderhold.healer.001","The cold won't agree with every part of his choice.","ovenna_reaction","Ovenna checks the communal vent","vent_low","compassionate conflict",3.9f},
 {UnpaidScene::Restitution,0,"npc.cinderhold.emberwarden.001","These marks were owed. Here is the transfer.","rheva_hand_ledger","count finite real coins into wage pouch","six_mark_count","accountable",3.5f},
 {UnpaidScene::Restitution,1,"npc.cinderhold.courier.001","I will work tomorrow. I will remember what it took.","bes_wage_receipt","Bes checks pouch, then signs the record","pen_scratch","relieved but not forgiving",4.0f},
 {UnpaidScene::Restitution,2,"stage","The cold days remain in his household history.","hearth_old_ash","camera holds on residue under the hearth","ash_settle","unresolved memory",3.0f},
 {UnpaidScene::MeraOffer,0,"npc.saltwake.courier.001","A city could owe me next. It already owes him.","mera_dock_profile","Mera examines real record at wet dock","rain_on_rope","measured skepticism",4.1f},
 {UnpaidScene::MeraOffer,1,"npc.saltwake.courier.001","Six marks at most. Mine, not the tide's.","mera_hand_coin","Mera counts savings but does not hand them over yet","coin_touch","explicit consent",4.2f},
 {UnpaidScene::MeraOffer,2,"stage","Her consent is recorded. The transfer still requires action.","saltwake_guarantee_prop","camera reveals real guarantee station","paper_folds","give player control",3.3f},
 {UnpaidScene::MeraSacrifice,0,"stage","The sum moves out of Mera's wages.","wage_transfer_split","simultaneous source and recipient cutaways","single_coin_transfer","avoid magical flourish",3.8f},
 {UnpaidScene::MeraSacrifice,1,"npc.saltwake.courier.001","He can keep the hearth. Now they must repay me.","mera_close","Mera holds her own diminished pouch","wet_paper_seal","firm, vulnerable",4.3f},
 {UnpaidScene::MeraSacrifice,2,"stage","The claim changes hands. It never disappears.","claim_board_detail","signed new creditor entry","ledger_stamp","unresolved debt motif",3.7f},
 {UnpaidScene::BesReturns,0,"stage","The hearth remembers the nights it went cold.","hearth_return_wide","repeat original shot with changed fuel and prop wear","room_tone_return","historical contrast",4.0f},
 {UnpaidScene::BesReturns,1,"npc.cinderhold.courier.001","You came back. That means I can tell you what changed.","bes_return_two_shot","Bes looks toward house then visitor","door_wood","specific familiarity",4.1f},
 {UnpaidScene::BesReturns,2,"stage","The journal and the household preserve exact work and warmth.","record_insert","show numbers only after sourced testimony","ledger_close","readable screen",3.0f},
 {UnpaidScene::MeraReturns,0,"stage","Saltwake's dock is wet. The claim is still legible.","saltwake_return_wide","same tide-side blocking as consent scene","dock_water","return motif",3.5f},
 {UnpaidScene::MeraReturns,1,"npc.saltwake.courier.001","I chose the loan. The debt is theirs to honor.","mera_receipt_close","Mera displays dated signed claim","salt_paper_unfold","agency intact",4.2f},
 {UnpaidScene::MeraReturns,2,"stage","A sacrifice survives where a debt was moved.","saltwake_ledger_insert","focus on outstanding balance not reward points","one_distant_bell","intimate",3.1f},
 {UnpaidScene::MaskPaid,0,"stage","Nhal-Vey encounters a debt paid with actual marks.","mask_witness_overlay","hearth and payer's hands cross-cut","iron_mark_on_table","counter-memory",4.0f},
 {UnpaidScene::MaskPaid,1,"npc.cinderhold.courier.001","You can bend a law. You still have to keep a promise.","bes_echo_portrait","Bes's preserved identity appears without teleporting body","ember_breath","steady testimony",4.7f},
 {UnpaidScene::MaskPaid,2,"stage","No victory is granted. The battle still belongs to the player.","boss_mask_return","return uninterrupted boss control","boss_ambience","no cinematic skip",3.5f},
 {UnpaidScene::MaskUnderwritten,0,"stage","Nhal-Vey sees a promise still owed to Mera.","mask_debt_overlay","two hands separate across distant realms","paper_seal","unfinished",3.9f},
 {UnpaidScene::MaskUnderwritten,1,"npc.saltwake.courier.001","I gave my pay. They have not earned my silence.","mera_echo_portrait","retain named source; only memory, not remote presence","tide_one_note","stubborn",4.6f},
 {UnpaidScene::MaskUnderwritten,2,"stage","The successor debt endures after the vision.","boss_mask_return","resume the same fight","boss_ambience","no free damage",3.5f},
 {UnpaidScene::MaskUnpaid,0,"stage","An empty work cart interrupts the impossible argument.","mask_absence_cut","negative space where worker stood","kiln_missing_rhythm","accusatory silence",4.0f},
 {UnpaidScene::MaskUnpaid,1,"npc.cinderhold.courier.001","I refused the shift. The cold was real.","bes_refusal_memory","heard as a witnessed echo only","coal_basket_empty","incomplete claim",4.5f},
 {UnpaidScene::MaskUnpaid,2,"stage","The disputed wage remains unpaid. The player must still fight.","boss_mask_return","restore combat immediately","boss_ambience","no outcome override",3.5f}
}};
// Descriptive captions are authored from the event identifier, not from the
// optional WAV asset. Keep each cue legible to players who cannot hear it.
inline const char* UnpaidFoleyCaption(const char* cue) noexcept {
    if(!cue)return "";
#define UNPAID_FOLEY_CAPTION(Id, Text) \
    if(std::strcmp(cue,#Id)==0)return "[" Text "]"
    UNPAID_FOLEY_CAPTION(ash_settle,"ash settles softly");
    UNPAID_FOLEY_CAPTION(ash_steps,"footsteps cross loose ash");
    UNPAID_FOLEY_CAPTION(basket_rattle,"an empty basket rattles");
    UNPAID_FOLEY_CAPTION(basket_wood_drop,"a wooden basket lands");
    UNPAID_FOLEY_CAPTION(boss_ambience,"the boss arena hums");
    UNPAID_FOLEY_CAPTION(cart_harness_unfasten,"cart harness unfastens");
    UNPAID_FOLEY_CAPTION(coal_basket_empty,"an empty coal basket shifts");
    UNPAID_FOLEY_CAPTION(coal_stack,"coal pieces settle into a stack");
    UNPAID_FOLEY_CAPTION(coin_touch,"a coin is touched");
    UNPAID_FOLEY_CAPTION(distant_kiln,"a distant kiln rumbles");
    UNPAID_FOLEY_CAPTION(dock_water,"water moves against the dock");
    UNPAID_FOLEY_CAPTION(door_draft,"wind slips through the doorway");
    UNPAID_FOLEY_CAPTION(door_wood,"a wooden door shifts");
    UNPAID_FOLEY_CAPTION(ember_breath,"an ember breathes low");
    UNPAID_FOLEY_CAPTION(ember_catch,"an ember catches");
    UNPAID_FOLEY_CAPTION(fire_dies,"the hearth fire dies down");
    UNPAID_FOLEY_CAPTION(iron_mark_on_table,"an iron mark taps the table");
    UNPAID_FOLEY_CAPTION(kiln_missing_rhythm,"the kiln rhythm falters");
    UNPAID_FOLEY_CAPTION(ledger_close,"the ledger closes");
    UNPAID_FOLEY_CAPTION(ledger_page_turn,"a ledger page turns");
    UNPAID_FOLEY_CAPTION(ledger_stamp,"a stamp strikes the ledger");
    UNPAID_FOLEY_CAPTION(one_distant_bell,"one distant bell rings");
    UNPAID_FOLEY_CAPTION(paper_folds,"paper folds once");
    UNPAID_FOLEY_CAPTION(paper_seal,"paper is sealed");
    UNPAID_FOLEY_CAPTION(pen_scratch,"a pen scratches across paper");
    UNPAID_FOLEY_CAPTION(quiet_ledger,"the ledger room falls quiet");
    UNPAID_FOLEY_CAPTION(rain_on_rope,"rain patters on dock rope");
    UNPAID_FOLEY_CAPTION(room_tone_return,"quiet room tone returns");
    UNPAID_FOLEY_CAPTION(salt_paper_unfold,"salt-stiff paper unfolds");
    UNPAID_FOLEY_CAPTION(single_coin_transfer,"one coin changes hands");
    UNPAID_FOLEY_CAPTION(six_mark_count,"six coins are counted");
    UNPAID_FOLEY_CAPTION(tide_one_note,"a single low tide note");
    UNPAID_FOLEY_CAPTION(vent_low,"air moves through a low vent");
    UNPAID_FOLEY_CAPTION(wet_paper_seal,"a wet paper seal presses down");
    UNPAID_FOLEY_CAPTION(wind_low,"low wind passes over the roof");
#undef UNPAID_FOLEY_CAPTION
    return "";
}
inline constexpr const UnpaidBeat* UnpaidBeatAt(UnpaidScene scene,int beat) noexcept{
    if(scene<UnpaidScene::FirstWage||scene>=UnpaidScene::Count||beat<0)return nullptr;
    for(const auto& s:UnpaidShiftBeats)
        if(s.scene==scene&&s.beat==beat)return &s;
    return nullptr;
}
inline constexpr int UnpaidBeatCount(UnpaidScene scene)noexcept{
    int count=0;
    for(const auto& s:UnpaidShiftBeats)if(s.scene==scene)++count;
    return count;
}
inline constexpr UnpaidScene BesReturnScene(const BesHouseholdSnapshot& h)noexcept{
    (void)h;
    return UnpaidScene::BesReturns; // text readouts remain save-specific
}
inline constexpr UnpaidScene BesMaskScene(BesPhase phase)noexcept{
    if(phase==BesPhase::Settled)return UnpaidScene::MaskPaid;
    if(phase==BesPhase::Underwritten)return UnpaidScene::MaskUnderwritten;
    return UnpaidScene::MaskUnpaid;
}
} // namespace UnmadeCore
