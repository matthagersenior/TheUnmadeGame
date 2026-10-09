#pragma once
// THE UNMADE authored equipment, consumables and earned rewards.
// Pure C++17: fully deterministic, offline and without Unreal/LLM dependencies.
#include <array>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <cstring>

namespace UnmadeCore {
enum class ItemKind { Weapon, Armor, Charm, Consumable, Material };
enum class Rarity { Common, Uncommon, Rare, Epic, Mythic };
enum class GearSlot { None=-1, Weapon=0, Armor=1, Charm=2 };
enum class ItemId : int {
    SalvagerBlade, TravelerMantle, HearthSalve, StrainVial,
    EchoGlass, BellMetal, ArchiveInk,
    BellwoldBulwark, PaperhavenLens, WayfarerBlade,
    LostHourMantle, OrchardSkin, DebtCollector,
    ReturnedVoice, SilentSole, LanternHeart,
    RegistryCloak, TalismanOfWitness, RiftSeed,
    NightwatchTonic, RootboundPoultice, MapmakersNeedle,
    DoorlessKey, PathOfThree, Waybreaker,
    UnwrittenCrown, AshOfPossibleLives, AtlasOfAbsence,
    BellheartAegis, RedactedScepter, PilgrimsSignet,
    IronScrap, WildHerbs, BlankParchment,
    TemperedEdge, LanternMail, InkboundCharm,
    StormglassCompass, EmberheartBlade,
    RefugeOathguard, ArchivistQuill, RoadboundCompass,
    BellmakersVerse, RememberedBridge, SecondName, ThreeDeedSteel,
    CommonCauseway, FutureReceipt, HollowMercy, BetweenHistories,
    UnbrokenLantern, CoastTrueAtlas,
    LastWarningAegis, ElsebornSmithEdge, CommonPathChart,
    BorrowedDawnHeart, TwoKeepersMirror, FirstAbsenceWitness,
    AfterlightWard, UnredactedLantern,
    Count
};
enum class Achievement : int {
    Starter, BellwoldLanterns, PaperhavenTestimony, ThreeVillages,
    EchoWell, PaperOrchard, SilentMile, BellGrave, DebtMarket,
    ShelterSupplies, ShelterChoice, ResearchChoice, LanguageVeyl,
    AllLandmarks, FirstStalker, FirstWatcher,
    HollowBell, RedactedCurator, UnfinishedPilgrim,
    SaltwakeStory, CinderholdStory,
    RefugeCovenant, ArchiveCovenant, RoadboundCovenant,
    RiteUnwriteLaw, RiteWitnesscraft, RiteBorrowedLives, RiteLegacyForging,
    RiteLivingRoads, RiteTomorrowDebt, RiteUnderstandingBosses,
    RiteParadoxConvergence, RiteOathbinding, RiteCartography,
    ConfluenceSilentAlarm, ConfluenceTwoNames, ConfluenceUnmappedWay,
    ConfluenceTomorrowPromise, ConfluenceTwoKeepers, ConfluenceFirstAbsence,
    AfterlightShelter, AfterlightNames, Count
};
struct ItemDef {
    ItemId id;
    const char* code;
    const char* name;
    const char* story;
    ItemKind kind;
    Rarity rarity;
    GearSlot slot;
    int maxStack, attack, armor, heal, strain;
};
inline constexpr std::array<ItemDef, static_cast<int>(ItemId::Count)> ItemCatalog = {{
    {ItemId::SalvagerBlade, "blade.salvager", "Salvager's Edge", "The first honest blade in a dishonest country.", ItemKind::Weapon,Rarity::Common,GearSlot::Weapon,1,3,0,0,0},
    {ItemId::TravelerMantle, "armor.traveler", "Traveler's Mantle", "A stitched coat with room for a second history.", ItemKind::Armor,Rarity::Common,GearSlot::Armor,1,0,2,0,0},
    {ItemId::HearthSalve, "elixir.hearth", "Hearth Salve", "A small warmth stolen from the coming winter.", ItemKind::Consumable,Rarity::Common,GearSlot::None,9,0,0,35,0},
    {ItemId::StrainVial, "elixir.strain", "Stillness Vial", "Four breaths free of the weight of possibility.", ItemKind::Consumable,Rarity::Uncommon,GearSlot::None,9,0,0,0,24},
    {ItemId::EchoGlass, "mat.echo_glass", "Echo Glass", "The well reflects what has not yet happened.", ItemKind::Material,Rarity::Rare,GearSlot::None,20,0,0,0,0},
    {ItemId::BellMetal, "mat.bell_metal", "Hourless Bellmetal", "A sound that cannot fit inside one hour.", ItemKind::Material,Rarity::Rare,GearSlot::None,20,0,0,0,0},
    {ItemId::ArchiveInk, "mat.archive_ink", "Unwritten Ink", "An oath not yet made, waiting to dry.", ItemKind::Material,Rarity::Rare,GearSlot::None,20,0,0,0,0},
    {ItemId::BellwoldBulwark, "armor.bellwold", "Bellwold's Last Refuge", "Guard the people even when the world calls them forgotten.", ItemKind::Armor,Rarity::Epic,GearSlot::Armor,1,0,12,0,0},
    {ItemId::PaperhavenLens, "charm.paperhaven", "Lens of the Unentered", "Truth survives even if the index does not.", ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,4,4,0,0},
    {ItemId::WayfarerBlade, "blade.wayfarer", "Three Roads, One Name", "Each road can carry you home, but none agrees where.", ItemKind::Weapon,Rarity::Rare,GearSlot::Weapon,1,12,0,0,0},
    {ItemId::LostHourMantle, "armor.lost_hour", "Mantle of the Missing Hour", "Time remembers the person beneath it.", ItemKind::Armor,Rarity::Epic,GearSlot::Armor,1,0,9,0,0},
    {ItemId::OrchardSkin, "armor.orchard", "Bark of the Neverborn", "Leaves bloom with signatures that were erased.", ItemKind::Armor,Rarity::Rare,GearSlot::Armor,1,0,6,0,0},
    {ItemId::DebtCollector, "blade.debt", "Tomorrow's Debt", "It collects the price before the wound.", ItemKind::Weapon,Rarity::Rare,GearSlot::Weapon,1,10,0,0,0},
    {ItemId::ReturnedVoice, "charm.voice", "The Voice Returned", "A warning you will speak only after you hear it.", ItemKind::Charm,Rarity::Rare,GearSlot::Charm,1,2,3,0,0},
    {ItemId::SilentSole, "charm.silent", "Boot-Sigil of the Silent Mile", "Your feet learn how to leave no first step.", ItemKind::Charm,Rarity::Rare,GearSlot::Charm,1,3,2,0,0},
    {ItemId::LanternHeart, "charm.lantern", "Heart of the Communal Flame", "Light that may never be purchased.", ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,2,7,0,0},
    {ItemId::RegistryCloak, "armor.registry", "Cloak of Contradictory Records", "One body, a thousand incompatible histories.", ItemKind::Armor,Rarity::Rare,GearSlot::Armor,1,0,7,0,0},
    {ItemId::TalismanOfWitness, "charm.witness", "Witness Stone", "Someone believes you because they saw you choose.", ItemKind::Charm,Rarity::Rare,GearSlot::Charm,1,5,1,0,0},
    {ItemId::RiftSeed, "mat.rift_seed", "Rift Seed", "A beginning without the usual beginning.", ItemKind::Material,Rarity::Epic,GearSlot::None,10,0,0,0,0},
    {ItemId::NightwatchTonic, "elixir.nightwatch", "Nightwatch Tonic", "Wakeful enough to see the next possibility.", ItemKind::Consumable,Rarity::Uncommon,GearSlot::None,6,0,0,65,0},
    {ItemId::RootboundPoultice, "elixir.rootbound", "Rootbound Poultice", "The orchard refuses to let you fade.", ItemKind::Consumable,Rarity::Rare,GearSlot::None,5,0,0,90,0},
    {ItemId::MapmakersNeedle, "blade.mapmaker", "Needle of Unfinished Maps", "Finds the place where the map was cut.", ItemKind::Weapon,Rarity::Epic,GearSlot::Weapon,1,19,0,0,0},
    {ItemId::DoorlessKey, "charm.key", "Key of the Doorless", "Unlocks nothing except your willingness to search.", ItemKind::Charm,Rarity::Rare,GearSlot::Charm,1,5,3,0,0},
    {ItemId::PathOfThree, "charm.threepaths", "Path of Three Settlements", "The people who remember are your true compass.", ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,6,5,0,0},
    {ItemId::Waybreaker, "blade.waybreaker", "Waybreaker, the Impossible Road", "Forged from shelter, testimony and the missing hour.", ItemKind::Weapon,Rarity::Mythic,GearSlot::Weapon,1,32,0,0,0},
    {ItemId::UnwrittenCrown, "charm.unwritten_crown", "The Unwritten Crown", "Sovereignty over what history refused to name.", ItemKind::Charm,Rarity::Mythic,GearSlot::Charm,1,12,10,0,0},
    {ItemId::AshOfPossibleLives, "elixir.possible_lives", "Ash of Possible Lives", "Once, you had the strength to be all of them.", ItemKind::Consumable,Rarity::Epic,GearSlot::None,3,0,0,100,60},
    {ItemId::AtlasOfAbsence, "charm.atlas", "Atlas of Absence", "Every missing country has a place upon the page.", ItemKind::Charm,Rarity::Mythic,GearSlot::Charm,1,8,12,0,0},
    {ItemId::BellheartAegis, "armor.bellheart", "Aegis of the Hollow Bell", "No living keeper remains to sound it.", ItemKind::Armor,Rarity::Mythic,GearSlot::Armor,1,0,18,0,0},
    {ItemId::RedactedScepter, "weapon.redacted", "Scepter of Missing Names", "The name of each wound vanishes from the record.", ItemKind::Weapon,Rarity::Epic,GearSlot::Weapon,1,23,0,0,0},
    {ItemId::PilgrimsSignet, "charm.pilgrim", "Signet of the Unfinished Pilgrim", "Every ending is only a path that turned.", ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,6,9,0,0},
    {ItemId::IronScrap,"mat.iron_scrap","Weathered Iron","The ore forgot which mountain it belonged to.",ItemKind::Material,Rarity::Common,GearSlot::None,20,0,0,0,0},
    {ItemId::WildHerbs,"mat.wild_herbs","Night-Mint","Harvested where the rain refuses to fall.",ItemKind::Material,Rarity::Common,GearSlot::None,20,0,0,0,0},
    {ItemId::BlankParchment,"mat.blank_parchment","Blank Witness Parchment","It accepts testimony that no one will sign.",ItemKind::Material,Rarity::Common,GearSlot::None,20,0,0,0,0},
    {ItemId::TemperedEdge,"blade.tempered","Oath-Tempered Edge","A blade that remembers whose hands made it.",ItemKind::Weapon,Rarity::Rare,GearSlot::Weapon,1,14,0,0,0},
    {ItemId::LanternMail,"armor.lanternmail","Lanternwoven Mail","Bellwold's embers stitched between iron rings.",ItemKind::Armor,Rarity::Epic,GearSlot::Armor,1,0,14,0,0},
    {ItemId::InkboundCharm,"charm.inkbound","Seal of Kept Testimony","Even the silenced leave marks behind.",ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,7,6,0,0},
    {ItemId::StormglassCompass,"charm.stormglass","Compass of Forgotten Tides","It points toward where the vanished sea still longs to return.",ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,9,4,0,0},
    {ItemId::EmberheartBlade,"blade.emberheart","Emberheart of Cinderhold","A blade that can be passed on without diminishing its flame.",ItemKind::Weapon,Rarity::Mythic,GearSlot::Weapon,1,27,0,0,0},
    {ItemId::RefugeOathguard,"armor.refuge_oathguard","Oathguard of the Open Hearth","There is no sanctuary without people willing to defend it.",ItemKind::Armor,Rarity::Epic,GearSlot::Armor,1,0,15,0,0},
    {ItemId::ArchivistQuill,"blade.archivist_quill","The Quill That Will Not Erase","Every false record must face someone who remembers.",ItemKind::Weapon,Rarity::Epic,GearSlot::Weapon,1,22,0,0,0},
    {ItemId::RoadboundCompass,"charm.roadbound","Compass of the Common Road","No single village can own a road that people share.",ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,7,8,0,0},
    {ItemId::BellmakersVerse,"charm.bellmakers_verse","The Bellmaker's Unwritten Verse","An interval of silence carved out of a condemned bell.",ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,6,7,0,0},
    {ItemId::RememberedBridge,"charm.remembered_bridge","The Bridge That Remembers","Each stone holds the name of one witness who dared to speak.",ItemKind::Charm,Rarity::Mythic,GearSlot::Charm,1,7,11,0,0},
    {ItemId::SecondName,"charm.second_name","The Second Name","An unlived childhood folded beneath the wearer's signature.",ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,11,3,0,0},
    {ItemId::ThreeDeedSteel,"blade.three_deed","The Three-Deed Temper","Its maker can name the lives protected instead of foes slain.",ItemKind::Weapon,Rarity::Mythic,GearSlot::Weapon,1,28,0,0,0},
    {ItemId::CommonCauseway,"charm.common_causeway","The Common Causeway","A road sworn open to everyone and owned by no one.",ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,5,9,0,0},
    {ItemId::FutureReceipt,"weapon.future_receipt","Tomorrow's Paid Receipt","The sunrise collects its debt; this receipt admits the cost.",ItemKind::Weapon,Rarity::Epic,GearSlot::Weapon,1,21,0,0,0},
    {ItemId::HollowMercy,"armor.hollow_mercy","Mercy of the Hollow Keeper","The keeper was seen at last, and chose to stand down.",ItemKind::Armor,Rarity::Mythic,GearSlot::Armor,1,0,19,0,0},
    {ItemId::BetweenHistories,"charm.between_histories","The Room Between Histories","Two incompatible homes shelter the same forgotten name.",ItemKind::Charm,Rarity::Mythic,GearSlot::Charm,1,10,12,0,0},
    {ItemId::UnbrokenLantern,"armor.unbroken_lantern","The Unbroken Shelter Oath","Every witness expects the light to survive your footsteps.",ItemKind::Armor,Rarity::Epic,GearSlot::Armor,1,0,16,0,0},
    {ItemId::CoastTrueAtlas,"charm.coast_true","Atlas of the Missing Coast","The navigator kept both the rumor and the shoreline she saw.",ItemKind::Charm,Rarity::Mythic,GearSlot::Charm,1,9,10,0,0},
    {ItemId::LastWarningAegis,"armor.last_warning","Aegis of the Last Warning","A bell's silence that still allows the village to hear.",ItemKind::Armor,Rarity::Mythic,GearSlot::Armor,1,0,23,0,0},
    {ItemId::ElsebornSmithEdge,"blade.elseborn_smith","Edge of the Elseborn Smith","Forged by the hands you had in another life.",ItemKind::Weapon,Rarity::Mythic,GearSlot::Weapon,1,34,0,0,0},
    {ItemId::CommonPathChart,"charm.common_chart","Chart of the Common Path","No border owns the choices of its travelers.",ItemKind::Charm,Rarity::Mythic,GearSlot::Charm,1,12,11,0,0},
    {ItemId::BorrowedDawnHeart,"charm.borrowed_dawn","Heart of the Borrowed Dawn","Promises repaid without transferring the debt to others.",ItemKind::Charm,Rarity::Mythic,GearSlot::Charm,1,14,8,0,0},
    {ItemId::TwoKeepersMirror,"armor.two_keepers","Mirror of the Two Keepers","The innocent and the monster both remain in its reflection.",ItemKind::Armor,Rarity::Mythic,GearSlot::Armor,1,0,26,0,0},
    {ItemId::FirstAbsenceWitness,"charm.first_absence","Witness of the First Absence","Every world begins with someone brave enough to remember.",ItemKind::Charm,Rarity::Mythic,GearSlot::Charm,1,20,17,0,0},
    {ItemId::AfterlightWard,"armor.afterlight_ward","Ward of the Second Night","Hessa hangs a light for each unregistered child. This protection cannot be bought.",ItemKind::Armor,Rarity::Epic,GearSlot::Armor,1,0,18,0,0},
    {ItemId::UnredactedLantern,"charm.unredacted_lantern","Lantern of Unredacted Names","Ivera remembers the families officials denied. Its glow never removes a name.",ItemKind::Charm,Rarity::Epic,GearSlot::Charm,1,11,9,0,0}
}};
inline const ItemDef* FindItem(ItemId id) noexcept {
    const int i = static_cast<int>(id);
    if (i < 0 || i >= static_cast<int>(ItemCatalog.size())) return nullptr;
    return &ItemCatalog[static_cast<std::size_t>(i)];
}
inline const ItemDef* FindItem(const char* code) noexcept {
    if (!code) return nullptr;
    for (const auto& item : ItemCatalog) if (std::strcmp(item.code,code)==0) return &item;
    return nullptr;
}
struct InventorySnapshot {
    std::array<int, static_cast<int>(ItemId::Count)> quantities{};
    std::array<int, 3> equipped{{-1,-1,-1}};
    std::uint64_t milestones = 0;
};
enum class ConsumeResult { Used, NotOwned, NotConsumable, NoBenefit };
enum class RewardResult { Awarded, AlreadyAwarded, NoRoom, Invalid };
class InventoryModel final {
public:
    const InventorySnapshot& Snapshot() const noexcept { return data_; }
    bool Restore(const InventorySnapshot& proposed) noexcept {
        constexpr auto rewardMask = (std::uint64_t{1}<<static_cast<int>(Achievement::Count))-1;
        if ((proposed.milestones & ~rewardMask)!=0) return false;
        for(int i=0;i<static_cast<int>(ItemCatalog.size());++i)
            if(proposed.quantities[i]<0 || proposed.quantities[i]>ItemCatalog[i].maxStack) return false;
        for(int s=0;s<3;++s) {
            const int id=proposed.equipped[s];
            if(id==-1) continue;
            const auto* item=FindItem(static_cast<ItemId>(id));
            if(!item || static_cast<int>(item->slot)!=s || proposed.quantities[id]<1) return false;
        }
        data_=proposed;
        return true;
    }
    int Quantity(ItemId id) const noexcept {
        const auto* item=FindItem(id);
        return item?data_.quantities[static_cast<int>(id)]:0;
    }
    bool HasClaimed(Achievement achievement) const noexcept {
        const int index=static_cast<int>(achievement);
        return index>=0 && index<static_cast<int>(Achievement::Count) &&
            (data_.milestones & (std::uint64_t{1}<<index))!=0;
    }
    // Explicit finite transfers for trusted shops/forges. Equipped gear cannot
    // be sold or consumed. All transaction coordinators stage on a copy.
    bool Add(ItemId id,int amount) noexcept {
        const auto* def=FindItem(id);
        if(!def || amount<=0 || amount>def->maxStack-Quantity(id)) return false;
        data_.quantities[static_cast<int>(id)]+=amount;
        return true;
    }
    bool Take(ItemId id,int amount) noexcept {
        const auto* def=FindItem(id);
        if(!def || amount<=0 || Quantity(id)<amount) return false;
        for(int worn : data_.equipped)
            if(worn==static_cast<int>(id)) return false;
        data_.quantities[static_cast<int>(id)]-=amount;
        return true;
    }
    bool Equip(ItemId id) noexcept {
        const auto* item=FindItem(id);
        if(!item || item->slot==GearSlot::None || Quantity(id)<1) return false;
        data_.equipped[static_cast<int>(item->slot)]=static_cast<int>(id);
        return true;
    }
    ItemId Equipped(GearSlot slot) const noexcept {
        const int s=static_cast<int>(slot);
        return s<0 || s>=3?ItemId::Count:static_cast<ItemId>(data_.equipped[s]);
    }
    int AttackBonus() const noexcept {
        int amount=0;for(int id:data_.equipped){const auto* item=FindItem(static_cast<ItemId>(id));if(item)amount+=item->attack;}return amount;
    }
    int ArmorBonus() const noexcept {
        int amount=0;for(int id:data_.equipped){const auto* item=FindItem(static_cast<ItemId>(id));if(item)amount+=item->armor;}return amount;
    }
    // Relics must alter playstyles, not merely increase inventory stats.
    int GlimpseStrainDiscount() const noexcept {
        const auto charm=Equipped(GearSlot::Charm);
        return charm==ItemId::ReturnedVoice ? 3
            : charm==ItemId::PaperhavenLens ? 2 : 0;
    }
    int FoldDurationBonus() const noexcept {
        return Equipped(GearSlot::Weapon)==ItemId::Waybreaker ? 3 : 0;
    }
    double RecoveryMultiplier() const noexcept {
        return Equipped(GearSlot::Charm)==ItemId::UnwrittenCrown ? 1.5 : 1.0;
    }
    ConsumeResult Consume(ItemId id, double missingHealth, double existingStrain, int& health, int& strain) noexcept {
        health=0;strain=0;
        const auto* item=FindItem(id);
        if(!item || item->kind!=ItemKind::Consumable) return ConsumeResult::NotConsumable;
        if(Quantity(id)<1) return ConsumeResult::NotOwned;
        if(!std::isfinite(missingHealth) || !std::isfinite(existingStrain) || missingHealth<0 || existingStrain<0)
            return ConsumeResult::NoBenefit;
        health=static_cast<int>(std::min(missingHealth,static_cast<double>(item->heal)));
        strain=static_cast<int>(std::min(existingStrain,static_cast<double>(item->strain)));
        if(health==0 && strain==0) return ConsumeResult::NoBenefit;
        --data_.quantities[static_cast<int>(id)];
        return ConsumeResult::Used;
    }
    RewardResult Claim(Achievement achievement) noexcept {
        const int index=static_cast<int>(achievement);
        if(index<0 || index>=static_cast<int>(Achievement::Count)) return RewardResult::Invalid;
        if(HasClaimed(achievement)) return RewardResult::AlreadyAwarded;
        InventorySnapshot next=data_;
        auto add=[&](ItemId id,int count) {
            const auto* item=FindItem(id);
            if(!item || next.quantities[static_cast<int>(id)]>item->maxStack-count) return false;
            next.quantities[static_cast<int>(id)]+=count;
            return true;
        };
        bool okay=false;
        switch(achievement) {
        case Achievement::Starter: okay=add(ItemId::SalvagerBlade,1)&&add(ItemId::TravelerMantle,1)&&add(ItemId::HearthSalve,3)&&add(ItemId::StrainVial,2);break;
        case Achievement::BellwoldLanterns: okay=add(ItemId::BellwoldBulwark,1)&&add(ItemId::BellMetal,1);break;
        case Achievement::PaperhavenTestimony: okay=add(ItemId::PaperhavenLens,1)&&add(ItemId::ArchiveInk,1);break;
        case Achievement::ThreeVillages: okay=add(ItemId::WayfarerBlade,1)&&add(ItemId::PathOfThree,1);break;
        case Achievement::EchoWell: okay=add(ItemId::ReturnedVoice,1)&&add(ItemId::EchoGlass,1);break;
        case Achievement::PaperOrchard: okay=add(ItemId::OrchardSkin,1)&&add(ItemId::RootboundPoultice,1);break;
        case Achievement::SilentMile: okay=add(ItemId::SilentSole,1);break;
        case Achievement::BellGrave: okay=add(ItemId::LostHourMantle,1);break;
        case Achievement::DebtMarket: okay=add(ItemId::DebtCollector,1);break;
        case Achievement::ShelterSupplies: okay=add(ItemId::LanternHeart,1)&&add(ItemId::NightwatchTonic,2);break;
        case Achievement::ShelterChoice: okay=add(ItemId::TalismanOfWitness,1);break;
        case Achievement::ResearchChoice: okay=add(ItemId::RegistryCloak,1);break;
        case Achievement::LanguageVeyl: okay=add(ItemId::DoorlessKey,1);break;
        case Achievement::AllLandmarks: okay=add(ItemId::UnwrittenCrown,1)&&add(ItemId::RiftSeed,1);break;
        case Achievement::FirstStalker: okay=add(ItemId::EchoGlass,1)&&add(ItemId::HearthSalve,1);break;
        case Achievement::FirstWatcher: okay=add(ItemId::MapmakersNeedle,1)&&add(ItemId::StrainVial,1);break;
        case Achievement::HollowBell: okay=add(ItemId::BellheartAegis,1)&&add(ItemId::BellMetal,1);break;
        case Achievement::RedactedCurator: okay=add(ItemId::RedactedScepter,1)&&add(ItemId::ArchiveInk,1);break;
        case Achievement::UnfinishedPilgrim: okay=add(ItemId::PilgrimsSignet,1)&&add(ItemId::EchoGlass,1);break;
        case Achievement::SaltwakeStory: okay=add(ItemId::StormglassCompass,1)&&add(ItemId::WildHerbs,2);break;
        case Achievement::CinderholdStory: okay=add(ItemId::EmberheartBlade,1)&&add(ItemId::IronScrap,2);break;
        case Achievement::RefugeCovenant: okay=add(ItemId::RefugeOathguard,1);break;
        case Achievement::ArchiveCovenant: okay=add(ItemId::ArchivistQuill,1);break;
        case Achievement::RoadboundCovenant: okay=add(ItemId::RoadboundCompass,1);break;
        case Achievement::RiteUnwriteLaw: okay=add(ItemId::BellmakersVerse,1);break;
        case Achievement::RiteWitnesscraft: okay=add(ItemId::RememberedBridge,1);break;
        case Achievement::RiteBorrowedLives: okay=add(ItemId::SecondName,1);break;
        case Achievement::RiteLegacyForging: okay=add(ItemId::ThreeDeedSteel,1);break;
        case Achievement::RiteLivingRoads: okay=add(ItemId::CommonCauseway,1);break;
        case Achievement::RiteTomorrowDebt: okay=add(ItemId::FutureReceipt,1);break;
        case Achievement::RiteUnderstandingBosses: okay=add(ItemId::HollowMercy,1);break;
        case Achievement::RiteParadoxConvergence: okay=add(ItemId::BetweenHistories,1);break;
        case Achievement::RiteOathbinding: okay=add(ItemId::UnbrokenLantern,1);break;
        case Achievement::RiteCartography: okay=add(ItemId::CoastTrueAtlas,1);break;
        case Achievement::ConfluenceSilentAlarm: okay=add(ItemId::LastWarningAegis,1);break;
        case Achievement::ConfluenceTwoNames: okay=add(ItemId::ElsebornSmithEdge,1);break;
        case Achievement::ConfluenceUnmappedWay: okay=add(ItemId::CommonPathChart,1);break;
        case Achievement::ConfluenceTomorrowPromise: okay=add(ItemId::BorrowedDawnHeart,1);break;
        case Achievement::ConfluenceTwoKeepers: okay=add(ItemId::TwoKeepersMirror,1);break;
        case Achievement::ConfluenceFirstAbsence: okay=add(ItemId::FirstAbsenceWitness,1);break;
        case Achievement::AfterlightShelter: okay=add(ItemId::AfterlightWard,1);break;
        case Achievement::AfterlightNames: okay=add(ItemId::UnredactedLantern,1);break;
        default: return RewardResult::Invalid;
        }
        if(!okay)return RewardResult::NoRoom;
        next.milestones|=std::uint64_t{1}<<index;
        data_=next;
        return RewardResult::Awarded;
    }
    bool ForgeWaybreaker() noexcept {
        if(Quantity(ItemId::Waybreaker)>0 ||
            !HasClaimed(Achievement::BellwoldLanterns) ||
            !HasClaimed(Achievement::PaperhavenTestimony) ||
            !HasClaimed(Achievement::ThreeVillages) ||
            Quantity(ItemId::EchoGlass)<2 || Quantity(ItemId::BellMetal)<1 ||
            Quantity(ItemId::ArchiveInk)<1) return false;
        --data_.quantities[static_cast<int>(ItemId::EchoGlass)];
        --data_.quantities[static_cast<int>(ItemId::EchoGlass)];
        --data_.quantities[static_cast<int>(ItemId::BellMetal)];
        --data_.quantities[static_cast<int>(ItemId::ArchiveInk)];
        data_.quantities[static_cast<int>(ItemId::Waybreaker)]=1;
        return true;
    }
private:
    InventorySnapshot data_{};
};
}
