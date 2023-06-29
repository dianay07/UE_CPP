#include "SAssetDetailsView.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "SAssetCheckBoxes.h"
#include "SJobEquipData.h"
#include "SJobSkillData.h"
#include "Animation/AnimMontage.h"
#include "Job/CJobDataAsset.h"
#include "NiagaraSystem.h"

bool SAssetDetailsView::bRefreshByCheckBoxes = false;
	
TSharedRef<IDetailCustomization> SAssetDetailsView::MakeInstance()
{
	return MakeShareable(new SAssetDetailsView());
}

void SAssetDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type =  UCJobDataAsset::StaticClass();

	DetailBuilder.HideCategory("CJobDataAsset");

	// class Setting
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromString("Class Settings"));
		category.AddProperty("AttachmentClass", type);
		category.AddProperty("EquipmentClass", type);
		category.AddProperty("ActiveSkillClass", type);
		category.AddProperty("JobName", type);
	}

	// Equipment Setting
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("EquipData", FText::FromString("Equip Data"));
		IDetailPropertyRow& row = category.AddProperty("EquipData", type);

		if (bRefreshByCheckBoxes == false)
		{
			TSharedPtr<SAssetCheckBoxes> checkBoxes = SJobEquipData::CreateCheckBoxes();
			checkBoxes->AddProperties(row.GetPropertyHandle());

			FEquipData data;

			int32 index = 0;
			checkBoxes->CheckDefaultObject(index++, data.Montage);
			checkBoxes->CheckDefaultValue(index++, data.PlayRate);
			checkBoxes->CheckDefaultValue(index++, data.bUseControllerRotate);
		}
	}

	// Skill Setting
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("SkillDatas", FText::FromString("Skill Data"));
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
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.bUseControllerRotate);
				// ESkillCoolType
				checkBoxes->CheckDefaultValue(index++, data.SkillCooltime);
				checkBoxes->CheckDefaultObject(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		} // if
	}
}
