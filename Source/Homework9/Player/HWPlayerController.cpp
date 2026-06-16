#include "Player/HWPlayerController.h"
#include "UI/HWUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "Player/HWPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Game/HWGameModeBase.h"
#include "Net/UnrealNetwork.h"

AHWPlayerController::AHWPlayerController()
{
	bReplicates = true;
}

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

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
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

void AHWPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
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

