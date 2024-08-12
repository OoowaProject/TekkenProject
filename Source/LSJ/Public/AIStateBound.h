// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateBound.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateBound : public UAIStateComponent
{
	GENERATED_BODY()
	
	float maxLocationZ;
	float currnetLocationZ;
	float minLocationZ;
public:
	void SetAttackInfo ( FAttackInfoInteraction& pAttackInfo );
	virtual void Enter ( class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;
};

