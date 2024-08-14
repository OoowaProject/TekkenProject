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
	animInstace->bFalling = false;
	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsKnockDown" ) , false );
	currentTime = 0;
	animInstace->bFalling = false;
}

void UAIStateKnockDown::Execute ( const float& deltatime )
{
	currentTime += deltatime;
	if ( currentTime >= knockDownTime || WasHit )
	{
		WasHit = false;
		isKnockDown = false;
		animInstace->bKnockDown = false;
		owner->PlayAnimMontage ( animInstace->standUpMontage );
		currentTime = 0;
	}
}

void UAIStateKnockDown::Exit ( )
{
	//종료 조건
	//녹다운 상태일때 animInstace->standUpMontage 가 끝나면 실행
	
	WasHit = false;
	currentTime = 0;
	//animInstace->bKnockDown = false;
	Super::Exit ( );
}

