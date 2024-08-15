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
	animInstace->bOnGround = false;
	animInstace->bKnockDown = true;
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
	//owner->GetCapsuleComponent ( )->AddForce ( FVector ( 0.0f , 0.0f , 500000000000000.0f ) , NAME_None , true );

	//owner->LaunchCharacter ( FVector (0,0 ,40000000) , true , true );
	//owner->GetMesh ( )->AddForceAtLocation ( FVector ( 0 , -10 , 2000 ) ,owner->aOpponentPlayer->GetActorLocation ( ) );
	

	//FVector knockbackDirection = owner->GetActorForwardVector ( ) * -1.0f * 10000.0f * owner->GetCapsuleComponent ( )->GetMass ( );
	FVector knockbackDirection = owner->GetActorForwardVector ( ) * -1.0f * 20000.0f * owner->GetCapsuleComponent ( )->GetMass ( );
	// 기본
	// knockbackDirection.Z = 20000.0f * owner->GetCapsuleComponent ( )->GetMass ( );
	knockbackDirection.Z = 110000.0f * owner->GetCapsuleComponent ( )->GetMass ( ) * 1.2f;
	//owner->GetCapsuleComponent ( )->AddForce ( FVector ( 0.0f , 0.0f , 100.0f * owner->GetCapsuleComponent ( )->GetMass ( ) ) , NAME_None , true );
	owner->GetCapsuleComponent ( )->SetPhysicsLinearVelocity ( FVector::ZeroVector );
	owner->GetCapsuleComponent ( )->SetCollisionResponseToChannel ( ECollisionChannel::ECC_GameTraceChannel4 , ECollisionResponse::ECR_Ignore );
	UE_LOG ( LogTemp , Error , TEXT ( "%f , %f , %f" ) , knockbackDirection.X , knockbackDirection.Y , knockbackDirection.Z );
	owner->GetCapsuleComponent ( )->AddForce ( knockbackDirection , NAME_None , false );

	if ( WasKnockDown ) //누워있을때 맞는 애니메이션 실행
	{ 
		animInstace->StopAllMontages ( 0.5f );
		animInstace->PlayMontageAtFrameRate ( animInstace->hitKnockDownMontage , attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
		animInstace->bFalling = false;
	}
	else
	if ( WasHitFalling ) //공중 상태에서 맞았다면
	{
		animInstace->bFalling = true;
		//공중 상태에서 띄우는 공격을 맞았다면 더 띄우기

		//공중 상태에서 보통 공격을 맞았다면 공격력에 따라서 띄우기
		LaunchDirection.Z += 10.f;
		LaunchVelocity = LaunchDirection * LaunchSpeed;
		//hitFalling 몽타주를 실행
		owner->PlayAnimMontage ( animInstace->hitFallingAirMontage );
		animInstace->bFalling = true;
	}
	//FAttackInfoInteraction attackInfo =  attackInfoArray[0],  
	else if (attackInfoArray.IsValidIndex ( 0 ) && attackInfoArray[0].hitMontage != nullptr ) //나에게 몽타주가 없는 것이라면 상대가 몽타주 정보를 전달해준다
	{
		animInstace->bFalling = true;
		animInstace->StopAllMontages ( 0.1f );
		owner->PlayAnimMontage ( attackInfoArray[0].hitMontage );
		//animInstace->PlayHitFallingMontage ( attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
	}
	else //서있는 상태 혹은 앉아있는 상태에서 공격받은 경우
	{
		animInstace->bFalling = true;
		animInstace->StopAllMontages ( 0.1f );
		//animInstace->PlayHitFallingMontage ( attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
		//animInstace->PlayHitFallingTurnMontage ( attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
	}
	//owner->GetCharacterMovement ( )->AddImpulse ( attackInfoArray[0].KnockBackDirection * 100.0f , true );
	
	// 캐릭터를 날립니다. (Z축 방향으로는 원하는 높이를 추가할 수 있음)
	//owner->LaunchCharacter ( FVector ( -4400 , -4400 , 400 ) , true , true );



	//UE_LOG ( LogTemp , Error , TEXT ( "%f , %f , %f" ) , LaunchVelocity.X , LaunchVelocity.Y , LaunchVelocity.Z );

	currentFrame = 0;
	targetFrame = attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame;
	isExitOneMore = false;

	StartLocation = owner->GetCapsuleComponent ( )->GetRelativeLocation ( );
	FVector relativePoint =
		(
			owner->GetActorForwardVector ( ) *-1.0f* 1.0 * owner->GetCapsuleComponent ( )->GetMass ( ) +
			owner->GetActorRightVector ( ) * 0.0* owner->GetCapsuleComponent ( )->GetMass ( ) +
			owner->GetActorUpVector ( ) * 3.0* owner->GetCapsuleComponent ( )->GetMass ( )
		);
	arrivedLocation = StartLocation + relativePoint;
	LerpAlpha = 0.0f;
	LerpSpeed = 0.1f;


}

