#include "Character/CEnemy.h"

#include "Component/CEquipComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/CUI_TargetingCursor.h"

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

	CursorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Targeting Cursor"));
	CursorWidget->SetRelativeLocation(FVector(0, 0, 180.0f));
	CursorWidget->SetupAttachment(GetMesh());
	CursorWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UCUI_TargetingCursor> UI_TargetingCursor(TEXT("WidgetBlueprint'/Game/07_UI/BP_CUI_TargetingCursor.BP_CUI_TargetingCursor_C'"));
	if (UI_TargetingCursor.Succeeded())
	{
		CursorWidget->SetWidgetClass(UI_TargetingCursor.Class);
		CursorWidget->SetDrawSize(FVector2D(200, 30));
	}
	CursorWidget->SetVisibility(false);
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

}

void ACEnemy::ActiveTargetCursor()
{
	if(IsValid(CursorWidget) && CursorWidget->IsActive() == false)
		CursorWidget->SetVisibility(true);

	GLog->Log(FText::FromString(this->GetName()));
}

void ACEnemy::DeactiveTargetCursor()
{
	if (IsValid(CursorWidget))
		CursorWidget->SetVisibility(false);
}
