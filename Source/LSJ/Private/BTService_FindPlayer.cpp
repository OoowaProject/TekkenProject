// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindPlayer.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AICharacter.h"
#include "AICharacterController.h"
UBTService_FindPlayer::UBTService_FindPlayer ( )
{
	NodeName = TEXT ( "FindPlayer" );
	Interval = 0.5f;
}

void UBTService_FindPlayer::TickNode ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds )
{
	AAICharacter* owner = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr==owner)
		return;
	if ( nullptr == owner->aOpponentPlayer )
		return;
	OwnerComp.GetBlackboardComponent ( )->SetValueAsObject ( TEXT("Player") , owner->aOpponentPlayer);
}
