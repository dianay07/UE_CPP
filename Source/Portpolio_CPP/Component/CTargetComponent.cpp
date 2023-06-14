#include "Component/CTargetComponent.h"

#include "Character/CCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/CEnemy.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
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

// 거리 밖으로 나갈시 타겟팅 취소
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

// 타겟 리턴
ACEnemy* UCTargetComponent::GetTarget()
{
	ACEnemy* ret = Cast<ACEnemy>(Target);

	return ret;
}

// 타겟 시작 or 변경
void UCTargetComponent::ToggleTarget()
{
	if (IsValid(Target))
	{
		GetTarget()->CursorWidget->SetVisibility(false);

		if (TargetIndex >= Targets.Num() - 1)
			TargetIndex = 0;
		else
			TargetIndex++;

		Begin_Target();
	}
	else if(!IsValid(Target))
	{
		Begin_Target();
	}
		
}

// 타켓팅 할때마다 주변 타켓가능 객체들 체크
void UCTargetComponent::Begin_Target()
{
	// 물체 판정용 파라미터
	FVector start = OwnerCharacter->GetActorLocation();		// 시작 지점
	FVector end = start;									// 끝 지점
	TArray<AActor*> param;									// 무시대상
	param.Add(OwnerCharacter);

	TArray<FHitResult> Output;								// 결과물 반환받을 배열
	
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceDistance, "Targeting", false, param, EDrawDebugTrace::None, Output, true);

	for(FHitResult hitResult : Output)
	{
		if(hitResult.Actor->GetClass() != OwnerCharacter->GetClass() && Cast<ACCharacterBase>(hitResult.Actor))
		{
			Targets.AddUnique(Cast<ACCharacterBase>(hitResult.Actor));
		}
	}

	// 시야각 기준 타겟 설정
	//ChangeTarget(Utility::GetNearlyFrontAngle(OwnerCharacter, Targets));

	// 번호 기준 타겟 설정
	ChangeTarget(Targets[TargetIndex]);

	GetTarget()->CursorWidget->SetVisibility(true);
}

// 타켓팅 종료
void UCTargetComponent::End_Target()
{
	if (GetTarget()->CursorWidget->IsActive())
		GetTarget()->CursorWidget->SetVisibility(false);

	Target = nullptr;
}

// 타겟 변경
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

	// 시점 고정
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
