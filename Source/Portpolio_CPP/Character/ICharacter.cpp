#include "ICharacter.h"

#include "CCharacterBase.h"
#include "Job/CJobStructure.h"

void IICharacter::CreateDynamicMaterial(ACharacter* InCharacter)
{
	for(int32 i = 0; i < InCharacter->GetMesh()->GetMaterials().Num(); i++)
	{
		UMaterialInterface* material = InCharacter->GetMesh()->GetMaterials()[i];

		InCharacter->GetMesh()->SetMaterial(i, UMaterialInstanceDynamic::Create(material, InCharacter));
	}
}

void IICharacter::ChangeColor(ACharacter* InCharacter, FLinearColor InColor)
{
	for(UMaterialInterface* material : InCharacter->GetMesh()->GetMaterials())
	{
		UMaterialInstanceDynamic* instance = Cast<UMaterialInstanceDynamic>(material);

		if (!!instance)
			instance->SetVectorParameterValue("BodyColor", InColor);
	}
}
