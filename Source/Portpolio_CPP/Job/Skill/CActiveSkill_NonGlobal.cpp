#include "Job/Skill/CActiveSkill_NonGlobal.h"

#include "Character/CCharacterBase.h"
#include "Component/CMovementComponent.h"
#include "Component/CStateComponent.h"

UCActiveSkill_NonGlobal::UCActiveSkill_NonGlobal()
{
}

void UCActiveSkill_NonGlobal::BeginPlay(ACCharacterBase* InOwner, ACAttachment* InAttachment, UCSkillBase* InSkill)
{
	Owner = InOwner;
	Attachment = InAttachment;
	ActiveSkill = InSkill;

	State = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	Movement = Cast<UCMovementComponent>(Owner->GetComponentByClass(UCMovementComponent::StaticClass()));
}

void UCActiveSkill_NonGlobal::Pressed()
{
	bInAction = true;
}

void UCActiveSkill_NonGlobal::Released()
{
	bInAction = false;
}
