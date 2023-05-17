#pragma once

#include "CoreMinimal.h"
#include "Item/CActiveSkill.h"
#include "CActiveSkill_Global.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCActiveSkill_Global : public UCActiveSkill
{
	GENERATED_BODY()

private:
	static float GlobalCoolTime;

public:
	virtual void ActiveSkill(int InIndex) override;

	float GetGlobalCoolTime() { return GlobalCoolTime; }
};
