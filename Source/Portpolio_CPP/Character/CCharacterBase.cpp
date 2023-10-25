#include "Character/CCharacterBase.h"
#include "Component/CMovementComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CTargetComponent.h"

ACCharacterBase::ACCharacterBase()
{
	Movement = CreateDefaultSubobject<UCMovementComponent>(TEXT("Movement Component"));
	State = CreateDefaultSubobject<UCStateComponent>(TEXT("State Component"));
	Status = CreateDefaultSubobject<UCStatusComponent>(TEXT("Status Compoenet"));
	Target = CreateDefaultSubobject<UCTargetComponent>(TEXT("Target Compoenet"));

	Movement->FixedCharacterSetting();
}