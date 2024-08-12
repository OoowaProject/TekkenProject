// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIStateInterface.h"
#include "BTTaskNode_ChangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTTaskNode_ChangeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	class AAICharacter* owner;
public:
	UBTTaskNode_ChangeAttack ( );

	virtual EBTNodeResult::Type ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory ) override;
	UPROPERTY(EditAnywhere, Category = "AI")
    TSubclassOf<class UAIStateComponent> newStateClass;

		UPROPERTY(EditAnywhere, Category = "AttackPoint")
    EAttackPoint attackPoint;
protected:
	virtual void TickTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds ) override;

private:
	UFUNCTION ()
	void OnStateCompleted ();
	bool bIsWaitingForState;
	class UBehaviorTreeComponent* cachedOwnerComp;
};

