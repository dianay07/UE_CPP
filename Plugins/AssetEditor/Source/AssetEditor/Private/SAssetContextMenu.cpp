#include "SAssetContextMenu.h"

#include "SAssetEditorWindow.h"
#include "Job/CJobDataAsset.h"

FAssetContextMenu::FAssetContextMenu(EAssetTypeCategories::Type InCategory)
{
	Category = InCategory;
}

FText FAssetContextMenu::GetName() const
{
	return FText::FromString("Create Job Asset");
}

UClass* FAssetContextMenu::GetSupportedClass() const
{
	return UCJobDataAsset::StaticClass();
}

FColor FAssetContextMenu::GetTypeColor() const
{
	return FColor::Red;
}

uint32 FAssetContextMenu::GetCategories()
{
	return Category;
}

void FAssetContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	if (InObjects.Num() < 1)
		return;

	FAssetEditorWindow::OpenWindow();
}
