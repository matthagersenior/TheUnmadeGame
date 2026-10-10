import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf8")
class IllustratedLoreContinuity(unittest.TestCase):
    def test_all_realms_from_source_represented(self):
        canon=read("docs/lore/living-world-continuity.md")
        atlas=read("Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h")
        for name in ("The Threefold Reach","The Rain That Forgot the Sea",
          "The Hearth Beneath","The Sea of Written Debts","The Upside-Down Choir",
          "The Bones of Yesterdays","The Hundred Unlived",
          "The Orchard of Unwritten Kings","The Place Before Place"):
            self.assertIn(name,atlas)
            self.assertIn(name,canon)
        self.assertIn("The Second Night",canon)
        self.assertIn("60 total",read("docs/lore/CONTINUITY_LOG.md"))
    def test_all_four_player_pillars_and_signature_mechanics(self):
        canon=read("docs/lore/living-world-continuity.md")
        for term in ("Freedom with consequences","A genuinely living world",
                     "Challenge without wasted time","Exploration worth the detour",
                     "Unwrite Law","Witnesscraft","Borrowed Lives",
                     "Legacy Forging","Living Roads","Tomorrow's Debt",
                     "Boss Understanding","Paradox Convergence","Oathbinding",
                     "Unreliable Cartography"):
            self.assertIn(term,canon)
    def test_authored_aftermath_and_not_shipped_claim(self):
        canon=read("docs/lore/living-world-continuity.md")
        model=read("Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h")
        self.assertIn("RealmAftermathSpecs",model)
        self.assertIn("RealmAftermathResult::Locked",model)
        self.assertIn("nine",canon.lower())
        self.assertIn("not navigable levels",canon.lower())
        self.assertIn("no engine compile or playtest",canon.lower())
        for title in ("The Road After the Rescue","The Storm's Second Invoice",
           "The Ember That Chose No Heir","The Unpaid Person",
           "The Note That Held a Home","The Quarry of Other Yesterdays",
           "The Street of Unclaimed Birthdays","The Crown No One Wanted",
           "The Place After the Ending"):
            self.assertIn(title,canon)
            self.assertIn(title,model)
if __name__=="__main__": unittest.main()
