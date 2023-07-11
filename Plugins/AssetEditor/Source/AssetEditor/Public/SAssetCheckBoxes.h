#pragma once

#include "CoreMinimal.h"
#include "Job/CJobStructure.h"

class ASSETEDITOR_API SAssetCheckBoxes
	: public TSharedFromThis<SAssetCheckBoxes>
{
public:
	// IPropertyHandle = 하나의 프로퍼티를 관리하는 핸들
	void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

	TSharedRef<SWidget> Draw(bool bBackGround = false);
	void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildrenBuilder);

	void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);

private:
	void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);

public:
	static bool CanDraw(TSharedPtr<IPropertyHandle> InHandle, int InCount);

public:
	void CheckDefaultObject(int32 InIndex, UObject* InValue);
	void CheckDefaultValue(int32 InIndex, int InValue);
	void CheckDefaultValue(int32 InIndex, float InValue);
	void CheckDefaultValue(int32 InIndex, bool InValue);
	void CheckDefaultValue(int32 InIndex, const FVector& InValue);
	void CheckDefaultValue(int32 InIndex, const FText& InValue);
	void CheckDefaultValue(int32 InIndex,  uint8 InValue);

private:
	struct FInternalData
	{
		bool bChecked;
		FString Name;
		TSharedPtr<IPropertyHandle> Handle;

		FInternalData(TSharedPtr<IPropertyHandle> InHandle)
		{
			bChecked = false;
			Handle = InHandle;

			Name = Handle->GetPropertyDisplayName().ToString();
		}
	};
	TArray<FInternalData> InternalDatas;
	TSharedPtr<class IPropertyUtilities> Utilities;
};
