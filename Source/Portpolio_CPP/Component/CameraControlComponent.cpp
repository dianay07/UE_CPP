#include "Component/CameraControlComponent.h"

#include "Character/CCharacterBase.h"
#include "Character/CPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCameraControlComponent::UCameraControlComponent()
{
	//OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
}


void UCameraControlComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACPlayer>(GetOwner());

	ZoomData.Speed = 20;
	ZoomData.MinRange = 100;
	ZoomData.MaxRange = 800;
	ZoomData.InterpSpeed = 5;
}

void UCameraControlComponent::OnHorizontalLook(float InAxis)
{
	if (bFixedCamera == true)
		return;

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCameraControlComponent::OnVerticalLook(float InAxis)
{
	if (bFixedCamera == true)
		return;

	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

void UCameraControlComponent::ApplyZoom(float InAxis)
{
	float value = 0;
	value =Zooming + ZoomData.Speed * InAxis;
	Zooming = UKismetMathLibrary::Clamp(value, ZoomData.MinRange, ZoomData.MaxRange);

	USpringArmComponent* SpringArm = OwnerCharacter->GetSpringArm();
	if(!UKismetMathLibrary::NearlyEqual_FloatFloat(Zooming, SpringArm->TargetArmLength))
	{
		float Length = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, Zooming, GetWorld()->GetDeltaSeconds(), ZoomData.InterpSpeed);
		SpringArm->TargetArmLength = Length;
	}
}

