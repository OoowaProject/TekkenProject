// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCameraPawn.generated.h"

UCLASS()
class JMH_API APlayerCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* RootComp;
	
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ACPP_Tekken8CharacterParent* playerA;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ACPP_Tekken8CharacterParent* playerB;

	FVector playerALoc;
	FVector playerBLoc;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float baseArmLength = 50.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float currentArmLength;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinDistance = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxDistance = 400.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraLagSpeed = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraLagRotSpeed = 15.f;
	
	UFUNCTION()
	void UpdateCameraDynamic(float DeltaTime);
	
	UFUNCTION()
	void CameraZoom(float DeltaTime, float playerDistance);

public:
	
	FVector InitialDirection;
	FVector PreviousDirection;
	float DistanceThreshold; // 거리 변화 임계값
	float RotationThreshold = 1.0f;// 회전 변화 임계값
	float newArmLength;
	
	bool bIsRotationFixed;

	UPROPERTY()
	class AGameMode_MH* gm;

	UFUNCTION()
	void RequestZoomEffect(FVector TargetLocation, float InZoomAmount, float InShakingValue, float InDuration);

	UFUNCTION()
	void ResetZoomEffect();
	
	// 타이머 핸들
	FTimerHandle ZoomEffectTimerHandle;
	// 줌 효과 지속 시간
	float EffectDuration;
	
	
public:
	FVector ZoomTargetLocation;

	float initZoomAmount = 0.5f;
	float ZoomAmount = initZoomAmount;
	float ShakingValue = 0.0f;
	bool bIsZoomActive= false;
	float ZoomDuration = 0.0f;
	float ZoomElapsedTime = 0.0f;


	float zoomtime = 0;
	
	float targetArmLength1;
	
	float  PlayerArmLength;
	
	// 카메라 위치도 원래 위치로 부드럽게 전환
	UPROPERTY()
	FVector originalLocation ;
	UPROPERTY()
	FVector centralLocation;

	UPROPERTY()
	FRotator centralRotation;

	


};
