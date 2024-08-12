// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateWalkForward.h"
#include "AICharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimTypes.h"
#include "BehaviorTree/BlackBoardComponent.h"
void UAIStateWalkForward::SetDistance ( float pDistance )
{
	distance = pDistance;
}

void UAIStateWalkForward::Enter (UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter(pAnimInstance);
	if ( nullptr == owner )
		return;
	if(nullptr == owner->aOpponentPlayer )
		return;

	attackRange = owner->GetBlackboardComponent ( )->GetValueAsFloat ( TEXT ( "AttackRange" ) );
	GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "start GetBTWDistance : %f " ) , owner->GetBTWDistance() ) );
	GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "attackRange : %f end" ) , attackRange ) );
	UE_LOG(LogTemp,Error, TEXT ( "start GetBTWDistance : %f " ) , owner->GetBTWDistance ( ));
	UE_LOG ( LogTemp , Error , TEXT ( "attackRange : %f end" ) , attackRange  );

	animInstace->PlayerWalkForwardMontage();
	//animInstace->StateWalkForward(true);
	direction=owner->GetActorForwardVector ( );

	//초기 스피드 설정
	walkSpeed = 100.0f;
	walkDashMinSpeed = 80.0f;
	walkTime = 0;

	//초기 상대를 바라보는 Rotator
	FVector opponentPlayerRotator = owner->aOpponentPlayer->GetMesh()->GetBoneLocation ((TEXT("head")));
	opponentPlayerRotator.Z = owner->GetActorLocation ( ).Z;
	lookPlayerRotator = (opponentPlayerRotator - owner->GetActorLocation ( )).Rotation ( );
	//몽타지 세션의 시작과 끝 시간
	owner->GetCurrentMontage ( )->GetSectionStartAndEndTime ( 0 , startFrame , endFrame );
	TArray<const FAnimNotifyEvent*> notifyEvents;
	//몽타지 노티파이의 끝나는 시간
	owner->GetCurrentMontage ( )->GetAnimNotifies ( startFrame , endFrame , false, notifyEvents );
	for ( const FAnimNotifyEvent* NotifyEvent : notifyEvents )
	{
		endFrame= NotifyEvent->GetTriggerTime ();
	}
}

void UAIStateWalkForward::Execute ( const float& deltatime )
{
	Super::Execute ( deltatime );
	if( nullptr==owner )
		return;
	if ( nullptr == owner->aOpponentPlayer )
		return;
	ToLookTargetRotate ( deltatime );

	//속도가 처음에 크고 끝에는 작아지게 하면 좋을 것 같다.
	float moveSpeed;
	if( walkTime < endFrame*0.6) 
		//점점 증가하고
		moveSpeed = FMath::Lerp ( walkDashMinSpeed , walkSpeed , (walkTime += deltatime)/endFrame );
	else 
		//느려지게
		moveSpeed = FMath::Lerp ( walkSpeed , 0 , (walkTime += deltatime)/endFrame );
	owner->AddMovementInput ( direction * moveSpeed * deltatime);
	//owner->SetActorLocation(direction * walkSpeed * deltatime);
}

void UAIStateWalkForward::Exit ( )
{
	//공격거리 + 대시거리 보다 거리가 작거나 같다면 
	// //대시 후 해당 공격 모션의 공격거리가 되었을때 플레이어가 앉은 상태인지 공중상태인지를 판단하여 공격 모션이 달라진다. - AICharacter.cpp
	// //대시 후 해당 공격 모션의 공격거리가 닿지 않는다면 한번 더 대시 한다.
	// //한번 더 대시 한 경우 랜덤으로 앉아 가드(일단 서서 가드) 하거나 공격( 플레이어가 앉은 상태인지 공중상태인지를 판단하여 공격 모션이 달라진다. - AICharacter.cpp)한다.
	isOneMore = true;
	//대시 후 해당 공격 모션의 공격거리가 되었을때 플레이어가 앉은 상태인지 공중상태인지를 판단하여 공격 모션이 달라진다. - AICharacter.cpp
	if ( owner->GetBTWDistance ( ) <= attackRange )
	{
		owner->GetBlackboardComponent ( )->SetValueAsEnum ( TEXT ( "ERandomAttack" ) , owner->ChangeAttackMotionDependingOpponentState ( ) );
		owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "InAttackRange" ) , true );
		
	}
	else 
	{
		//해당 공격 모션의 공격거리가 닿지 않는다면 한번 더 대시 한다.
		
		//한번 더 대시 한 경우 랜덤으로 앉아 가드(일단 서서 가드) 하거나 공격( 플레이어가 앉은 상태인지 공중상태인지를 판단하여 공격 모션이 달라진다. - AICharacter.cpp)한다.
		if ( isOneMore )
		{
			int8 randNum = FMath::RandRange ( 0 , 1 );
			if ( randNum )
			{
				//랜덤으로 앉아 가드(일단 서서 가드) 
				owner->SetStateIdle ( );
			}
			else
			{	
				//공격( 플레이어가 앉은 상태인지 공중상태인지를 판단하여 공격 모션이 달라진다. - AICharacter.cpp)한다.
				owner->GetBlackboardComponent ( )->SetValueAsEnum ( TEXT ( "ERandomAttack" ) , owner->ChangeAttackMotionDependingOpponentState ( ) );
				owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "InAttackRange" ) , true );
			}
			isOneMore = false;
		}
	}
	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsOneMoreDash" ) , isOneMore );
	Super::Exit ( );
}