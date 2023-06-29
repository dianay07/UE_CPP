#include "Job/CJobDataAsset.h"

#include "Character/CCharacterBase.h"
#include "GameFramework/Character.h"
#include "Item/CActiveSkill.h"
#include "Item/CAttachment.h"
#include "Item/CEquipment.h"

UCJobDataAsset::UCJobDataAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
	ActiveSkillClass = UCActiveSkill::StaticClass();
}

void UCJobDataAsset::BeginPlay(ACCharacterBase* InOwner)
{
	ACharacter* Owner = Cast<ACharacter>(InOwner);

	if (!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = Owner;

		Attachment = Owner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	if (!!EquipmentClass)
	{
		Equipment = NewObject<UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay(InOwner, EquipData);

		if (!!Attachment)
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}

	if (!!ActiveSkillClass)
	{
		ActiveSkill = NewObject<UCActiveSkill>(this, ActiveSkillClass);
		ActiveSkill->BeginPlay(Attachment, Equipment, InOwner, SkillDatas, HitDatas);
	}

	for(FSkillData data : SkillDatas)
	{
		// TODO : 글쿨 스킬이면 글쿨 따라가게, 논글쿨이면 정해진 스킬쿨 되게
		if (data.Type == ESkillCoolType::Global)
			data.SkillCooltime = 2.14f;
	}
}

int32 UCJobDataAsset::GetSkillDataCount()
{
	UE_LOG(LogTemp, Display, TEXT("%d"), SkillDatas.Num());
	return SkillDatas.Num();
}

#if WITH_EDITOR
void UCJobDataAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (FApp::IsGame() == true)
		return;

	bool bRefresh = false;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("SkillDatas") == 0;

	if(bRefresh)
	{
		bool bCheck = false;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate;

		if(bCheck)
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			TSharedPtr<IDetailsView> detailsView = prop.FindDetailView("AssetEditorDetailsView");

			if (detailsView.IsValid())
				detailsView->ForceRefresh();
		}
	}
}
#endif

