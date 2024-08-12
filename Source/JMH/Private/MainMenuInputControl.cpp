// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuInputControl.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainMenuInputControl::AMainMenuInputControl()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainMenuInputControl::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController> ( this->GetController() );
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( PlayerController->GetLocalPlayer ( ) );
	Subsystem->ClearAllMappings ( );
	Subsystem->AddMappingContext ( InputMapping , 0 );

	
	
}

// Called every frame
void AMainMenuInputControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainMenuInputControl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
	Input->BindAction ( IA_MoveUP_P1 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnMoveUpP1 );
	Input->BindAction ( IA_MoveDown_P1 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnMoveDownP1 );
	Input->BindAction ( IA_MoveLeft_P1 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnMoveLeftP1 );
	Input->BindAction ( IA_MoveRight_P1 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnMoveRightP1 );
	Input->BindAction ( IA_Select_P1 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnSelectP1 );
	Input->BindAction ( IA_Cancle_P1 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnCancelP1 );

	Input->BindAction ( IA_MoveUP_P2 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnMoveUpP2 );
	Input->BindAction ( IA_MoveDown_P2 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnMoveDownP2 );
	Input->BindAction ( IA_MoveLeft_P2 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnMoveLeftP2 );
	Input->BindAction ( IA_MoveRight_P2 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnMoveRightP2 );
	Input->BindAction ( IA_Select_P2 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnSelectP2 );
	Input->BindAction ( IA_Cancle_P2 , ETriggerEvent::Triggered , this , &AMainMenuInputControl::OnCancelP2 );

}

void AMainMenuInputControl::OnMoveUpP1(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnMoveDownP1(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnMoveLeftP1(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnMoveRightP1(const FInputActionValue& Value)
{
}


void AMainMenuInputControl::OnSelectP1(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnCancelP1(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnMoveUpP2(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnMoveDownP2(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnMoveLeftP2(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnMoveRightP2(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnSelectP2(const FInputActionValue& Value)
{
	
}

void AMainMenuInputControl::OnCancelP2(const FInputActionValue& Value)
{
	
}



