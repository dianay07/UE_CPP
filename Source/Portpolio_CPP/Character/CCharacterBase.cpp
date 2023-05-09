#include "Character/CCharacterBase.h"
#include "Component/CMontageComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACCharacterBase::ACCharacterBase()
{
	Montages = CreateDefaultSubobject<UCMontageComponent>(TEXT("Montage Component"));
	Movement = CreateDefaultSubobject<UCMovementComponent>(TEXT("Movement Component"));
	State = CreateDefaultSubobject<UCStateComponent>(TEXT("State Component"));
	Weapon = CreateDefaultSubobject<UCWeaponComponent>(TEXT("Weapon Component"));

	Movement->DisableControlRotation();
}

void ACCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACCharacterBase::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{

}