// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CKeySettingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTPOLIO_CPP_API UCKeySettingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCKeySettingComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetBasicSetting();												// �⺻ ����
	void ChangeActionKeySetting(FName ActionName, const FKey Key);		// ���� ����

private:
	// �ʿ� ���� ��?
	UPROPERTY(VisibleAnywhere)
		TMap<FKey, FName> KeyBindings;
};
