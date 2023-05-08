#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontageComponent.generated.h"

USTRUCT()
struct FMontageData
	: public FTableRowBase
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montgae;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCMontageComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* DataTable;

public:	
	UCMontageComponent();

protected:
	virtual void BeginPlay() override;

public:
	void PlayEvadeMode();

private:
	void PlayAnimMontage(EStateType InType);

private:
	class ACCharacterBase* OwnerCharacter;
	FMontageData* Datas[(int32)EStateType::Max];
};
