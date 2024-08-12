// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateHitFalling.h"
#include "CPP_Tekken8CharacterParent.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIStateBound.h"
#include "Components/BoxComponent.h"
#include "AIStateKnockDown.h"
void UAIStateHitFalling::SetAttackInfo ( FAttackInfoInteraction& pAttackInfo )
{
	attackInfoArray.Empty ( );
	attackInfoArray.Add ( pAttackInfo );
	currnetLocationZ = 0;
	maxLocationZ = owner->GetMesh()->GetSocketLocation(TEXT("root")).Z;
	minLocationZ = maxLocationZ;
}

void UAIStateHitFalling::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	//owner->collisionLower->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	animInstace->bKnockDown = false;
	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsKockDown" ) , false );
	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsHitFalling" ) , false );
	


	//FVector Direction = owner->GetActorLocation ( ) - (owner->aOpponentPlayer->GetActorLocation ( )+ owner->aOpponentPlayer->GetActorForwardVector()*-10000.f);
	//Direction.Z = 0.0f; // 수평 방향으로만 계산 (필요 시)

	//// 벡터 정규화
	//Direction.Normalize();

	//// 반전된 벡터에 강도 적용 //attackInfoArray[0].KnockBackDirection.X
	//FVector v = attackInfoArray[0].KnockBackFallingDirection;
	//v.X = 0;
	//v.Y = 0;
	////FVector LaunchVelocity = Direction * -1.f * 300.f * v;
	//FVector LaunchVelocity = Direction * -1.f * 1100.f + v;
	//if ( WasKnockDown )
	//{
	//	LaunchVelocity.Z = 0;
	//}
	// 상대방과 자신의 위치를 가져옵니다.
	FVector SelfLocation = owner->GetActorLocation ( );
	FVector TargetLocation = owner->aOpponentPlayer->GetActorLocation ( );

	// 상대방에서 자신의 위치로 향하는 벡터를 계산합니다.
	FVector LaunchDirection = SelfLocation - TargetLocation;

	// 벡터를 단위 벡터(길이가 1인 벡터)로 만듭니다.
	LaunchDirection.Normalize ( );
	// 원하는 속도 (100m/s)
	float LaunchSpeed = 500.0f;  // 1초에 100m 이동 (1m/s = 100cm/s)

	// 계산된 방향 벡터에 속도를 곱합니다.
	FVector LaunchVelocity = LaunchDirection * LaunchSpeed;


	//owner->LaunchCharacter ( FVector (0,0 ,400) , true , true );
	//owner->GetMesh ( )->AddForceAtLocation ( FVector ( 0 , -10 , 2000 ) ,owner->aOpponentPlayer->GetActorLocation ( ) );
	if ( WasKnockDown )
	{ //누워있을때 맞는 애니메이션 실행
		animInstace->StopAllMontages ( 0.5f );
		animInstace->PlayMontageAtFrameRate ( animInstace->hitKnockDownMontage,attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
	}
	//FAttackInfoInteraction attackInfo =  attackInfoArray[0], 
	else if (attackInfoArray.IsValidIndex ( 0 ) && attackInfoArray[0].hitMontage != nullptr )
	{
		animInstace->StopAllMontages ( 0.1f );
		owner->PlayAnimMontage ( attackInfoArray[0].hitMontage );
		//animInstace->PlayHitFallingMontage ( attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
	}
	else
	{
		animInstace->StopAllMontages ( 0.1f );
		animInstace->PlayHitFallingMontage ( attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
		//animInstace->PlayHitFallingTurnMontage ( attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
	}
	//owner->GetCharacterMovement ( )->AddImpulse ( attackInfoArray[0].KnockBackDirection * 100.0f , true );
	
	// 캐릭터를 날립니다. (Z축 방향으로는 원하는 높이를 추가할 수 있음)
	owner->LaunchCharacter ( LaunchVelocity , true , true );

	currentFrame = 0;
	targetFrame = attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame;
	isExitOneMore = false;
		//owner->GetCharacterMovement ( )->AddImpulse ( attackInfoArray[0].KnockBackDirection * 100.0f , true );
		//owner->LaunchCharacter ( attackInfoArray[0].KnockBackDirection , true , true );
}

void UAIStateHitFalling::Execute ( const float& deltatime )
{
	currnetLocationZ = owner->GetMesh ( )->GetSocketLocation ( TEXT ( "root" ) ).Z;
	if( maxLocationZ < currnetLocationZ )
		maxLocationZ = currnetLocationZ;

	//if ( false == owner->GetCharacterMovement ( )->IsFalling ( )&& maxLocationZ - minLocationZ > 5.f )
	//spine_01 의 위치가 -85.f 보다 작을 때 해당 애니메이션이 끝남

	if ( owner->GetMesh ( )->GetSocketLocation ( TEXT ( "spine_01" ) ).Z  < -85.f )
	{
		if ( !WasKnockDown )
			Exit();
	}
	currentFrame++;
	if ( targetFrame <= currentFrame && !isExitOneMore)
	{
		isExitOneMore = true;
		if ( WasKnockDown )
		{
			Exit ( );
		}
	
	}
}

void UAIStateHitFalling::Exit ( )
{
	if ( WasKnockDown )
	{
		owner->GetAIStateKnockDown ( )->WasHit = true;
		animInstace->bKnockDown = true;
		owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsKnockDown" ) , true );
		WasKnockDown = false;
	}
	else
	if ( maxLocationZ - minLocationZ > 10.f )
	{
		owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsBound" ) , true );
		owner->GetAIStateBound ( )->SetAttackInfo ( attackInfoArray[0] );
	}
	else
	{
		animInstace->bKnockDown = true;
		owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsKnockDown" ) , true );
	}
	Super::Exit ( );
}
