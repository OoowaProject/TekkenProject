// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UObject/SoftObjectPtr.h"
#include "InputMappingContext.h"
#include "CPP_InputControl.generated.h"


class UInputMappingContext;
class UInputAction;

UCLASS()
class KYOULEE_API ACPP_InputControl : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ACPP_InputControl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	// Input Mapping All
	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "EnhancedInput" )
	class UInputMappingContext* InputMapping;


	// Input Action Player1
	
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer1")
	UInputAction* IA_P1Up;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer1")
	UInputAction* IA_P1Down;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer1")
	UInputAction* IA_P1Back;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer1")
	UInputAction* IA_P1Foward;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer1")
	UInputAction* IA_P1LP;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer1")
	UInputAction* IA_P1RP;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer1")
	UInputAction* IA_P1LK;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer1")
	UInputAction* IA_P1RK;

	// Input Action Player2
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer2")
	UInputAction* IA_P2Up;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer2")
	UInputAction* IA_P2Down;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer2")
	UInputAction* IA_P2Back;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer2")
	UInputAction* IA_P2Foward;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer2")
	UInputAction* IA_P2LP;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer2")
	UInputAction* IA_P2RP;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer2")
	UInputAction* IA_P2LK;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly, Category="InputPlayer2")
	UInputAction* IA_P2RK;
	

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite )
	TSubclassOf<class ACPP_CharacterPaul> Player1Class;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite )
	TSubclassOf<class ACPP_CharacterPaul> Player2Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class ACPP_CharacterPaul* Player1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class ACPP_CharacterPaul* Player2 ;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite )
	TSubclassOf<class AAICharacter> PlayerAIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class AAICharacter* PlayerAI1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class AAICharacter* PlayerAI2 ;

	// player1 setting
	UFUNCTION()
	void InputP1Up(const FInputActionValue& Value);
	UFUNCTION()
	void InputP1Down(const FInputActionValue& Value);
	UFUNCTION()
	void InputP1Back(const FInputActionValue& Value);
	UFUNCTION()
	void InputP1Foward(const FInputActionValue& Value);
	UFUNCTION()
	void InputP1LP(const FInputActionValue& Value);
	UFUNCTION()
	void InputP1RP(const FInputActionValue& Value);
	UFUNCTION()
	void InputP1LK(const FInputActionValue& Value);
	UFUNCTION()
	void InputP1RK(const FInputActionValue& Value);

	// player2 setting
	UFUNCTION()
	void InputP2Up(const FInputActionValue& Value);
	UFUNCTION()
	void InputP2Down(const FInputActionValue& Value);
	UFUNCTION()
	void InputP2Back(const FInputActionValue& Value);
	UFUNCTION()
	void InputP2Foward(const FInputActionValue& Value);
	UFUNCTION()
	void InputP2LP(const FInputActionValue& Value);
	UFUNCTION()
	void InputP2RP(const FInputActionValue& Value);
	UFUNCTION()
	void InputP2LK(const FInputActionValue& Value);
	UFUNCTION()
	void InputP2RK(const FInputActionValue& Value);

};
