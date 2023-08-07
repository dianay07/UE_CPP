#include "Job/Skill/NonGlobalSkill/CNonGlobal_Warrior.h"
#include "Component/CStateComponent.h"
#include "Item/CAttachment.h"

void UCNonGlobal_Warrior::Pressed()
{
	if (State->IsIdleMode() == false) return;

	Super::Pressed();

	State->SetActionMode();

	ActionData.ActiveSkill(Owner);
}

void UCNonGlobal_Warrior::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	//Attachment->OnAttachmentBeginOverlap.Remove(Skill, "OnAttachmentBeginOverlap");
}

