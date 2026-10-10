#pragma once
// GENERATED from Authoring/world_content_pack.json. Do not edit directly.
// Regenerate with: python Scripts/build_world_content.py --write
#include <array>
#include <cstring>
namespace UnmadeCore {
struct OuterPersonSpec {
    const char* id; const char* displayName; const char* realm;
    const char* dayLine; const char* nightLine;
};
inline constexpr std::array<OuterPersonSpec,18> OuterPeople = {{
    {"npc.tidal.ledgerkeeper.001","Veynu of the Ninth Due","TidalLedger","I keep the due dates, but no ledger permits a child to inherit an unwritten loan.","At night I hear the unpaid winters knocking against the pier's empty posts."},
    {"npc.tidal.ferryman.001","Orshel Drywake","TidalLedger","The ferry goes even without a sea; the child on my route still needs a shore.","The oars make no sound now. That is how I know someone is listening."},
    {"npc.tidal.witness.001","Ezzun Aftermark","TidalLedger","This signature belongs to me, but the contract was written before my grandmother's birth.","I dreamed of signing twice, then found a third date in the keel."},
    {"npc.sky.choirmaster.001","Yllesh Underchord","SkyBelow","Count the rests. A missing voice can be heard most clearly when we pause.","Tonight the high houses drift toward a note none of us admits singing."},
    {"npc.sky.structuralist.001","Vhaurn Counterweight","SkyBelow","The sleepers are my calculation; I refuse to call them an acceptable loss.","Every nail loosens at night if the choir's breathing falters."},
    {"npc.sky.dissenter.001","Orru of the Removed Rest","SkyBelow","Their names are not dissonance. The city invented silence to avoid hearing them.","My forbidden score has six voices more than the public one."},
    {"npc.bones.seamwarden.001","Ghraet Last-Seam","CinderSpine","There is no clean cut. Tell me whose name is inside the next block.","I know what the quarry remembers when our lamps go out."},
    {"npc.bones.mason.001","Torrun-Mortar","CinderSpine","An honest brace should hold a house without stealing the foundation from someone else.","The wall breathes at night; I think it dreams of the family we forgot."},
    {"npc.bones.descendant.001","Issil Againborn","CinderSpine","The portrait recognizes me more readily than the census does.","My grandmother speaks from the mortar only when the house is empty."},
    {"npc.unlived.censuskeeper.001","Nhel of No-Entry","HundredUnlived","A blank space in the census is not a blank person.","The doors count our footsteps at night, and the numbers never agree."},
    {"npc.unlived.caretaker.001","Auvet Keep-Quiet","HundredUnlived","Tell me what would make you safe. A name can wait.","Some guests sleep better if I leave the lamps on and ask nothing."},
    {"npc.unlived.challenger.001","Veysha Never-Signed","HundredUnlived","I have lived here every day. The city must learn to say so without claiming me.","Every morning I ask the same question: who signed my absence?"},
    {"npc.orchard.untitled.001","Serriv Crownless","OrchardOfKings","I tend the court and hold no throne. That must be more than a costume.","At dusk the roots keep voting after the people have gone home."},
    {"npc.orchard.gardener.001","Imro Root-Vote","OrchardOfKings","A road is not public if its fruit feeds only the people who control the roots.","The orchard counts empty baskets before it counts its rulers."},
    {"npc.orchard.historian.001","Eliun No-Coronation","OrchardOfKings","The earliest law is a refusal signed by the people the court calls subjects.","The empty throne has more witnesses than any occupied one."},
    {"npc.absence.firstwitness.001","Orriv Before-Arrival","FirstAbsence","I remember meeting you before either of us reached this shore.","The earlier footprint is still warm when the universe goes quiet."},
    {"npc.absence.caretaker.001","Thae of the Near Door","FirstAbsence","Stay within sight of the return path. You owe no one an explanation yet.","A safe doorway is still kind even when the wall around it is false."},
    {"npc.absence.otherwitness.001","Veilune Both-Mornings","FirstAbsence","I saw the world begin twice. Both times there was someone holding the door.","At night both remembered dawns cast shadows through my room."},
}};
inline const OuterPersonSpec* FindOuterPerson(const char* id) noexcept {
    if(!id)return nullptr;
    for(const auto& p:OuterPeople)if(std::strcmp(id,p.id)==0)return &p;
    return nullptr;
}
} // namespace UnmadeCore
