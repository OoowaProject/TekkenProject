// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "inGameUI.generated.h"

/**
 * 
 */

UCLASS()
class JMH_API UinGameUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY()
	class AGameMode_MH* gm;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TSubclassOf<class USettingsUI> SettingsUI;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UPlayerInfoUI* WBP_PlayerInfo;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_Timer;


	UFUNCTION()
	void UpdateTimerDisplay(float TimerValue);


	//라운드 UI num 변경
	UFUNCTION()
	void ShowRoundText(int32 RoundNum);

	UFUNCTION()
	void HideRoundText();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowTextVisibility(const FString& TextName);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideTextVisibility(const FString& TextName);

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_Round;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_Timer_0;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_Round;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_GameOver;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_Ready;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_Fight;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_FinalRound;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_TimeUp;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_YouWin;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_YouLose;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_Draw;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* can_KO;

	//UI 애니메이션
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Roundnum;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* canReady;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* canFight;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* canDraw;
	
	
	UFUNCTION()
	void PlayAnimRoundnum();
	UFUNCTION()
	void PlayAnimcanReady();
	UFUNCTION()
	void PlayAnimcanFight();

	
	// 타이머 핸들
	FTimerHandle TimerHandle;

	// 애니메이션이 끝났을 때의 핸들
	FDelegateHandle OnAnimationFinishedHandle;
	
};
