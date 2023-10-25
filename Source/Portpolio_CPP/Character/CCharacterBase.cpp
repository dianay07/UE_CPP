#include "Character/CCharacterBase.h"
#include "Component/CMontageComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CTargetComponent.h"

ACCharacterBase::ACCharacterBase()
{
	Montages = CreateDefaultSubobject<UCMontageComponent>(TEXT("Montage Component"));
	Movement = CreateDefaultSubobject<UCMovementComponent>(TEXT("Movement Component"));
	State = CreateDefaultSubobject<UCStateComponent>(TEXT("State Component"));
	Status = CreateDefaultSubobject<UCStatusComponent>(TEXT("Status Compoenet"));
	Target = CreateDefaultSubobject<UCTargetComponent>(TEXT("Target Compoenet"));

	Movement->FixedCharacterSetting();
}