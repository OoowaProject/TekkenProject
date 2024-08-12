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
		owner->LaunchCharacter ( LaunchVelocity , true , false );
		//owner->GetCharacterMovement ( )->AddImpulse ( attackInfoArray[0].KnockBackDirection * 100.0f , true );
	}
	//공격 받는 애니메이션 추가
	if(owner->GetCurrentState()!=owner->GetAIStateBound())
		animInstace->StopAllMontages ( 1.0f );
	//animInstace->PlayBoundMontage ( );
}

void UAIStateBound::Execute ( const float& deltatime )
{
	if ( animInstace->velocityZ < 0.1 && owner->GetCharacterMovement ( )->IsFalling())
	{
		FVector Gravity = FVector ( 0 , 0 , -980.0f * 1.0f * deltatime ); // 기본 중력 값은 -980 cm/s^2
		FVector NewLocation = owner->GetActorLocation ( ) + (Gravity);
		owner->SetActorLocation ( NewLocation );
	}
	else if ( false == owner->GetCharacterMovement ( )->IsFalling ( ) && owner->GetActorLocation ( ).Z < -80.f )
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
