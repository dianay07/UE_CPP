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

// 타겟이 거리밖으로 나가거나 죽으면 타겟 취소
void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 타겟이 존재할때 기동
	if(IsValid(Target))
	{
		UCStateComponent* TargetState = Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));

		if (TargetState->IsDeadMode() == true)
		{
			ControlCursor(*Target);
			SearchedTargetArray.Remove(Target);
		}
		else   // 타겟된 액터가 아직 살아있으면
		{
			// 인지 범위보다 넓어지면
			if (OwnerCharacter->GetDistanceTo(Target) > TraceDistance)
			{
				ControlCursor(*Target);
				Target = nullptr;
			}
		}
	}
}

// Tab키로 타겟팅 시작 = 근처 아무거나 타겟물체로 입력
void UCTargetComponent::ToggleTarget()
{
	// 이미 검색된 타겟이 있으면 그 안에서 타겟 변경
	if(SearchedTargetArray.Num() > 0)
	{
		TargetIndex++;

		// 검색된 액터 수보다 많아지면 다시 처음 놈으로
		if (TargetIndex >= SearchedTargetArray.Num())
			TargetIndex = 0;

		Begin_Target(SearchedTargetArray[TargetIndex]);
	}
	else
	{
		Begin_Target();
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
	// 검색 범위 설정용 변수
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
			SearchedTargetArray.AddUnique(Cast<ACCharacterBase>(hitResult.Actor));
		}
	}

	// 마우스 클릭 아닌 Tab키로 타겟기능 켯을 때
	if(IsValid(InTarget) == false)
		ChangeTarget(SearchedTargetArray[TargetIndex]);
	else	// 마우스 클릭으로 타겟이 지정 됫을 때
		ChangeTarget(InTarget);
}

// 타켓팅 종료
void UCTargetComponent::FinishTargeting()
{
	ControlCursor(*Target);

	SearchedTargetArray.Remove(Target);

	State->SetIsBattle(false);
}

// 타겟 변경
void UCTargetComponent::ChangeTarget(ACCharacterBase* InCandidate)
{
	// 혹시 지정된 타겟이 없다면
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
		// 선택된 캐릭터 타겟으로 지정
		Target = InCandidate;
		ControlCursor(*Target);
	}
}

// Control Cursor = 커서 On / Off
void UCTargetComponent::ControlCursor(ACCharacterBase& InTarget)
{
	ACEnemy* enemy = Cast<ACEnemy>(&InTarget);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

	// 켜져있으면 끄고 꺼져있으면 키고
	//if(enemy->CursorBillboard->GetVisibleFlag() == false)
	//	enemy->CursorBillboard->SetVisibility(true);
	enemy->CursorBillboard->SetVisibility(!enemy->CursorBillboard->GetVisibleFlag());

	// 타겟 인포 위젯은 걍 끄고
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