#include "Game/HWGameModeBase.h"
#include "Player/HWPlayerState.h"
#include "Player/HWPlayerController.h"
#include "EngineUtils.h"

void AHWGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AHWPlayerController* HWPlayerController = Cast<AHWPlayerController>(NewPlayer);
	if (IsValid(HWPlayerController) == true)
	{
		AllPlayerControllers.Add(HWPlayerController);

		static int32 PlayerNumber = 0;
		AHWPlayerState* HWPS = HWPlayerController->GetPlayerState<AHWPlayerState>();
		if (IsValid(HWPS) == true)
		{
			HWPS->PlayerNameString = TEXT("Player") + FString::FromInt(++PlayerNumber);
		}
	}
}
