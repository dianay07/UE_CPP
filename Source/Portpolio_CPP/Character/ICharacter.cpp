#include "ICharacter.h"

#include "CCharacterBase.h"
#include "Blueprint/UserWidget.h"
#include "Component/CTargetComponent.h"

void IICharacter::ToggleTarget(UCTargetComponent* InTargetComp, UUserWidget* InWidget)
{
	// 타겟 컴포넌트 없으면 취소
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
