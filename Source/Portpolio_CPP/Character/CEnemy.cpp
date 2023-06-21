#include "Character/CEnemy.h"

#include "Kismet/GameplayStatics.h"
#include "CPlayer.h"
#include "Component/CEquipComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/CUI_TargetingCursor.h"

ACEnemy::ACEnemy()
{
	// CharacterBase 세팅
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

	// Cursor Widget 생성 및 설정
	CursorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Targeting Cursor"));
	CursorWidget->SetRelativeLocation(FVector(0, 0, 180.0f));
	CursorWidget->SetupAttachment(GetMesh());
	CursorWidget->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UCUI_TargetingCursor> UI_TargetingCursor(TEXT("WidgetBlueprint'/Game/07_UI/BP_CUI_TargetingCursor.BP_CUI_TargetingCursor_C'"));
	if (UI_TargetingCursor.Succeeded())
	{
		CursorWidget->SetWidgetClass(UI_TargetingCursor.Class);
		CursorWidget->SetDrawSize(FVector2D(80, 160));
		CursorWidget->SetPivot(FVector2D(0.5f, 0.8f));
		CursorWidget->SetRelativeLocation(FVector(0, 0, 400));
	}
	CursorWidget->SetVisibility(false);
}
 
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ACEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACEnemy::SetWidgetVisble()
{
	if (CursorWidget->IsVisible() == true)
		CursorWidget->SetVisibility(false);
	else
		CursorWidget->SetVisibility(true);

}
