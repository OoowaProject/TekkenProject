// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateRun.h"
#include "AICharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/CapsuleComponent.h"
void UAIStateRun::SetDistance ( float pDistance )
{
	//distance = pDistance;
}

void UAIStateRun::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	animInstace->bRun = true;
	attackRange = owner->GetBlackboardComponent ( )->GetValueAsFloat(TEXT("AttackRange" ));
	moveSpeed = 400.0f;
}

void UAIStateRun::Execute ( const float& deltatime )
{
	if(nullptr==owner)
		return;
	if( nullptr == owner->aOpponentPlayer)
		return;
	if ( animInstace->bRun == true)
	{
		owner->LookTarget ( deltatime );
		//owner->GetCharacterMovement()->AddInputVector(owner->GetActorForwardVector()*deltatime* moveSpeed * 1000000 );
			owner->GetCapsuleComponent ( )->AddRelativeLocation ( owner->GetActorForwardVector ( ) * deltatime * moveSpeed ); //( owner->GetActorForwardVector ( ) * deltatime * moveSpeed );

	}
	if(owner->GetBTWDistance()<=attackRange)
	{
		Exit();
		return;
	}
}

void UAIStateRun::Exit ( )
{
	animInstace->bRun = false;
	owner->GetBlackboardComponent ( )->SetValueAsEnum ( TEXT ( "ERandomAttack" ) , owner->ChangeAttackMotionDependingOpponentState ( ) );
	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "InAttackRange" ) , true );
	Super::Exit ( );
}
