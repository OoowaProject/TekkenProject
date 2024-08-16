// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateBound.h"
#include "CPP_Tekken8CharacterParent.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIStateIdle.h"
void UAIStateBound::SetAttackInfo ( FAttackInfoInteraction& pAttackInfo )
{
	attackInfoArray.Empty ( );
	attackInfoArray.Add ( pAttackInfo );
}

void UAIStateBound::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	if ( false == owner->GetBlackboardComponent ( )->GetValueAsBool ( TEXT ( "IsBound" ) ) )
	{
		Exit ( );
		return;
	}

	animInstace->bOnGround = false;
	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsBound" ) , false );

	{
		FVector Direction = owner->GetActorLocation ( ) - owner->aOpponentPlayer->GetActorLocation ( ) + owner->aOpponentPlayer->GetActorForwardVector ( ) * -1000.f;
		Direction.Z = 0.0f; // 수평 방향으로만 계산 (필요 시)

		// 벡터 정규화
		Direction.Normalize ( );

		// 반전된 벡터에 강도 적용 //attackInfoArray[0].KnockBackDirection.X
		FVector v = attackInfoArray[0].KnockBackDirection * 0.9f;
		v.X = 0;
		v.Y = 0;
		FVector LaunchVelocity = v;
		//owner->LaunchCharacter ( LaunchVelocity , true , false );
		//owner->GetCharacterMovement ( )->AddImpulse ( attackInfoArray[0].KnockBackDirection * 100.0f , true );
	}
	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsGuard" ) , false ); // 원하는 값을 설정
	float upLength = FVector::DotProduct ( attackInfoArray[0].KnockBackDirection , owner->GetActorUpVector ( ) );
	//if ( attackInfoArray[0].DamageAmount == 10 )
	//10000.0f 1칸
	//+2000 1칸?
	//2칸 12000.0f
	//3칸 14000.0f                                                                                                                            
	//20000.0f 5칸 발차기를 5칸 
	FVector knockbackDirection = owner->GetActorUpVector ( ) * (upLength * 66.67 + 8000) * owner->GetCapsuleComponent ( )->GetMass ( );
	// 기본
	// knockbackDirection.Z = 20000.0f * owner->GetCapsuleComponent ( )->GetMass ( );
	//FVector knockbackDirection = owner->GetActorForwardVector ( ) * -1.0f * 20000.0f * owner->GetCapsuleComponent ( )->GetMass ( );

	owner->GetCapsuleComponent ( )->SetPhysicsLinearVelocity ( FVector::ZeroVector );
	owner->GetCapsuleComponent ( )->AddForce ( knockbackDirection , NAME_None , false );
	//공격 받는 애니메이션 추가
	if(owner->GetCurrentState()!=owner->GetAIStateBound())
		animInstace->StopAllMontages ( 1.0f );
	//animInstace->PlayBoundMontage ( );
}

void UAIStateBound::Execute ( const float& deltatime )
{
	//공중일때
//	if ( animInstace->velocityZ < 0.1 && owner->GetCharacterMovement ( )->IsFalling())
	if ( false == animInstace->bOnGround )
	{
		//FVector Gravity = FVector ( 0 , 0 , -980.0f * 1.0f * deltatime ); // 기본 중력 값은 -980 cm/s^2
		//FVector NewLocation = owner->GetActorLocation ( ) + (Gravity);
		//owner->SetActorLocation ( NewLocation );
	}

//	else if ( false == owner->GetCharacterMovement ( )->IsFalling ( ) && owner->GetActorLocation ( ).Z < -80.f )
	else if ( true == animInstace->bOnGround )
	{
		owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsKnockDown" ) , true );
		Exit ( );
	}

}

void UAIStateBound::Exit ( )
{
	animInstace->bKnockDown = true;

	Super::Exit ( );
}
