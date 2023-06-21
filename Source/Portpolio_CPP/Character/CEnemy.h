#pragma once

#include "CoreMinimal.h"
#include "Character/CCharacterBase.h"
#include "CEnemy.generated.h"

UCLASS()
class PORTPOLIO_CPP_API ACEnemy : public ACCharacterBase
{
	GENERATED_BODY()

public:

	// TODO :: 타겟임을 알리는 Widget 스프라이트, Billboard 컴포넌트로 변경 필요
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
