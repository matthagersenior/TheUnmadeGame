#include "Items/UnmadeFieldCraftRules.h"
#include <cassert>
using namespace UnmadeCore;
int main(){assert(FieldRecipes.size()==9);for(int a=0;a<9;++a){InventoryModel bag;const auto& r=FieldRecipes[a];assert(bag.Add(r.a,r.na));assert(bag.Add(r.b,r.nb));auto original=bag.Snapshot();assert(CraftField(bag,a,false)==EconomyResult::InsufficientSkill);assert(bag.Snapshot().quantities==original.quantities);assert(CraftField(bag,a,true)==EconomyResult::Completed);assert(bag.Quantity(r.output)==r.count);assert(bag.Quantity(r.a)==0&&bag.Quantity(r.b)==0);auto after=bag.Snapshot();assert(CraftField(bag,a,true)==EconomyResult::NotEnoughMaterials);assert(bag.Snapshot().quantities==after.quantities);InventoryModel full;assert(full.Add(r.a,r.na));assert(full.Add(r.b,r.nb));assert(full.Add(r.output,FindItem(r.output)->maxStack));auto before=full.Snapshot();assert(CraftField(full,a,true)==EconomyResult::NoCapacity);assert(full.Snapshot().quantities==before.quantities);}
 InventoryModel empty;assert(CraftField(empty,-1,true)==EconomyResult::Invalid);assert(CraftField(empty,9,true)==EconomyResult::Invalid);
}
