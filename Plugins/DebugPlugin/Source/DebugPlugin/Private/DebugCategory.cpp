#include "DebugCategory.h"
#include "CanvasItem.h"
#include "DrawDebugHelpers.h"
#include "Character/CPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleEventManager.h"

FDebugCategory::FDebugCategory()
{
	bShowOnlyWithDebugActor = false;
}

FDebugCategory::~FDebugCategory()
{
}

TSharedRef<FGameplayDebuggerCategory> FDebugCategory::MakeInstance()
{
	return MakeShareable(new FDebugCategory());
}

void FDebugCategory::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	FGameplayDebuggerCategory::CollectData(OwnerPC, DebugActor);

	//ACharacter* player = OwnerPC->GetPawn<ACharacter>();
	ACPlayer* player = OwnerPC->GetPawn<ACPlayer>();

	// Player
	{
		PlayerActorData.bDraw = true;
		PlayerActorData.Name = player->GetName();
		PlayerActorData.Location = player->GetActorLocation();
		PlayerActorData.Forward = player->GetActorForwardVector();
		PlayerActorData.TargetArmLength = player->GetSpringArm()->TargetArmLength;
	}

	// Forward Actor
	{
		// 충돌 체크용 라인 
		FHitResult hitResult;
		FVector start = PlayerActorData.Location;
		FVector end = start + player->GetActorForwardVector() * TraceDistance;

		FCollisionQueryParams params;
		params.AddIgnoredActor(player);

		player->GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params);

		if(hitResult.bBlockingHit)
		{
			ForwardActorData.bDraw = true;
			ForwardActorData.Name = hitResult.GetActor()->GetName();
			ForwardActorData.Location = hitResult.GetActor()->GetActorLocation();
			ForwardActorData.Forward = hitResult.GetActor()->GetActorForwardVector();
		}
	}

	if (!!DebugActor)
	{
		DebugActorData.bDraw = true;
		DebugActorData.Name = DebugActor->GetName();
		DebugActorData.Location = DebugActor->GetActorLocation();
		DebugActorData.Forward = DebugActor->GetActorForwardVector();
	}

}

void FDebugCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);

	FVector start = PlayerActorData.Location;
	FVector end = start + PlayerActorData.Forward * TraceDistance;
	
	FCanvasTileItem item(FVector2D(10, 10), FVector2D(300, 215), FLinearColor(0, 0, 0, 0.25f));
	item.BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaBlend;
	CanvasContext.DrawItem(item, CanvasContext.CursorX, CanvasContext.CursorY);

	CanvasContext.Printf(FColor::Green, L"  -- Player Pawn --");
	CanvasContext.Printf(FColor::White, L"  Name : %s", *PlayerActorData.Name);
	CanvasContext.Printf(FColor::White, L"  Location : %s", *PlayerActorData.Location.ToString());
	CanvasContext.Printf(FColor::White, L"  Forward : %s", *PlayerActorData.Forward.ToString());
	CanvasContext.Printf(FColor::White, L"  TargetArmLength : %f", PlayerActorData.TargetArmLength);
	CanvasContext.Printf(FColor::White, L"");

	if (ForwardActorData.bDraw)
	{
		CanvasContext.Printf(FColor::Green, L"  -- Forward Actor --");
		CanvasContext.Printf(FColor::White, L"  Name : %s", *ForwardActorData.Name);
		CanvasContext.Printf(FColor::White, L"  Location : %s", *ForwardActorData.Location.ToString());
		CanvasContext.Printf(FColor::White, L"  Forward : %s", *ForwardActorData.Forward.ToString());
		CanvasContext.Printf(FColor::White, L"");
	}

	if (DebugActorData.bDraw)
	{
		CanvasContext.Printf(FColor::Green, L"  -- Select Actor --");
		CanvasContext.Printf(FColor::White, L"  Name : %s", *DebugActorData.Name);
		CanvasContext.Printf(FColor::White, L"  Location : %s", *DebugActorData.Location.ToString());
		CanvasContext.Printf(FColor::White, L"  Forward : %s", *DebugActorData.Forward.ToString());
	}
}
