#include "AddOns/CArrow.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACArrow::ACArrow()
{
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCollisionProfileName("BlockAll");
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);

	Projectile->Deactivate();
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);
}

void ACArrow::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->Activate();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpanAfterCollision);

	for (AActor* actor : Ignores)
	{
		if (actor == OtherActor)
			return;
	}

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character && OnHit.IsBound())
		OnHit.Broadcast(this, character);
}
