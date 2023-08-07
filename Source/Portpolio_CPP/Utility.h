#pragma once

#include "CoreMinimal.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Character/CCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"

class PORTPOLIO_CPP_API Utility
{
public:

	static void PlayEffect(UWorld* InWorld, UFXSystemAsset* InAsset, const FTransform& InTransform, USkeletalMeshComponent* InMesh = nullptr, FName InSocketName = NAME_None)
	{
		UParticleSystem* particle = Cast<UParticleSystem>(InAsset);
		UNiagaraSystem* niagara = Cast<UNiagaraSystem>(InAsset);

		FVector location = InTransform.GetLocation();
		FRotator rotation = (FRotator)InTransform.GetRotation();
		FVector scale = InTransform.GetScale3D();
		//FVector scale = InScale;

		if (!!InMesh)
		{
			if (!!particle)
			{
				UGameplayStatics::SpawnEmitterAttached(particle, InMesh, InSocketName, location, rotation, scale);
				return;
			}

			if (!!niagara)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, InMesh, InSocketName, location, rotation, scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);
				return;
			}
		}

		if (!!particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, InTransform);
			return;
		}

		if (!!niagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, location, rotation, scale);
			return;
		}
	}

	static ACCharacterBase* GetNearlyFrontAngle(ACCharacterBase* InCharacter, TArray<ACCharacterBase*> InArray)
	{
		float Angle = 0.0f;
		ACCharacterBase* Candidate = nullptr;

		for(ACCharacterBase* character : InArray)
		{
			if(IsValid(character))
			{
				FVector direct = (character->GetActorLocation() - InCharacter->GetActorLocation()).GetSafeNormal2D();
				FVector forward = InCharacter->GetActorForwardVector();

				if(UKismetMathLibrary::Dot_VectorVector(direct, forward) >= Angle)
				{
					Angle = UKismetMathLibrary::Dot_VectorVector(direct, forward);
					Candidate = character;
				}
			}
		}

		return Candidate;
	}
};
