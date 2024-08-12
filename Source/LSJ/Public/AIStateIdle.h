// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateIdle.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateIdle : public UAIStateComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay ( ) override;
public:
	virtual void Enter ( class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;
};
