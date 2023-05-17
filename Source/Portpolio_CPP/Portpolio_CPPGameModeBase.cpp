#include "Portpolio_CPPGameModeBase.h"

#include "Character/CPlayer.h"

APortpolio_CPPGameModeBase::APortpolio_CPPGameModeBase()
{
	DefaultPawnClass = ConstructorHelpers::FClassFinder<ACPlayer>(TEXT("Blueprint'/Game/01_Player/BP_CPlayer.BP_CPlayer_C'")).Class;
		//ACPlayer::StaticClass();
}
