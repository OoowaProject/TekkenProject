// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateBackDash.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateBackDash : public UAIStateComponent
{
	GENERATED_BODY()
	FVector dashLocation;
	FVector startLocation;
	float lerpAlpha;
	FRotator lookPlayerRotator;
public:
	virtual void Enter (class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;
	virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;
};
