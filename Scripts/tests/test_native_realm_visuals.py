"""Keep source-owned native realm skyline cues nonblocking and unrelated to player choice."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source(path):return (ROOT/path).read_text(encoding="utf-8")
class NativeRealmVisuals(unittest.TestCase):
    def test_nine_authored_kit_variants_use_unchanged_world_tags(self):
        cpp=source("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
        self.assertIn('#include "World/UnmadeProceduralArtRules.h"',cpp)
        self.assertIn("RealmVisualDesigns[a]",cpp)
        self.assertIn("Design.silhouette[j]",cpp)
        self.assertIn("UnmadeVisualGuide.%d.%d",cpp)
        self.assertIn("FVector(Piece.dx,Piece.dy,Piece.dz)",cpp)
        self.assertIn("GetWorld()->SpawnActor<AStaticMeshActor>",cpp)
        self.assertIn("SetCollisionEnabled(ECollisionEnabled::NoCollision)",cpp)
        self.assertIn("SetGenerateOverlapEvents(false)",cpp)
        self.assertIn("SetCanEverAffectNavigation(false)",cpp)
        self.assertNotIn("StyleWorldProp(Block",cpp)
        self.assertIn("SpawnBlock(at,scale,FName",cpp)
    def test_no_source_gameplay_authority_or_unreal_completion_claim(self):
        header=source("Source/TheUnmadeGame/Public/World/UnmadeProceduralArtRules.h")
        self.assertIn("RealmCrown::CharterArch",header)
        self.assertIn("RealmCrown::PairedThreshold",header)
        self.assertNotIn("SaveGameToSlot",header)
        self.assertNotIn("CommitExpeditions",header)
        self.assertNotIn("GiveToOrrel",header)
        self.assertIn("Tests/world/realm_visual_guides_test.cpp",
                      source(".github/workflows/static-checks.yml"))
if __name__=="__main__":unittest.main()
