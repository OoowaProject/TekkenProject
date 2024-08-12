// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Distance.h"
#include "AICharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Tekken8CharacterParent.h"

UBTDecorator_Distance::UBTDecorator_Distance()
{
	NodeName = TEXT ( "DistancePlayer" );
}


bool UBTDecorator_Distance::CalculateRawConditionValue ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory ) const
{
	bool bResult = Super::CalculateRawConditionValue ( OwnerComp , NodeMemory );

	auto* ControllingPawn =Cast<ACPP_Tekken8CharacterParent>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return false;
	
	auto* Target = ControllingPawn->aOpponentPlayer;
	if ( nullptr == Target )
		return false;
	/*TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld ( ) , ACharacter::StaticClass ( ) , outActors );*/
	//UE_LOG(LogTemp,Error,TEXT("%f" ), ControllingPawn->GetDistanceTo ( Target ) );
	bResult = (ControllingPawn->GetDistanceTo(Target) <= distance && ControllingPawn->GetDistanceTo ( Target )>minDistance);
	return bResult;
}
