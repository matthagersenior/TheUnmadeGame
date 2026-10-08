using UnrealBuildTool;
using System.Collections.Generic;

public class TheUnmadeGameEditorTarget : TargetRules
{
    public TheUnmadeGameEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("TheUnmadeGame");
    }
}
