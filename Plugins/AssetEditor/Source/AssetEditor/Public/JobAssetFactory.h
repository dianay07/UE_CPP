#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "JobAssetFactory.generated.h"

UCLASS()
class ASSETEDITOR_API UJobAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UJobAssetFactory();

	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext);

};
