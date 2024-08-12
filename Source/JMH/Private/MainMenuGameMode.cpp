// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "MainMenuInputControl.h"
#include "MainUI.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

class UEnhancedInputLocalPlayerSubsystem;

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 첫 번째 플레이어의 컨트롤러
	if (Player1ControllerClass)
	{
		APlayerController* Player1Controller = GetWorld()->SpawnActor<APlayerController>(Player1ControllerClass);
		if (Player1Controller)
		{
			GetWorld()->GetFirstPlayerController()->UnPossess();
			GetWorld()->GetFirstPlayerController()->Possess(Cast<APawn>(Player1Controller));
		}
	}

	if (Player2ControllerClass)
	{
		APlayerController* Player2Controller = GetWorld()->SpawnActor<APlayerController>(Player2ControllerClass);
		if (Player2Controller)
		{
			// 적절한 플레이어 컨트롤러를 설정
			// 예를 들어, 스플릿 스크린 모드 등에서 사용될 수 있음
			if (UWorld* World = GetWorld())
			{
				for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
				{
					APlayerController* PC = Iterator->Get();
					if (PC && PC->GetLocalPlayer() && PC->GetLocalPlayer()->GetControllerId() == 1)
					{
						PC->UnPossess();
						PC->Possess(Player2Controller->GetPawn());
						break;
					}
				}
			}
		}
	}
}
