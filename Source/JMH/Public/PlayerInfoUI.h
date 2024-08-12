// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoUI.generated.h"

/**
 * 
 */
UCLASS()
class JMH_API UPlayerInfoUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY()
	class AGameMode_MH* gm;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_NameA;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_NameB;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_EndHPA;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_EndHPB;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_PlayerA;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_PlayerB;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint1A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint2A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint3A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint1B;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint2B;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint3B;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HPBarA;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HitBarA;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HPBarB;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HitBarB;
	
	
	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* HP_A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* HP_B;

	// pual=1,kazuya=2,jin=3
	UFUNCTION()
	void SetPlayerinfo(ACharacter* PlayerA,ACharacter* PlayerB,int32 playerAnum,int32 playerBnum);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText KAZUYA = FText::FromString("KAZUYA");

	UPROPERTY(EditAnywhere,Category="MySettings")
	UTexture* img_kazuya;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText PAUL = FText::FromString("PAUL");
	
	UPROPERTY(EditAnywhere,Category="MySettings")
	UTexture* img_paul;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText JIN = FText::FromString("JIN");

	// 플레이어 HP를 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealthBars(float Player1CurrentHP, float Player1MaxHP, float Player2CurrentHP, float Player2MaxHP);

	float CalculateHealthPercentage(float CurrentHP, float MaxHP) const;

	//라운드 이미지 업데이트
	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateRoundImages(int32 RoundScoreA,int32 RoundScoreB);

	UFUNCTION(BlueprintCallable, Category="UI")
	void InitRoundImages();

	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateEndHP(float playerAHP, float playerBHP);

	UFUNCTION(BlueprintCallable, Category="UI")
	void HiddenEndHP();

	UFUNCTION(BlueprintCallable, Category="UI")
	void RequestShakeEffect(float ShakeIntensity);

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShakeEffect(float Intensity);

	UPROPERTY()
	class UTimelineComponent* ShakeTimeline;
	
	// 흔들림 강도
	float ShakeAmount;
	
	// 흔들림 애니메이션을 위한 Curve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UCurveFloat* ShakeCurve;

	//HPUI 애니메이션
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* HP_AShake;

	//HPUI 애니메이션
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* HP_BShake;
	
	// 애니메이션을 호출하는 함수
	UFUNCTION()
	void PlayHPShakeAnim(int32 playernum);
	
	
};
