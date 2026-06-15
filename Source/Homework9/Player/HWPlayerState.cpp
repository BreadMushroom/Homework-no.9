#include "Player/HWPlayerState.h"
#include "Net/UnrealNetwork.h"

AHWPlayerState::AHWPlayerState()
	:PlayerNameString(TEXT("None"))
{
	bReplicates = true;
}

void AHWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
}
