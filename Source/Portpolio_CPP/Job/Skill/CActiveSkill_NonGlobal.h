#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CActiveSkill_NonGlobal.generated.h"

UCLASS(Abstract)
class PORTPOLIO_CPP_API UCActiveSkill_NonGlobal : public UObject
{
	GENERATED_BODY()

public:
	UCActiveSkill_NonGlobal();

public:
	virtual void BeginPlay(class ACCharacterBase* InOwner, class ACAttachment* InAttachment);

public:
	virtual void Pressed();
	virtual void Released();

public:
	UFUNCTION(BlueprintNativeEvent)
		void Begin_SubAction();
	virtual void Begin_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void End_SubAction();
	virtual void End_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {}

protected:
	bool bInAction;

	class ACCharacterBase* Owner;
	class ACAttachment* Attachment;
	class UCSkillBase* Skill;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;
	
};
