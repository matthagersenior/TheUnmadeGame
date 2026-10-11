#pragma once
#include "World/UnmadeUnpaidShiftSceneRules.h"
namespace UnmadeCore {
// Reuses the caption, safe camera and optional Foley player. No fabricated VO.
inline constexpr std::array<UnpaidBeat,12> CinderholdBeats={{
 {UnpaidScene::Count,0,"npc.cinderhold.blackslag.scout.001","People call it a monster from here. The tracks tell a smaller story.","varda_wide_profile","Varda turns toward the covered west path","ash_steps","dry observation; leave room for disagreement",4.2f},
 {UnpaidScene::Count,1,"npc.cinderhold.blackslag.scout.001","Read my service log. Cool the vent. Learn the cadence before you answer it.","varda_hand_insert","Varda gestures toward the service log with one hand","quiet_ledger","precise field instructions, no mystic flourish",4.8f},
 {UnpaidScene::Count,2,"npc.cinderhold.blackslag.scout.001","Then bring me what happened. Bes will need more than a story about a beast.","varda_close","Varda faces the visitor and waits","wind_low","practical concern",4.0f},
 {UnpaidScene::Count,0,"npc.cinderhold.blackslag.scout.001","I will enter what you reported, and name you as its source.","varda_hand_detail","Varda makes a small ledger gesture","pen_scratch","explicitly reported, not eyewitness certainty",4.0f},
 {UnpaidScene::Count,1,"npc.cinderhold.blackslag.scout.001","FIELD_REPORT_DYNAMIC","varda_mid_eye","Varda holds the visitor's gaze","vent_low","assess the specific outcome without a victory speech",4.8f},
 {UnpaidScene::Count,2,"npc.cinderhold.blackslag.scout.001","Take that account home. A safe road and an unpaid shift are different entries.","varda_wide","Varda looks toward Cinderhold","quiet_ledger","firm boundary",4.0f},
 {UnpaidScene::Count,0,"npc.cinderhold.courier.001","BES_REPORT_DYNAMIC","bes_close","Bes listens, then answers without reaching for the harness","basket_rattle","familiarity earned by a real return",4.6f},
 {UnpaidScene::Count,1,"npc.cinderhold.courier.001","You found something I could not see from the cart. I still know what the cart cost.","bes_hand_close","Bes's hand pauses at the basket","cart_harness_unfasten","gratitude alongside remembered work",4.6f},
 {UnpaidScene::Count,2,"npc.cinderhold.courier.001","Come back to the shelf at home. Look again, with the same eyes.","bes_two_shot","Bes faces the player; no remote relocation","door_wood","invitation, not absolution",3.8f},
 {UnpaidScene::Count,0,"stage","The road's story crosses the threshold. The household keeps its own record.","hearth_return_wide","hold on the real shelf inside the open house","room_tone_return","quiet continuity",4.2f},
 {UnpaidScene::Count,1,"stage","HOME_RETURN_DYNAMIC","hearth_basket_detail","frame current fuel and remembered cold; do not invent a delivery","ash_settle","read the saved evidence plainly",5.0f},
 {UnpaidScene::Count,2,"stage","The visit is remembered. Tomorrow still asks who will carry the baskets.","hearth_return_wide","return camera and actor control without awarding pay","wind_low","an ending with daily life still moving",4.2f}
}};
inline constexpr const UnpaidBeat* CinderholdBeatAt(int scene,int beat) {
    return scene>=0&&scene<4&&beat>=0&&beat<3?&CinderholdBeats[scene*3+beat]:nullptr;
}
}
