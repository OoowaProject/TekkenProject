// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraPawn.h"

#include "GameMode_MH.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Tekken8CharacterParent.h"
// Sets default values
APlayerCameraPawn::APlayerCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComp);
	//카메라
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void APlayerCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	gm = Cast<AGameMode_MH>(GetWorld()->GetAuthGameMode());
	//월드에 있는 플레이어 변수에 적용 // 플레이어 A,B
	if (gm)
	{
		playerA = gm->playerA;
		playerB = gm->playerB;
	}
	// 초기화
	DistanceThreshold = 350.0f; // 이상의 변화가 있을 때만 arm 길이 조정

	if (playerA && playerB)
	{
		InitialDirection = (playerA->GetActorLocation() - playerB->GetActorLocation()).GetSafeNormal();
		PreviousDirection = InitialDirection; // 이전 방향 초기화
	}
}

void APlayerCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//플레이어가 호출하는 상황 일 때
	if (bIsZoomActive)
	{
		// 줌 시간이 경과했는지 확인
		ZoomElapsedTime += DeltaTime;
		if (ZoomElapsedTime >= ZoomDuration)
		{
			// 플레이어들 간의 거리 계산

			// 거리 변화가 임계값을 초과하는지 확인
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("11111111"));
			FVector playerALoc1 = playerA->GetActorLocation();
			FVector playerBLoc1 = playerB->GetActorLocation();
			originalLocation = (playerALoc1 + playerBLoc1)*.5f;
			// 원래 위치로 부드럽게 전환
			float playerDistance = FVector::Dist(playerALoc1, playerBLoc1);
			
			targetArmLength1 = FMath::Clamp(250, MinDistance , MaxDistance);

			//Lerp를 사용하여 카메라 줌을 부드럽게 변경
			//newArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength , targetArmLength1 , DeltaTime , CameraLagSpeed);
			//SpringArmComp->TargetArmLength = newArmLength;
			
			// 카메라 위치도 원래 위치로 부드럽게 전환
			FVector newLocation1 = FMath::VInterpTo(GetActorLocation(),originalLocation,
												   DeltaTime , CameraLagSpeed);
			SetActorLocation(newLocation1 + FVector(0 , 0 , 1.58));

			//카메라 줌
			float newArmLength1 = FMath::FInterpTo(SpringArmComp->TargetArmLength ,
												   targetArmLength1 , DeltaTime ,
												   CameraLagSpeed);
			// 전환이 완료된 것으로 간주할 수 있는 허용 범위 정의
			float armLengthTolerance = 1.f; // 허용 오차, 필요에 따라 조정
			float locationTolerance = 100.f; // 허용 오차, 필요에 따라 조정
			ShakingValue = 0.0f;
			if (FMath::Abs(SpringArmComp->TargetArmLength - targetArmLength1) < armLengthTolerance)
			{
				// 전환이 완료되면 수치 초기화
				ResetZoomEffect();
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("!!!!!!!!"));
			}
			else
			{
				// 전환을 계속 진행
				SpringArmComp->TargetArmLength = newArmLength1;
			}
		}
		
		else{
		//플레이이어가 호출하면
		//GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("bIsZoomActive!"));
		// 줌 효과 적용
			//
			FVector playerALoc1 = playerA->GetActorLocation();
			FVector playerBLoc1 = playerB->GetActorLocation();
			// 원래 위치로 부드럽게 전환
			float playerDistance = FVector::Dist(playerALoc1, playerBLoc1);
//
			float targetArmLength = FMath::Clamp((baseArmLength + playerDistance * ZoomAmount), MinDistance , MaxDistance);
		SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength , targetArmLength , DeltaTime ,CameraLagSpeed);

		//카메라 쉐이크
		FVector shakeOffset = FVector::ZeroVector;
		if (ShakingValue > 0.0f)
		{
			shakeOffset = FMath::VRand() * ShakingValue;
		}

		FVector cameraLocation = FMath::VInterpTo(GetActorLocation() , ZoomTargetLocation + shakeOffset , DeltaTime ,
		                                          CameraLagSpeed);
		SetActorLocation(cameraLocation);

		}
	}

	else
	{
		UpdateCameraDynamic(DeltaTime);
	}
}

void APlayerCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCameraPawn::UpdateCameraDynamic(float DeltaTime)
{
	if (!playerA || !playerB) return;

	// 플레이어들 간의 거리 계산
	float playerDistance = FVector::Dist(playerALoc , playerBLoc);
	// 거리 변화가 임계값을 초과하는지 확인
	float distanceChange = FMath::Abs(playerDistance);

	FString PlayerDistanceString = FString::Printf(TEXT("Player Distance: %.2f"), playerDistance);

	// 화면에 디버그 메시지로 출력
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, PlayerDistanceString);
	
    //두 플레이어의 거리
	playerALoc = playerA->GetActorLocation();
	playerBLoc = playerB->GetActorLocation();
	/*
		if (bIsZoomActive)
		{
			//플레이이어가 호출하면
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("bIsZoomActive!"));
			// 줌 효과 적용
			float targetArmLength = FMath::Clamp((baseArmLength * ZoomAmount)*.15f, MinDistance , MaxDistance);
			PlayerArmLength = SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength , targetArmLength , DeltaTime ,
			                                                  CameraLagSpeed);
	
			//카메라 쉐이크
			FVector shakeOffset = FVector::ZeroVector;
			if (ShakingValue > 0.0f)
			{
				shakeOffset = FMath::VRand() * ShakingValue;
			}
			
			FVector cameraLocation = FMath::VInterpTo(GetActorLocation() , ZoomTargetLocation + shakeOffset , DeltaTime ,
			                                          CameraLagSpeed);
			SetActorLocation(cameraLocation);
		}
		else
		{*/
	//플레이어들 중앙값
	centralLocation = (playerALoc + playerBLoc) * 0.5f; // + playerBLoc; //그게 그거인듯
	// 카메라의 위치 업데이트
	SetActorLocation(centralLocation + FVector(0 , 0 , 20));

	//카메라 회전
	FVector currentDirection = (playerALoc - playerBLoc).GetSafeNormal();
	currentDirection.Z = 0;
	currentDirection.Normalize();
	float angleDifference = FMath::RadiansToDegrees(
		FMath::Acos(FVector::DotProduct(PreviousDirection , currentDirection)));

	// 목표 회전 계산
	FRotator targetRotation = currentDirection.Rotation() + FRotator(0 , 90 , 0);
	// 현재 회전값 가져오기
	FRotator currentRotation = GetActorRotation();

	//카메라 회전각 제어(180도 무시)
	if (angleDifference < 170.0f || angleDifference > 190.0f)
	{
		if (!bIsRotationFixed)
		{
			// 목표 회전을 부드럽게 보간
			FRotator newRotation = FMath::RInterpTo(currentRotation , targetRotation , DeltaTime ,
			                                        CameraLagRotSpeed);
			SetActorRotation(newRotation);

			PreviousDirection = currentDirection;
		}
	}
	else
	{
		bIsRotationFixed = true;
	}

	if (FMath::Abs(angleDifference) < 90.0f)
	{
		bIsRotationFixed = false;
	}

	if (distanceChange > DistanceThreshold)
	{
		CameraZoom(DeltaTime , playerDistance);
	}
	//}
}

void APlayerCameraPawn::CameraZoom(float DeltaTime , float playerDistance)
{
	//플레이어 둘다
	//float targetArmLength = FMath::Clamp(baseArmLength + playerDistance * ZoomAmount , MinDistance , MaxDistance);
	//GEngine->AddOnScreenDebugMessage(-2 , 5.f , FColor::Red , TEXT("CameraZoom!"));
	float targetArmLength = FMath::Clamp(baseArmLength + playerDistance * ZoomAmount , MinDistance , MaxDistance);

	//Lerp를 사용하여 카메라 줌을 부드럽게 변경
	newArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength , targetArmLength , DeltaTime , CameraLagSpeed);
	SpringArmComp->TargetArmLength = newArmLength;
}

void APlayerCameraPawn::RequestZoomEffect(FVector TargetLocation , float InZoomAmount , float InShakingValue ,
                                          float InDuration)
{
	ZoomTargetLocation = TargetLocation;
	ZoomAmount = InZoomAmount;
	ShakingValue = InShakingValue;
	ZoomDuration = InDuration;
	ZoomElapsedTime = 0.0f;
	currentArmLength = SpringArmComp->TargetArmLength;
	//originalLocation = playerALoc+playerBLoc/2;
	centralRotation = SpringArmComp->GetTargetRotation();
	bIsZoomActive = true;
}

void APlayerCameraPawn::ResetZoomEffect()
{
	ZoomAmount = 0.5f;
	ShakingValue = 0.0f;
	ZoomDuration = 0.0f;
	ZoomElapsedTime = 0.0f;
	//SpringArmComp->TargetArmLength = currentArmLength;
	bIsZoomActive = false;
}