#include "Job/Skill/CActiveSkill_Global.h"

#include "Character/CCharacterBase.h"
#include "Component/CStateComponent.h"

float UCActiveSkill_Global::GlobalCoolTime = 2.5f;

void UCActiveSkill_Global::ActiveSkill(int InIndex)
{
	if (SkillDatas.Num() < 1)	return;

	if (State->IsActionMode())	return;

	Super::ActiveSkill(InIndex);

	State->SetIsBattle(true);
	index = InIndex;
	SkillDatas[InIndex].ActiveSkill(OwnerCharacter);
}

void UCActiveSkill_Global::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);

	if (InOther == nullptr) return;

	for (ACharacter* hitted : Hitted)
		if (hitted == InOther) return;

	Hitted.AddUnique(InOther);

	if (HitDatas.Num() - 1 < index)
		return;

	HitDatas[index].SendDamage(Cast<ACCharacterBase>(InAttacker), InAttackCauser, Cast<ACCharacterBase>(InOther));
}

void UCActiveSkill_Global::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{
	Super::OnAttachmentEndOverlap(InAttacker, InOther);

	Hitted.Empty();

	/*float angle = -2.0f;
	ACharacter* candidate = nullptr;

	for (ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();


		FVector forward = FQuat(OwnerCharacter->GetActorRotation()).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);
		if (dot >= angle)
		{
			angle = dot;
			candidate = hitted;
		}
	}

	if (!!candidate)
	{
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
		FRotator target = FRotator(0, rotator.Yaw, 0);

		AController* controller = OwnerCharacter->GetController<AController>();
		controller->SetControlRotation(target);
	}

	Hitted.Empty();*/
}
