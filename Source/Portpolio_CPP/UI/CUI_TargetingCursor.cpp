#include "UI/CUI_TargetingCursor.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

void UCUI_TargetingCursor::NativeConstruct()
{
	Super::NativeConstruct();

	Box->WidthOverride = 120;
	Box->HeightOverride = 400;
}

void UCUI_TargetingCursor::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector CamLocation = CameraManager->GetCameraLocation();
}