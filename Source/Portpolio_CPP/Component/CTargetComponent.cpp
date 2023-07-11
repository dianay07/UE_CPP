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
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
}

// 거리 밖으로 나갈시 타겟팅 취소
void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IsValid(Target))
	{
		UCStateComponent* TargetState = Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));

		if(IsValid(TargetState))
		{

			if (TargetState->IsDeadMode() == true)
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

// Tab키로 타겟 온
void UCTargetComponent::ToggleTarget()
{
	// Tab키로 들어오면 타겟 목표가 없이 들어옴
	// 처음 들어오면 타겟들 배열에 아무것도 없으니까 처리
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

// 타겟 시작 or 변경
void UCTargetComponent::ToggleTarget(class ACCharacterBase* InTarget)
{
	Begin_Target(InTarget);
}

// 타켓팅 할때마다 주변 타켓가능 객체들 체크
void UCTargetComponent::Begin_Target(ACCharacterBase* InTarget)
{
	// 마우스 클릭 아닌 Tab키로 타겟기능 켯을 때
	if(IsValid(InTarget) == false)
	{
		// 물체 판정용 파라미터
		FVector start = OwnerCharacter->GetActorLocation();		// 시작 지점
		FVector end = start;									// 끝 지점
		TArray<AActor*> param;									// 무시대상
		param.Add(OwnerCharacter);

		TArray<FHitResult> Output;								// 결과물 반환받을 배열

		UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceDistance, "Targeting", false, param, EDrawDebugTrace::None, Output, true);

		for (FHitResult hitResult : Output)
		{
			if (hitResult.Actor->GetClass() != OwnerCharacter->GetClass() && Cast<ACCharacterBase>(hitResult.Actor))
			{
				Targets.AddUnique(Cast<ACCharacterBase>(hitResult.Actor));
			}
		}

		// 시야각 기준 타겟 설정
		//ChangeTarget(Utility::GetNearlyFrontAngle(OwnerCharacter, Targets));

		// 번호 기준 타겟 설정
		ChangeTarget(Targets[TargetIndex]);
	}
	else	// 마우스 클릭으로 타겟이 지정 됫을 때
	{
		ChangeTarget(InTarget);
	}
}

// 타켓팅 종료
void UCTargetComponent::End_Target()
{
	ControlCursor(Target);
	Targets.Remove(Target);
}

// 타겟 변경
void UCTargetComponent::ChangeTarget(ACCharacterBase* InCandidate)
{
	// 혹시 지정된 타겟이 없다면
	if (IsValid(InCandidate) == false)
		End_Target();

	if (Target != nullptr)
		ControlCursor(Target);

	// 선택된 캐릭터 타겟으로 지정
	Target = InCandidate;
	ControlCursor(Target);
}

// Control Cursor = 커서 On / Off
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

void UCTargetComponent::TickTargeting()
{
	ACPlayer* player = Cast<ACPlayer>(GetOwner());

	// 타겟 위젯 방향 조절
	{
		FVector WidgetLocation = GetTargetActor()->GetActorLocation();
		FVector CameraLocation = Cast<UCameraComponent>(player->GetComponentByClass(UCameraComponent::StaticClass()))->GetComponentLocation();
		FRotator Direction = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);

		//UE_LOG(LogTemp, Warning, TEXT("%p, %p, %p"), &Direction.Pitch, &Direction.Yaw, &Direction.Roll);
		Cast<ACEnemy>(Target)->CursorWidget->SetRelativeRotation(Direction + FRotator(0, 90, 0));
	}

	// 타겟팅 시 캐릭터 방향 조절
	/*{
		FRotator ControlRotation = player->GetActorRotation();
		FRotator OwnerToTarget = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), Target->GetActorLocation());

		if (UKismetMathLibrary::EqualEqual_RotatorRotator(ControlRotation, OwnerToTarget, FinishAngle))
		{
			FRotator direction = UKismetMathLibrary::MakeRotator(ControlRotation.Roll, ControlRotation.Pitch, OwnerToTarget.Yaw);
			FRotator ret = UKismetMathLibrary::RInterpTo(OwnerCharacter->GetActorRotation(), direction, GetWorld()->GetDeltaSeconds(), InterpSpeed);

			OwnerCharacter->GetController()->SetControlRotation(ret);
		}
	}*/
}

bool UCTargetComponent::IsTargetsArrayEmpty()
{
	if (Targets.Num() == 0)
		return true;
	else
		return false;
}