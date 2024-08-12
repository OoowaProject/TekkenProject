// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateComboLaserAttack.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CPP_CharacterPaul.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
void UAIStateComboLaserAttack::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	animInstace->PlayComboLaserMontage ( );
	aiCharacter = Cast<AAICharacter> ( owner );
	direction=aiCharacter->GetActorForwardVector();
	attackCount = 0;
	//회전
	//lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( aiCharacter->GetActorLocation ( ) , aiCharacter->aOpponentPlayer->GetActorLocation ( ) );
	//aiCharacter->targetRotator=lookPlayerRotator;
	//aiCharacter->bLookTarget = true;

}
void UAIStateComboLaserAttack::FinishStep ( )
{
	bIsStepping = false;
}

void UAIStateComboLaserAttack::StepAndAttack ( FVector Direction , float Distance , float Duration )
{
	if ( bIsStepping )
	{
		return;
	}

	MoveDirection = Direction.GetSafeNormal ( );
	MoveDistance = Distance;
	MoveDuration = Duration;
	MoveElapsedTime = 0.0f;
	bIsStepping = true;
}

void UAIStateComboLaserAttack::Execute ( const float& deltatime )
{
	//owner->GetMovementComponent ( )->AddInputVector ( MoveDirection * 10 );
	if ( bIsStepping )
	{
		MoveElapsedTime += deltatime;
		//owner->GetMovementComponent ( )->AddInputVector ( MoveDirection * 100000);
		
		if ( MoveElapsedTime >= MoveDuration )
		{
			FinishStep ( );
		}
		else
		{
			FVector v = owner->GetActorForwardVector ( );
			MoveDirection = v.GetSafeNormal( );
			aiCharacter->AddMovementInput ( MoveDirection , MoveDistance / MoveDuration * deltatime );
		}
	}


	//밀쳐내기
	FVector start = aiCharacter->GetActorLocation ( );
	FVector end = aiCharacter->collisionLF->GetComponentLocation ( );
	FHitResult hitResult;
	if ( GetWorld ( )->LineTraceSingleByObjectType ( hitResult , start , end , FCollisionObjectQueryParams ( EObjectTypeQuery::ObjectTypeQuery1 ) ) )
	{
		if ( hitResult.GetActor ( )->IsA ( ACPP_CharacterPaul::StaticClass ( ) ) )
		{
			DrawDebugLine ( GetWorld ( ) , start , end , FColor ( 255 , 0 , 0 ) , false , .1f , 0 , 12.3f );
			//확대할 위치, 줌 정도 0.5 기본, 흔들림정도, 흔들림 시간
			//aiCharacter->aMainCamera->RequestZoomEffect(aiCharacter->GetActorLocation(),0.5f,1.0f,3.0f);
			//UE_LOG ( LogTemp , Error , TEXT ( "%s" ) , *hitResult.GetActor ( )->GetName ( ) );
			//hitResult.GetActor ( )->SetActorLocation( hitResult.GetActor ( )->GetActorLocation() + 1000.0f*deltatime * aiCharacter->GetActorForwardVector ( ));
		}
	}
	//owner->GetMovementComponent()->AddInputVector(owner->GetActorForwardVector()*deltatime,true);
}

void UAIStateComboLaserAttack::Exit ( )
{
	Super::Exit ( );
}

