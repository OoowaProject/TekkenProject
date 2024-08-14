// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateAttackLH.h"
#include "AICharacterAnimInstance.h"
#include "CPP_CharacterPaul.h"
#include "AICharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimTypes.h"

void UAIStateAttackLH::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	attackCount = 0;

	startLocation = owner->GetMesh ( )->GetBoneLocation ( (TEXT ( "head" )) );
	//공격거리 head 기준 174.0f RH
	//공격거리 head 기준 92.0f LH
	//사운드
	//UGameplayStatics::PlaySound2D ( GetWorld ( ) , owner->attackLHSFV , 0.5f );

	animInstace->PlayeAttackLHMontage ( );

	owner->GetCurrentMontage ( )->GetSectionStartAndEndTime ( 0 , startFrame , endFrame );
	TArray<const FAnimNotifyEvent*> notifyEvents;
	//몽타지 노티파이의 끝나는 시간
	owner->GetCurrentMontage ( )->GetAnimNotifies ( startFrame , endFrame , false , notifyEvents );
	for ( const FAnimNotifyEvent* NotifyEvent : notifyEvents )
	{
		endFrame = NotifyEvent->GetTriggerTime ( );
	}
	totalTime = 0;
	btest = false;
	walkSpeed = 200.0f;
	walkTime = .0f;
}

void UAIStateAttackLH::Execute ( const float& deltatime )
{
	ToLookTargetRotate ( deltatime );
	totalTime += deltatime;
	if ( totalTime >= endFrame && !btest )
	{
		GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "range : %f " ) , FVector::Dist ( owner->collisionLH->GetComponentLocation ( ) , startLocation ) ) );
		btest = true;
	}
	if ( walkTime <= endFrame )
	{
		float moveSpeed = FMath::Lerp ( walkSpeed , 0 , (walkTime += deltatime) / endFrame );
		owner->GetCapsuleComponent ( )->AddRelativeLocation ( owner->GetActorForwardVector ( ) * moveSpeed * deltatime );
		GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "moveSpeed : %f " ) , moveSpeed ) );
	}

}

void UAIStateAttackLH::Exit ( )
{
	Super::Exit ( );
}

