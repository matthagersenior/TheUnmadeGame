#pragma once
#include "World/UnmadeWorldAtlas.h"
#include <array>
namespace UnmadeCore {
struct ExpeditionSpec { Realm realm;const char* city,*frontier,*refuge,*dungeon,*boss,*weapon,*armor,*technique,*brief,*kill_return,*mercy_return; std::array<const char*,5> people; std::array<const char*,3> clues;double x,y; };
#include "World/UnmadeCodexExpeditionGenerated.inc"
}
