// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsUI.h"

#include "MainUI.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

void USettingsUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	btn_chooseCarac->OnClicked.AddDynamic(this , &USettingsUI::ShowChooseWidget);
	btn_Mainmenu->OnClicked.AddDynamic(this , &USettingsUI::ShowMainMenuWidget);
	btn_quit->OnClicked.AddDynamic(this , &USettingsUI::QuitGame);
	Btn_Back->OnClicked.AddDynamic(this , &USettingsUI::OnClickedBack);

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
}

void USettingsUI::ShowChooseWidget()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveAllViewportWidgets();
	}
	
	// 새로운 위젯을 화면에 추가합니다.
	if (MainMenuWidget)
	{
		UMainUI* WidgetInstance = CreateWidget<UMainUI>(GetWorld(), MainMenuWidget);
		if (WidgetInstance)
		{
			
			WidgetInstance->AddToViewport();
			WidgetInstance->WidgetSwitcher->SetActiveWidgetIndex(2);
		}
	}
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}

void USettingsUI::ShowMainMenuWidget()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveAllViewportWidgets();
	}
	
	// 새로운 위젯을 화면에 추가합니다.
	if (MainMenuWidget)
	{
		UMainUI* WidgetInstance = CreateWidget<UMainUI>(GetWorld(), MainMenuWidget);
		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport();
			WidgetInstance->WidgetSwitcher->SetActiveWidgetIndex(0);
		}
	}
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}

void USettingsUI::QuitGame()
{
	if (GEngine)
	{
		// Terminate the game session
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	}
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}

void USettingsUI::OnClickedBack()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		
		// Remove the widget from the viewport
		RemoveFromParent();

		PlayerController->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}
