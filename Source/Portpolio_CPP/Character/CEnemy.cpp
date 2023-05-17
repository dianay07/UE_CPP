#include "Character/CEnemy.h"

#include "Component/CEquipComponent.h"

ACEnemy::ACEnemy()
{
	// CharacterBase ¼¼ÆÃ
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh>
			SK_Mesh(TEXT("SkeletalMesh'/Game/99_Assets/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
		if(SK_Mesh.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(SK_Mesh.Object);
		}
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

		TSubclassOf<UAnimInstance> instance;
		instance = ConstructorHelpers::FClassFinder<UAnimInstance>(TEXT("AnimBlueprint'/Game/02_Enemy/ABP_Enemy.ABP_Enemy_C'")).Class;
		GetMesh()->SetAnimClass(instance);
	}

	// Component
	{
		Equip = CreateDefaultSubobject<UCEquipComponent>(TEXT("Equip Component"));
	}
}
