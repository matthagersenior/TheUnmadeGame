#include "Items/UnmadeFieldCraftRules.h"
#include "Save/UnmadeSaveSession.h"
#include "UI/UnmadeRpgFeedback.h"
#include "World/UnmadeWorldAtlas.h"
#include "World/UnmadeExpeditionRegistry.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "Save/UnmadeRecoveryRules.h"
#include "Items/UnmadeMilestoneRules.h"
#include "Items/UnmadeStarterRules.h"
#include "World/UnmadeTenfoldChronicle.h"
#include "World/UnmadeConfluenceRules.h"
#include "Save/UnmadePrototypeSave.h"
#include "Player/UnmadeCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UUnmadeEquipmentComponent::UUnmadeEquipmentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UUnmadeEquipmentComponent::BeginPlay()
{
    Super::BeginPlay();
    const UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(
        UnmadeSave::LoadGameFromSlot());
    if (Save && Save->SchemaVersion == 1 && Save->bHasInventorySnapshot)
    {
        UnmadeCore::InventorySnapshot Snapshot;
        bool bValid = Save->ItemQuantities.Num() >= 28 &&
            Save->ItemQuantities.Num() <= static_cast<int32>(UnmadeCore::ItemId::Count)
            && Save->EquippedItems.Num() == 3 && Save->AwardedMilestoneBits >= 0;
        if (bValid)
        {
            for (int32 i=0;i<Save->ItemQuantities.Num();++i)
                Snapshot.quantities[i]=Save->ItemQuantities[i];
            for (int32 i=0;i<3;++i)
                Snapshot.equipped[i]=Save->EquippedItems[i];
            Snapshot.milestones=static_cast<std::uint64_t>(Save->AwardedMilestoneBits);
            bValid=Inventory.Restore(Snapshot);
        }
        if (!bValid)
        {
            bSaveRejected = true;
            UE_LOG(LogTemp,Error,TEXT("Invalid inventory save: refused to overwrite existing data"));
            return;
        }
    }

    if(Save && Save->SchemaVersion==1 && Save->bHasEconomySnapshot)
    {
        UnmadeCore::EconomySnapshot PreviousEconomy;
        bool bValid=Save->ProfessionSkills.Num()==3 && Save->PaidContractMask>=0;
        if(bValid)
        {
            PreviousEconomy.marks=Save->TradeMarks;
            for(int32 i=0;i<3;++i) PreviousEconomy.craftsmanship[i]=Save->ProfessionSkills[i];
            PreviousEconomy.completedContracts=static_cast<uint32>(Save->PaidContractMask);
            bValid=Economy.Restore(PreviousEconomy);
        }
        if(!bValid)
        {
            bSaveRejected=true;
            UE_LOG(LogTemp,Error,TEXT("Corrupt economy state rejected; no overwrite"));
            return;
        }
    }
    UnmadeCore::Expeditions Earned;
    if(!ReadOwnedExpeditions(Earned)){bSaveRejected=true;return;}
    if(Save&&Save->bHasUnifiedLoadout){if(Save->UnifiedRealmLoadout.Num()!=3){bSaveRejected=true;return;}Loadout={Save->UnifiedRealmLoadout[0],Save->UnifiedRealmLoadout[1],Save->UnifiedRealmLoadout[2]};if(!Loadout.Valid(Earned)){bSaveRejected=true;return;}}
    else Loadout=UnmadeCore::UnifiedLoadout::Migrate(Earned);
    RefreshCombatBonuses();
}

