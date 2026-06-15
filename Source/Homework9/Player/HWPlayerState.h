#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HWPlayerState.generated.h"

UCLASS()
class HOMEWORK9_API AHWPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AHWPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated)
	FString PlayerNameString;
};
