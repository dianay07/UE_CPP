#include "Component/CJobComponent.h"

#include "CStatusComponent.h"
#include "CTargetComponent.h"
#include "Utility.h"
#include "Character/CCharacterBase.h"
#include "Character/CPlayer.h"
#include "Components/UniformGridPanel.h"
#include "Item/CEquipment.h"
#include "Item/CSkillBase.h"
#include "Job/CJobDataAsset.h"
#include "Job/Skill/CActiveSkill_NonGlobal.h"
#include "UI/CHUDLayout.h"
#include "UI/CUI_QuickSlots.h"
#include "UI/CUI_Slot.h"

UCJobComponent::UCJobComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCJobComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
	for (int i = 0; i < 2; i++)
		if (DataAssets[i] != nullptr)
			DataAssets[i]->BeginPlay(OwnerCharacter);

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

	if (!!GetNonGlobal())
		GetNonGlobal()->Tick(DeltaTime);
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

UCActiveSkill_NonGlobal* UCJobComponent::GetNonGlobal()
{
	if (State->IsInBattle() == false) return nullptr;

	return DataAssets[static_cast<int32>(JobName)]->GetNonGlobal();
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
	if (State->IsInBattle())
		return;

	if (Cast<ACPlayer>(OwnerCharacter))
		QuickSlots = Cast<ACPlayer>(OwnerCharacter)->GetLayout()->Slots;

	// �ش� ������ ������ ���� ����
	if (DataAssets[static_cast<int32>(JobName)] != nullptr)
	{
		DataAssets[static_cast<int32>(JobName)]->DestroyAttachmentWeapon();

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
		OnJobChanged.Broadcast(prevJob, InCurrentJob);

		DataAssets[static_cast<int32>(JobName)]->SpawnAttachmentWeapon(OwnerCharacter);

		Utility::PlayEffect(OwnerCharacter->GetWorld(), ParticleAsset, 
			FTransform(OwnerCharacter->GetActorLocation() + FVector(0,0,-90)));
	}
}

void UCJobComponent::PlayEquipMotion()
{
	DataAssets[(int32)JobName]->GetEquipment()->Equip();
	State->SetIsBattle(true);
}

void UCJobComponent::PlayUnequipMotion()
{
	DataAssets[(int32)JobName]->GetEquipment()->Unequip();
	State->SetIsBattle(false);
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

// ���� ���
// ������ = 0 �̸� ù���� ���´�~
void UCJobComponent::UseFirstSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(Cast<UCUI_Slot>(QuickSlots->Slots->GetChildAt(0))->SkillIndex);
}

void UCJobComponent::UseSecondSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(Cast<UCUI_Slot>(QuickSlots->Slots->GetChildAt(1))->SkillIndex);
}

void UCJobComponent::UseThirdSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(Cast<UCUI_Slot>(QuickSlots->Slots->GetChildAt(2))->SkillIndex);
}

void UCJobComponent::UseFourthSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(Cast<UCUI_Slot>(QuickSlots->Slots->GetChildAt(3))->SkillIndex);
}

void UCJobComponent::UseFifthSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(Cast<UCUI_Slot>(QuickSlots->Slots->GetChildAt(4))->SkillIndex);
}

void UCJobComponent::UseSixthSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(Cast<UCUI_Slot>(QuickSlots->Slots->GetChildAt(5))->SkillIndex);
}

void UCJobComponent::UseSeventhSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(Cast<UCUI_Slot>(QuickSlots->Slots->GetChildAt(6))->SkillIndex);
}

void UCJobComponent::UseEighthSlot()
{
	if (OnSkillActivate.IsBound())
		OnSkillActivate.Broadcast(Cast<UCUI_Slot>(QuickSlots->Slots->GetChildAt(7))->SkillIndex);
}

void UCJobComponent::UseNonGlobal_Pressed()
{
	if (!!GetNonGlobal())
		GetNonGlobal()->Pressed();
}

void UCJobComponent::UseNonGlobal_Released()
{
	if (!!GetNonGlobal())
		GetNonGlobal()->Released();
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