bool UUnmadeEquipmentComponent::Persist(double UpdatedStrain)
{
    if (bSaveRejected) return false;
    UUnmadePrototypeSave* Save=UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return false;
    UnmadeCore::Expeditions Earned;if(!ReadOwnedExpeditions(Earned)||!Loadout.Valid(Earned))return false;
    Save->bHasUnifiedLoadout=true;Save->UnifiedRealmLoadout={Loadout.weapon,Loadout.armor,Loadout.technique};
    const auto& Snapshot=Inventory.Snapshot();
    Save->ItemQuantities.Reset();
    for (int Amount : Snapshot.quantities) Save->ItemQuantities.Add(Amount);
    Save->EquippedItems.Reset();
    for (int Id : Snapshot.equipped) Save->EquippedItems.Add(Id);
    Save->AwardedMilestoneBits=static_cast<int64>(Snapshot.milestones);
    Save->bHasInventorySnapshot=true;
    const auto EconomyState=Economy.Snapshot();
    Save->bHasEconomySnapshot=true;
    Save->TradeMarks=EconomyState.marks;
    Save->ProfessionSkills.Reset();
    for(int Rank:EconomyState.craftsmanship)Save->ProfessionSkills.Add(Rank);
    Save->PaidContractMask=static_cast<int32>(EconomyState.completedContracts);
    if (UpdatedStrain >= 0.0)
    {
        Save->PlayerStrain = UpdatedStrain;
        Save->bHasFractureSnapshot = true;
    }
    return UnmadeSave::SaveGameToSlot(Save);
}

FIntPoint UUnmadeEquipmentComponent::NativeGearBonuses()const{int attack=Inventory.AttackBonus(),armor=Inventory.ArmorBonus();for(int slot=0;slot<2;++slot)if((slot==0?Loadout.weapon:Loadout.armor)>=0)if(auto* item=UnmadeCore::FindItem(Inventory.Equipped(static_cast<UnmadeCore::GearSlot>(slot)))){attack-=item->attack;armor-=item->armor;}return FIntPoint(FMath::Max(0,attack+TemporaryAttack+ExpeditionAttack+BuildAttack),FMath::Max(0,armor+TemporaryArmor+ExpeditionArmor+BuildArmor));}
void UUnmadeEquipmentComponent::RefreshCombatBonuses(){auto* p=Cast<AUnmadeCharacter>(GetOwner());if(p&&p->GetCombat()){auto b=NativeGearBonuses();p->GetCombat()->SetGearBonuses(b.X,b.Y);}}

bool UUnmadeEquipmentComponent::Claim(UnmadeCore::Achievement Reward)
{
    if (bSaveRejected) return false;
    const auto Before=Inventory.Snapshot();
    if (Inventory.Claim(Reward)!=UnmadeCore::RewardResult::Awarded) return false;
    if (!Persist()) { Inventory.Restore(Before); return false; }
    if (GEngine) UnmadeRpgFeedback(-1,6.f,FColor::Yellow,
        TEXT("EARNED REWARD: press I for items, weapon and armor."));
    return true;
}

bool UUnmadeEquipmentComponent::ReconcileEarnedMilestones()
{
    if(bSaveRejected||!UnmadeSave::IsSessionWritable(Cast<AUnmadeCharacter>(GetOwner())))return false;
    bool complete=UnmadeCore::CommitStarterKit(Inventory,true,[&]{return Persist();});
    RefreshCombatBonuses();
    const UUnmadePrototypeSave* Save=Cast<UUnmadePrototypeSave>(
        UnmadeSave::LoadGameFromSlot());
    if (bSaveRejected || !Save || Save->SchemaVersion!=1) return false;
    UnmadeCore::ForEachEarnedMilestone(*Save,
        [&](const char* evidence){return Save->LexiconEvidence.Contains(FName(UTF8_TO_TCHAR(evidence)));},
        [&](UnmadeCore::Achievement reward){if(!Inventory.HasClaimed(reward)&&!Claim(reward))complete=false;});
    return ReconcileCivicContracts(Save)&&complete;
}

