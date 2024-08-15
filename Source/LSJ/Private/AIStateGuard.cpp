// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateGuard.h"
#include "AICharacterAnimInstance.h"
#include "AIStateComponent.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
void UAIStateGuard::SetAttackInfo ( FAttackInfoInteraction& pAttackInfo )
{
	attackInfoArray.Empty ( );
	attackInfoArray.Add ( pAttackInfo );
}
void UAIStateGuard::BeginPlay ( )
{
	Super::BeginPlay ( );

}

void UAIStateGuard::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	//animInstace->PlayerIdleMontage();
	

	
	//기존 애니메이션 실행 중지
	
	//가드 애니메이션 실행
	//애니메이션 실행 시간은 지연 시간 동안 실행

	//가드 이펙트는 HitD함수에서 실행
	//가드 되는 동안 또 공격 당하면 가드상태

	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsGuard" ) , false ); // 원하는 값을 설정

	//if ( attackInfoArray[0].DamageAmount == 10 )
	//10000.0f 1칸
	//+2000 1칸?
	//2칸 12000.0f
	//3칸 14000.0f                                                                                                                            
	//20000.0f 5칸 발차기를 5칸 
	FVector knockbackDirection = owner->GetActorForwardVector ( ) * -1.0f * 14000.0f * owner->GetCapsuleComponent ( )->GetMass ( );
	// 기본
	// knockbackDirection.Z = 20000.0f * owner->GetCapsuleComponent ( )->GetMass ( );
	//FVector knockbackDirection = owner->GetActorForwardVector ( ) * -1.0f * 20000.0f * owner->GetCapsuleComponent ( )->GetMass ( );

	owner->GetCapsuleComponent ( )->SetPhysicsLinearVelocity ( FVector::ZeroVector );
	owner->GetCapsuleComponent ( )->AddForce ( knockbackDirection , NAME_None , false );

	//공격 받는 애니메이션 추가
	animInstace->StopAllMontages ( 0.1f );

	if ( nullptr != attackInfoArray[0].hitMontage )
		animInstace->PlayMontageAtFrameRate ( attackInfoArray[0].hitMontage , attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
	else
	{
		if ( !bSit )
			animInstace->PlayMontageAtFrameRate ( animInstace->guardMontage ,attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
		else
			animInstace->PlayHitMiddleMontage ( attackInfoArray[0].RetrieveFrame + attackInfoArray[0].OppositeHitFrame , 30.0f );
	}
	bSit = false;
}

void UAIStateGuard::Execute ( const float& deltatime )
{

}

void UAIStateGuard::Exit ( )
{
	Super::Exit();

}
