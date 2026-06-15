#include "UI/HWUserWidget.h"
#include "Components/EditableTextBox.h"
#include "Player/HWPlayerController.h"

void UHWUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHWUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHWUserWidget::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommiMethod)
{
	if (CommiMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			AHWPlayerController* OwningHWPlayerController = Cast<AHWPlayerController>(OwningPlayerController);
			if (IsValid(OwningHWPlayerController) == true)
			{
				OwningHWPlayerController->SetChatMessageString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
