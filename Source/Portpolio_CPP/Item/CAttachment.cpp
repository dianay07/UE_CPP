#include "Item/CAttachment.h"

#include "Character/CCharacterBase.h"

ACAttachment::ACAttachment()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());

	Super::BeginPlay();
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}
