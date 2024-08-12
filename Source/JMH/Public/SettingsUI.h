// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsUI.generated.h"

/**
 * 
 */
UCLASS()
class JMH_API USettingsUI : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TSubclassOf<class UMainUI> MainMenuWidget;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_chooseCarac;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Mainmenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_quit;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Back;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ShowChooseWidget();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ShowMainMenuWidget();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void QuitGame();
	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnClickedBack();
	
	
};