bool UUnmadeEquipmentComponent::ReadOwnedExpeditions(UnmadeCore::Expeditions& Out)const{const auto* Save=Cast<UUnmadePrototypeSave>(UnmadeSave::LoadGameFromSlot());if(!Save)return true;if(Save->SchemaVersion!=1)return false;if(!Save->bHasExpeditions)return true;std::vector<int> p;for(int v:Save->ExpeditionState)p.push_back(v);return Out.Restore(p);}
bool UUnmadeEquipmentComponent::SelectRealmReward(int Arc){if(bSaveRejected||Arc<0||Arc>=9)return false;UnmadeCore::Expeditions Earned;if(!ReadOwnedExpeditions(Earned)||!Earned.Arc(Arc)->reward)return false;if(!UnmadeCore::CommitRealmSelection(Loadout,Earned,Arc,[&]{return Persist();}))return false;auto b=Loadout.Benefit();SetExpeditionBonuses(b.attack,b.armor,b.glimpseDiscount,b.foldSeconds);return true;}
bool UUnmadeEquipmentComponent::EquipNext(UnmadeCore::GearSlot Slot){
 if(bSaveRejected)return false;UnmadeCore::Expeditions Earned;if(!ReadOwnedExpeditions(Earned))return false;const auto Before=Inventory.Snapshot();auto Old=Loadout;
 int count=static_cast<int>(UnmadeCore::ItemId::Count),current=static_cast<int>(Inventory.Equipped(Slot));int selected=Slot==UnmadeCore::GearSlot::Weapon?Loadout.weapon:Slot==UnmadeCore::GearSlot::Armor?Loadout.armor:-1;if(selected>=0)current=count+selected;int total=count+(Slot==UnmadeCore::GearSlot::Charm?0:9);
 for(int step=1;step<=total;++step){int index=(current+step)%total;if(index>=count){int a=index-count;if(Earned.Arc(a)->reward!=(Slot==UnmadeCore::GearSlot::Weapon?1:2))continue;if(Slot==UnmadeCore::GearSlot::Weapon)Loadout.weapon=a;else Loadout.armor=a;Loadout.technique=a;}else{auto* Def=UnmadeCore::FindItem(static_cast<UnmadeCore::ItemId>(index));if(!Def||Def->slot!=Slot||Inventory.Quantity(Def->id)<1)continue;if(!Inventory.Equip(Def->id))return false;if(Slot==UnmadeCore::GearSlot::Weapon)Loadout.weapon=-1;if(Slot==UnmadeCore::GearSlot::Armor)Loadout.armor=-1;}
  if(!Persist()){Inventory.Restore(Before);Loadout=Old;return false;}auto b=Loadout.Benefit();SetExpeditionBonuses(b.attack,b.armor,b.glimpseDiscount,b.foldSeconds);return true;
 }return false;
}
FString UUnmadeEquipmentComponent::DescribeComparison()const{FString t=TEXT("EQUIPMENT COMPARISON / one active weapon and armor\n");int weapon=Loadout.weapon>=0?7+Loadout.weapon*2:0,armor=Loadout.armor>=0?5+Loadout.armor:0;if(Loadout.weapon<0)if(auto* d=UnmadeCore::FindItem(Inventory.Equipped(UnmadeCore::GearSlot::Weapon)))weapon=d->attack;if(Loadout.armor<0)if(auto* d=UnmadeCore::FindItem(Inventory.Equipped(UnmadeCore::GearSlot::Armor)))armor=d->armor;UnmadeCore::Expeditions e;if(!ReadOwnedExpeditions(e))return TEXT("Equipment record invalid; protected.");for(const auto& item:UnmadeCore::ItemCatalog)if(Inventory.Quantity(item.id)>0&&(item.slot==UnmadeCore::GearSlot::Weapon||item.slot==UnmadeCore::GearSlot::Armor)){bool w=item.slot==UnmadeCore::GearSlot::Weapon;t+=FString::Printf(TEXT("%s: %+d %s compared with active slot\n"),UTF8_TO_TCHAR(item.name),w?item.attack-weapon:item.armor-armor,w?TEXT("attack"):TEXT("armor"));}for(int a=0;a<9;++a)if(e.Arc(a)->reward){bool w=e.Arc(a)->reward==1;int value=w?7+a*2:5+a;t+=FString::Printf(TEXT("%s: +%d %s (%+d compared with active slot) %s\n"),UTF8_TO_TCHAR(w?UnmadeCore::ExpeditionSpecs[a].weapon:UnmadeCore::ExpeditionSpecs[a].armor),value,w?TEXT("attack"):TEXT("armor"),value-(w?weapon:armor),(w?Loadout.weapon:Loadout.armor)==a?TEXT("[ACTIVE]"):TEXT("[OWNED]"));}auto benefit=Loadout.Benefit();t+=FString::Printf(TEXT("Technique: +%d attack, +%d armor; Glimpse discount %d, Fold extension %d. Charm and build effects remain additional.\n"),Loadout.technique>=0?benefit.attack-(Loadout.weapon>=0?7+Loadout.weapon*2:0):0,Loadout.technique>=0?benefit.armor-(Loadout.armor>=0?5+Loadout.armor:0):0,benefit.glimpseDiscount,benefit.foldSeconds);return t;}

