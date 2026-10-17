#include "Save/UnmadeCombinedSaveRules.h"
#include "combined_record_fixture.h"
#include <cassert>
#include <limits>
using namespace UnmadeCore;
int main(){
    SaveFixture::Record record;
    record.bHasLivingWorldSnapshot=true;record.bHasLivingEconomy=true;
    record.LivingEmployerMarks.assign(9,32);record.LivingEarnedMarks.assign(9,0);
    record.LivingOwedMarks.assign(9,0);record.LivingPaidShifts.assign(9,0);
    record.LivingPaidDays.assign(9,1);record.LivingWorkerReceipts.assign(9,0);
    record.LivingForeignStock.assign(2,0);record.LivingRouteReceipts.assign(2,0);
    record.LivingTradePayments.assign(2,0);record.LivingConsumedSupport.assign(2,0);
    const auto valid=[](const auto& r){return ValidCombinedRecord(r,SaveFixture::Text);};
    assert(valid(record));
    auto bad=record;
    bad.LivingEmployerMarks[0]=bad.LivingEarnedMarks[0]=std::numeric_limits<int>::max();
    assert(!valid(bad));
    bad=record;bad.LivingRouteReceipts.assign(2,std::numeric_limits<int>::max());
    assert(!valid(bad));
    bad=record;bad.LivingRouteReceipts.assign(2,std::numeric_limits<int>::min());
    assert(!valid(bad));
    assert(valid(record));
}
