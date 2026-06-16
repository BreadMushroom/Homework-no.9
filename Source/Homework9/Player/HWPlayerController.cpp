#include "Player/HWPlayerController.h"
#include "UI/HWUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "Player/HWPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Game/HWGameModeBase.h"

void AHWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UHWUserWidget>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
}

void AHWPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		AHWPlayerState* HWPS = GetPlayerState<AHWPlayerState>();
		if (IsValid(HWPS) == true)
		{
			FString CombinedMessageString = HWPS->PlayerNameString + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void AHWPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	if (IsValid(ChatInputWidgetInstance) == true)
	{
		ChatInputWidgetInstance->AddChatMessage(FText::FromString(InChatMessageString));
	}
}

void AHWPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AHWPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GMB = UGameplayStatics::GetGameMode(this);
	if (IsValid(GMB) == true)
	{
		AHWGameModeBase* HWGMB = Cast<AHWGameModeBase>(GMB);
		if (IsValid(HWGMB) == true)
		{
			HWGMB->PrintChatMessageString(this, InChatMessageString);
		}

	}
}

