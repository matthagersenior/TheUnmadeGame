#pragma once
#include "Save/UnmadeCombinedSaveRules.h"
#include "combined_record_fixture.h"
#include <optional>
namespace SaveFixture {
inline bool Valid(const Record& r){return UnmadeCore::ValidCombinedRecord(r,Text);}
using UnmadeCore::ChooseStoredRecord;
using UnmadeCore::StoredRecord;
using UnmadeCore::CommitCombinedRecord;
// In-memory storage fault injection, using the production validation/selection
// and commit rules. Not a simulation of Unreal serialization or OS atomicity.
struct Storage {
    std::optional<Record> primary,backup;
    bool recovery=false;int writes=0,failAt=0;bool damageOnFailure=false;
    const Record* Load(){
        const bool compatible=!primary||primary->SchemaVersion==1;
        const auto choice=ChooseStoredRecord(primary.has_value(),compatible,
            primary&&Valid(*primary),backup&&Valid(*backup),recovery);
        if(choice==StoredRecord::Primary)return &*primary;
        if(choice==StoredRecord::Backup){recovery=true;return &*backup;}
        return nullptr;
    }
    bool Commit(const Record& next){
        const bool ok=CommitCombinedRecord(next,primary?&*primary:nullptr,
            backup?&*backup:nullptr,recovery,primary.has_value()||backup.has_value(),
            SaveFixture::Text,[&](const Record& r,bool isBackup){
            ++writes;auto& target=isBackup?backup:primary;
            if(writes==failAt){if(damageOnFailure){target=r;target->bHasFieldSurveys=true;target->FieldSurveyStages.clear();}return false;}
            target=r;return true;
        });
        if(ok)recovery=false;
        return ok;
    }
};
}
