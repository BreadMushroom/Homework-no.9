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

	FString GenerateNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay()override;

	void PrintChatMessageString(AHWPlayerController* InChattingPlayerController, const FString& InChatMessageString);

protected:
	FString NumberString;

	TArray<TWeakObjectPtr<AHWPlayerController>> AllPlayerControllers;
};
