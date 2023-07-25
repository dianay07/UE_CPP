#include "Component/CTargetComponent.h"

#include "Camera/CameraComponent.h"
#include "Character/CCharacterBase.h"
#include "Character/CEnemy.h"
#include "Character/CPlayer.h"
#include "Components/BillboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/CUI_TargetingCursor.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
}

// Ÿ���� �Ÿ������� �����ų� ������ Ÿ�� ���
void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Ÿ���� �����Ҷ� �⵿
	if(IsValid(Target))
	{
		UCStateComponent* TargetState = Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));

		if (TargetState->IsDeadMode() == true)
		{
			ControlCursor(*Target);
			SearchedTargetArray.Remove(Target);
		}
		else   // Ÿ�ٵ� ���Ͱ� ���� ���������
		{
			// ���� �������� �о�����
			if (OwnerCharacter->GetDistanceTo(Target) > TraceDistance)
			{
				ControlCursor(*Target);
				Target = nullptr;
			}
		}
	}
}

// TabŰ�� Ÿ���� ���� = ��ó �ƹ��ų� Ÿ�ٹ�ü�� �Է�
void UCTargetComponent::ToggleTarget()
{
	// �̹� �˻��� Ÿ���� ������ �� �ȿ��� Ÿ�� ����
	if(SearchedTargetArray.Num() > 0)
	{
		TargetIndex++;

		// �˻��� ���� ������ �������� �ٽ� ó�� ������
		if (TargetIndex >= SearchedTargetArray.Num())
			TargetIndex = 0;

		Begin_Target(SearchedTargetArray[TargetIndex]);
	}
	else
	{
		Begin_Target();
	}
}

// Ÿ�� ���� or ����
void UCTargetComponent::ToggleTarget(class ACCharacterBase* InTarget)
{
	Begin_Target(InTarget);
}

// Ÿ���� �Ҷ����� �ֺ� Ÿ�ϰ��� ��ü�� üũ
void UCTargetComponent::Begin_Target(ACCharacterBase* InTarget)
{
	// �˻� ���� ������ ����
	FVector start = OwnerCharacter->GetActorLocation();		// ���� ����
	FVector end = start;									// �� ����
	TArray<AActor*> param;									// ���ô��
	param.Add(OwnerCharacter);

	TArray<FHitResult> Output;								// ����� ��ȯ���� �迭

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceDistance, "Targeting", false, param, EDrawDebugTrace::None, Output, true);
	for (FHitResult hitResult : Output)
	{
		if (hitResult.Actor->GetClass() != OwnerCharacter->GetClass() && Cast<ACCharacterBase>(hitResult.Actor))
		{
			SearchedTargetArray.AddUnique(Cast<ACCharacterBase>(hitResult.Actor));
		}
	}

	// ���콺 Ŭ�� �ƴ� TabŰ�� Ÿ�ٱ�� ���� ��
	if(IsValid(InTarget) == false)
		ChangeTarget(SearchedTargetArray[TargetIndex]);
	else	// ���콺 Ŭ������ Ÿ���� ���� ���� ��
		ChangeTarget(InTarget);
}

// Ÿ���� ����
void UCTargetComponent::FinishTargeting()
{
	ControlCursor(*Target);

	SearchedTargetArray.Remove(Target);

	State->SetIsBattle(false);
}

// Ÿ�� ����
void UCTargetComponent::ChangeTarget(ACCharacterBase* InCandidate)
{
	// Ȥ�� ������ Ÿ���� ���ٸ�
	if (IsValid(InCandidate) == false)
		FinishTargeting();

	if (Target != nullptr)
	{
		ControlCursor(*Target);
		Target = InCandidate;
		ControlCursor(*Target);
	}
	else
	{
		// ���õ� ĳ���� Ÿ������ ����
		Target = InCandidate;
		ControlCursor(*Target);
	}
}

// Control Cursor = Ŀ�� On / Off
void UCTargetComponent::ControlCursor(ACCharacterBase& InTarget)
{
	ACEnemy* enemy = Cast<ACEnemy>(&InTarget);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

	// ���������� ���� ���������� Ű��
	//if(enemy->CursorBillboard->GetVisibleFlag() == false)
	//	enemy->CursorBillboard->SetVisibility(true);
	enemy->CursorBillboard->SetVisibility(!enemy->CursorBillboard->GetVisibleFlag());

	// Ÿ�� ���� ������ �� ����
	if(OnControlWidget.IsBound() == false)
		OnControlWidget.AddUObject(player, &ACPlayer::OffTargetInfo);

	OnControlWidget.Broadcast();
	OnControlWidget.Clear();
}

bool UCTargetComponent::IsTargetsArrayEmpty()
{
	if (SearchedTargetArray.Num() == 0)
		return true;
	else
		return false;
}