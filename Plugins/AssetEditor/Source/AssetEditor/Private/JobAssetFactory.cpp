#include "JobAssetFactory.h"

#include "Job/CJobDataAsset.h"

UJobAssetFactory::UJobAssetFactory()
{
	bCreateNew = true;

	SupportedClass = UCJobDataAsset::StaticClass();
}

UObject* UJobAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                            UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UCJobDataAsset>(InParent, InClass, InName, Flags);
}