UnmadeCore::ConsumableCommit UUnmadeEquipmentComponent::UseConsumable(UnmadeCore::ItemId Id,
    double CurrentStrain,int32& OutStrain)
{
    OutStrain=0;
    if (bSaveRejected) return UnmadeCore::ConsumableCommit::SaveFailed;
    AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner());
    if(!IsValid(Player) || !IsValid(Player->GetCombat()) || Player->GetCombat()->IsDefeated())
        return UnmadeCore::ConsumableCommit::Unavailable;
    const auto Before=Inventory.Snapshot();
    int heal=0,restore=0;
    if(Inventory.Consume(Id,Player->GetCombat()->GetMissingHealth(),
        CurrentStrain,heal,restore)!=UnmadeCore::ConsumeResult::Used) return UnmadeCore::ConsumableCommit::Unavailable;
    if(!UnmadeCore::CommitHealing(*Player->GetCombat(), heal, [&] {
        return Persist(FMath::Max(0.0,CurrentStrain-static_cast<double>(restore)));
    }))
    {
        Inventory.Restore(Before);
        return UnmadeCore::ConsumableCommit::SaveFailed;
    }
    OutStrain=restore;
    return UnmadeCore::ConsumableCommit::Used;
}

bool UUnmadeEquipmentComponent::ForgeWaybreaker()
{
    if (bSaveRejected) return false;
    const auto Before=Inventory.Snapshot();
    if(!Inventory.ForgeWaybreaker()) return false;
    if(!Persist()) { Inventory.Restore(Before); return false; }
    if(GEngine) UnmadeRpgFeedback(-1,10.f,FColor::Orange,
        TEXT("MYTHIC FORGED: WAYBREAKER, THE IMPOSSIBLE ROAD"));
    return true;
}

bool UUnmadeEquipmentComponent::ReconcileCivicContracts(const UUnmadePrototypeSave* Save)
{
    if(bSaveRejected || !Save || Save->SchemaVersion!=1)return false;
    bool complete=true;
    UnmadeCore::ForEachCivicContract(*Save,[&](UnmadeCore::SettlementId town,int stage){
        const auto Before=Economy.Snapshot();
        if(Economy.PayContract(town,stage)!=UnmadeCore::EconomyResult::Completed)return;
        if(!Persist()){Economy.Restore(Before);complete=false;}
    });
    return complete;
}

bool UUnmadeEquipmentComponent::Buy(UnmadeCore::ItemId Id,
    UnmadeCore::SettlementId Village,int Trust)
{
    if(bSaveRejected)return false;
    const auto BeforeItems=Inventory.Snapshot();
    const auto BeforeMoney=Economy.Snapshot();
    if(Economy.Buy(Inventory,Id,Village,1,Trust)!=UnmadeCore::EconomyResult::Completed)
        return false;
    if(!Persist())
    {
        Inventory.Restore(BeforeItems);
        Economy.Restore(BeforeMoney);
        return false;
    }
    return true;
}

bool UUnmadeEquipmentComponent::Sell(UnmadeCore::ItemId Id,
    UnmadeCore::SettlementId Village)
{
    if(bSaveRejected)return false;
    const auto BeforeItems=Inventory.Snapshot();
    const auto BeforeMoney=Economy.Snapshot();
    if(Economy.Sell(Inventory,Id,Village,1)!=UnmadeCore::EconomyResult::Completed)
        return false;
    if(!Persist())
    {
        Inventory.Restore(BeforeItems);
        Economy.Restore(BeforeMoney);
        return false;
    }
    return true;
}

