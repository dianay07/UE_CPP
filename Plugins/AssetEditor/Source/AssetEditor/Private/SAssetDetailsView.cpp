#include "SAssetDetailsView.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "NiagaraSystem.h"
#include "SAssetCheckBoxes.h"
#include "SJobEquipData.h"
#include "SJobSkillDamageData.h"
#include "SJobSkillData.h"
#include "Animation/AnimMontage.h"
#include "Job/CJobAsset.h"

bool SAssetDetailsView::bRefreshByCheckBoxes = false;
	
TSharedRef<IDetailCustomization> SAssetDetailsView::MakeInstance()
{
	return MakeShareable(new SAssetDetailsView());
}

void SAssetDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type =  UCJobAsset::StaticClass();

	DetailBuilder.HideCategory("CJobDataAsset");

	// class Setting
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromString("Class Settings"));
		category.AddProperty("AttachmentClass", type);
		category.AddProperty("EquipmentClass", type);
		category.AddProperty("ActiveSkillClass", type);
		category.AddProperty("NonGlobalClass", type);
		category.AddProperty("JobName", type);
		category.AddProperty("AttackRange", type);
	}

	// Auto Attack Montage
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("AutoAttackMontages", FText::FromString("AutoAttack Montage"));
		category.AddProperty("AutoAttackMontages", type);
	}

	// Equipment Setting
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DrawWeaponData", FText::FromString("DrawWeapon Data"));
		IDetailPropertyRow& row = category.AddProperty("DrawWeaponData", type);

		if (bRefreshByCheckBoxes == false)
		{
			TSharedPtr<SAssetCheckBoxes> checkBoxes = SJobEquipData::CreateCheckBoxes();
			checkBoxes->AddProperties(row.GetPropertyHandle());

			FDrawWeaponData data;

			int32 index = 0;
			checkBoxes->CheckDefaultObject(index++, data.Montage);
			checkBoxes->CheckDefaultValue(index++, data.PlayRate);
			checkBoxes->CheckDefaultValue(index++, data.bUseControllerRotate);
		}
	}

	// Skill Setting
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("GlobalSkillDatas", FText::FromString("Global Skill Data"));
		IDetailPropertyRow& row = category.AddProperty("SkillDatas", type);

		if (bRefreshByCheckBoxes == false)
		{
			// ��ų �迭�� ����ִ� ���� ���Ϲ޾ƿ���
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);

			// ù ��Ͻ� üũ�ڽ� �ʱ�ȭ
			SJobSkillData::EmptyCheckBoxes();

			FSkillData data;
			for (uint32 i = 0; i < count; i++)
			{
				// ��ų �迭 �ڵ� �����ͼ� �� �ڽ� �ڵ� ��������
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);

				// � �ڵ��� üũ�ڽ��� ������ ����
				TSharedPtr<SAssetCheckBoxes> checkBoxes = SJobSkillData::AddCheckBoxes();
				checkBoxes->AddProperties(handle);

				int32 index = 0;
				checkBoxes->CheckDefaultValue(index++, data.DemandLevel);
				checkBoxes->CheckDefaultValue(index++, data.Name);
				checkBoxes->CheckDefaultValue(index++, data.Description);
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.SkillCooltime);
				checkBoxes->CheckDefaultValue(index++, data.IsRequireTarget);
				checkBoxes->CheckDefaultObject(index++, data.Icon);
				checkBoxes->CheckDefaultObject(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		} // if
	}

	// HitData Setting
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("HitData", FText::FromString("Hit Data"));
		IDetailPropertyRow& row = category.AddProperty("HitDatas", type);

		if (bRefreshByCheckBoxes == false)	
		{
			// ��ų �迭�� ����ִ� ���� ���Ϲ޾ƿ���
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);

			// ù ��Ͻ� üũ�ڽ� �ʱ�ȭ
			SJobSkillDamageData::EmptyCheckBoxes();

			FSkillDamageData data;
			for (uint32 i = 0; i < count; i++)
			{
				// ��ų �迭 �ڵ� �����ͼ� �� �ڽ� �ڵ� ��������
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);

				// � �ڵ��� üũ�ڽ��� ������ ����
				TSharedPtr<SAssetCheckBoxes> checkBoxes = SJobSkillDamageData::AddCheckBoxes();
				checkBoxes->AddProperties(handle);

				int32 index = 0;
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.Damage);
				//checkBoxes->CheckDefaultObject(index++, data.Sound);
				checkBoxes->CheckDefaultObject(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		} // if
	}
}

