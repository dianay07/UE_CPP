#include "Character/CCharacterBase.h"
#include "Component/CMontageComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CEquipComponent.h"
#include "Component/CTargetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACCharacterBase::ACCharacterBase()
{
	Montages = CreateDefaultSubobject<UCMontageComponent>(TEXT("Montage Component"));
	Movement = CreateDefaultSubobject<UCMovementComponent>(TEXT("Movement Component"));
	State = CreateDefaultSubobject<UCStateComponent>(TEXT("State Component"));
	Status = CreateDefaultSubobject<UCStatusComponent>(TEXT("Status Compoenet"));
	Target = CreateDefaultSubobject<UCTargetComponent>(TEXT("Target Compoenet"));

	Movement->FixedCharacterSetting();
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