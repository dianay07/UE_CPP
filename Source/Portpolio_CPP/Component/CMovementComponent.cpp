#include "Component/CMovementComponent.h"

#include "Character/CCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCMovementComponent::UCMovementComponent()
{
	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
}


void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::FixedCameraSetting()
{
	bFixedCamera = true;
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::FixedCharacterSetting()
{
	bFixedCamera = false;
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::RotateToTarget(ACCharacterBase* InTarget)
{
	FRotator LookAtRotate = 
		UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), InTarget->GetActorLocation());

	GetOwner()->SetActorRotation(LookAtRotate);
}

void UCMovementComponent::OnMoveForward(float InAxis)
{
	if (bCanMove == false)
		return;

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	if (bCanMove == false)
		return;

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}
