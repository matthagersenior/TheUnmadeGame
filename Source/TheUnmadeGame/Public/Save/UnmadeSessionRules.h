#pragma once
#include <cmath>
namespace UnmadeCore {
inline bool SameSessionWorld(const void* registeredWorld,const void* sourceWorld){return registeredWorld&&sourceWorld&&registeredWorld==sourceWorld;}
enum class CheckpointSave { Saved, PendingDefeat, Failed };
// Session checkpoints must not claim completeness while an encounter receipt
// awaits its own transaction. That transaction still uses the normal writer.
template<class Writer>
CheckpointSave CommitSessionCheckpoint(bool pendingDefeat,Writer write){
 if(pendingDefeat)return CheckpointSave::PendingDefeat;
 return write()?CheckpointSave::Saved:CheckpointSave::Failed;
}
inline bool ValidProfile(int n){return n>=0&&n<=3;}
inline bool ValidResume(double x,double y,double z,double health,double stamina){return std::isfinite(x)&&std::isfinite(y)&&std::isfinite(z)&&std::abs(x)<=500000&&std::abs(y)<=150000&&z>=0&&z<=1000&&std::isfinite(health)&&health>=0&&health<=100&&std::isfinite(stamina)&&stamina>=0&&stamina<=100;}
inline double SimulationInterval(double distance,bool combat){if(!std::isfinite(distance)||distance<0)return 2;return distance<3500?combat?0:.05:distance<18000?.25:2;}
inline int WorkMotion(int realm,int person){return realm>=0&&realm<9&&person>=0&&person<5?(realm+person)%3:-1;}
}
namespace UnmadeCore {
enum class StoredRecord {Primary,Backup,Missing,Incompatible};
inline StoredRecord ChooseStoredRecord(bool primaryExists,bool compatible,bool readable,bool backupReadable,bool preferBackup){if(primaryExists&&!compatible)return StoredRecord::Incompatible;if(preferBackup)return backupReadable?StoredRecord::Backup:StoredRecord::Missing;if(readable)return StoredRecord::Primary;return backupReadable?StoredRecord::Backup:StoredRecord::Missing;}
template<class Record,class Writer> bool CommitWithPrevious(const Record& next,const Record* previous,Writer write){if(previous&&!write(*previous,true))return false;return write(next,false);}
}
