#include "Item/CSkillBase.h"

#include "Utility.h"
#include "Character/CCharacterBase.h"
#include "Component/CMovementComponent.h"
#include "Component/CTargetComponent.h"
#include "Component/CStatusComponent.h"

UCSkillBase::UCSkillBase()
{
}

void UCSkillBase::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACCharacterBase* InOwner,
	const TArray<FSkillData>& InSkillDatas, const TArray<FSkillDamageData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	Movement = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));

	SkillDatas = InSkillDatas;
	HitDatas = InHitDatas;
}

void UCSkillBase::ActiveSkill(int InIndex)
{
	State->SetActionMode();

	// Ÿ���� ������ ��ų�� ��������, �ڵ� ������ �Ҷ����� ī�޶�� ������ �ΰ� ĳ���Ͱ� Ÿ���� �ٶ���� �Ѵ�....
	// �� ���� : ī�޶� ������ �δ� ���� ����������, ��Ʈ�ѷ� ��ü�� ������ ������ ? ī�޶� ȹ����
	// �߰��߰� : ù Ÿ���� �������� ��� �ٶ󺸴� �� ����
	//if (IsValid(OwnerCharacter->GetTarget()->GetTargetActor()))
	//{
	//	UE_LOG(LogTemp, Error, TEXT("%s"), *OwnerCharacter->GetTarget()->GetTargetActor()->GetName())

	//	static ACCharacterBase* target = OwnerCharacter->GetTarget()->GetTargetActor();
	//	FRotator direction = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), target->GetActorLocation());

	//	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), direction.Roll, direction.Pitch, direction.Yaw);

	//	// pitch�� �ٲ�� Y�� ������ -> ���� Controller�� Y��
	//	// �׽�Ʈ �ʿ�
	//	FRotator ret = UKismetMathLibrary::MakeRotator(OwnerCharacter->GetControlRotation().Roll, OwnerCharacter->GetControlRotation().Pitch, direction.Yaw);
	//	OwnerCharacter->GetController()->SetControlRotation(ret);
	//}

	UE_LOG(LogTemp, Display, TEXT("Current Type : Action"));
}

void UCSkillBase::Begin_ActiveSkill()
{
	bBeginAction = true;
}

void UCSkillBase::End_ActiveSkill()
{
	bBeginAction = false;

	State->SetIdleMode();

	UE_LOG(LogTemp, Display, TEXT("Current Type : Idle"));
}

void UCSkillBase::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	if (Effect == nullptr)
		return;

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	Utility::PlayEffect(InWorld, Effect, transform);
}

void UCSkillBase::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	if (Effect == nullptr)
		return;

	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	Utility::PlayEffect(InWorld, Effect, transform);
}