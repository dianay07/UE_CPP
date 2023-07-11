#pragma once

#include "CoreMinimal.h"
#include "Item/CSkillBase.h"
#include "CActiveSkill_Global.generated.h"

UCLASS()
class PORTPOLIO_CPP_API UCActiveSkill_Global : public UCSkillBase
{
	GENERATED_BODY()

private:
	static float GlobalCoolTime;

public:
	virtual void ActiveSkill(int InIndex) override;

	float GetGlobalCoolTime() { return GlobalCoolTime; }
};
