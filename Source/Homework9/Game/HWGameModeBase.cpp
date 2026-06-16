#include "Game/HWGameModeBase.h"
#include "Player/HWPlayerState.h"
#include "Player/HWPlayerController.h"
#include "EngineUtils.h"

void AHWGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AHWPlayerController* HWPlayerController = Cast<AHWPlayerController>(NewPlayer);
	if (IsValid(HWPlayerController) == true)
	{
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
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (UniqueDigits.Num() <= 2)
		{
			bIsUnique = false;
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
		for (int32 i = AllPlayerControllers.Num() - 1; i >= 0; --i)
		{
			AHWPlayerController* HWPlayerController = AllPlayerControllers[i].Get();
			if (IsValid(HWPlayerController) == false)
			{
				AllPlayerControllers.RemoveAt(i);
				continue;
			}

			FString CombineMessageString = InChatMessageString + TEXT("->") + JudgeResultString;
			HWPlayerController->ClientRPCPrintChatMessageString(CombineMessageString);
		}
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
