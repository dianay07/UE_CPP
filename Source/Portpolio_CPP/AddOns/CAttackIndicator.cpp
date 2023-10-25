#include "AddOns/CAttackIndicator.h"

#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

ACAttackIndicator::ACAttackIndicator()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultRoot);
	Circum = CreateDefaultSubobject<UDecalComponent>(TEXT("Circum"));
	Circum->SetupAttachment(DefaultRoot);

	Area = CreateDefaultSubobject<UDecalComponent>(TEXT("Area"));
	Area->SetupAttachment(DefaultRoot);

	static ConstructorHelpers::FObjectFinder<UMaterial> MATERIAL(TEXT("Material'/Game/03_Addons/Mat_Indicator.Mat_Indicator'"));
	if(MATERIAL.Succeeded())
	{
		material = MATERIAL.Object;
	}
}

void ACAttackIndicator::BeginPlay()
{
	Super::BeginPlay();

	Circum->SetDecalMaterial(material);
	CircumRef = Circum->CreateDynamicMaterialInstance();
	CircumRef->SetScalarParameterValue("Circum1Area0", 1);
	CircumRef->SetScalarParameterValue("BoundarySize", BoundarySize);

	Area->SetDecalMaterial(material);
	AreaRef = Area->CreateDynamicMaterialInstance();
	AreaRef->SetScalarParameterValue("Circum1Area0", 0);
	AreaRef->SetScalarParameterValue("BoundarySize", BoundarySize);
}

void ACAttackIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Timeline.TickTimeline(DeltaTime);

	const float size = AreaRef->K2_GetScalarParameterValue("AreaValue");
	//const float value = size + 0.05 * DeltaTime;
	AreaRef->SetScalarParameterValue("AreaValue", AreaRef->K2_GetScalarParameterValue("AreaValue") + 0.01f * ActionSpeed * DeltaTime);

	if(AreaRef->K2_GetScalarParameterValue("AreaValue") >= BoundarySize)
	{
		/*TArray<AActor*> IsHitActor;
		TArray<TEnumAsByte<EObjectTypeQuery>> TypeQueries;
		TypeQueries.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));*/
				/*if(IsHitActor.Num() > 0)
		{
			for(int32 i = 0; i < IsHitActor.Num(); i++)
				UE_LOG(LogActor, Warning, TEXT("%s is overlap"), *IsHitActor[i]->GetName());

			ApplyDamage(GetOwner(), IsHitActor);
		}*/
		ApplyDamage();

		Destroy();
	}
}

void ACAttackIndicator::ApplyDamage()
{
	TArray<AActor*> Ignores = { this, GetOwner() };

	// boundary 에 실제 radius offset 곱해주기
	//UKismetSystemLibrary::SphereOverlapActors(GetWorld(), this->GetActorLocation(), BoundarySize * 400, TypeQueries, nullptr, Ignores, IsHitActor);
	DrawDebugSphere(GetWorld(), this->GetActorLocation(), 200, 10, FColor(0, 180, 0),
		true, -1, 0, 2);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), 50.0f, this->GetActorLocation(),
		BoundarySize * 400, nullptr, Ignores, this, nullptr, true);
}
