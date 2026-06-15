#include "Player/HWPlayerController.h"
#include "UI/HWUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "Player/HWPlayerState.h"

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
	//UKismetSystemLibrary::PrintString(this, InChatMessageString, true, true, FLinearColor::Red, 10.0f);
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
	for (TActorIterator<AHWPlayerController> It(GetWorld()); It; ++It)
	{
		AHWPlayerController* HWPlayerController = *It;
		if (IsValid(HWPlayerController) == true)
		{
			HWPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}
}

