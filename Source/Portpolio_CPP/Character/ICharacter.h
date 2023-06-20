#pragma once

#include "CoreMinimal.h"
#include "Job/CJobStructure.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(Blueprintable, MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class PORTPOLIO_CPP_API IICharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = "GamePlay")
		virtual void ToggleTarget(class UCTargetComponent* InTargetComp, UUserWidget* InWidget);

	UFUNCTION(Category = "GamePlay")
		virtual void Damage(ACharacter* InAttacker, TArray<ACharacter*> InDamagedObjs, FHitData InHitData);

};
