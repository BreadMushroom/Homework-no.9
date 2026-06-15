#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HWPlayerController.generated.h"

class UHWUserWidget;

UCLASS()
class HOMEWORK9_API AHWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHWUserWidget> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UHWUserWidget> ChatInputWidgetInstance;

	FString ChatMessageString;
};
