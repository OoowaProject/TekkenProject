// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateWalkForward.generated.h"

/**
 * 
 */

UCLASS()
class LSJ_API UAIStateWalkForward : public UAIStateComponent
{
	GENERATED_BODY()
	FRotator lookPlayerRotator;
	float walkTime;
	float walkDashMinSpeed;
	float walkSpeed;
	FVector direction;
	float startFrame;
	float endFrame;

	float attackRange;
	bool isOneMore;
public:
	void SetDistance ( float pDistance );
	UPROPERTY(EditAnywhere, Category = "AI")
	float distance=200.0f;
	virtual void Enter (class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;

	//virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;
};
