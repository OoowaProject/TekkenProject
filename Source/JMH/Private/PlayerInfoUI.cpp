// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoUI.h"

#include "GameMode_MH.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/TimelineComponent.h"

void UPlayerInfoUI::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UPlayerInfoUI::SetPlayerinfo(ACharacter* PlayerA , ACharacter* PlayerB , int32 playerAnum , int32 playerBnum)
{
	if (PlayerA)
	{
		// pual=1,kazuya=2,jin=3
		if (playerAnum == 1)
		{
			//플레이어 1이 폴
			//폴이름, 사진
			text_NameA->SetText(PAUL);

			FSlateBrush Brush;
			Brush.SetResourceObject(img_paul);
			img_PlayerA->SetBrush(Brush);
		}
		else if (playerAnum == 2)
		{
			//카즈야
			//카즈야이름, 사진
			text_NameA->SetText(KAZUYA);

			FSlateBrush Brush;
			Brush.SetResourceObject(img_kazuya);
			img_PlayerA->SetBrush(Brush);
		}
	}

	if (PlayerB)
	{
		if (playerBnum == 1)
		{
			//플레이어2 폴
			//폴이름, 사진
			text_NameB->SetText(PAUL);

			FSlateBrush Brush;
			Brush.SetResourceObject(img_paul);
			img_PlayerB->SetBrush(Brush);
		}
		else if (playerBnum == 2)
		{
			//플레이어2 카즈야
			//카즈야이름, 사진
			text_NameB->SetText(KAZUYA);

			FSlateBrush Brush;
			Brush.SetResourceObject(img_kazuya);
			img_PlayerB->SetBrush(Brush);
		}
	}
	//hp 흔들리게, 라운드 연결, hp 깎이기
}

void UPlayerInfoUI::UpdateHealthBars(float Player1CurrentHP , float Player1MaxHP , float Player2CurrentHP ,
                                     float Player2MaxHP)
{
	if (Pro_HPBarA)
	{
		// 플레이어 1의 체력 바 업데이트
		float Player1HealthPercentage = CalculateHealthPercentage(Player1CurrentHP , Player1MaxHP);
		Pro_HPBarA->SetPercent(Player1HealthPercentage);
		FString AHPString = FString::Printf(TEXT("%.0f") , Player1CurrentHP);
		text_EndHPA->SetText(FText::FromString(AHPString));
	}

	if (Pro_HPBarB)
	{
		// 플레이어 2의 체력 바 업데이트
		float Player2HealthPercentage = CalculateHealthPercentage(Player2CurrentHP , Player2MaxHP);
		Pro_HPBarB->SetPercent(Player2HealthPercentage);
		FString BHPString = FString::Printf(TEXT("%.0f") , Player2CurrentHP);
		text_EndHPB->SetText(FText::FromString(BHPString));
	}
}

float UPlayerInfoUI::CalculateHealthPercentage(float CurrentHP , float MaxHP) const
{
	if (MaxHP <= 0.0f)
	{
		return 0.0f;
	}

	return FMath::Clamp(CurrentHP / MaxHP , 0.0f , 1.0f);
}

void UPlayerInfoUI::UpdateRoundImages(int32 RoundScoreA , int32 RoundScoreB)
{
	//playerA와 playerB의 라운드 스코어를 받아와 이미지 업데이트

	switch (RoundScoreA)
	{
	case 1:
		img_VictoryPoint1A->SetVisibility(ESlateVisibility::Visible);
		break;
	case 2:
		img_VictoryPoint1A->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint2A->SetVisibility(ESlateVisibility::Visible);
		break;
	case 3:
		img_VictoryPoint1A->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint2A->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint3A->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}

	switch (RoundScoreB)
	{
	case 1:
		img_VictoryPoint1B->SetVisibility(ESlateVisibility::Visible);
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("RoundScore1."));
		break;
	case 2:
		img_VictoryPoint1B->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint2B->SetVisibility(ESlateVisibility::Visible);
		GEngine->AddOnScreenDebugMessage(-2 , 5.f , FColor::Red , TEXT("RoundScore2."));
		break;
	case 3:
		img_VictoryPoint1B->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint2B->SetVisibility(ESlateVisibility::Visible);
		img_VictoryPoint3B->SetVisibility(ESlateVisibility::Visible);
		GEngine->AddOnScreenDebugMessage(-3 , 5.f , FColor::Red , TEXT("RoundScore3."));
		break;
	default:
		break;
	}
}

void UPlayerInfoUI::InitRoundImages()
{
	img_VictoryPoint1A->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint2A->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint3A->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint1B->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint2B->SetVisibility(ESlateVisibility::Hidden);
	img_VictoryPoint3B->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerInfoUI::UpdateEndHP(float playerAHP , float playerBHP)
{
	text_EndHPA->SetVisibility(ESlateVisibility::Visible);
	text_EndHPB->SetVisibility(ESlateVisibility::Visible);
	
	//문자열로 변환
	FString AHPString = FString::Printf(TEXT("%.0f") , playerAHP);
	FString BHPString = FString::Printf(TEXT("%.0f") , playerBHP);

	if (AHPString == TEXT("0") || BHPString == TEXT("0"))
	{
		text_EndHPA->SetText(FText::FromString(TEXT("0")));
		text_EndHPB->SetText(FText::FromString(TEXT("0")));
	}
	else
	{
		// 텍스트 블록에 설정
		text_EndHPA->SetText(FText::FromString(AHPString));
		text_EndHPA->SetText(FText::FromString(BHPString));
	}
}

void UPlayerInfoUI::HiddenEndHP()
{
	text_EndHPA->SetVisibility(ESlateVisibility::Hidden);
	text_EndHPB->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerInfoUI::RequestShakeEffect(float ShakeIntensity)
{
	if (ShakeIntensity > 0.0f)
	{
		ShakeEffect(ShakeIntensity);
	}
}

void UPlayerInfoUI::ShakeEffect(float Intensity)
{
	// 흔들림 애니메이션을 위해 간단한 Timeline을 사용할 수 있습니다.
	if (ShakeTimeline == nullptr)
	{
		// Timeline 생성 및 설정
		ShakeTimeline = NewObject<UTimelineComponent>(this);
		ShakeTimeline->RegisterComponent();
		ShakeTimeline->SetTimelineLength(0.5f); // 애니메이션 지속 시간 설정

		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("UpdateShakeEffect"));
		ShakeTimeline->AddInterpFloat(ShakeCurve, TimelineCallback);
	}
	// 흔들림 강도 설정
	ShakeAmount = Intensity;
	ShakeTimeline->PlayFromStart();
}

void UPlayerInfoUI::PlayHPShakeAnim(int32 playernum)
{
	if(playernum == 1)
	{
		if(HP_AShake)
		{
			PlayAnimation(HP_AShake);
		}
	}
	
	if(playernum == 2)
	{
		if(HP_BShake)
		{
			PlayAnimation(HP_BShake);
		}
	}
}

