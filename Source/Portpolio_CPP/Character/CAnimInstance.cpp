#include "Character/CAnimInstance.h"

#include "CCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACCharacterBase>(TryGetPawnOwner());
	if (Owner == nullptr)
		return;


}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner == nullptr)
		return;

	Speed = Owner->GetVelocity().Size2D();
	FRotator rotator = Owner->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = Owner->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, Owner->GetBaseAimRotation().Pitch, DeltaSeconds, 25);


}
