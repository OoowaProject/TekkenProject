// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SelectMove.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AICharacterController.h"

UBTTaskNode_SelectMove::UBTTaskNode_SelectMove ( )
{
	NodeName = TEXT ( "SelectMove" );
}

EBTNodeResult::Type UBTTaskNode_SelectMove::ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory )
{
	AAICharacter* owner = Cast<AAICharacter> ( OwnerComp.GetAIOwner ( )->GetPawn ( ) );
	if ( nullptr == owner )
		return EBTNodeResult::Failed;
	if ( nullptr == owner->aOpponentPlayer )
		return EBTNodeResult::Failed;
	int32 randomAttackIndex;
	float attackRange;
	float WalkForwardRange = 132;
	//공격할 모션을 랜덤으로 공격전에 미리 선택
	randomAttackIndex = FMath::RandRange ( 1 , 3 );
	switch ( randomAttackIndex )
	{
	case 1: //상단
		attackRange = 95.0f;
		break;
	case 2: //중단
		attackRange = 155.0f;
		break;
	case 3: //하단
		attackRange = 97.0f;
		break;
	default:
		attackRange = 100.0f;
		break;
	}

	float distance = owner->GetBTWDistance ( );
	//공격거리안에 상대가 있다면 
	// //공격
	// //공격 후 결과는 hit blocked miss
	if ( distance <= attackRange )
	{
		randomAttackIndex = owner->ChangeAttackMotionDependingOpponentState ( );
		OwnerComp.GetBlackboardComponent ( )->SetValueAsBool( TEXT ( "InAttackRange" ) , true );
		OwnerComp.GetBlackboardComponent ( )->SetValueAsEnum ( TEXT ( "EMoveState" ) , 0 );
	}
	//공격거리 + 대시거리 보다 거리가 크다면 
	// //달리기 중 해당 공격 모션의 공격거리가 되었을때 플레이어가 앉은 상태인지 공중상태인지를 판단하여 공격 모션이 달라진다. - AICharacter.cpp
	else if ( distance > WalkForwardRange + attackRange )
	{
		OwnerComp.GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "InAttackRange" ) , false );
		//Run 상태
		OwnerComp.GetBlackboardComponent ( )->SetValueAsEnum ( TEXT ( "EMoveState" ) , 1 );
	}
	//공격거리 + 대시거리 보다 거리가 작거나 같다면 
	// //대시 후 해당 공격 모션의 공격거리가 되었을때 플레이어가 앉은 상태인지 공중상태인지를 판단하여 공격 모션이 달라진다. - AICharacter.cpp
	// //대시 후 해당 공격 모션의 공격거리가 닿지 않는다면 한번 더 대시 한다.
	// //한번 더 대시 한 경우 랜덤으로 앉아 가드(일단 서서 가드) 하거나 공격( 플레이어가 앉은 상태인지 공중상태인지를 판단하여 공격 모션이 달라진다. - AICharacter.cpp)한다.
	else if ( distance <= WalkForwardRange + attackRange )
	{
		OwnerComp.GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "InAttackRange" ) , false );
		//DashForward 상태
		OwnerComp.GetBlackboardComponent ( )->SetValueAsEnum ( TEXT ( "EMoveState" ) , 2 );
	}

	//공격 불가능

	////공격 가능 //여러곳에서 쓰일 수 도 있지만 일단은 쓰자
	//if ( owner->aOpponentPlayer->eCharacterState == ECharacterStateInteraction::Air )
	//{
	//	//공중 상태일때 상단공격
	//	randomAttackIndex = 0;
	//}
	//else if ( owner->aOpponentPlayer->eCharacterState == ECharacterStateInteraction::Sit )
	//{
	//	//앉은 상태일때 중단 혹은 하단 공격
	//	randomAttackIndex = FMath::RandRange ( 1 , 2 );
	//}
	//else
	//{
	//	//스탠드 상태일때 랜덤 공격
	//	randomAttackIndex = FMath::RandRange ( 0 , 2 );
	//}


	OwnerComp.GetBlackboardComponent ( )->SetValueAsEnum ( TEXT ( "ERandomAttack" ) , randomAttackIndex );
	OwnerComp.GetBlackboardComponent ( )->SetValueAsFloat ( TEXT ( "AttackRange" ) , attackRange );
	return EBTNodeResult::Succeeded;
}
