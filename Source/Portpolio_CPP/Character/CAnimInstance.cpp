#include "Character/CAnimInstance.h"

#include "CEnemy.h"
#include "CPlayer.h"
#include "Component/CMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACCharacterBase>(TryGetPawnOwner());
	if (Owner == nullptr)
		return;

	Job = Cast<UCJobComponent>(Owner->GetComponentByClass(UCJobComponent::StaticClass()));
	if(!!Job)
		Job->OnJobChanged.AddDynamic(this, &UCAnimInstance::OnJobTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner == nullptr)
		return;

	Speed = Owner->GetVelocity().Size2D();
	/*if (Cast<ACEnemy>(Owner))
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, FString::SanitizeFloat(Owner->GetVelocity().Size2D()));*/
	
	FRotator rotator = Owner->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = Owner->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;
	
	Pitch = UKismetMathLibrary::FInterpTo(Pitch, Owner->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	IsJump = Owner->GetCharacterMovement()->IsFalling();
	
	IsFixedCamera = Owner->GetMovement()->IsFixedCamera();

	IsBattle = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()))->IsInBattle();
}

void UCAnimInstance::OnJobTypeChanged(EJob InPrevType, EJob InNewType)
{
	JobType = InNewType;
}
