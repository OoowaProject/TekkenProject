// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InputControl.h"

#include "AICharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CPP_CharacterPaul.h"

// Sets default values
ACPP_InputControl::ACPP_InputControl()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACPP_InputControl::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController> ( this->GetController() );

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( PlayerController->GetLocalPlayer ( ) );

	Subsystem->ClearAllMappings ( );
	Subsystem->AddMappingContext ( InputMapping , 0 );

//	if ( this->Player1Class )
//	{
//		this->Player1 = this->GetWorld ( )->SpawnActor<ACPP_CharacterPaul> ( this->Player1Class , FVector ( 0 , -300 , 0 ) , FRotator ( 0 , 90 , 0 ) );
//		
//	}
//	if ( this->Player2Class )
//	{
//		this->Player2 = this->GetWorld ( )->SpawnActor<ACPP_CharacterPaul> ( this->Player2Class , FVector ( 0 , 300 , 0 ) , FRotator ( 0 , -90 , 0 ) );
//		if (this->Player1)
//		{
//			this->Player1->aOpponentPlayer = Player2;
//			this->Player2->aOpponentPlayer = Player1;
//		}
//	}

//	if (!this->Player1Class)
//	{
//		GetWorld()->SpawnActor<AAICharacter>( PlayerAIClass , FVector ( 0 , -300 , 0 ) , FRotator ( 0 , 90 , 0 ));
//	}
//	if (!this->Player2Class)
//	{
//		GetWorld()->SpawnActor<AAICharacter>( PlayerAIClass , FVector ( 0 , 300 , 0 ) , FRotator ( 0 , -90 , 0 ) );
//	}
}

// Called every frame
void ACPP_InputControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	FVector vMiddle = (Player2->GetActorLocation ( ) - Player1->GetActorLocation ( )) / 2;
// 	FVector vPointMiddle = vMiddle + Player1->GetActorLocation ( );
	//this->SetActorLocationAndRotation(vPointMiddle, Player1->GetActorRotation() + FRotator(0,90,0));
	//this->SetActorLocation ( this->GetActorForwardVector() * vMiddle.Length() );
}

// Called to bind functionality to input
void ACPP_InputControl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 여기에서 'ETriggerEvent' 열거형 값을 변경하여 원하는 트리거 이벤트를 바인딩할 수 있습니다.
	//Input->BindAction ( IA_P1Down , ETriggerEvent::Triggered , this , &ACPP_InputControl::SomeCallbackFunc );

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
	Input->BindAction ( IA_P1Up , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP1Up );
	Input->BindAction ( IA_P1Down , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP1Down );
	Input->BindAction ( IA_P1Back , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP1Back );
	Input->BindAction ( IA_P1Foward , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP1Foward );
	Input->BindAction ( IA_P1LP , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP1LP );
	Input->BindAction ( IA_P1RP , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP1RP );
	Input->BindAction ( IA_P1LK , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP1LK );
	Input->BindAction ( IA_P1RK , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP1RK );
	Input->BindAction ( IA_P2Up , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP2Up );
	Input->BindAction ( IA_P2Down , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP2Down );
	Input->BindAction ( IA_P2Back , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP2Back );
	Input->BindAction ( IA_P2Foward , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP2Foward );
	Input->BindAction ( IA_P2LP , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP2LP );
	Input->BindAction ( IA_P2RP , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP2RP );
	Input->BindAction ( IA_P2LK , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP2LK );
	Input->BindAction ( IA_P2RK , ETriggerEvent::Triggered , this , &ACPP_InputControl::InputP2RK );
}

// 
// void ACPP_InputControl::Shoot ( const FInputActionValue& Value )
// {
// 	// 여기에서 원하는 타입의 입력 액션 값을 가져옵니다...;
// 	bool BoolValue = Value.Get<bool> ( );
// 
// 	// 여기서 멋진 작업을 수행하세요!
// 	UE_LOG ( LogTemp , Warning , TEXT ( "test %d" ) , BoolValue );
// 
// }


// *******
// 플레이어 1
// *******


/**
 * @title player1 input
 */
