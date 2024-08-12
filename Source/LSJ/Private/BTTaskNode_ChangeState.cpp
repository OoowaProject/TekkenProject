// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChangeState.h"
#include "AICharacter.h"
#include "AIController.h"
#include "AIStateBackDash.h"
#include "AIStateRun.h"
#include "AIStateWalkBack.h"
#include "AIStateWalkForward.h"
#include "AIStateIdle.h"
#include "AIStateHit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIStateHitFalling.h"
#include "AIStateBound.h"
#include "AIStateWalkCross.h"
#include "AIStateKnockDown.h"
UBTTaskNode_ChangeState::UBTTaskNode_ChangeState ( )
{
	
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_ChangeState::ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory )
{
	EBTNodeResult::Type Result = Super::ExecuteTask ( OwnerComp , NodeMemory );
	AAIController* aiController = OwnerComp.GetAIOwner ( );
	if ( aiController )
	{
		AAICharacter* Enemy = Cast<AAICharacter> ( aiController->GetPawn ( ) );
		if ( Enemy )
		{
			currentState = nullptr;

			// 적절한 상태 클래스에 따라 상태 변경
			if ( newStateClass == UAIStateWalkBack::StaticClass())
			{
				currentState = Enemy->GetAIStateWalkBack();
			}
			else if ( newStateClass == UAIStateWalkForward::StaticClass())
			{
				currentState = Enemy->GetAIStateWalkForward();
			}
			else if ( newStateClass == UAIStateBackDash::StaticClass())
			{
				currentState = Enemy->GetAIStateBackDash();
			}
			else if ( newStateClass == UAIStateHit::StaticClass ( ) )
			{
				currentState = Enemy->GetAIStateHit ( );
			}
			else if ( newStateClass == UAIStateHitFalling::StaticClass ( ) )
			{
				currentState = Enemy->GetAIStateHitFalling ( );
			}
			else if ( newStateClass == UAIStateBound::StaticClass ( ) )
			{
				currentState = Enemy->GetAIStateBound ( );
			}
			else if ( newStateClass == UAIStateWalkCross::StaticClass ( ) )
			{
				currentState = Enemy->GetAIStateWalkCross ( );
			}
			else if ( newStateClass == UAIStateRun::StaticClass ( ) )
			{
				currentState = Enemy->GetAIStateRun ( );
			}
			else if ( newStateClass == UAIStateKnockDown::StaticClass ( ) )
			{
				currentState = Enemy->GetAIStateKnockDown ( );
			}
			if ( currentState )
			{
				// 상태 완료시 호출될 델리게이트 바인딩
				if ( UAIStateWalkBack* stateWalkBack = Cast<UAIStateWalkBack> ( currentState ) )
				{
					stateWalkBack->SetDistance(distance);
					if ( !stateWalkBack->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
					stateWalkBack->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateWalkForward* stateWalkForward = Cast<UAIStateWalkForward> ( currentState ) )
				{
					stateWalkForward->SetDistance ( distance );
					if ( !stateWalkForward->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
					stateWalkForward->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateBackDash* stateBackDash = Cast<UAIStateBackDash> ( currentState ) )
				{
					if ( !stateBackDash->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
						stateBackDash->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateHit* stateHit = Cast<UAIStateHit> ( currentState ) )
				{
					if ( !stateHit->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
						stateHit->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateHitFalling* stateHitFalling = Cast<UAIStateHitFalling> ( currentState ) )
				{
					if ( !stateHitFalling->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
						stateHitFalling->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateBound* stateBound = Cast<UAIStateBound> ( currentState ) )
				{
					if ( !stateBound->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
						stateBound->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateRun* stateRun = Cast<UAIStateRun> ( currentState ) )
				{
					if ( !stateRun->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
						stateRun->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateWalkCross* stateWalkCross = Cast<UAIStateWalkCross> ( currentState ) )
				{
					stateWalkCross->bClockwise = bClockwise;
					if ( !stateWalkCross->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
						stateWalkCross->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateKnockDown* stateKnockDown = Cast<UAIStateKnockDown> ( currentState ) )
				{
					if ( !stateKnockDown->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
						stateKnockDown->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				bIsWaitingForState = true;
				cachedOwnerComp = &OwnerComp;
				Enemy->ChangeState ( Cast<IAIStateInterface> ( currentState ) );
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTaskNode_ChangeState::TickTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds )
{
	Super::TickTask ( OwnerComp , NodeMemory , DeltaSeconds );

	// 상태 완료 대기 중일 때는 Tick을 계속 호출
	if ( !bIsWaitingForState )
	{
		FinishLatentTask ( OwnerComp , EBTNodeResult::Succeeded );
	}
}

void UBTTaskNode_ChangeState::OnStateCompleted ( )
{
	bIsWaitingForState = false;

}
