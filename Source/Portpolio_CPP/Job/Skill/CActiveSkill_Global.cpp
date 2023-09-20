#include "Job/Skill/CActiveSkill_Global.h"

#include "Character/CCharacterBase.h"
#include "Character/CPlayer.h"
#include "Component/CStateComponent.h"
#include "Component/CTargetComponent.h"
#include "Kismet/GameplayStatics.h"

float UCActiveSkill_Global::GlobalCoolTime = 2.5f;

void UCActiveSkill_Global::ActiveAvailable(int InIndex)
{
	if (SkillDatas.Num() < 1)	return;

	if (State->IsActionMode())	return;

	if (InIndex == 999)	return;

	Super::ActiveAvailable(InIndex);

	State->SetIsBattle(true);
	index = InIndex;
	SkillDatas[InIndex].ActiveSkill(OwnerCharacter);

	if(SkillDatas[InIndex].Effect != nullptr)
		SkillDatas[InIndex].PlayEffect(GetWorld(), OwnerCharacter->GetActorLocation());

	if(Cast<ACPlayer>(OwnerCharacter))
	{
		//UGameplayStatics::ApplyDamage(OwnerCharacter->GetTarget()->GetTargetActor(), SkillDatas[InIndex].Damage, OwnerCharacter->GetController(), OwnerCharacter, NULL);

		FSkillDamageEvent e;
		e.HitData = &HitDatas[InIndex];

		OwnerCharacter->GetTarget()->GetTargetActor()->TakeDamage(SkillDatas[InIndex].Damage, e, OwnerCharacter->GetController(), NULL);
		UE_LOG(LogActor, Warning, TEXT("%s is Attack %s, DamageAmount is %f"), *OwnerCharacter->GetName(), *OwnerCharacter->GetTarget()->GetTargetActor()->GetName(), SkillDatas[InIndex].Damage);
	}
}
