#include "Component/CTargetComponent.h"

#include "Character/CCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility.h"
#include "Particles/ParticleSystemComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
}

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

void UCTargetComponent::ToggleTarget()
{
	if (IsValid(Target))
		End_Target();
	else
		Begin_Target();
}

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
		if(hitResult.Actor->GetClass() != OwnerCharacter->GetClass())
		{
			Targets.AddUnique(Cast<ACCharacterBase>(hitResult.Actor));
		}
	}

	ChangeTarget(Utility::GetNearlyFrontAngle(OwnerCharacter, Targets));
}

void UCTargetComponent::End_Target()
{
	Target = nullptr;

	// particle ���� ����
	if (IsValid(Particle) == true)
		Particle->DestroyComponent();
}

void UCTargetComponent::ChangeTarget(ACCharacterBase* InChandidate)
{
	if(IsValid(InChandidate) == false)
		End_Target();

	if (IsValid(Particle) == true)
		Particle->DestroyComponent();

	Particle = UGameplayStatics::SpawnEmitterAttached(ParticleAsset, InChandidate->GetMesh(), "Root",
		FVector(0, 0, -10), FRotator::ZeroRotator, FVector(0.5f, 0.5f, 0.5f));

	Target = InChandidate;
}

void UCTargetComponent::TickTargeting()
{
	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	FRotator OwnerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());

	if(UKismetMathLibrary::EqualEqual_RotatorRotator(ControlRotation, OwnerToTarget, FinishAngle))
	{
		FRotator direction = UKismetMathLibrary::MakeRotator(OwnerToTarget.Roll, ControlRotation.Pitch, OwnerToTarget.Yaw);
		FRotator ret = UKismetMathLibrary::RInterpTo(ControlRotation, direction, GetWorld()->GetDeltaSeconds(), InterpSpeed);

		OwnerCharacter->GetController()->SetControlRotation(ret);
	}
	
}
