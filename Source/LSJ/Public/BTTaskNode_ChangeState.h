// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ChangeState.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTTaskNode_ChangeState : public UBTTaskNode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,Category="Distance",meta=(AllowPrivateAccess="true") )
	float distance;
	UPROPERTY(EditAnywhere,Category="WalkCross",meta=(AllowPrivateAccess="true") )
	bool bClockwise;
	UActorComponent* currentState;
public:
	UBTTaskNode_ChangeState ( );

	virtual EBTNodeResult::Type ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory ) override;
	UPROPERTY(EditAnywhere, Category = "AI")
    TSubclassOf<class UAIStateComponent> newStateClass;
protected:
	virtual void TickTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds ) override;

private:
	UFUNCTION ()
	void OnStateCompleted ();
	bool bIsWaitingForState;
	class UBehaviorTreeComponent* cachedOwnerComp;
};
