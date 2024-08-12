// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateKnockDown.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateKnockDown : public UAIStateComponent
{
	GENERATED_BODY()
	float knockDownTime = 10.0f;
	float currentTime;
public:
	bool WasHit;
	bool isKnockDown;
	void SetAttackInfo ( FAttackInfoInteraction& pAttackInfo );
	virtual void Enter ( class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;
};

