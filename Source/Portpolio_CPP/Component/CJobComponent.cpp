#include "Component/CJobComponent.h"

#include "CStatusComponent.h"
#include "CTargetComponent.h"
#include "Item/CEquipment.h"
#include "Character/CCharacterBase.h"
#include "Item/CSkillBase.h"
#include "Job/CJobDataAsset.h"
#include "Kismet/KismetMathLibrary.h"

UCJobComponent::UCJobComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCJobComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
	for (int i = 0; i < 2; i++)
	{
		if (DataAssets[i] != nullptr)
		{
			DataAssets[i]->BeginPlay(OwnerCharacter);
		}
	}

	// �ɷ�ġ, ���� ���� ������Ʈ ���
	Status = Cast<UCStatusComponent>(OwnerCharacter->GetComponentByClass(UCStatusComponent::StaticClass()));
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));

	OnSkillActivate.AddDynamic(this, &UCJobComponent::SkillActivate);
}

void UCJobComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(OwnerCharacter->GetTarget()->GetTargetActor()))
		GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimerHandle);
}

ACAttachment* UCJobComponent::GetAttachment()
{
	return DataAssets[static_cast<int32>(JobName)]->GetAttachment();
}

UCEquipment* UCJobComponent::GetEquipment()
{
	return DataAssets[static_cast<int32>(JobName)]->GetEquipment();
}

UCSkillBase* UCJobComponent::GetActiveSkill()
{
	return DataAssets[static_cast<int32>(JobName)]->GetActiveSkill();
}

TArray<FSkillData> UCJobComponent::GetSkillData()
{
	return DataAssets[static_cast<int32>(JobName)]->SkillDatas;
}

////////////////////////////////////////////////////////////////////////////

EJob UCJobComponent::GetCurrentJob()
{
	return JobName;
}

// TODO ������ : ���Կ� ��ȣ�� ID(��ư ����)�� �ְ� ���� ������ �����ϰԲ� �ϸ� ����
void UCJobComponent::SkillActivate(int InCount)
{

	if (GetActiveSkill() == nullptr)
		return;

	// ���� �Ÿ�
	if (OwnerCharacter->GetTarget()->GetTargetActor() == nullptr)
		return;

	distance = UKismetMathLibrary::Vector_Distance(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetTarget()->GetTargetActor()->GetActorLocation());
	if (distance > DataAssets[static_cast<int32>(JobName)]->AttackRange)
		return;

	State->SetIsBattle(true);

	// TODO : ���� / ����� ��ų���� �ۼ�
	GetActiveSkill()->ActiveSkill(InCount);
}


// �� ����ɶ� �ؾ��� �͵�
void UCJobComponent::ChangeJob(EJob InCurrentJob)
{
	/*if (JobName == InCurrentJob)
		return;*/

	// �ش� ������ ������ ���� ����
	if (DataAssets[static_cast<int32>(JobName)] != nullptr)
	{
		AutoAttackMontages.Reset();

		// �� ���� ����
		EJob prevJob = JobName;
		JobName = InCurrentJob;

		if (JobName == EJob::Max)
			return;

		static UCJobDataAsset* dataAsset = DataAssets[static_cast<int32>(JobName)];
		for (int i = 0; i < dataAsset->AutoAttackMontages.Num(); i++)
		{
			static UAnimMontage* montage = dataAsset->AutoAttackMontages[i];

			AutoAttackMontages.AddUnique(montage);
		}

		FString EnumString = StaticEnum<EJob>()->GetNameStringByValue(static_cast<int32>(JobName));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *EnumString);
	}
}

void UCJobComponent::PlayEquipMotion()
{
	DataAssets[(int32)JobName]->GetEquipment()->Equip();
}

// ���� ������ �� �ڵ� ����
void UCJobComponent::DoingAutoAttack()
{
	if(State->IsInBattle() == false)
		GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimerHandle);

	// ���� �Ÿ�
	distance = UKismetMathLibrary::Vector_Distance(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetTarget()->GetTargetActor()->GetActorLocation());
	if (distance > DataAssets[static_cast<int32>(JobName)]->AttackRange)
		return;

	AttackMontageIndex = UKismetMathLibrary::RandomIntegerInRange(0, AutoAttackMontages.Num() - 1);
	OwnerCharacter->PlayAnimMontage(AutoAttackMontages[AttackMontageIndex], 0.8);

	//FSkillDamageEvent event;
	//event.HitData = 
	//Cast<ACharacter>(OwnerCharacter->GetTarget())->TakeDamage(10,)
}

void UCJobComponent::OnAutoAttack()
{
	// Ÿ���� ������
	if (IsValid(OwnerCharacter->GetTarget()->GetTargetActor()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OwnerCharacter->GetTarget()->GetTarget()->GetName());
		GetWorld()->GetTimerManager().SetTimer(AutoAttackTimerHandle, this, &UCJobComponent::DoingAutoAttack, AutoAttackCoolTime, true);
	}
}

// ��ų ���
void UCJobComponent::UseFirstSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(0);
}

void UCJobComponent::UseSecondSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(1);
}

///////////////////////////////////////////////////////////////////////////////////////

void UCJobComponent::SetWarrior()
{
	if (IsIdleMode() == false)
	{
		return;
	}

	ChangeJob(EJob::Warrior);
}

void UCJobComponent::SetDragoon()
{
	if (IsIdleMode() == false)
	{
		return;
	}

	ChangeJob(EJob::Dragoon);
}

bool UCJobComponent::IsIdleMode()
{
	return Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()))->IsIdleMode();
}
