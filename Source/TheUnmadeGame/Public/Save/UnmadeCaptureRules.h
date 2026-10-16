#pragma once
namespace UnmadeCore {
// Validate caller-owned staged fields before live capture can replace anything,
// then validate the complete captured record before either storage target writes.
// Record may be a noncopyable Unreal SaveGame; callers own failure rollback.
template<class Record,class Validate,class Capture>
bool PrepareCombinedCapture(Record& record,Validate valid,Capture capture){
 if(!valid(record))return false;
 if(!capture(record))return false;
 return valid(record);
}
// World capture reads the live survey owner. Stage the advance before the writer
// and retain the previous stage unless the entire combined transaction succeeds.
template<class Stages,class Writer>
bool CommitFieldSurveyStage(Stages& stages,int index,Writer write){
 if(stages.Num()!=6||index<0||index>=6)return false;
 for(int stage:stages)if(stage<0||stage>2)return false;
 const int before=stages[index];if(before>=2)return false;
 stages[index]=before+1;
 if(write())return true;
 stages[index]=before;return false;
}
}
