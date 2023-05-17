#include "Job/Skill/CActiveSkill_Global.h"

#include "Component/CStateComponent.h"

float UCActiveSkill_Global::GlobalCoolTime = 3.0f;

void UCActiveSkill_Global::ActiveSkill(int InIndex)
{
	if (SkillDatas.Num() < 1)
		return;

	if (State->IsActionMode())
		return;

	Super::ActiveSkill(InIndex);
	SkillDatas[InIndex].ActiveSkill(OwnerCharacter);
}
