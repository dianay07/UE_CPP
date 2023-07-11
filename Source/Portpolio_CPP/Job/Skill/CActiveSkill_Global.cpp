#include "Job/Skill/CActiveSkill_Global.h"

#include "Component/CStateComponent.h"

float UCActiveSkill_Global::GlobalCoolTime = 2.5f;

void UCActiveSkill_Global::ActiveSkill(int InIndex)
{
	if (SkillDatas.Num() < 1)
		return;

	if (State->IsIdleMode() == false)
		return;

	Super::ActiveSkill(InIndex);
	SkillDatas[InIndex].ActiveSkill(OwnerCharacter);
}
