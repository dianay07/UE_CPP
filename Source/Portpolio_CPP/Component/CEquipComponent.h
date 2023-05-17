#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CEquipComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCEquipComponent();

protected:
	virtual void BeginPlay() override;

private:
	bool IsIdleMode();

public:
	void CheckEquipType();

private:
	class ACPlayer* OwnerCharacter;
};
