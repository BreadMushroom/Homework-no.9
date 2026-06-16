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

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	UPROPERTY(Replicated)
	int32 MaxGuessCount;

	FString GetPlayerInfoString();
};
