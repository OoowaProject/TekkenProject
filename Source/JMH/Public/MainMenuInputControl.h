// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuInputControl.generated.h"

UCLASS()
class JMH_API AMainMenuInputControl : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainMenuInputControl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	
	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "EnhancedInput" )
	class UInputMappingContext* InputMapping;

	// Input action mappings for Player 1
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	TSubclassOf<class UInputMappingContext> Player1InputMappingContext;

	
	//1
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_MoveUP_P1;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_MoveDown_P1;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_MoveLeft_P1;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_MoveRight_P1;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_Select_P1;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_Cancle_P1;

	//2
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_MoveUP_P2;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_MoveDown_P2;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_MoveLeft_P2;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_MoveRight_P2;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_Select_P2;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="InputPlayer1")
	class UInputAction* IA_Cancle_P2;

	//1
	UFUNCTION()
	void OnMoveUpP1(const FInputActionValue& Value);

	UFUNCTION()
	void OnMoveDownP1(const FInputActionValue& Value);

	UFUNCTION()
	void OnMoveLeftP1(const FInputActionValue& Value);

	UFUNCTION()
	void OnMoveRightP1(const FInputActionValue& Value);

	UFUNCTION()
	void OnSelectP1(const FInputActionValue& Value);

	UFUNCTION()
	void OnCancelP1(const FInputActionValue& Value);

	//2
	UFUNCTION()
	void OnMoveUpP2(const FInputActionValue& Value);

	UFUNCTION()
	void OnMoveDownP2(const FInputActionValue& Value);

	UFUNCTION()
	void OnMoveLeftP2(const FInputActionValue& Value);

	UFUNCTION()
	void OnMoveRightP2(const FInputActionValue& Value);

	UFUNCTION()
	void OnSelectP2(const FInputActionValue& Value);

	UFUNCTION()
	void OnCancelP2(const FInputActionValue& Value);
	
	
};
