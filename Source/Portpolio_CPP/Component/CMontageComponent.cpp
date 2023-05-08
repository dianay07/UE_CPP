#include "Character/CCharacterBase.h"
#include "Component/CMontageComponent.h"

UCMontageComponent::UCMontageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	if(DataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable is not available");

		return;
	}

	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	for(int i = 0; i < (int32)EStateType::Max; i++)
	{
		for(FMontageData* data : datas)
		{
			if((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}
	}

#if LOG_UCMontagesComponent
	for (FMontagesData* data : datas)
	{
		if (data == nullptr)
			continue;

		FString str;
		str.Append(StaticEnum<EStateType>()->GetValueAsString(data->Type));
		str.Append(" / ");
		str.Append(data->Montage->GetPathName());

		CLog::Log(str);
	}
#endif
}

void UCMontageComponent::PlayEvadeMode()
{
	PlayAnimMontage(EStateType::Evade);
}

void UCMontageComponent::PlayAnimMontage(EStateType InType)
{
	if (OwnerCharacter == nullptr)
		return;

	FMontageData* data = Datas[(int32)InType];

	if(data == nullptr || data->Montgae == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None montages data");

		return;
	}

	OwnerCharacter->PlayAnimMontage(data->Montgae, data->PlayRate);
}

