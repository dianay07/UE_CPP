#include "Item/CSkillBase.h"

#include "Utility.h"
#include "Character/CCharacterBase.h"
#include "Component/CMovementComponent.h"

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

void UCSkillBase::ActiveAvailable(int InIndex)
{
	State->SetActionMode();

	// 타겟이 있으면 스킬을 쓸때마다, 자동 공격을 할때마다 카메라는 가만히 두고 캐릭터가 타겟을 바라봐야 한다....
	//if (IsValid(OwnerCharacter->GetTarget()->GetTargetActor()))
	//{
	//	UE_LOG(LogTemp, Error, TEXT("%s"), *OwnerCharacter->GetTarget()->GetTargetActor()->GetName())

	//	static ACCharacterBase* target = OwnerCharacter->GetTarget()->GetTargetActor();
	//	FRotator direction = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), target->GetActorLocation());

	//	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), direction.Roll, direction.Pitch, direction.Yaw);

	//	// pitch가 바뀌면 Y가 고정됨 -> 기존 Controller의 Y을
	//	// 테스트 필요
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