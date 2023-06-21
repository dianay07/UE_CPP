#pragma once

#include "CoreMinimal.h"
#include "Character/CCharacterBase.h"
#include "CEnemy.generated.h"

UCLASS()
class PORTPOLIO_CPP_API ACEnemy : public ACCharacterBase
{
	GENERATED_BODY()

public:

	// TODO :: Ÿ������ �˸��� Widget ��������Ʈ, Billboard ������Ʈ�� ���� �ʿ�
	UPROPERTY(VisibleAnywhere, Category = "Widgets")
		class UWidgetComponent* CursorWidget;

public:
	ACEnemy();

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	FORCEINLINE UWidgetComponent* GetWidget() { return CursorWidget; }

public:
	void SetWidgetVisble();

public:
	UPROPERTY(VisibleAnywhere)
		class UCEquipComponent* Equip;
};
