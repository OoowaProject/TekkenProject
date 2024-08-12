// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateWalkCross.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
void UAIStateWalkCross::Enter ( UAICharacterAnimInstance* pAnimInstance )
{	
	Super::Enter ( pAnimInstance );

	if ( nullptr==owner->aOpponentPlayer )
	{
		return;
	}
	//속도가 처음에 크고 끝에는 작아지게 하면 좋을 것 같다.
	//처음 적정 값 OrbitAroundTarget ( owner->aOpponentPlayer , 100.0f );
	OrbitAroundTarget ();
	//int randNum = FMath::RandRange(0,1);
	if ( false==bClockwise )
	{
		direction = -1.0f;
		animInstace->PlayCrossWalkCounterclockwiseMontage ( );
	}
		
	else
	{
		direction = 1.0f;
		animInstace->PlayCrossWalkClockwiseMontage ( );
	}
	owner->GetCurrentMontage ( )->GetSectionStartAndEndTime ( 0 ,startFrame,endFrame);
	//animInstace->StateWalkForward(true);

}

void UAIStateWalkCross::Execute ( const float& deltatime )
{
	if ( nullptr == owner||nullptr == owner->aOpponentPlayer)
	{
		return;
	}

	//원의 지름 이동
	MoveStep(deltatime);

	//상대방을 쳐다보자
	FVector opponentPlayerRotator = owner->aOpponentPlayer->GetMesh ( )->GetBoneLocation ((TEXT("head" )));
	opponentPlayerRotator.Z = owner->GetActorLocation ( ).Z;
	FRotator lookRotator = (opponentPlayerRotator - owner->GetActorLocation()).Rotation ( );
	owner->SetActorRotation ( FMath::RInterpTo ( owner->GetActorRotation ( ) , lookRotator , deltatime , 100.0f ) );
}

void UAIStateWalkCross::Exit ( )
{
	owner->SetStateIdle ( );
	//animInstace->StateWalkForward ( false );
	Super::Exit ( );
	
}

void UAIStateWalkCross::OrbitAroundTarget ()
{
	if ( !owner->aOpponentPlayer )
	{
		return;
	}
	// 적 캐릭터와 현재 캐릭터 사이의 거리로 반경을 설정
	targetCenter = owner->aOpponentPlayer->GetMesh ( )->GetBoneLocation ( (TEXT ( "head" )) );
	targetCenter.Z = owner->GetActorLocation ( ).Z;
	orbitRadius = FVector::Dist ( targetCenter , owner->GetActorLocation());

	// 현재 위치를 기준으로 초기 각도를 설정
	FVector Direction = (owner->GetActorLocation ( ) - targetCenter).GetSafeNormal ( );
	currentAngle = FMath::RadiansToDegrees ( FMath::Atan2 ( Direction.Y , Direction.X ) );
}

void UAIStateWalkCross::MoveStep ( float DeltaTime )
{
	if ( !owner->aOpponentPlayer )
	{
		return;
	}

	//속도가 처음에 크고 끝에는 작아지게 하면 좋을 것 같다.
	//최댓값이 330.0f 이고 거리에 따라 이 값이 줄어들게

	targetCenter = owner->aOpponentPlayer->GetMesh ( )->GetBoneLocation ( (TEXT ( "head" )) );
	targetCenter.Z = owner->GetActorLocation ( ).Z;
	// 거리가 최소 거리보다 작다면 최대 속도 사용
	if ( orbitRadius < minDistance)
	{
		orbitSpeed = maxSpeed;
	}
	// 거리가 최대 거리보다 크다면 최소 속도 사용
	else if ( orbitRadius >=maxDistance )
	{
		orbitSpeed = minSpeed;
	}
	// 그 중간 값이라면 거리 비율에 따라 보간
	else
	{
		float LerpFactor = (orbitRadius - minDistance) / (maxDistance - minDistance);
		orbitSpeed = FMath::Lerp ( maxSpeed , minSpeed , LerpFactor );
	}
	//속도가 처음에 크고 끝에는 작아지게 하면 좋을 것 같다.
	orbitSpeed =FMath::Lerp(orbitSpeed,0, startFrame+=DeltaTime);

	// 현재 각도를 시간에 따라 업데이트
	currentAngle += orbitSpeed * DeltaTime * direction;
	if ( currentAngle > 360.0f )
	{
		currentAngle -= 360.0f;
	}

	// 적 캐릭터와 현재 캐릭터 사이의 거리로 반경을 설정
	float RadAngle = FMath::DegreesToRadians (currentAngle);

	//최소 거리가 아니라면 상대플레이어에게 조금씩 가까이 이동
	if( orbitRadius > 115.0f )
		orbitRadius -= reduceRange * DeltaTime;

	//위치 테스트값
	//FVector orbitLocation = targetCenter + FVector ( FMath::Cos ( RadAngle )* 115 , FMath::Sin ( RadAngle) * 115 , 0.0f );
	//위치 원래값
	FVector orbitLocation = targetCenter + FVector ( FMath::Cos ( RadAngle ) * orbitRadius , FMath::Sin ( RadAngle ) * orbitRadius , 0.0f );
	//SetActorLocation을 사용하여 이동
	owner->SetActorLocation ( orbitLocation );
	//AddMovementInput 거리가 일정하게만 유지되고 여러 조절이 안됨
	//owner->AddMovementInput ( MoveDirection ,1.0f,true);
}