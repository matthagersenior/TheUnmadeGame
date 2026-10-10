"""Source test for durable player identity and editor-facing customization contract."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class CharacterProfileSource(unittest.TestCase):
    def test_identity_is_saved_and_restored_with_old_saves(self):
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for needle in ("bHasCharacterIdentity","CharacterIdentityChoices","CharacterChosenName"):
            self.assertIn(needle,save)
            self.assertIn(needle,player)
        self.assertIn("Identity.Restore(",player)
        self.assertIn("bCharacterIdentitySaveRejected",player)
        self.assertIn("Identity.Restore(Before)",player)
        self.assertIn("SaveCharacterIdentity()",player)
    def test_profile_can_be_used_by_future_umg_without_ai(self):
        h=read("Source/TheUnmadeGame/Public/Player/UnmadeCharacter.h")
        cpp=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for action in ("SetCharacterFeature", "SetCharacterChosenName",
                       "GetCharacterChosenName", "ApplyIdentitySilhouette",
                       "ShowCharacterProfile"):
            self.assertIn(action,h)
            self.assertIn(action,cpp)
        self.assertIn("SetRelativeScale3D(",cpp)
        self.assertIn("CharacterProfile",read("Config/DefaultInput.ini"))
if __name__=="__main__":unittest.main()
