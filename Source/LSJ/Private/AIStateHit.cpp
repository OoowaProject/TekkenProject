// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateHit.h"
#include "CPP_Tekken8CharacterParent.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

void UAIStateHit::SetAttackInfo ( FAttackInfoInteraction& pAttackInfo )
{
	attackInfoArray.Empty ( );
	attackInfoArray.Add(pAttackInfo);
}

void UAIStateHit::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );

	owner->GetBlackboardComponent()->SetValueAsBool ( TEXT ( "IsHit" ) , false ); // 원하는 값을 설정

	//if ( attackInfoArray[0].DamageAmount == 10 )
	{
		FVector direction = owner->GetActorLocation ( ) - owner->aOpponentPlayer->GetActorLocation ( ) + owner->aOpponentPlayer->GetActorForwardVector ( ) * -1000.f;
		direction.Z = 0.0f; // 수평 방향으로만 계산 (필요 시)

		// 벡터 정규화
		direction.Normalize ( );

		// 반전된 벡터에 강도 적용
		FVector LaunchVelocity = direction * -1.f * attackInfoArray[0].KnockBackFallingDirection.X;
		//owner->LaunchCharacter ( LaunchVelocity , true , true );
		owner->GetMesh ()->AddForceAtLocation ( LaunchVelocity , owner->aOpponentPlayer->GetActorLocation());
		//owner->GetCharacterMovement ( )->AddImpulse ( attackInfoArray[0].KnockBackDirection * 100.0f , true );
	}

	//공격 받는 애니메이션 추가
	animInstace->StopAllMontages ( 0.1f );

	if ( nullptr != attackInfoArray[0].hitMontage )
		animInstace->PlayMontageAtFrameRate ( attackInfoArray[0].hitMontage , attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
	else
	{
		if ( attackInfoArray[0].DamagePoint == EDamagePointInteraction::Top )
			animInstace->PlayHitTopMontage ( attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
		else
			animInstace->PlayHitMiddleMontage ( attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
	}



}

void UAIStateHit::Execute ( const float& deltatime )
{
	
}

void UAIStateHit::Exit ( )
{
	Super::Exit();
}