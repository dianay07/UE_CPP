#include "Component/CKeySettingComponent.h"

#include "GameFramework/InputSettings.h"

UCKeySettingComponent::UCKeySettingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCKeySettingComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCKeySettingComponent::SetBasicSetting()
{
	UInputSettings* InputSettings = UInputSettings::StaticClass()->GetDefaultObject<UInputSettings>();

	if (InputSettings)
	{
		// 축 세팅
		InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
		InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.f));

		InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));
		InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));

		InputSettings->AddAxisMapping(FInputAxisKeyMapping("HorizontalLook", EKeys::MouseX, 1.f));
		InputSettings->AddAxisMapping(FInputAxisKeyMapping("VerticalLook", EKeys::MouseY, -1.f));

		// 키 세팅

		// 슬롯
		InputSettings->AddActionMapping(FInputActionKeyMapping("Slot1", EKeys::One));
		InputSettings->AddActionMapping(FInputActionKeyMapping("Slot2", EKeys::Two));
		InputSettings->AddActionMapping(FInputActionKeyMapping("Slot3", EKeys::Three));
		InputSettings->AddActionMapping(FInputActionKeyMapping("Slot4", EKeys::Four));
		InputSettings->AddActionMapping(FInputActionKeyMapping("Slot5", EKeys::Five));
		InputSettings->AddActionMapping(FInputActionKeyMapping("Slot6", EKeys::Six));
		InputSettings->AddActionMapping(FInputActionKeyMapping("Slot7", EKeys::Seven));
		InputSettings->AddActionMapping(FInputActionKeyMapping("Slot8", EKeys::Eight));
		
		InputSettings->AddActionMapping(FInputActionKeyMapping("OpenBook", EKeys::K));
		
		InputSettings->AddActionMapping(FInputActionKeyMapping("SubSkill", EKeys::RightMouseButton));
		
		InputSettings->AddActionMapping(FInputActionKeyMapping("SwapToWarrior", EKeys::One, true));
		InputSettings->AddActionMapping(FInputActionKeyMapping("SwapToDragoon", EKeys::Two, true));
	}
}

void UCKeySettingComponent::ChangeActionKeySetting(FName ActionName, const FKey Key)
{
	UInputSettings* InputSettings = UInputSettings::StaticClass()->GetDefaultObject<UInputSettings>();

	if (InputSettings)
	{
		// 입력 액션 이름과 변경할 키 바인딩 정보 설정
		FInputChord NewKeyBinding(Key);

		TArray<FInputActionKeyMapping> out;
		InputSettings->GetActionMappingByName(ActionName, out);
		for(auto& element : out)
			InputSettings->RemoveActionMapping(element);

		//FInputActionKeyMapping NewKeyMapping(ActionName, NewKeyBinding.Key);
		InputSettings->AddActionMapping(FInputActionKeyMapping(ActionName, Key));

		// 변경된 키 설정 저장
		InputSettings->SaveKeyMappings();
	}
}
