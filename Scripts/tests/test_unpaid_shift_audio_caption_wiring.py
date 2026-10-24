"""Unpaid Shift v7: no silent cinematic, no clipped giant text card, no fake
narration; real reproducible WAV Foley and an actual optional Unreal UI."""
from pathlib import Path
import re
import unittest

ROOT=Path(__file__).resolve().parents[2]
def source(p): return (ROOT/p).read_text(encoding="utf-8")

class AudioAndCaptionProduction(unittest.TestCase):
    def test_39_authoritative_beats_have_35_concrete_sound_names(self):
        rules=source("Source/TheUnmadeGame/Public/World/UnmadeUnpaidShiftSceneRules.h")
        cue=re.findall(r'\{UnpaidScene::\w+,\s*\d+,\s*"[^"]+",\s*"[^"]+",\s*"[^"]+",\s*"[^"]+",\s*"([a-z_]+)"',rules)
        self.assertEqual(len(cue),39)
        self.assertEqual(len(set(cue)),35)
        mapping=rules.split("inline const char* UnpaidFoleyCaption",1)[1].split("inline constexpr const UnpaidBeat* UnpaidBeatAt",1)[0]
        captions=set(re.findall(r'UNPAID_FOLEY_CAPTION\(([a-z_]+),"([^"]+)"\)',mapping))
        self.assertEqual({name for name,_ in captions},set(cue))
        self.assertTrue(all(text for _,text in captions))
        self.assertIn("UnmadeCore::UnpaidFoleyCaption(Beat->foley)",source("Source/TheUnmadeGame/Private/World/UnmadeUnpaidShiftPresentation.cpp"))
        runtime=source("Source/TheUnmadeGame/Private/World/UnmadeUnpaidShiftPresentation.cpp")
        for marker in ("CompleteCaption.ParseIntoArray(CaptionWords,TEXT(\" \"),true)",
                       "1.f+static_cast<float>(CaptionWords.Num())/3.f",
                       "FMath::Max(Beat->duration,","FString(UTF8_TO_TCHAR(Beat->blocking)),BeatDuration",
                       "FString(UTF8_TO_TCHAR(Beat->voice)),BeatDuration",
                       "FMath::Max(0.5f,BeatDuration)"):
            self.assertIn(marker,runtime)
        generator=source("Scripts/build_unpaid_shift_audio.py")
        for marker in ("with wave.open(", "RATE=32000","wf.setnchannels(2)",
                       "wf.setsampwidth(2)","sha256","unique_cues",
                       "ORIGINAL_SYNTHETIC_PLACEHOLDER_FOLEY_NOT_FINAL_VO"):
            self.assertIn(marker,generator)
        self.assertNotIn("text_to_speech",generator)
    def test_real_sounds_are_loaded_only_if_present_and_captions_never_depend_on_them(self):
        runtime=source("Source/TheUnmadeGame/Private/World/UnmadeUnpaidShiftPresentation.cpp")
        pos_caption=runtime.index("BesSubtitleOverlay->DisplayBeat(")
        pos_sound=runtime.index("StaticLoadObject(")
        self.assertLess(pos_caption,pos_sound)
        for marker in ("BesSubtitleOverlay->ClearBeat()","CreateWidget<UUnmadeUnpaidShiftSubtitleWidget>",
                       "AddToViewport(50)","StaticLoadObject(","LOAD_NoWarn",
                       "PlaySound2D(","OnBesDirectedBeat.Broadcast(",
                       "SetTimer(BesSubtitleTimer"):
            self.assertIn(marker,runtime)
        self.assertIn("UMG",source("Source/TheUnmadeGame/TheUnmadeGame.Build.cs"))
    def test_subtitles_are_in_safe_lower_third_and_never_intercept_input(self):
        widget=source("Source/TheUnmadeGame/Private/UI/UnmadeUnpaidShiftSubtitleWidget.cpp")
        for marker in ("FAnchors(0.5f,0.97f)","Slot->SetAutoSize(true)",
                       "CaptionWidthConstraint->SetWidthOverride(DesiredWidth)",
                       "const float DesiredWidth=FMath::Max(280.f,ViewportWidth*.86f)",
                       "SetAutoWrapText(true)","HitTestInvisible",
                       "FLinearColor(0.012f,0.016f,0.027f,0.87f)",
                       "SetVisibility(ESlateVisibility::Collapsed)",
                       "FoleyText->SetText(FText::FromString(FoleyCaption))",
                       "void UUnmadeUnpaidShiftSubtitleWidget::NativeTick(",
                       "Preferences->TextScale","FMath::Clamp(Preferences->TextScale,.85f,1.6f)",
                       "FMath::RoundToInt(25.f*Scale)","FMath::RoundToInt(17.f*Scale)",
                       "Preferences->HighContrast","FLinearColor::Black"):
            self.assertIn(marker,widget)
        header=source("Source/TheUnmadeGame/Public/UI/UnmadeUnpaidShiftSubtitleWidget.h")
        self.assertIn("DisplayBeat(",header)
        self.assertIn("FoleyCaption",header)
        self.assertIn("ClearBeat()",header)
    def test_editor_import_guard_and_windows_integration(self):
        importer=source("Scripts/unreal_editor/import_unpaid_shift_audio.py")
        quick=source("Scripts/first_pc_quickstart.ps1")
        direct=source("Scripts/first_pc_build_and_test.ps1")
        ci=source(".github/workflows/static-checks.yml")
        for marker in ("UNMADE_AUDIO_IMPORT_APPROVED","replace_existing\",False",
                       "SHA", "with wave.open(", "unreal.SoundWave"):
            if marker=="SHA":marker="sha256"
            self.assertIn(marker,importer)
        self.assertIn("UNMADE_AUDIO_IMPORT_APPROVED",quick)
        self.assertIn("UNPAID_SHIFT_SOUNDWAVES_INCOMPLETE",quick)
        self.assertIn("build_unpaid_shift_audio.py",direct)
        self.assertIn("build_unpaid_shift_audio.py --check",ci)
    def test_canon(self):
        for path in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                     "docs/lore/CONTINUITY_LOG.md",
                     "docs/lore/living-world-continuity.md"):
            self.assertIn("The Audible Shift",source(path))

if __name__=="__main__":unittest.main()
