#pragma once

#include "CoreMinimal.h"
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
	virtual void End_Hitted() {}
	virtual void End_Dead() {}

public:
	void CreateDynamicMaterial(class ACharacter* InCharacter);
	void ChangeColor(class ACharacter* InCharacter, FLinearColor InColor);
};
