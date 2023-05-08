#include "Portpolio_CPPGameModeBase.h"

#include "Character/CPlayer.h"

APortpolio_CPPGameModeBase::APortpolio_CPPGameModeBase()
{
	DefaultPawnClass = ACPlayer::StaticClass();
}
