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
void UAIStateRun::SetDistance ( float pDistance )
{
	//distance = pDistance;
}

void UAIStateRun::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	animInstace->bRun = true;
	attackRange = owner->GetBlackboardComponent ( )->GetValueAsFloat(TEXT("AttackRange" ));
}

void UAIStateRun::Execute ( const float& deltatime )
{
	if(nullptr==owner)
		return;
	if( nullptr == owner->aOpponentPlayer)
		return;
	owner->LookTarget (deltatime);
	owner->GetCharacterMovement()->AddInputVector(owner->GetActorForwardVector()*deltatime* moveSpeed );

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
