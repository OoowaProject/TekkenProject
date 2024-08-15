// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateGuard.h"
#include "AICharacterAnimInstance.h"
#include "AIStateComponent.h"
#include "AICharacter.h"
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
}

void UAIStateGuard::Execute ( const float& deltatime )
{

}

void UAIStateGuard::Exit ( )
{
	Super::Exit();

}
