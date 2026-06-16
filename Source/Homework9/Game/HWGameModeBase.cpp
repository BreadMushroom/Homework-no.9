#include "Game/HWGameModeBase.h"
#include "Player/HWPlayerState.h"
#include "Player/HWPlayerController.h"
#include "EngineUtils.h"
#include "UI/HWUserWidget.h"

void AHWGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AHWPlayerController* HWPlayerController = Cast<AHWPlayerController>(NewPlayer);
	if (IsValid(HWPlayerController) == true)
	{
		HWPlayerController->NotificationText = FText::FromString(TEXT("서버연결 완료!"));

		AllPlayerControllers.Add(HWPlayerController);

		static int32 PlayerNumber = 0;
		AHWPlayerState* HWPS = HWPlayerController->GetPlayerState<AHWPlayerState>();
		if (IsValid(HWPS) == true)
		{
			HWPS->PlayerNameString = TEXT("Player") + FString::FromInt(++PlayerNumber);
		}
	}
}

FString AHWGameModeBase::GenerateNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}
//Numbers 배열에 1부터 9까지 숫자를 저장.
//현재 배열의 인덱스 범위에서 RandRange()함수를 통해 랜덤 인덱스를 선택. 
//선택된 숫자는 int32자료형을 FString 자료형으로 변환 해주기 위해 FString::FromInt()함수를 사용하여 변환 후 Append()함수로 문자열 결과(Result)에 추가. 
//중복 선택을 막기 위해 RemoveAt()함수로 배열에서 제거. 
//이 인덱스 선정 과정을 3번 반복하여 중복 없는 3자리 정답 문자열을 생성.

bool AHWGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do 
	{
		if (InNumberString.Len() != 3)
		{
			UE_LOG(LogTemp, Warning, TEXT("3자리 수를 입력해 주십쇼."));
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				UE_LOG(LogTemp, Warning, TEXT("문자 또는 0을 포함하지 마십쇼."));
				break;
			}

			UniqueDigits.Add(C);
		}

		if (UniqueDigits.Num() <= 2)
		{
			bIsUnique = false;
			UE_LOG(LogTemp, Warning, TEXT("3자리 수를 입력해 주십쇼."));
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
	} while (false);

	return bCanPlay;
}

FString AHWGameModeBase::JudgeResult(const FString& InNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("Strike: %d / Ball: %d"), StrikeCount, BallCount);
}

void AHWGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	NumberString = GenerateNumber();
}

void AHWGameModeBase::PrintChatMessageString(AHWPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(NumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);

		for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
		{
			AHWPlayerController* HWPlayerController = AllPlayerControllers[i].Get();
			if (IsValid(HWPlayerController) == false)
			{
				AllPlayerControllers.RemoveAt(i);
				continue;
			}

			AHWPlayerState* HWPS = InChattingPlayerController->GetPlayerState<AHWPlayerState>();
			if (IsValid(HWPS) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString + HWPS->GetPlayerInfoString();
				HWPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}

		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
		{
			AHWPlayerController* HWPlayerController = AllPlayerControllers[i].Get();
			if (IsValid(HWPlayerController) == false)
			{
				AllPlayerControllers.RemoveAt(i);
				continue;
			}

			HWPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}
}

void AHWGameModeBase::IncreaseGuessCount(AHWPlayerController* InChattingPlayerController)
{
	AHWPlayerState* HWPS = InChattingPlayerController->GetPlayerState<AHWPlayerState>();
	if (IsValid(HWPS) == true)
	{
		HWPS->CurrentGuessCount++;
	}
}

void AHWGameModeBase::ResetGame()
{
	NumberString = GenerateNumber();
	UE_LOG(LogTemp, Error, TEXT("%s"), *NumberString);

	for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
	{
		AHWPlayerController* HWPlayerController = AllPlayerControllers[i].Get();
		if (IsValid(HWPlayerController) == true)
		{
			AHWPlayerState* HWPS = HWPlayerController->GetPlayerState<AHWPlayerState>();
			if (IsValid(HWPS) == true)
			{
				HWPS->CurrentGuessCount = 0;
			}
		}
	}
}

void AHWGameModeBase::JudgeGame(AHWPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		AHWPlayerState* HWPS = InChattingPlayerController->GetPlayerState<AHWPlayerState>();

		for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
		{
			AHWPlayerController* HWPlayerController = AllPlayerControllers[i].Get();
			if (IsValid(HWPlayerController) == true)
			{
				if (IsValid(HWPS) == true)
				{
					FString CombinedMessageString = HWPS->PlayerNameString + TEXT(" 이/가 게임에서 승리 하셨습니다.");
					HWPlayerController->NotificationText = FText::FromString(CombinedMessageString);
				}
			}
		}

		ResetGame();
	}
	else
	{
		bool bIsDraw = true;
		for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
		{
			AHWPlayerController* HWPlayerController = AllPlayerControllers[i].Get();
			if (IsValid(HWPlayerController) == true)
			{
				AHWPlayerState* HWPS = HWPlayerController->GetPlayerState<AHWPlayerState>();
				if (IsValid(HWPS) == true)
				{
					if (HWPS->CurrentGuessCount < HWPS->MaxGuessCount)
					{
						bIsDraw = false;
						break;
					}
				}
			}
		}

		if (true == bIsDraw)
		{
			for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
			{
				AHWPlayerController* HWPlayerController = AllPlayerControllers[i].Get();
				if (IsValid(HWPlayerController) == true)
				{
					HWPlayerController->NotificationText = FText::FromString(TEXT("무승부!"));
				}
			}

			ResetGame();
		}
	}
}
