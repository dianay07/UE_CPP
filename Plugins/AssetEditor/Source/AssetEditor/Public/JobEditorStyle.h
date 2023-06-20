#pragma once

#include "CoreMinimal.h"

class ASSETEDITOR_API FJobEditorStyle
{
public:
	static TSharedRef<FJobEditorStyle> Get();
	static void Shutdown();

private:
	static TSharedPtr<FJobEditorStyle> Instance;

public:
	FJobEditorStyle();
	~FJobEditorStyle();

private:
	void RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon);

private:
	static const FName StyleSetName;

private:
	TSharedPtr<class FSlateStyleSet> StyleSet;

public:
	FSlateIcon ToolBar_Icon;

	TSharedPtr<struct FSlateImageBrush> Array_Image;

public:
	const FVector2D DesiredWidth = FVector2D(250, 1000);
};
