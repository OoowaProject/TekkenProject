// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateKnockDown.h"
#include "CPP_Tekken8CharacterParent.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIStateIdle.h"

void UAIStateKnockDown::SetAttackInfo ( FAttackInfoInteraction& pAttackInfo )
{
	attackInfoArray.Empty ( );
	attackInfoArray.Add ( pAttackInfo );
}

void UAIStateKnockDown::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsKnockDown" ) , false );
	currentTime = 0;
	isKnockDown = true;

}

void UAIStateKnockDown::Execute ( const float& deltatime )
{
	currentTime += deltatime;
	if ( currentTime >= knockDownTime || WasHit )
	{
		WasHit = false;
		owner->PlayAnimMontage ( animInstace->standUpMontage );
		currentTime = 0;
		isKnockDown = false;
	}
}

void UAIStateKnockDown::Exit ( )
{
	WasHit = false;
	isKnockDown = false;
	currentTime = 0;
	//animInstace->bKnockDown = false;
	//owner->ChangeState ( owner->GetAIStateIdle ( ) );
	Super::Exit ( );
}

