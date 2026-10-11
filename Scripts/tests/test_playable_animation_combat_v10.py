"""Source and generated-source acceptance for the Playable World v10 pass.
Not an Unreal Editor compile or proof that NavMeshBoundsVolume was baked.
"""
import json, subprocess, sys, tempfile, unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def src(path):return (ROOT/path).read_text(encoding="utf-8")
class PerformanceV10(unittest.TestCase):
 def test_authoritative_skeletal_sources_and_fallback(self):
  player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
  npc=src("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
  beast=src("Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp")
  for text in (player,npc,beast):
   self.assertIn("SetSkeletalMesh",text)
   self.assertIn("PlayAnimation",text)
   self.assertIn("SetVisibility(false)",text)
  self.assertIn("BeginDirectedBlocking",npc)
  self.assertIn("EndDirectedBlocking",npc)
  self.assertIn("NotifyPlayerHit",beast)
  self.assertIn("NotifyPlayerHit",player)
 def test_navigation_and_real_prestrike_warning(self):
  source=src("Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp")
  for x in ("FindPathToLocationSynchronously","UNavigationPath","IsPartial()",
            "NextPathRefreshAt","WindupEndsAt","NextWindupAllowedAt",
            "AddMovementInput","CombatSignal","TelegraphLight","LineTraceTestByChannel"):
   self.assertIn(x,source)
  self.assertNotIn("AddActorWorldOffset",source)
  self.assertIn('"NavigationSystem"',src("Source/TheUnmadeGame/TheUnmadeGame.Build.cs"))
 def test_all_six_differentiated_landmarks_and_materials(self):
  s=src("Source/TheUnmadeGame/Private/World/UnmadePlayableEncounters.cpp")
  for label in ("RibOne","PrismTall","SilentBell","ArchiveSpineA",
                "WreckHull","Chimney","MI_AshBone","MI_PaleGlass",
                "MI_BellPatina","MI_ArchiveInk","MI_SaltFoam","MI_KilnSlag"):
   self.assertIn(label,s)
  self.assertIn("SetMaterial(0,Mat)",s)
 def test_cinematic_speaker_is_physical_and_skip_releases(self):
  s=src("Source/TheUnmadeGame/Private/World/UnmadeUnpaidShiftPresentation.cpp")
  for item in ("BeginDirectedBlocking","EndDirectedBlocking","BesStagedSpeaker",
               "UnmadeBesCameraObstruction","RestoreBesCamera"):
   self.assertIn(item,s)
 def test_generated_rigs_include_expanded_valid_keyed_clips(self):
  generator=ROOT/"Scripts/build_skeletal_performance_kit.py"
  with tempfile.TemporaryDirectory() as d:
   subprocess.run([sys.executable,str(generator),"--out",d],check=True,
       stdout=subprocess.DEVNULL,stderr=subprocess.PIPE,text=True)
   clips=0
   for name in ("Resident","Beast","Wraith"):
    g=json.loads((Path(d)/("SK_Unmade"+name+".gltf")).read_text())
    self.assertGreaterEqual(len(g["skins"][0]["joints"]),4)
    self.assertEqual(len(g["animations"]),17)
    for anim in g["animations"]:
     self.assertTrue(anim["channels"],anim["name"])
     self.assertTrue(anim["samplers"],anim["name"])
    clips+=len(g["animations"])
   self.assertEqual(clips,51)
   palette=json.loads((Path(d)/"UnmadeSixRegionPBR.gltf").read_text())
   self.assertEqual(len(palette["materials"]),6)
if __name__=="__main__":unittest.main()
