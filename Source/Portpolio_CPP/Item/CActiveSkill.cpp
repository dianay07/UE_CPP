#include "Item/CActiveSkill.h"

#include "Utility.h"
#include "Character/CCharacterBase.h"
#include "Component/CMovementComponent.h"
#include "Component/CStatusComponent.h"

UCActiveSkill::UCActiveSkill()
{
}

void UCActiveSkill::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACCharacterBase* InOwner,
	const TArray<FSkillData>& InSkillDatas, const TArray<FHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	Movement = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));

	SkillDatas = InSkillDatas;
	HitDatas = InHitDatas;
}

void UCActiveSkill::ActiveSkill(int InIndex)
{
	State->SetActionMode();
}

void UCActiveSkill::Begin_ActiveSkill()
{
	bBeginAction = true;
}

void UCActiveSkill::End_ActiveSkill()
{
	bBeginAction = false;

	State->SetIdleMode();

}

void UCActiveSkill::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	if (Effect == nullptr)
		return;

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	Utility::PlayEffect(InWorld, Effect, transform);
}

void UCActiveSkill::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	if (Effect == nullptr)
		return;

	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	Utility::PlayEffect(InWorld, Effect, transform);
}
