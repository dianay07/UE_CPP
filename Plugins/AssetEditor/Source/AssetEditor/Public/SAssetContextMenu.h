#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "AssetTypeCategories.h"

class ASSETEDITOR_API FAssetContextMenu
	: public FAssetTypeActions_Base
{
public:
	FAssetContextMenu(EAssetTypeCategories::Type InCategory);

public:
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;

private:
	EAssetTypeCategories::Type Category;
};
