#include "ICharacter.h"

#include "CCharacterBase.h"
#include "Blueprint/UserWidget.h"
#include "Component/CTargetComponent.h"

void IICharacter::ToggleTarget(UCTargetComponent* InTargetComp, UUserWidget* InWidget)
{
	// Ÿ�� ������Ʈ ������ ���
	/*if (InTargetComp == nullptr)
		return;
	
	if(IsValid(Cast<ACharacter>(InTargetComp->GetTarget())))
	{
		if(InWidget->IsVisible())
		{
			InWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}*/

	
}

void IICharacter::Damage(ACharacter* InAttacker, TArray<ACharacter*> InDamagedObjs, FHitData InHitData)
{

}
