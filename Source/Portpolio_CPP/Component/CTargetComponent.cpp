#include "Component/CTargetComponent.h"

#include "Character/CCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility.h"
#include "Character/CEnemy.h"
#include "UI/CUI_TargetingCursor.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	/*static ConstructorHelpers::FClassFinder<UCUI_TargetingCursor> targetCursor(TEXT("WidgetBlueprint'/Game/07_UI/BP_CUI_TargetingCursor.BP_CUI_TargetingCursor_C'"));
	if (targetCursor.Succeeded())
	{
		TargetingCursorClass = targetCursor.Class;
	}*/
}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());

	/*if(IsValid(TargetingCursorClass))
	{
		TargetingCursor = Cast<UCUI_TargetingCursor>(CreateWidget(GetWorld(), TargetingCursorClass));

		if(IsValid(TargetingCursor))
		{
			TargetingCursor->AddToViewport();
		}
	}*/
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

// Ÿ�� ���� or ����
void UCTargetComponent::ToggleTarget()
{
	if (IsValid(Target))
	{
		Cast<ACEnemy>(Target)->DeactiveTargetCursor();

		if (TargetIndex >= Targets.Num() - 1)
			TargetIndex = 0;
		else
			TargetIndex++;

		Begin_Target();
	}
	else if(!IsValid(Target))
	{
		if (Targets.Num() <= 0)
			End_Target();
		else
			Begin_Target();
	}
		
}

// Ÿ���� �Ҷ����� �ֺ� Ÿ�ϰ��� ��ü�� üũ
void UCTargetComponent::Begin_Target()
{
	// ��ü ������ �Ķ����
	FVector start = OwnerCharacter->GetActorLocation();		// ���� ����
	FVector end = start;									// �� ����
	TArray<AActor*> param;									// ���ô��
	param.Add(OwnerCharacter);

	TArray<FHitResult> Output;								// ����� ��ȯ���� �迭
	
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceDistance, "Targeting", false, param, EDrawDebugTrace::None, Output, true);

	for(FHitResult hitResult : Output)
	{
		if(hitResult.Actor->GetClass() != OwnerCharacter->GetClass() && Cast<ACCharacterBase>(hitResult.Actor))
		{
			Targets.AddUnique(Cast<ACCharacterBase>(hitResult.Actor));
		}
	}

	// �þ߰� ���� Ÿ�� ����
	//ChangeTarget(Utility::GetNearlyFrontAngle(OwnerCharacter, Targets));

	// ��ȣ ���� Ÿ�� ����
	ChangeTarget(Targets[TargetIndex]);
}

// Ÿ���� ����
void UCTargetComponent::End_Target()
{
	GetTarget()->DeactiveTargetCursor();
	Target = nullptr;
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
	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	FRotator OwnerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());

	// ���� ����
	/*if(UKismetMathLibrary::EqualEqual_RotatorRotator(ControlRotation, OwnerToTarget, FinishAngle))
	{
		FRotator direction = UKismetMathLibrary::MakeRotator(OwnerToTarget.Roll, ControlRotation.Pitch, OwnerToTarget.Yaw);
		FRotator ret = UKismetMathLibrary::RInterpTo(ControlRotation, direction, GetWorld()->GetDeltaSeconds(), InterpSpeed);

		OwnerCharacter->GetController()->SetControlRotation(ret);
	}*/
}