void UAIStateHitFalling::Execute ( const float& deltatime )
{
	currnetLocationZ = owner->GetMesh ( )->GetSocketLocation ( TEXT ( "root" ) ).Z;
	if( maxLocationZ < currnetLocationZ )
		maxLocationZ = currnetLocationZ;
	
	//if ( false == owner->GetCharacterMovement ( )->IsFalling ( )&& maxLocationZ - minLocationZ > 5.f )
	//spine_01 의 위치가 -85.f 보다 작을 때 해당 애니메이션이 끝남
	currentFrame++;
	if ( WasKnockDown ) // 누워있을때 타격 되면
	{
		
		if ( targetFrame <= currentFrame && !isExitOneMore )
		{
			isExitOneMore = true;
			Exit ( );
		}
	}
	else //공격 받은 경우
	{
		//if ( owner->GetMesh ( )->GetSocketLocation ( TEXT ( "spine_01" ) ).Z < -85.f )
		if(animInstace && animInstace->bOnGround)
		{
			Exit ( );
		}
	}
	LerpAlpha += deltatime * LerpSpeed;
	if ( currentFrame <= attackInfoArray[0].OppositeHitFrame )
	{
		FVector CurrentLocation = owner->GetCapsuleComponent ( )->GetRelativeLocation ( );
		FVector NewLocation = FMath::Lerp ( StartLocation , arrivedLocation , LerpAlpha ) - CurrentLocation;
		//owner->GetCapsuleComponent ( )->AddRelativeLocation ( NewLocation , true);
	}
	else
	{
		LerpAlpha += deltatime * LerpSpeed;
	}

	if ( currentFrame <= attackInfoArray[0].OppositeHitFrame )
	{
		FVector relativePoint =
			(
				owner->GetActorForwardVector ( ) * 200.0 +
				owner->GetActorRightVector ( ) * 0.0 +
				owner->GetActorUpVector ( ) * 300.0
			);
		//owner->GetActorForwardVector() * 
		
		//owner->GetCapsuleComponent ( )->AddRelativeLocation ( FMath::Lerp ( FVector ( 0 , 0 , 0 ) , relativePoint , ((float)currentFrame / attackInfoArray[0].OppositeHitFrame)) );
	}
}

void UAIStateHitFalling::Exit ( )
{
	owner->GetCapsuleComponent ( )->SetCollisionResponseToChannel ( ECollisionChannel::ECC_GameTraceChannel4 , ECollisionResponse::ECR_Block );

	if ( WasKnockDown ) //누워있을때 맞은경우 다음 상태 녹다운
	{
		owner->GetAIStateKnockDown ( )->WasHit = true;
		owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsKnockDown" ) , true );
		WasKnockDown = false;
	}
	else
	if ( maxLocationZ - minLocationZ > 10.f ) //높이 띄운 경우 다음 상태 바운드
	{
		owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsBound" ) , true );
		owner->GetAIStateBound ( )->SetAttackInfo ( attackInfoArray[0] );
	}
	else
	{
		owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsKnockDown" ) , true );
	}
	WasHitFalling = false;
	Super::Exit ( );
}
