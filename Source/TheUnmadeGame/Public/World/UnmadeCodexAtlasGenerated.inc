// GENERATED FROM Authoring/ability_codex.json. Do not hand-edit.
// These tables live inside namespace UnmadeCore after RealmSpec and AtlasPassage.
inline constexpr std::array<RealmSpec,static_cast<int>(Realm::Count)> WorldAtlas = {{
    {Realm::ThreefoldReach,"The Threefold Reach","roads stitched into contradictory maps","the charterless settlements","refuge or testimony","witness gear and path relics",0},
    {Realm::WidowedRain,"The Rain That Forgot the Sea","permanent storm over hollow beaches","the rain-counting flotillas","whose absence buys fresh water","weather-forged weapons",1},
    {Realm::HearthBeneath,"The Hearth Beneath","warm cave cities under a frozen sky","keepers of the borrowed ember","who owns a common flame","bell and shelter armors",1},
    {Realm::TidalLedger,"The Sea of Written Debts","black tide that remembers future bargains","the navigators of unsettled prices","creditors foreclose on memories","debt and oath blades",2},
    {Realm::SkyBelow,"The Upside-Down Choir","islands suspended beneath an inward sky","the downward singers","songs hold the gravity of cities","gravity-changing relics",2},
    {Realm::CinderSpine,"The Bones of Yesterdays","a mountain range made from fossilized timelines","the seam-walkers","mining history erases descendants","time-tempered smithing",3},
    {Realm::HundredUnlived,"The Hundred Unlived","cities built for people never born","the unliving census","who has the right to existence","namebound artifacts",3},
    {Realm::OrchardOfKings,"The Orchard of Unwritten Kings","a forest of buried laws and crowns","the untitled courts","a kingdom without a past","regalia that carries consequences",4},
    {Realm::FirstAbsence,"The Place Before Place","unformed land that refuses geometry","the impossible witnesses","whether reality should remain singular","the Atlas of Absence",5}
}};
inline constexpr std::array<AtlasPassage,13> AtlasPassages = {{
    {Realm::ThreefoldReach,Realm::WidowedRain,0},
    {Realm::ThreefoldReach,Realm::HearthBeneath,0},
    {Realm::WidowedRain,Realm::TidalLedger,1},
    {Realm::WidowedRain,Realm::SkyBelow,2},
    {Realm::HearthBeneath,Realm::SkyBelow,1},
    {Realm::HearthBeneath,Realm::CinderSpine,2},
    {Realm::TidalLedger,Realm::HundredUnlived,2},
    {Realm::SkyBelow,Realm::HundredUnlived,2},
    {Realm::SkyBelow,Realm::OrchardOfKings,3},
    {Realm::CinderSpine,Realm::OrchardOfKings,3},
    {Realm::HundredUnlived,Realm::FirstAbsence,4},
    {Realm::OrchardOfKings,Realm::FirstAbsence,4},
    {Realm::TidalLedger,Realm::CinderSpine,3}
}};
