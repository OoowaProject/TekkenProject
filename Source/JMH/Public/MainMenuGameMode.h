// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class JMH_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMainUI> MainUI;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TSubclassOf<class AMainMenuInputControl> Player1ControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TSubclassOf<class AMainMenuInputControl> Player2ControllerClass;
	
};