void ACPP_InputControl::InputP1Up ( const FInputActionValue& Value )
{
	 bool BoolValue = Value.Get<bool> ( );

	 if (!this->Player1)
		return;
	 // UE_LOG ( LogTemp , Warning , TEXT ( "InputP1Up %d" ) , BoolValue );

	FInputKey key = this->Player1->GetCurrInputKey();
	key.bKey8 = BoolValue;
	this->Player1->SetCurrInputKey(key);
}

void ACPP_InputControl::InputP1Down ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );

	if ( !this->Player1 )
		return;
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP1Down %d" ) , BoolValue );

	FInputKey key = this->Player1->GetCurrInputKey ( );
	key.bKey2 = BoolValue;
	this->Player1->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP1Back ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );
	if ( !this->Player1 )
		return;
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP1Left %d" ) , BoolValue );

	FInputKey key = this->Player1->GetCurrInputKey ( );
	key.bKey4 = BoolValue;
	this->Player1->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP1Foward ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );

	if ( !this->Player1 )
		return;
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP1Right %d" ) , BoolValue );

	FInputKey key = this->Player1->GetCurrInputKey ( );
	key.bKey6 = BoolValue;
	this->Player1->SetCurrInputKey ( key );

}

void ACPP_InputControl::InputP1LP ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );

	if ( !this->Player1 )
		return;
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP1LP %d" ) , BoolValue );

	FInputKey key = this->Player1->GetCurrInputKey ( );
	key.bLeftPunch = BoolValue;
	this->Player1->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP1RP ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );

	if ( !this->Player1 )
		return;
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP1RP %d" ) , BoolValue );

	FInputKey key = this->Player1->GetCurrInputKey ( );
	key.bRightPunch = BoolValue;
	this->Player1->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP1LK ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );

	if ( !this->Player1 )
		return;
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP1LK %d" ) , BoolValue );

	FInputKey key = this->Player1->GetCurrInputKey ( );
	key.bLeftKick = BoolValue;
	this->Player1->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP1RK ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );

	if ( !this->Player1 )
		return;
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP1RK %d" ) , BoolValue );

	FInputKey key = this->Player1->GetCurrInputKey ( );
	key.bRightKick = BoolValue;
	this->Player1->SetCurrInputKey ( key );
}


// *******
// 플레이어 2
// *******

/**
 * @title player2 input
 */
void ACPP_InputControl::InputP2Up ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP2Up %d" ) , BoolValue );

	if ( !this->Player2 )
		return;

	FInputKey key = this->Player2->GetCurrInputKey ( );
	key.bKey8 = BoolValue;
	this->Player2->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP2Down ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP2Down %d" ) , BoolValue );

	if ( !this->Player2 )
		return;

	FInputKey key = this->Player2->GetCurrInputKey ( );
	key.bKey2 = BoolValue;
	this->Player2->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP2Back ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP2Left %d" ) , BoolValue );

	if ( !this->Player2 )
		return;

	FInputKey key = this->Player2->GetCurrInputKey ( );
	key.bKey4 = BoolValue;
	this->Player2->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP2Foward ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP2Right %d" ) , BoolValue );

	if ( !this->Player2 )
		return;
	FInputKey key = this->Player2->GetCurrInputKey ( );
	key.bKey6 = BoolValue;
	this->Player2->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP2LP ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP2LP %d" ) , BoolValue );

	if ( !this->Player2 )
		return;

	FInputKey key = this->Player2->GetCurrInputKey ( );
	key.bLeftPunch = BoolValue;
	this->Player2->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP2RP ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP2RP %d" ) , BoolValue );

	if ( !this->Player2 )
		return;
	FInputKey key = this->Player2->GetCurrInputKey ( );
	key.bRightPunch = BoolValue;
	this->Player2->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP2LK ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP2LK %d" ) , BoolValue );

	if ( !this->Player2 )
		return;
	FInputKey key = this->Player2->GetCurrInputKey ( );
	key.bLeftKick = BoolValue;
	this->Player2->SetCurrInputKey ( key );
}

void ACPP_InputControl::InputP2RK ( const FInputActionValue& Value )
{
	bool BoolValue = Value.Get<bool> ( );
	// UE_LOG ( LogTemp , Warning , TEXT ( "InputP2RK %d" ) , BoolValue );

	if ( !this->Player2 )
		return;
	FInputKey key = this->Player2->GetCurrInputKey ( );
	key.bRightKick = BoolValue;
	this->Player2->SetCurrInputKey ( key );
}
