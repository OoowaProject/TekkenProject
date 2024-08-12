// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateHit.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateHit : public UAIStateComponent
{
	GENERATED_BODY()

public:
	void SetAttackInfo( FAttackInfoInteraction& pAttackInfo );
	virtual void Enter ( class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;
};