bool UUnmadeEquipmentComponent::CraftAt(UnmadeCore::SettlementId village,int recipe){
 if(bSaveRejected)return false;auto old=Inventory.Snapshot();auto money=Economy.Snapshot();auto* player=Cast<AUnmadeCharacter>(GetOwner());int borrowed=player?player->GetBorrowedCraftLevel():0;auto result=Economy.Craft(Inventory,static_cast<UnmadeCore::RecipeId>(recipe),village,borrowed);if(result!=UnmadeCore::EconomyResult::Completed)return false;if(!Persist()){Inventory.Restore(old);Economy.Restore(money);return false;}return true;
}
bool UUnmadeEquipmentComponent::CraftFieldRecipe(int realm,bool learned){if(bSaveRejected)return false;auto old=Inventory.Snapshot();auto money=Economy.Snapshot();if(UnmadeCore::CraftField(Inventory,realm,learned)!=UnmadeCore::EconomyResult::Completed)return false;Economy.Practice(UnmadeCore::FieldRecipes[realm].profession);if(!Persist()){Inventory.Restore(old);Economy.Restore(money);return false;}return true;}
FString UUnmadeEquipmentComponent::DescribeRecipe(int n)const{
 if(n<0||n>=15)return TEXT("Select a recipe.");const char* name;UnmadeCore::ItemId a,b,out;int na,nb,count;
 if(n<6){auto& r=UnmadeCore::Recipes[n];name=r.title;a=r.inputA;b=r.inputB;na=r.unitsA;nb=r.unitsB;out=r.output;count=r.unitsOut;}else{auto& r=UnmadeCore::FieldRecipes[n-6];name=r.name;a=r.a;b=r.b;na=r.na;nb=r.nb;out=r.output;count=r.count;}
 FString requirement; if(n<6){const auto& r=UnmadeCore::Recipes[n];requirement=FString::Printf(TEXT("Workshop: %s. Profession %s, skill %d (current %d; borrowed skill may apply).\n"),r.workshop==UnmadeCore::SettlementId::Bellwold?TEXT("Bellwold"):TEXT("Paperhaven"),r.profession==UnmadeCore::Profession::Smith?TEXT("Smith"):r.profession==UnmadeCore::Profession::Scribe?TEXT("Scribe"):TEXT("Apothecary"),r.skill,Economy.Skill(r.profession));}
 return requirement+FString::Printf(TEXT("%s\nNeeds %s %d (owned %d), %s %d (owned %d).\nCreates %s x%d. Materials are spent only after a successful saved craft.\n"),UTF8_TO_TCHAR(name),UTF8_TO_TCHAR(UnmadeCore::FindItem(a)->name),na,Inventory.Quantity(a),UTF8_TO_TCHAR(UnmadeCore::FindItem(b)->name),nb,Inventory.Quantity(b),UTF8_TO_TCHAR(UnmadeCore::FindItem(out)->name),count);
}

FString UUnmadeEquipmentComponent::DescribeInventory() const
{
    FString Summary=FString::Printf(TEXT("GEAR: +%d attack, +%d armor | "),
        NativeGearBonuses().X,NativeGearBonuses().Y);
    for(int32 i=0;i<3;++i)
    {
        const auto* Def=UnmadeCore::FindItem(
            Inventory.Equipped(static_cast<UnmadeCore::GearSlot>(i)));
        Summary+=FString::Printf(TEXT("%s: %s | "),
            i==0?TEXT("Weapon"):i==1?TEXT("Armor"):TEXT("Charm"),
            i==0&&Loadout.weapon>=0?UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[Loadout.weapon].weapon):i==1&&Loadout.armor>=0?UTF8_TO_TCHAR(UnmadeCore::ExpeditionSpecs[Loadout.armor].armor):Def?UTF8_TO_TCHAR(Def->name):TEXT("Empty"));
    }
    Summary+=FString::Printf(TEXT("Marks %d | Smith %d, Apothecary %d, Scribe %d | BAG: "),
        Economy.Marks(),Economy.Skill(UnmadeCore::Profession::Smith),
        Economy.Skill(UnmadeCore::Profession::Apothecary),
        Economy.Skill(UnmadeCore::Profession::Scribe));
    for(const auto& Def:UnmadeCore::ItemCatalog)
        if(Inventory.Quantity(Def.id)>0)
            Summary+=FString::Printf(TEXT("%s x%d; "),
                UTF8_TO_TCHAR(Def.name),Inventory.Quantity(Def.id));
    return Summary+TEXT("\n")+DescribeComparison();
}

int32 UUnmadeEquipmentComponent::GetAtlasAttunement() const
{
    return UnmadeCore::AttunementFromRewards(Inventory);
}
