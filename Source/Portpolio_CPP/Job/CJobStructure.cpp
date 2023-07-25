#include "CJobStructure.h"

#include "Character/CCharacterBase.h"
#include "Component/CMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility.h"

void FSkillData::ActiveSkill(ACCharacterBase* InOwner)
{
	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FSkillData::SetSkillCooltime(float InTime)
{
	SkillCooltime = InTime;
}

//////////////////////////////////////////////////////

void FSkillDamageData::SendDamage(ACCharacterBase* InAttacker, AActor* InAttackCauser, ACCharacterBase* InOther)
{
	FSkillDamageEvent e;
	e.HitData = this;

	InOther->TakeDamage(Damage, e, InAttacker->GetController(), InAttackCauser);
}

//void FSkillDamageData::PlaySoundWave(ACCharacterBase* InOwner)
//{
//	if (Sound == nullptr)
//		return;
//
//	UWorld* world = InOwner->GetWorld();
//	FVector location = InOwner->GetActorLocation();
//
//	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
//}

void FSkillDamageData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	if (Effect == nullptr)
		return;

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	Utility::PlayEffect(InWorld, Effect, transform);
}

void FSkillDamageData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	if (Effect == nullptr)
		return;

	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	Utility::PlayEffect(InWorld, Effect, transform);
}

//////////////////////////////////////////////////////
