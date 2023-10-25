#include "Job/Skill/NonGlobalSkill/CNonGlobal_Warrior.h"

#include "DrawDebugHelpers.h"
#include "Character/CPlayer.h"
#include "Component/CStateComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/CAttachment.h"
#include "Item/CSkillBase.h"

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

	Attachment->OnAttachmentBeginOverlap.Remove(ActiveSkill, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCNonGlobal_Warrior::OnAttachmentBeginOverlap);
}

void UCNonGlobal_Warrior::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(ActiveSkill, &UCSkillBase::OnAttachmentBeginOverlap);

	State->SetIdleMode();

	Hitted.Empty();
}

void UCNonGlobal_Warrior::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

}

void UCNonGlobal_Warrior::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCuaser, ACharacter* InOther)
{
	if(InOther == nullptr) return;

	for (ACharacter* character : Hitted)
		if(character == InOther) return;

	Hitted.AddUnique(InOther);

	HitData.SendDamage(Owner, InAttackCuaser, Cast<ACCharacterBase>(InOther));
}

