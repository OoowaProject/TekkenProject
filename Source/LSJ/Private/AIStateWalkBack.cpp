// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateWalkBack.h"
#include "AICharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AICharacter.h"



void UAIStateWalkBack::SetDistance ( float pDistance )
{
	distance = pDistance;
}

void UAIStateWalkBack::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , owner->aOpponentPlayer->GetActorLocation ( ) );
	owner->SetActorRotation (lookPlayerRotator);
	animInstace->PlayerWalkBackMontage();
	animInstace->StateWalkBack ( true );
	startPos = owner->GetActorLocation();
}

void UAIStateWalkBack::Execute ( const float& deltatime )
{
	//lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( GetOwner ( )->GetActorLocation ( ) , player->GetActorLocation ( ) );
	//owner->GetMovementComponent ( )->AddInputVector ( owner->GetActorForwardVector ( ) * -1.0f );
}

void UAIStateWalkBack::Exit ( )
{
	//animInstace->StateWalkBack ( false );
	//owner->SetActorLocation( owner->GetMesh());
	Super::Exit ( );

}

void UAIStateWalkBack::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
	//if ( (owner->GetActorLocation() -startPos).Size() > distance )
	//	Exit ( );
	if (!animInstace->StateWalkBack())
		Exit ( );
	Execute ( DeltaTime);
	//owner->SetActorRotation ( FMath::RInterpConstantTo ( owner->GetActorRotation ( ) , lookPlayerRotator , DeltaTime , 200.0f ) );
	//애니메이션 에서 종료
	// ...

}
