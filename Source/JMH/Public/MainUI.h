// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */

UCLASS()
class JMH_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	
public:
	virtual void NativeConstruct() override;
	
	//widget
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TSubclassOf<class USettingsUI> SettingsUI;
	
	UPROPERTY()
	class UMainGameInstance_MH* MainGameInstance;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Can_MainMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Can_ChooseSide;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Can_ChooseCharacter;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Can_VS;

	//MainMenu
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Tournament;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_PVSP;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_PVSCPU;

	//
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_OPTIONS;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_COMMUNITY;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_PLAYDATA;


	//ChoosePlaySide
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Left;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Right;

	//chooseCharacter
	UPROPERTY()
	bool bPlayer1Charac=false;
	
	UPROPERTY()
	bool bPlayer2Charac=false;
	
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* Player1Info;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* Player2Info;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Player1Input;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (BindWidget))
	class UOverlay* Player2Input;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (BindWidget))
	class UImage* img_Player1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (BindWidget))
	class UImage* img_Player2;

	//UPROPERTY(meta = (BindWidget))
	//class UButton* Btn_JIN;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (BindWidget))
	class UButton* Btn_Paul;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (BindWidget))
	class UButton* Btn_Random;

	//UPROPERTY(meta = (BindWidget))
	//class UButton* Btn_Kazuya;

	//Player1 Info

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Info01;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Info02;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Info03;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1Info04;

	
	// Player2 Info
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Name;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Info01;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Info02;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Info03;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2Info04;



	//VS
	
	UPROPERTY(meta = (BindWidget))
	class UImage* img_P1;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* img_P2;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_P1Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_P2Name;

	//기타
	
	UPROPERTY(meta = (BindWidget))
	class UOverlay* ove_Empty;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Back;

	//input 체크
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsClickedB;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsClickedO;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsUnHoveredB;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsUnHoveredO;

public:
	//Mainmenu
//토너먼트는 보류
	UFUNCTION()
	void OnClickedTournament();

	UFUNCTION()
	void OnClickedPlayerVSPlayer();
	
	UFUNCTION()
	void OnClickedPlayerVSCPU();

	UFUNCTION()
	void OnClickedSettings();

	UFUNCTION()
	void OnClickedCommu();

	UFUNCTION()
	void OnClickedPalyData();

	//PlaySide
	UFUNCTION()
	void OnClickedLeftSide();

	UFUNCTION()
	void OnClickedRightSide();

	//chooseCharacter
	UFUNCTION()
	void StartChoose();
	
	UFUNCTION()
	void ChooseCharactter(int32 characnum);

	UFUNCTION(BlueprintCallable)
	void OnClickedPaul();

	UFUNCTION(BlueprintCallable)
	void OnClickedRandom();

	UFUNCTION()
	void ShowP1InputWindow();

	UFUNCTION()
	void ShowP2InputWindow();

	//기타
	UFUNCTION()  
	void OnClickedEmptyBack();

	//기타
	UFUNCTION(BlueprintCallable)  
	void OnHoveredPaul();

	UFUNCTION(BlueprintCallable)  
	void OnUnHoveredPaul();

	UPROPERTY()
	bool bPlayer1Input=true;

	UPROPERTY()
	bool bPlayer2Input=true;
	
	
};
