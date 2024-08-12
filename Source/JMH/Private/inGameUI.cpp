// Fill out your copyright notice in the Description page of Project Settings.


#include "inGameUI.h"

#include "GameMode_MH.h"
#include "PlayerInfoUI.h"
#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"


void UinGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	HideTextVisibility(TEXT("ALL"));
}

void UinGameUI::UpdateTimerDisplay(float TimerValue)
{
	if (text_Timer)
	{
		// 타이머 값을 문자열로 변환
		FString TimerString = FString::Printf(TEXT("%.0f") , TimerValue);

		if (TimerString == TEXT("0"))
		{
			text_Timer->SetText(FText::FromString(TEXT("0")));
		}
		else
		{
			// 텍스트 블록에 설정
			text_Timer->SetText(FText::FromString(TimerString));
		}
	}
}

//라운드 넘버 바꿔주기
void UinGameUI::ShowRoundText(int32 RoundNum)
{
	// int32를 FString으로 변환하여 텍스트 블록에 설정
	if (RoundNum)
	{
		FString Text = FString::Printf(TEXT("%d") , RoundNum);
		if (text_Round)
		{
			text_Round->SetText(FText::FromString(Text));
		}
		if (can_Round)
		{
			HideTextVisibility(TEXT("ALL"));
			can_Round->SetVisibility(ESlateVisibility::Visible);
			PlayAnimRoundnum();
		}
	}
}

void UinGameUI::HideRoundText()
{
	if (can_Round)
	{
		can_Round->SetVisibility(ESlateVisibility::Collapsed);
	}
}

//텍스트 비지블
void UinGameUI::ShowTextVisibility(const FString& TextName)
{
	HideTextVisibility(TEXT("ALL"));
	
	// UWidget 이름을 기반으로 캔버스 패널을 찾기
	UWidget* Widget = GetWidgetFromName(FName(*TextName));

	// 찾은 위젯을 UCanvasPanel으로 변환
	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(Widget);

	if (CanvasPanel)
	{
		CanvasPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UinGameUI::HideTextVisibility(const FString& TextName)
{
	if(TextName == "ALL")
	{
		can_Round->SetVisibility(ESlateVisibility::Collapsed);
		can_GameOver->SetVisibility(ESlateVisibility::Collapsed);
		can_Ready->SetVisibility(ESlateVisibility::Collapsed);
		can_Fight->SetVisibility(ESlateVisibility::Collapsed);
		can_FinalRound->SetVisibility(ESlateVisibility::Collapsed);
		can_TimeUp->SetVisibility(ESlateVisibility::Collapsed);
		can_YouWin->SetVisibility(ESlateVisibility::Collapsed);
		can_YouLose->SetVisibility(ESlateVisibility::Collapsed);
		can_KO->SetVisibility(ESlateVisibility::Collapsed);
		
		return;
	}
	
	// UWidget 이름을 기반으로 캔버스 패널을 찾기
	UWidget* Widget = GetWidgetFromName(FName(*TextName));

	// 찾은 위젯을 UCanvasPanel으로 변환
	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(Widget);
	
	if (CanvasPanel)
	{
		CanvasPanel->SetVisibility(ESlateVisibility::Visible);
	}
	if(TextName=="can_Draw")
	{
		PlayAnimation(canDraw);
	}
}

void UinGameUI::PlayAnimRoundnum()
{
	// 애니메이션 재생
	PlayAnimation(Roundnum);
	float AnimDuration = Roundnum->GetEndTime();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UinGameUI::PlayAnimcanReady, AnimDuration, false);
}

void UinGameUI::PlayAnimcanReady()
{
	if (canReady)
	{
		can_Ready->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(canReady);
		float AnimDuration = canReady->GetEndTime();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UinGameUI::PlayAnimcanFight, AnimDuration, false);
	}
}

void UinGameUI::PlayAnimcanFight()
{
	if (canFight)
	{
		can_Fight->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(canFight);
	}
}
