// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateAttackRH.h"
#include "AICharacterAnimInstance.h"
#include "CPP_CharacterPaul.h"
#include "AICharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimTypes.h"

void UAIStateAttackRH::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter(pAnimInstance);
	attackCount = 0;

	startLocation = owner->GetMesh ( )->GetBoneLocation ( (TEXT ( "head" )) );
	//공격거리 head 기준 175.0f RH Middle
	//공격거리 head 기준 81.0f LH Top
	//공격거리 head 기준 157 LF Lower
	//사운드
	//UGameplayStatics::PlaySound2D ( GetWorld ( ) , owner->attackRHSFV , 0.5f );
	animInstace->PlayeAttackRHMontage ();

	owner->GetCurrentMontage ( )->GetSectionStartAndEndTime ( 0 , startFrame , endFrame );
	TArray<const FAnimNotifyEvent*> notifyEvents;
	//몽타지 노티파이의 끝나는 시간
	owner->GetCurrentMontage ( )->GetAnimNotifies ( startFrame , endFrame , false , notifyEvents );
	int count = 0;
	for ( const FAnimNotifyEvent* NotifyEvent : notifyEvents )
	{
		if(count == 0)
			moveStartTime = NotifyEvent->GetTriggerTime ( );
		else if ( count == 1 )
			moveEndTime = NotifyEvent->GetTriggerTime ( );
		else if ( count == 2 )
			endFrame = NotifyEvent->GetTriggerTime ( );

		count++;
	}
	totalTime = 0;
	btest = false;
	walkSpeed = 400.0f;
}

void UAIStateAttackRH::Execute ( const float& deltatime )
{
	ToLookTargetRotate ( deltatime );
	totalTime += deltatime;
	if ( totalTime >= endFrame && !btest)
	{
		//GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "range : %f " ) , FVector::Dist ( owner->collisionRH->GetComponentLocation ( ) , startLocation ) ) );
		btest = true;
	}

	if ( totalTime >= moveStartTime &&totalTime <= moveEndTime )
	{
		float moveSpeed = FMath::Lerp ( 0 , walkSpeed , (totalTime += deltatime) / endFrame );
		owner->GetCapsuleComponent ( )->AddRelativeLocation ( owner->GetActorForwardVector ( ) * moveSpeed * deltatime );
		//GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "moveSpeed : %f " ) , moveSpeed ) );
	}
}

void UAIStateAttackRH::Exit ( )
{
	Super::Exit ( );
}
