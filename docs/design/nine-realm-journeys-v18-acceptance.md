# Nine Realm Journeys v18 acceptance

The source audit covers all nine established city/frontier/dungeon/return paths and their personal/optional wing quests. It repairs identified integration gaps; it does not establish an engine-playtested finished RPG.

## Source changes

- Actual chamber arrival commits the same ordered visit state used by encounter activation and defeat receipts. An earlier room and arena branch remain prerequisites. Failed writes roll back entry; combat remains disabled until it is committed.
- Journal equipment modifiers resolve the actual player component; the old kit selector no longer invents current slots or refers to an undeclared player variable.
- Sixteen derived objective stages distinguish unclaimed equipment, personal offer/account/evidence/accord, wing fieldwork/guardian/reward and a resolved return. No new save schema or forced purchase/completion grind.
- Irreversible branch and one-equipment decisions preview then confirm within twenty seconds. Changing input replaces the preview; expiry cancels it. Invalid input/time reversal cannot confirm. Ordinary reequipping needs no confirmation.
- Return witnesses need the actual leader and unobstructed sight. Yielded wing guardian state is valid only in Drevlach, matching the authored encounter.
- Forty-five next-chamber direction signs and thirty-six return-walk signs follow actual existing geometry. Established names remain; duplicated articles in return lines are corrected.

## Portable acceptance

`Tests/world/realm_journey_test.cpp` composes expedition, personal quest and experience state through 54 branch/accord/captive-timing paths, with 1,944 reload checkpoints. It checks all nine physical room footprints, encounter eligibility, branch activation, combat and evidence outcomes, both permanent reward types, scenes, finite insight, daily/total service limits, repairs, mediation and invalid-state refusal. A cumulative session retains nine rewards and independently selected weapon/armor/technique. Source wiring tests check the journal owner, confirmations, arrival commits and all tracker stages. These are rule and integration contracts, not executable Unreal walkthroughs.

## Engine walkthrough still required

For every city, arrive via the established realm/campaign route; read its board; visit frontier/refuge; enter all six rooms from their actual doors; fight or bypass warned ordinary roles; inspect three records; release captives before or after the actual boss outcome; confirm each council branch on separate saves; fight or surrender the boss; follow every marked return corner; ensure a leader behind cover cannot witness; claim each equipment option on separate saves; restore and verify slots. Hear the personal offer/witness, inspect evidence and confirm both accords on separate saves. Complete each realm-specific fieldwork task, resolve its guardian, claim insight, meet remembered residents across days and test purchases/repairs/mediation. Reload at each major transition and force write failures.

Also run the existing main campaign, all ten ability chapters, household wages/consent and late-campaign ending acceptance. Inspect collisions, navigation, skeletal imports, blade contacts, projectile cover, warning readability, controller/menu focus, audio, moving-platform support and first-person accessibility at actual target resolutions. Current environments and performances remain prototypes. Unreal compilation/import/render/playtest stay deferred under the creator's instruction.
