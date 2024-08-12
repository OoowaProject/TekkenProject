// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacterController.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API AAICharacterController : public AAIController
{
	GENERATED_BODY()
	protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	public:
		AAICharacterController ();
		class UBehaviorTree* behaviortree;
		class UBehaviorTree* behaviortreePlayer2;
		class UBlackboardData* blackboardData;
		void SetBehaviorTree (int32 pNum);
};
