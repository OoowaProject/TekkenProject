// Fill out your copyright notice in the Description page of Project Settings.
#include "AIStateAttackLF.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CPP_CharacterPaul.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimTypes.h"
#include "Components/CapsuleComponent.h"
void UAIStateAttackLF::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	attackCount = 0;

	startLocation = owner->GetMesh ( )->GetBoneLocation ( (TEXT ( "head" )) );
	startLocation.Z = 0;
	//사운드
	//UGameplayStatics::PlaySound2D ( GetWorld ( ) , owner->attackLFSFV , 0.5f );
	if(attackPoint==EAttackPoint::Lower)
		animInstace->PlayeAttackLowerLFMontage ( );
	else
		animInstace->PlayeAttackLFMontage ( );

	owner->GetCurrentMontage ( )->GetSectionStartAndEndTime ( 0 , startFrame , endFrame );
	TArray<const FAnimNotifyEvent*> notifyEvents;
	//몽타지 노티파이의 끝나는 시간
	owner->GetCurrentMontage ( )->GetAnimNotifies ( startFrame , endFrame , false , notifyEvents );
	int count = 0;
	for ( const FAnimNotifyEvent* NotifyEvent : notifyEvents )
	{
		if ( count == 0 )
			moveEndTime = NotifyEvent->GetTriggerTime ( );
		else if ( count == 1 )
			endFrame = NotifyEvent->GetTriggerTime ( );

		count++;
	}
	totalTime = 0;
	btest = false;
	walkSpeed = 600.0f;
}

void UAIStateAttackLF::Execute ( const float& deltatime )
{
	totalTime += deltatime;
	ToLookTargetRotate ( deltatime );

	if ( totalTime >= endFrame && !btest )
	{
		btest = true;
		FVector location =owner->collisionLF->GetComponentLocation ( );
		location.Z = 0;
		//GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "range : %f " ) , FVector::Dist ( location , startLocation ) ) );
	}
	if ( totalTime <= moveEndTime )
	{
		float moveSpeed = FMath::Lerp ( 0 , walkSpeed , (totalTime) / endFrame );
		owner->GetCapsuleComponent ( )->AddRelativeLocation ( owner->GetActorForwardVector ( ) * moveSpeed * deltatime );
		//GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "moveSpeed : %f " ) , moveSpeed ) );
	}
}

void UAIStateAttackLF::Exit ( )
{
	Super::Exit ( );
}
