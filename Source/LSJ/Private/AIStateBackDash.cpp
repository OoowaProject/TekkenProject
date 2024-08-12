// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateBackDash.h"
#include "AICharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICharacter.h"

void UAIStateBackDash::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	attackCount = 0;

	lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , owner->aOpponentPlayer->GetActorLocation ( ) );
	dashLocation = owner->GetActorForwardVector ( ) * -1.0f * 50.f;
	startLocation = owner->GetActorLocation ( );
	//owner->LaunchCharacter ( owner->GetActorForwardVector()*-1.0f*1000.0f , true , false );
	animInstace->PlayerBackDashMontage ( );
}

void UAIStateBackDash::Execute ( const float& deltatime )
{

}

void UAIStateBackDash::Exit ( )
{
	Super::Exit ( );
}

void UAIStateBackDash::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
	Execute ( DeltaTime);
	lerpAlpha+=DeltaTime;
	owner->SetActorRelativeLocation (UKismetMathLibrary::VLerp ( startLocation,dashLocation, lerpAlpha ));
	if ( lerpAlpha > 1.f )
	{
		Exit ( );
	}
	//애니메이션 에서 종료

	// ...
}
