// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "CPP_Tekken8CharacterParent.h"
#include <Kismet/GameplayStatics.h>

AAICharacterController::AAICharacterController ( )
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject ( TEXT ( "/Script/AIModule.BehaviorTree'/Game/LSJ/AI/BT_AI.BT_AI'" ) );
	if ( BTObject.Succeeded ( ) )
	{
		behaviortree = BTObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject2 ( TEXT ( "/Script/AIModule.BehaviorTree'/Game/LSJ/AI/BT_AI1.BT_AI1'" ) );
	if ( BTObject2.Succeeded ( ) )
	{
		behaviortreePlayer2 = BTObject2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject ( TEXT ( "/Script/AIModule.BlackboardData'/Game/LSJ/AI/BB_AI.BB_AI'" ) );
	if ( BBObject.Succeeded ( ) )
	{
		blackboardData = BBObject.Object;
	}
}

void AAICharacterController::SetBehaviorTree (int32 pNum)
{
	if ( pNum == 1 )
	{
		//카메라 우측이라면
		if ( !RunBehaviorTree ( behaviortree ) )
		{
			UE_LOG ( LogTemp , Error , TEXT ( "LSJ LSJ LSJ AIController couldn't run behavior tree!" ) );
		}
	}
	else
	{
		//카메라 좌측이라면
		if ( !RunBehaviorTree ( behaviortreePlayer2 ) )
		{
			UE_LOG ( LogTemp , Error , TEXT ( "LSJ LSJ LSJ AIController couldn't run behavior tree!" ) );
		}
	}
}

void AAICharacterController::OnPossess ( APawn* InPawn )
{
	Super::OnPossess ( InPawn );
	////카메라 좌측인지 오른쪽인지 체크
	//auto* aiOwner = Cast<ACPP_Tekken8CharacterParent> ( GetOwner ( ) );
	//float cameraDirection = FVector::DotProduct ( aiOwner->aMainCamera->GetActorRightVector ( ) , aiOwner->GetActorLocation ( ) );
	//if ( cameraDirection > 0 )
	//{
	//	//카메라 우측이라면
	//	if ( !RunBehaviorTree ( behaviortree ) )
	//	{
	//		UE_LOG ( LogTemp , Error , TEXT ( "LSJ LSJ LSJ AIController couldn't run behavior tree!" ) );
	//	}
	//}
	//else
	//{
	//	//카메라 좌측이라면
	//	if ( !RunBehaviorTree ( behaviortreePlayer2 ) )
	//	{
	//		UE_LOG ( LogTemp , Error , TEXT ( "LSJ LSJ LSJ AIController couldn't run behavior tree!" ) );
	//	}
	//}
}

void AAICharacterController::BeginPlay ( )
{
	Super::BeginPlay ( );
	
}

