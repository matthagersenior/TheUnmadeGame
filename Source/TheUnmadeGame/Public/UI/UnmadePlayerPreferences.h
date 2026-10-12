#pragma once
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "UObject/Object.h"
#include "UnmadePlayerPreferences.generated.h"
UCLASS(Config=GameUserSettings)
class THEUNMADEGAME_API UUnmadePlayerPreferences : public UObject {
 GENERATED_BODY()
public:
 UPROPERTY(Config) float TextScale=1;
 UPROPERTY(Config) float CameraSensitivity=1;
 UPROPERTY(Config) float AudioVolume=1;
 UPROPERTY(Config) bool HighContrast=false;
 UPROPERTY(Config) bool ReducedFlashes=false;
 UPROPERTY(Config) bool ControllerPrompts=false;
 UPROPERTY(Config) bool ShowTutorials=true;
 UPROPERTY(Config) FKey AttackKey=EKeys::LeftMouseButton;
 UPROPERTY(Config) FKey DodgeKey=EKeys::LeftShift;
 UPROPERTY(Config) FKey InteractKey=EKeys::E;
 UPROPERTY(Config) FKey GuardKey=EKeys::RightMouseButton;
 UPROPERTY(Config) FKey TechniqueKey=EKeys::F11;
 UPROPERTY(Config) FKey PadGuard=EKeys::Gamepad_LeftTrigger;
 UPROPERTY(Config) FKey PadTechnique=EKeys::Gamepad_LeftThumbstick;
 UPROPERTY(Config) FKey PadAttack=EKeys::Gamepad_RightTrigger;
 UPROPERTY(Config) FKey PadDodge=EKeys::Gamepad_FaceButton_Right;
 UPROPERTY(Config) FKey PadInteract=EKeys::Gamepad_FaceButton_Left;
 void Validate();
 void ApplyMappings();
 bool Bind(int Action,FKey Key,FString& Reason);
 FString Describe()const;
 FString ControlPrompt()const;
};
