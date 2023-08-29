#include "Job/CJobAsset.h"

#include "Character/CCharacterBase.h"
#include "GameFramework/Character.h"
#include "Item/CSkillBase.h"
#include "Item/CAttachment.h"
#include "Item/CEquipment.h"
#include "Skill/CActiveSkill_NonGlobal.h"

UCJobAsset::UCJobAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
	ActiveSkillClass = UCSkillBase::StaticClass();
}

void UCJobAsset::BeginPlay(ACCharacterBase* InOwner, class UCJobData** OutJobData)
{
	//UCSkillBase* activeSkill = nullptr;
	//if (!!ActiveSkillClass)
	//{
	//	activeSkill = NewObject<UCSkillBase>(this, ActiveSkillClass);
	//	activeSkill->BeginPlay(Attachment, Equipment, InOwner, SkillDatas, HitDatas);
	//}

	//*OutJobData = NewObject<UCJobData>();
	//Datas = OutJobData;
}

int32 UCJobAsset::GetSkillDataCount()
{
	UE_LOG(LogTemp, Display, TEXT("%d"), SkillDatas.Num());
	return SkillDatas.Num();
}

void UCJobAsset::SpawnAttachmentWeapon(class ACCharacterBase* InOwner)
{
	Owner = Cast<ACharacter>(InOwner);

	ACAttachment* attachment = nullptr;
	if (!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = Owner;

		attachment = Owner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	UCSkillBase* activeSkill = nullptr;
	if (!!ActiveSkillClass)
	{
		activeSkill = NewObject<UCSkillBase>(this, ActiveSkillClass);
		activeSkill->BeginPlay(Attachment, Equipment, InOwner, SkillDatas, HitDatas);

		if (!!attachment)
		{
			attachment->OnAttachmentBeginCollision.AddDynamic(activeSkill, &UCSkillBase::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(activeSkill, &UCSkillBase::OnAttachmentEndCollision);

			attachment->OnAttachmentBeginOverlap.AddDynamic(activeSkill, &UCSkillBase::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(activeSkill, &UCSkillBase::OnAttachmentEndOverlap);
		}
	}

	UCEquipment* equipment = nullptr;
	if (!!EquipmentClass)
	{
		equipment = NewObject<UCEquipment>(this, EquipmentClass);
		equipment->BeginPlay(InOwner, DrawWeaponData);

		if (!!attachment)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(attachment, &ACAttachment::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}
	}

	UCActiveSkill_NonGlobal* nonGlobalSkill = nullptr;
	if(!!NonGlobalClass)
	{
		nonGlobalSkill = NewObject<UCActiveSkill_NonGlobal>(this, NonGlobalClass);
		nonGlobalSkill->BeginPlay(InOwner, Attachment, ActiveSkill);
	}

	(*Datas)->Attachment = attachment;
	(*Datas)->Equipment = equipment;
	(*Datas)->ActiveSkill = activeSkill;
	(*Datas)->NonGlobalSkill = nonGlobalSkill;
}

void UCJobAsset::DestroyAttachmentWeapon()
{
	if(!!Attachment)
		Attachment->Destroy();
}

#if WITH_EDITOR
void UCJobAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
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

