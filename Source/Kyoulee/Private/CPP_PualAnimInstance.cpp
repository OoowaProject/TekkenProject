// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PualAnimInstance.h"
#include "CPP_CharacterPaul.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

void UCPP_PualAnimInstance::NativeInitializeAnimation ( )
{
	Super::NativeInitializeAnimation ( );

	if ( aOwner == nullptr )
	{
		aOwner = Cast<ACPP_CharacterPaul> ( TryGetPawnOwner ( ) );	// 소유자의 Pawn 를 가져온다.
	}

	OnMontageEnded.AddDynamic ( this , &UCPP_PualAnimInstance::HandleOnMontageEnded );
}

void UCPP_PualAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACPP_CharacterPaul* player = Cast<ACPP_CharacterPaul>(TryGetPawnOwner());

	if (nullptr == player)
		return;

//	FVector velocity = player->GetVelocity();
	FVector velocity = player->locationlenght;
	this->fVertical = FVector::DotProduct(player->GetActorForwardVector(), velocity);
	this->fHorizontal = FVector::DotProduct(player->GetActorRightVector(), velocity);

	this->bCrouch = player->bCrouched;
	this->bFalling = player->bFalling;
	this->bNuckDown = player->bNuckDown;
	this->fFallingValue = player->fFallingValue;
	
	this->bDead = player->bIsDead;
}

void UCPP_PualAnimInstance::HandleOnMontageEnded ( UAnimMontage* Montage , bool bInterrupted )
{
	aOwner->SetToWorldLocationPoint ( aOwner->GetActorLocation ( ) );
	aOwner->bMoveTo = true;
	aOwner->eCharacterState = ECharacterStateInteraction::GuardStand;
}
