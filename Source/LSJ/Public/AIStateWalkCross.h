// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStateComponent.h"
#include "AIStateWalkCross.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAIStateWalkCross : public UAIStateComponent
{
	GENERATED_BODY()
	//FVector Center;
	//float RadAngle;
	float direction = 1.0f;
	float reduceRange = 20.f;
	float orbitRadius;
	float orbitSpeed;
	float currentAngle;
	float startFrame;
	float endFrame;
	FVector targetCenter;
	// 최소 거리와 최대 거리
	const float minDistance = 100.0f;
	const float maxDistance = 500.0f;

	// 최소 속도와 최대 속도
	const float minSpeed = 33.0f;
	const float maxSpeed = 150.0f;
	//FVector OrbitLocation;
public:
	UPROPERTY(EditAnywhere, Category = "AI")
	bool  bClockwise;
	virtual void Enter (class UAICharacterAnimInstance* pAnimInstance ) override;
	virtual void Execute ( const float& deltatime ) override;
	virtual void Exit ( ) override;

	void OrbitAroundTarget ();
	void MoveStep ( float DeltaTime );
};
