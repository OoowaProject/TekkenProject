// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateRun.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateRun : public UAIStateComponent
{
	GENERATED_BODY()
	float moveSpeed = 200.0f;
	float attackRange;
public:
	void SetDistance ( float pDistance );
	UPROPERTY(EditAnywhere, Category = "AI")
	float distance=200.0f;

	virtual void Enter (class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;

	//virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;
};
