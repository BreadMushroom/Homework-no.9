#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HWUserWidget.generated.h"

class UEditableTextBox;
class UScrollBox;

UCLASS()
class HOMEWORK9_API UHWUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;

	virtual void NativeDestruct()override;

	UFUNCTION()
	void AddChatMessage(const FText& Text);

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommiMethod);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_ChatHistory;

};
