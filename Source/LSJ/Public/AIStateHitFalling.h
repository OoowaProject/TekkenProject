// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateHitFalling.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateHitFalling : public UAIStateComponent
{
	GENERATED_BODY()
	float maxLocationZ;
	float currnetLocationZ;
	float minLocationZ;
	int32 currentFrame = 0;
	int32 targetFrame = 0;
	bool isExitOneMore;

public:
	bool WasKnockDown;
	void SetAttackInfo ( FAttackInfoInteraction& pAttackInfo );
	virtual void Enter ( class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;
};

