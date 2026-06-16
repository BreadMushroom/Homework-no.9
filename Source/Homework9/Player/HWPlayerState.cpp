#include "Player/HWPlayerState.h"
#include "Net/UnrealNetwork.h"

AHWPlayerState::AHWPlayerState()
	: PlayerNameString(TEXT("None"))
	, CurrentGuessCount(0)
	, MaxGuessCount(3)
{
	bReplicates = true;
}

void AHWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString AHWPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = TEXT(" [현재 시도 횟수: ") + FString::FromInt(CurrentGuessCount) + TEXT("/ 최대 시도 횟수: ") + FString::FromInt(MaxGuessCount) + TEXT("]");
	return PlayerInfoString;
}
