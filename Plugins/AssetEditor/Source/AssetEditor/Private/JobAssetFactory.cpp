#include "JobAssetFactory.h"

#include "Job/CJobAsset.h"

UJobAssetFactory::UJobAssetFactory()
{
	bCreateNew = true;

	SupportedClass = UCJobAsset::StaticClass();
}

UObject* UJobAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                            UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UCJobAsset>(InParent, InClass, InName, Flags);
}
