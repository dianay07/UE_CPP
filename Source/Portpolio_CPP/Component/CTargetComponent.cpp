#include "Component/CTargetComponent.h"

#include "Character/CCharacterBase.h"
#include "Character/CEnemy.h"
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
}

// �Ÿ� ������ ������ Ÿ���� ���
void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IsValid(Target))
	{
		UCStateComponent* State = Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));

		if(GetTarget()->CursorWidget->IsActive() == true)
		{
			TickTargeting();
		}

		if(IsValid(State))
		{
			if(State->IsDeadMode() == false)
			{
				if (OwnerCharacter->GetDistanceTo(Target) <= TraceDistance)
					TickTargeting();
				else
					End_Target();
			}
		}
	}

}

// Ÿ�� ����
ACEnemy* UCTargetComponent::GetTarget()
{
	ACEnemy* ret = Cast<ACEnemy>(Target);

	return ret;
}

// TabŰ�� Ÿ�� ��
void UCTargetComponent::ToggleTarget()
{
	if (IsValid(Target))
	{
		GetTarget()->CursorWidget->SetVisibility(false);

		if (TargetIndex >= Targets.Num() - 1)
			TargetIndex = 0;
		else
			TargetIndex++;

		Begin_Target(Target);
	}
	else if(!IsValid(Target))
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
	if(IsValid(InTarget) == false)
	{
		// ��ü ������ �Ķ����
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
				Targets.AddUnique(Cast<ACCharacterBase>(hitResult.Actor));
			}
		}

		// �þ߰� ���� Ÿ�� ����
		//ChangeTarget(Utility::GetNearlyFrontAngle(OwnerCharacter, Targets));

		// ��ȣ ���� Ÿ�� ����
		ChangeTarget(Targets[TargetIndex]);

		GetTarget()->CursorWidget->SetVisibility(true);
	}
	else
	{
		ChangeTarget(InTarget);
	}

	ControlCursor(Target);
}

// Ÿ���� ����
void UCTargetComponent::End_Target()
{
	if (GetTarget()->CursorWidget->IsActive())
		GetTarget()->CursorWidget->SetVisibility(false);

	Target = nullptr;
}

// Control Cursor
void UCTargetComponent::ControlCursor(ACCharacterBase* InTarget)
{
	ACEnemy* enemy = Cast<ACEnemy>(InTarget);

	if(enemy->CursorWidget->IsWidgetVisible() == false)
	{
		enemy->CursorWidget->SetVisibility(true);
	}
	else
	{
		enemy->CursorWidget->SetVisibility(false);
	}
}

// Ÿ�� ����
void UCTargetComponent::ChangeTarget(ACCharacterBase* InCandidate)
{
	if(IsValid(InCandidate) == false)
		End_Target();

	Target = InCandidate;
}

void UCTargetComponent::TickTargeting()
{
	FVector WidgetLocation = GetTarget()->GetActorLocation();
	FVector PlayerLocation = GetOwner()->GetActorLocation();
	FRotator Direction = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, PlayerLocation);

	GetTarget()->CursorWidget->SetRelativeRotation(Direction + FRotator(0,90,0));
	//UE_LOG(LogTemp, Warning, TEXT("%p, %p, %p"), &Direction.Pitch, &Direction.Yaw, &Direction.Roll);

	// ���� ����
	/*if(UKismetMathLibrary::EqualEqual_RotatorRotator(ControlRotation, OwnerToTarget, FinishAngle))
	{
		FRotator direction = UKismetMathLibrary::MakeRotator(OwnerToTarget.Roll, ControlRotation.Pitch, OwnerToTarget.Yaw);
		FRotator ret = UKismetMathLibrary::RInterpTo(ControlRotation, direction, GetWorld()->GetDeltaSeconds(), InterpSpeed);

		OwnerCharacter->GetController()->SetControlRotation(ret);
	}*/
}

bool UCTargetComponent::IsTargetsArrayEmpty()
{
	if (Targets.Num() == 0)
		return true;
	else
		return false;
}