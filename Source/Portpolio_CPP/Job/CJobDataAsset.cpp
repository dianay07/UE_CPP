#include "Job/CJobDataAsset.h"

#include "Character/CCharacterBase.h"
#include "GameFramework/Character.h"
#include "Item/CSkillBase.h"
#include "Item/CAttachment.h"
#include "Item/CEquipment.h"

UCJobDataAsset::UCJobDataAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
	ActiveSkillClass = UCSkillBase::StaticClass();
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
		Equipment->BeginPlay(InOwner, DrawWeaponData);

		if (!!Attachment)
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}

	if (!!ActiveSkillClass)
	{
		ActiveSkill = NewObject<UCSkillBase>(this, ActiveSkillClass);
		ActiveSkill->BeginPlay(Attachment, Equipment, InOwner, SkillDatas, HitDatas);

		if(!!Attachment)
		{
			Attachment->OnAttachmentBeginCollision.AddDynamic(ActiveSkill, &UCSkillBase::OnAttachmentBeginCollision);
			Attachment->OnAttachmentEndCollision.AddDynamic(ActiveSkill, &UCSkillBase::OnAttachmentEndCollision);

			Attachment->OnAttachmentBeginOverlap.AddDynamic(ActiveSkill, &UCSkillBase::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(ActiveSkill, &UCSkillBase::OnAttachmentEndOverlap);
		}
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

