#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HWGameModeBase.generated.h"

class AHWPlayerController;

UCLASS()
class HOMEWORK9_API AHWGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer)override;

protected:
	TArray<TWeakObjectPtr<AHWPlayerController>> AllPlayerControllers;
};
