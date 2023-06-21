#include "Component/CTargetComponent.h"

#include "Camera/CameraComponent.h"
#include "Character/CCharacterBase.h"
#include "Character/CEnemy.h"
#include "Character/CPlayer.h"
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

		if(IsValid(State))
		{

			if (State->IsDeadMode() == true)
			{
				ControlCursor(Target);
				Targets.Remove(Target);
			}
			else
			{
				if (OwnerCharacter->GetDistanceTo(Target) <= TraceDistance)
					TickTargeting();
				else
				{
					ControlCursor(Target);
					Target = nullptr;
				}
			}
		}
	}
}

// Ÿ���� �� ĳ���� ��ȯ
ACCharacterBase* UCTargetComponent::GetTarget()
{
	return Target;
}

// TabŰ�� Ÿ�� ��
void UCTargetComponent::ToggleTarget()
{
	// TabŰ�� ������ Ÿ�� ��ǥ�� ���� ����
	// ó�� ������ Ÿ�ٵ� �迭�� �ƹ��͵� �����ϱ� ó��
	if(Targets.Num() < 1)
		Begin_Target();
	else
	{
		TargetIndex++;

		if (TargetIndex >= Targets.Num())
			TargetIndex = 0;

		Begin_Target(Targets[TargetIndex]);
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
	// ���콺 Ŭ�� �ƴ� TabŰ�� Ÿ�ٱ�� ���� ��
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
	}
	else	// ���콺 Ŭ������ Ÿ���� ���� ���� ��
	{
		ChangeTarget(InTarget);
	}
}

// Ÿ���� ����
void UCTargetComponent::End_Target()
{
	ControlCursor(Target);
	Targets.Remove(Target);
}

// Control Cursor = Ŀ�� On / Off
void UCTargetComponent::ControlCursor(ACCharacterBase* InTarget)
{
	ACEnemy* enemy = Cast<ACEnemy>(InTarget);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

	if(OnControlWidget.IsBound() == false)
	{
		OnControlWidget.AddUObject(enemy, &ACEnemy::SetWidgetVisble);
		OnControlWidget.AddUObject(player, &ACPlayer::OffTargetInfo);
	}
	OnControlWidget.Broadcast();
	OnControlWidget.Clear();
}

// Ÿ�� ����
void UCTargetComponent::ChangeTarget(ACCharacterBase* InCandidate)
{
	// Ȥ�� ������ Ÿ���� ���ٸ�
	if(IsValid(InCandidate) == false)
		End_Target();

	if(Target != nullptr)
		ControlCursor(Target);

	// ���õ� ĳ���� Ÿ������ ����
	Target = InCandidate;
	ControlCursor(Target);
}

void UCTargetComponent::TickTargeting()
{
	FVector WidgetLocation = GetTarget()->GetActorLocation();
	//FVector PlayerLocation = GetOwner()->GetActorLocation();
	ACPlayer* player = Cast<ACPlayer>(GetOwner());
	UCameraComponent* camera = Cast<UCameraComponent>(player->GetComponentByClass(UCameraComponent::StaticClass()));

	FVector PlayerLocation = camera->GetComponentLocation();

	FRotator Direction = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, PlayerLocation);

	Cast<ACEnemy>(Target)->CursorWidget->SetRelativeRotation(Direction + FRotator(0,90,0));
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