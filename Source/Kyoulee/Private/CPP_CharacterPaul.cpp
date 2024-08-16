﻿// Fill out your copyright notice in the Description page of Project Settings.
#define MYDEBUGMODE 0;

#include "CPP_CharacterPaul.h"

#include "GameMode_MH.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode_MH.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "CPP_PualAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Internationalization/Text.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACPP_CharacterPaul::ACPP_CharacterPaul ( )
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->uCharacterMesh = this->GetMesh ( );

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkeletalMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'" ) );
	if ( tempSkeletalMesh.Succeeded ( ) )
		uCharacterMesh->SetSkeletalMeshAsset ( tempSkeletalMesh.Object );
	uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , -80 ) );
	uCharacterMesh->SetRelativeRotation ( FRotator ( 0 , 0 , -90 ) );
	uCharacterMesh->SetWorldScale3D(FVector(0.1,0.1,0.1));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	collisionTop = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "collisionTop" ) );
	collisionTop->SetupAttachment ( GetMesh ( ) , TEXT ( "head" ) );
	collisionMiddle = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "collisionMiddle" ) );
	collisionMiddle->SetupAttachment ( GetMesh ( ) , TEXT ( "spine_01" ) );
	collisionLower = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "collisionLower" ) );
	collisionLower->SetupAttachment ( GetMesh ( ) , TEXT ( "pelvis" ) );
	collisionTop->SetBoxExtent ( FVector ( 311.232643 , 166.634513 , 240.066655 ) );
	collisionMiddle->SetBoxExtent ( (FVector ( 311.232643 , 315.176916 , 264.956523 )) );
	collisionMiddle->SetUsingAbsoluteRotation ( true );
	collisionMiddle->SetRelativeLocation ( FVector ( 0 , 0 , 99.999999 ) );
	collisionLower->SetBoxExtent ( FVector ( 319.717793 , 321.694333 , 336.888169 ) );
	collisionLower->SetUsingAbsoluteRotation ( true );
	collisionLower->SetRelativeLocation ( FVector ( 0.092165 , 531.000000 , 117.000000 ) );

	RootArrowComp = CreateDefaultSubobject<UArrowComponent> ( TEXT("RootArrowComp"));
	RootArrowComp->SetupAttachment(this->GetCapsuleComponent());
	RootArrowComp->SetRelativeLocation( FVector( 0, 0, -60 ));
}
// Called when the game starts or when spawned
void ACPP_CharacterPaul::BeginPlay ( )
{
	Super::BeginPlay ( );

	// 커멘드를 설정합니다.
	this->SettingCommandTree ( );
	// 위치 세팅
	this->ToLocation = this->GetActorLocation ( ) + this->GetActorForwardVector ( ) * 100;
	this->sFrameStatus.FrameBlockUsing = 0;
	this->sFrameStatus.FrameUsing = 0;
	GameModeMH = Cast<AGameMode_MH>( GetWorld ( )->GetAuthGameMode ( ) );
	
	uAnim = this->GetMesh ( )->GetAnimInstance ( );

}


// Called to bind functionality to input
void ACPP_CharacterPaul::SetupPlayerInputComponent ( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent ( PlayerInputComponent );
}


/************************************************************************/
/*       프레임 작업에 대한 정보입니다                                  */
/************************************************************************/


int32 testinput ( int32 currKeyValue )
{
	return (currKeyValue & 0b0001111000000000);
}

// Called every frame
void ACPP_CharacterPaul::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );
	player1 = 0;
	
	if (this->bIsDead)
		return;
// 	if ( this->GameModeMH->Player1 == this )
// 	{
// 		this->player1 = 1;
// 		this->DebugMode = true;
// 	}
	this->currKeyValue = this->GetCurrInputKeyValue ( );
	if (currKeyValue != prevKeyValue || sCurrCommand->NextTrees.Find ( prevKeyValue ) )
	{
		prevKeyValue = currKeyValue;
		if ( sCurrCommand->NextTrees.Find ( currKeyValue ))
		{
			if ( (sCurrCommand->NextTrees[currKeyValue]->timingStart <= iCurrFrame && iCurrFrame < sCurrCommand->NextTrees[currKeyValue]->timingEnd) ||  sCurrCommand->loopCommand )
				sNextCommand = sCurrCommand->NextTrees[currKeyValue];
		}
		else if ( testinput ( currKeyValue ) )
		{
			int32 tempValue = testinput ( currKeyValue );
			if ( sCurrCommand->NextTrees.Find ( tempValue ) )
			{
				if ( sCurrCommand->NextTrees[tempValue]->timingStart <= iCurrFrame && iCurrFrame < sCurrCommand->NextTrees[tempValue]->timingEnd )
				{
					sNextCommand = sCurrCommand->NextTrees[tempValue];
					FString trs = "";
					for (int32 vaa : sNextCommand->PrevKeys)
						trs += FString::Printf(TEXT("%d , "), vaa);
					UE_LOG ( LogTemp , Warning , TEXT ( "String : %s" ) , *trs );
				}
			}
			else
			{
				if ( mBaseCommandTree[0]->NextTrees.Find ( tempValue ) )
				{
					sNextCommand = mBaseCommandTree[0]->NextTrees[tempValue];
				}
			}
 		}
	}
	else if ( !CheckKeyArray ( ) && !sCurrCommand->loopCommand)
	{
		sNextCommand = mBaseCommandTree[0];
		iCurrFrame = 0;
	}

	// 주의 모든 작업은 FrameSystem에서 하는걸 권장합니다.
	this->fCurrTimeForFrame += DeltaTime;

	if ( this->fCurrTimeForFrame <= this->fFrameTime )
		return;
	AnimationFrame ( );
	sFrameStatus.FrameBlockUsing--;
	sFrameStatus.FrameUsing--;
	sAttackInfo.ActionFrame--;
	iCurrFrame++;
	//UE_LOG ( LogTemp , Warning , TEXT ( "%d" ) , sFrameStatus.FrameUsing );
// 
// 	// 다음 행동 정의

	this->FrameSystem ( );
	this->prevKeyValue = currKeyValue;
}

bool ACPP_CharacterPaul::CheckKeyArray ( )
{
	TArray<int32> tempNextKeys;
	tempNextKeys = sCurrCommandKeys;
	for ( int32 nextKey : sCurrCommandKeys )
	{
		if ( sCurrCommand->NextTrees[nextKey]->timingEnd < iCurrFrame )
			tempNextKeys.Remove ( nextKey );
	}
	if (tempNextKeys.IsEmpty())
		return false;
	return true;
}

/************************************************************************/
/*                커멘드 크리에 대한 정보                               */
/************************************************************************/

// Command Setting Function

void ACPP_CharacterPaul::SettingCommandTree ( )
{
	this->AddCommandTree ( mBaseCommandTree , 0 , 0 , 0 , &ACPP_CharacterPaul::CommandIdle, true );
	mBaseCommandTree[0]->NextTrees.Add ( InputKeyValue ( 0 , 0 , 0 , 0 , 0 ) , mBaseCommandTree[0] );

	int32 forwardkey = InputKeyValue ( 6 , 0 , 0 , 0 , 0 );
	int32 backkey = InputKeyValue ( 4 , 0 , 0 , 0 , 0 );
	int32 upkey = InputKeyValue ( 8 , 0 , 0 , 0 , 0 );
	int32 downkey = InputKeyValue ( 2 , 0 , 0 , 0 , 0 );
	int32 LP = InputKeyValue ( 0 , true , 0 , 0 , 0 );
	int32 RP = InputKeyValue ( 0 , 0 , true , 0 , 0 );
	int32 LK = InputKeyValue ( 0 , 0 , 0 , true , 0 );
	int32 RK = InputKeyValue ( 0 , 0 , 0 , 0 , true );
	
	int32 i1key = backkey + downkey;
	int32 i3key = forwardkey + downkey;
	int32 i7key = backkey + upkey;
	int32 i9key = forwardkey + upkey;

	int32 downLPkey = downkey + LP;
	int32 downRPkey = downkey + RP;
	int32 downLKkey = downkey + LK;
	int32 downRKkey = downkey + RK;

	/************************************************************************/
	/*                          Command Tree Create                         */
	/************************************************************************/

	/**
	 *  ForwardKey
	 */
	this->AddCommandBaseTree ( { 0 } , forwardkey , 0 , 5 , &ACPP_CharacterPaul::CommandMoveForward , true);
	// MoveForward Dash
	this->AddCommandBaseTree ( { 0, forwardkey } , 0 , 0 , 5 , &ACPP_CharacterPaul::CommandStar , false );

	// DASH while
	this->AddCommandBaseTree ( { 0, forwardkey, 0 } , forwardkey , 1 , 5 , &ACPP_CharacterPaul::CommandMoveForwarDash , true );
	this->AddCommandBaseTree ( { 0, forwardkey, 0, forwardkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandEnd , false );

	/**
	 *  BackKey
	 */
	this->AddCommandBaseTree ( { 0 } , backkey , 0 , 5 , &ACPP_CharacterPaul::CommandMoveBack, true );
	// MoveBack Dash
	this->AddCommandBaseTree ( { 0, backkey } , 0 , 0 , 5 , &ACPP_CharacterPaul::CommandStar, false );
	// DASH while
	this->AddCommandBaseTree ( { 0, backkey, 0 } , backkey , 3  , 5 , &ACPP_CharacterPaul::CommandMoveBackDash, false );
	this->AddCommandBaseTree ( { 0, backkey, 0, backkey } , backkey , 0 , 12 , &ACPP_CharacterPaul::CommandEnd, false );
	this->AddCommandBaseTree ( { 0, backkey, 0, backkey } , 0 , 0 , 12 , &ACPP_CharacterPaul::CommandEnd, false );

	this->AddCommandBaseTree ( { 0 , backkey } , backkey + LP, 4, 10,&ACPP_CharacterPaul::CommandSangBong1, false );
	this->AddCommandBaseTree ( { 0 , backkey,  backkey + LP } , backkey + RP , 0 , 10 , &ACPP_CharacterPaul::CommandSangBong2 , false );
	this->AddCommandBaseTree ( { 0 , backkey,  backkey + LP, backkey + RP } , 0 , 0 , 12 , &ACPP_CharacterPaul::CommandEnd , false );

	/**
	 * UPKey
	 */
	 // Move Lateral Plus
	this->AddCommandBaseTree ( { 0 } , upkey , 0 , 3 , &ACPP_CharacterPaul::CommandStarWaitNext, false );
	// 3frame wait
	this->AddCommandBaseTree ( { 0, upkey } , 0 , 2, 4 , &ACPP_CharacterPaul::CommandMoveLateralUpDash, false);
	this->AddCommandBaseTree ( { 0, upkey, 0 } , 0 , 5 , 6 , &ACPP_CharacterPaul::CommandStar, false );
	// Move while Lateral
	this->AddCommandBaseTree ( { 0, upkey, 0, 0 } , upkey , 1 , 5 , &ACPP_CharacterPaul::CommandMoveLateralUpLoop, true );
	this->AddCommandBaseTree ( { 0,upkey,0, 0, upkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandEnd, false );

	// Jump
	this->AddCommandBaseTree ( { 0 , upkey } , upkey , 5 ,6 , &ACPP_CharacterPaul::CommandJump, false );
	this->AddCommandBaseTree ( { 0 , upkey, upkey } , upkey , 0 , 3 , &ACPP_CharacterPaul::CommandEnd, false );
	this->AddCommandBaseTree ( { 0, upkey, upkey } , upkey + LK , 5 , 20, &ACPP_CharacterPaul::CommandJumpKneeKick , false );
	this->AddCommandBaseTree ( { 0, upkey, upkey, upkey + LK } , upkey + RK , 5 , 20 , &ACPP_CharacterPaul::CommandJumpAxeKick , false );
	this->AddCommandBaseTree ( { 0, upkey, upkey, upkey + LK , upkey + RK } , 0 , 5 , 20 , &ACPP_CharacterPaul::CommandEnd , false );

	this->AddCommandBaseTree ( { 0, upkey } , i9key , 3 , 6 , &ACPP_CharacterPaul::CommandJumpForward , false );
	this->AddCommandBaseTree ( { 0, forwardkey } , i9key , 0 , 3 , &ACPP_CharacterPaul::CommandJumpForward , false );
	this->AddCommandBaseTree ( { 0, upkey, i9key } , i9key + LK , 0 , 20, &ACPP_CharacterPaul::CommandJumpKneeKick , false );
	this->SetLinkTree ( { 0, forwardkey, i9key }, { 0, upkey, i9key,  i9key + LK } );
	this->AddCommandBaseTree ( { 0, upkey, i9key, i9key + LK } , i9key + RK , 5 , 20 , &ACPP_CharacterPaul::CommandJumpAxeKick , false );
	this->AddCommandBaseTree ( { 0, upkey, i9key, i9key + LK , i9key + RK }, 0 , 5 , 20 , &ACPP_CharacterPaul::CommandEnd , false );

	this->AddCommandBaseTree ( { 0, upkey } , i7key , 3 , 6 , &ACPP_CharacterPaul::CommandJumpBackward , false );
	this->AddCommandBaseTree ( { 0, backkey } , i7key , 0 , 3 , &ACPP_CharacterPaul::CommandJumpBackward , false );
	this->AddCommandBaseTree ( { 0, upkey, i7key } , i7key + LK , 0 , 20, &ACPP_CharacterPaul::CommandJumpKneeKick , false );
	this->AddCommandBaseTree ( { 0, upkey, i7key, i7key + LK } , i7key + RK , 0 , 20 , &ACPP_CharacterPaul::CommandJumpAxeKick , false );
	this->AddCommandBaseTree ( { 0, upkey, i7key, i7key + LK, i7key + RK } , 0 , 5 , 20 , &ACPP_CharacterPaul::CommandEnd , false );
 
	/**
	 * DownKey
	 */
	 // Move Lateral Minus
	this->AddCommandBaseTree ( { 0 } , downkey , 0 , 3 , &ACPP_CharacterPaul::CommandStarWaitNext, false );
 	this->AddCommandBaseTree ( { 0 ,downkey } , 0 , 4 , 6 , &ACPP_CharacterPaul::CommandMoveLateralDownDash, false );
 	this->AddCommandBaseTree ( { 0 ,downkey, 0 } , 0 , 5 , 10 , &ACPP_CharacterPaul::CommandStar,  false);
// 	// Move while Lateral
 	this->AddCommandBaseTree ( { 0 ,downkey, 0 } , downkey , 1 , 5  , &ACPP_CharacterPaul::CommandMoveLateralDownLoop , true);
 	this->AddCommandBaseTree ( { 0,downkey,0, downkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandEnd, false );

// 	// Crouch
	this->AddCommandBaseTree ( { 0 ,downkey } , downkey , 4 , 6 , &ACPP_CharacterPaul::CommandDownCrouch , true );
	this->AddCommandBaseTree ( { 0 ,downkey } , downLPkey , 4 , 6 , &ACPP_CharacterPaul::CommandDownCrouch , true );
	this->AddCommandBaseTree ( { 0 ,downkey } , i3key, 4 , 6 , &ACPP_CharacterPaul::CommandDownCrouch , true );
	this->AddCommandBaseTree ( { 0 ,downkey } , i1key, 4 , 6 , &ACPP_CharacterPaul::CommandDownCrouch , true );
	this->AddCommandBaseTree ( { 0 ,downkey } , downLKkey , 4 , 6 , &ACPP_CharacterPaul::CommandDownCrouch , true );

	this->SetLinkTree ( { 0 , downkey, downLPkey } , { 0 ,downkey, downkey } );
	this->SetLinkTree ( { 0 , downkey, downLKkey } , { 0 ,downkey, downkey} );

	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , 0 , 1 , 2 , & ACPP_CharacterPaul::CommandUpCrouch , false );
	this->AddCommandBaseTree ( { 0 ,downkey, i3key } , 0 , 1 , 2 , & ACPP_CharacterPaul::CommandUpCrouch , false );
	this->AddCommandBaseTree ( { 0 ,downkey, i1key } , 0 , 1 , 2 , & ACPP_CharacterPaul::CommandUpCrouch , false );

	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , i3key , 0 , 3 , &ACPP_CharacterPaul::CommandDownForwardCrouch , true );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, i3key } , 0 , 1 , 2 , & ACPP_CharacterPaul::CommandUpCrouch , false );

	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , downkey + backkey , 0 , 3 , & ACPP_CharacterPaul::CommandDownBackCrouch , true );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey + backkey } , 0 , 1 , 2 , & ACPP_CharacterPaul::CommandUpCrouch , false );

	this->SetLinkTree ( { 0 , forwardkey } , { 0 ,downkey, downkey,  downkey + forwardkey } );
	this->SetLinkTree ( { 0 , backkey } , { 0 ,downkey, downkey,  downkey + backkey } );

	this->SetLinkTree ( { 0 ,downkey, downkey, downkey + forwardkey } , { 0 ,downkey, downkey } );
	this->SetLinkTree ( { 0 ,downkey, downkey, downkey + backkey } , { 0 ,downkey, downkey } );

	this->SetLinkTree ( { 0 ,downkey, downkey, downkey + forwardkey } , { 0 ,forwardkey } );
	this->SetLinkTree ( { 0 ,downkey, downkey, downkey + backkey } , { 0 ,backkey} );

// 	// Skell Couch
	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , downLPkey , 0 , 2 , & ACPP_CharacterPaul::CommandSitJab , false );
	this->AddCommandBaseTree ( { 0, downkey, i3key } , i3key + LP , 0 , 3 , & ACPP_CharacterPaul::CommandSitJab , false );

	this->AddCommandBaseTree ( { 0, downkey, i3key } , i3key + RP , 0 , 20 , & ACPP_CharacterPaul::CommandBullA , false );

	this->AddCommandBaseTree ( { 0, forwardkey } , forwardkey + RP , 0 , 20 , & ACPP_CharacterPaul::CommandBullA , false );

 	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downLPkey } , 0 , 1 , 2 , &ACPP_CharacterPaul::CommandUpCrouch , false);

	this->SetLinkTree ( { 0 ,downkey, downkey, downLPkey } , { 0 ,downkey, downkey } );
	this->SetLinkTree ( { 0 ,downkey, downkey, downLPkey } , { 0 ,downkey, downLPkey } );

	this->SetLinkTree ( { 0 ,downkey, downkey, downkey + backkey } , { 0 ,downkey, downkey, downLPkey } );
	this->SetLinkTree ( { 0 ,downkey, downkey, downkey + forwardkey } , { 0 ,downkey, downkey } );
	this->SetLinkTree ( { 0 ,downkey, downkey, downkey + backkey } , { 0 ,downkey, downkey } );
	
  	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , downLKkey , 1 , 2 , &ACPP_CharacterPaul::CommandSitSpineKick, false );
 	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downLKkey } , 0 , 1 , 2 , & ACPP_CharacterPaul::CommandUpCrouch, false );

	this->SetLinkTree ( { 0 ,downkey, downkey, downLKkey } , { 0 ,downkey, downLKkey } );

// 
// 	// Branch Add function 만들기 하나의 가지로 합쳐주는 함수이다.
// 	// 현재 겹치는 부분 함수들 목록
// 	// 짧게
// 	this->AddCommandBaseTree ( { 0 ,downkey, downkey } , 0 , 0 , 3 , &ACPP_CharacterPaul::CommandUpCrouch );
// 	this->AddCommandBaseTree ( { 0 ,downkey, downkey,  0 } , 0 , 4 , 6 , &ACPP_CharacterPaul::CommandStar );
// 	SetSelfReLinkTree ( { 0,downkey, downkey, 0, 0 } );
// 	// 길게
// 	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey } , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandUpCrouch );
// 	this->AddCommandBaseTree ( { 0 ,downkey, downkey, downkey, 0 } , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandStar );
// 	SetSelfReLinkTree ( { 0,downkey, downkey, downkey, 0, 0 } );
// 	// 서면서 때리기
// 	this->AddCommandBaseTree ( { 0,downkey, downkey, 0, 0} , LP , 0 , 0 , &ACPP_CharacterPaul::CommandJinJee );
// 	//this->AddCommandBaseTree ( { 0,downkey, downkey, 0, 0, LP } , LP , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandEnd );
// 	//SetSelfReLinkTree ( { 0,downkey, downkey, 0, 0, LP , LP} );
// 	this->AddCommandBaseTree ( { 0,downkey, downkey, downkey, 0, 0 } , LP , 0 , 0 , & ACPP_CharacterPaul::CommandJinJee );
// 	//this->AddCommandBaseTree ( { 0,downkey, downkey, downkey, 0, 0, LP } , LP , 0 , 0 , 0 , & ACPP_CharacterPaul::CommandEnd );
// 	//SetSelfReLinkTree ( { 0,downkey, downkey, downkey, 0, 0, LP , LP } );
// 
// 	// 붕권
// 	this->AddCommandBaseTree ( { 0, forwardkey } , forwardkey + RP, 1 , 3 , &ACPP_CharacterPaul::CommandBungGuan );
// 	this->AddCommandBaseTree ( { 0, downkey, forwardkey + RP } , forwardkey + RP , 1 , 10 , &ACPP_CharacterPaul::CommandEnd );
// 
// 	/**
// 	 * Punch
// 	 */
// 	// LeftRightCombo
// 	// LeftRight 1 
	this->AddCommandBaseTree ( { 0 } , LP , 0 , 3 , & ACPP_CharacterPaul::CommandLeadJab , false );
	this->AddCommandBaseTree ( { 0, LP } , 0 , 3 , 5 , & ACPP_CharacterPaul::CommandEnd , false );

	this->AddCommandBaseTree ( { 0, LP } , RP , 3 , 5 , &ACPP_CharacterPaul::CommandCrossStaight, false );
	this->AddCommandBaseTree ( { 0, LP, RP } , RP , 0 , 5 , &ACPP_CharacterPaul::CommandEnd, false );

	this->AddCommandBaseTree ( { 0 } , RP , 0 , 3 , & ACPP_CharacterPaul::CommandCrossStaight, false );
 	this->AddCommandBaseTree ( { 0, RP } , 0 , 3 , 5 , & ACPP_CharacterPaul::CommandEnd, false );

// 	/**
// 	 * Kick
// 	 */
 
	this->AddCommandBaseTree ( { 0 } , LK , 0 , 3 , &ACPP_CharacterPaul::CommandJingun, false );
	this->AddCommandBaseTree ( { 0, LK } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd, false);
 
	this->AddCommandBaseTree ( { 0 } , RK , 0 , 3 , &ACPP_CharacterPaul::CommandHighKick , false );
	this->AddCommandBaseTree ( { 0, RK } , 0 , 0 , 3 , & ACPP_CharacterPaul::CommandEnd, false );

	this->AddCommandBaseTree ( { 0, downkey, i3key } , forwardkey , 0 , 12 , & ACPP_CharacterPaul::CommandSlidingDash, false );
	this->AddCommandBaseTree ( { 0, downkey, i3key, forwardkey } , forwardkey + RP , 0 , 14 , & ACPP_CharacterPaul::CommandBungGuan, false );

// 	/**
// 	 * combo Command
// 	 */
 	this->AddCommandBaseTree ( { 0 } , i3key + RP , 0 , 3 , & ACPP_CharacterPaul::CommandBullA, false );
 	this->AddCommandBaseTree ( { 0, i3key + RP } , 0 , 0 , 10 , & ACPP_CharacterPaul::CommandEnd, false );

	this->AddCommandBaseTree ( { 0 } , i3key + LK , 0 , 20 , & ACPP_CharacterPaul::CommandLeftTiger, false );
	this->AddCommandBaseTree ( { 0, i3key + LK} , 0 , 0 , 10 , & ACPP_CharacterPaul::CommandEnd, false );

	this->AddCommandBaseTree ( { 0 ,downkey } , downkey + RK , 4 , 6 , & ACPP_CharacterPaul::CommandMtgLuOu1 , false );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey + RK } , downkey + RP , 0 , 16 , & ACPP_CharacterPaul::CommandMtgLuOu2 , false );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey + RK ,  downkey + RP } , RP + LP , 0 , 16 , & ACPP_CharacterPaul::CommandMtgLuOu3 , false );
	this->AddCommandBaseTree ( { 0 ,downkey, downkey + RK ,  downkey + RP, RP + LP } , 0 , 0 , 6 , & ACPP_CharacterPaul::CommandEnd , false );

// // 
// // 

// 
// 	this->AddCommandBaseTree ( { 0, backkey, backkey + LP, backkey + LP } , backkey + RP , 2 , 30 , & ACPP_CharacterPaul::CommandSangBong2 );
// 	this->AddCommandBaseTree ( { 0, backkey, backkey + LP ,  backkey + RP } , 0 , 0 , 10 , & ACPP_CharacterPaul::CommandEnd );


this->mCurrCommandTree = mBaseCommandTree;
this->sCurrCommand = mBaseCommandTree[0];
this->sNextCommand = mBaseCommandTree[0];
}



/************************************************************************/
/*                커멘드 함수에 대한 정보                               */
/************************************************************************/

// Command Test 
void ACPP_CharacterPaul::CommandStarWaitNext ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandStarWaitNext Pressed" ) );

}
void ACPP_CharacterPaul::CommandStarTest2 ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandStarTest2 Pressed" ) );
	//this->eCharacterState = ECharacterStateInteraction::GuardStand;
	iCurrFrame = 0;
}

// Command Function

void ACPP_CharacterPaul::CommandIdle ( )
{
   	if ( DebugMode )
   		UE_LOG ( LogTemp , Warning , TEXT ( "CommandIdle Pressed" ) );

	if ( this->GetZValue() > fHeightValue )
		return;
	this->bCrouched =false;
	this->bJumpping = false;
	this->eCharacterState = ECharacterStateInteraction::GuardStand;
}

void ACPP_CharacterPaul::CommandStar ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandStar Pressed" ) );

}

void ACPP_CharacterPaul::CommandEnd ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandEnd Pressed" ) );
	this->bCrouched = false;
	this->bJumpping =false;
}


void ACPP_CharacterPaul::CommandMoveForward ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveForward Pressed" ) );

	eCharacterState = ECharacterStateInteraction::WalkForward;

	// 애니메이션으로 바꾼다면?
	this->SetToRelativeLocationFrame ( FVector ( 15 , 0 , 0 ) , 5);

	sFrameStatus.FrameUsing = 2;
}

void ACPP_CharacterPaul::CommandMoveForwarDash ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "MoveForwarDash Pressed" ) );

	eCharacterState = ECharacterStateInteraction::Run;

	this->SetToRelativeLocationFrame ( FVector ( 50 , 0 , 0 ) , 5);
	this->sFrameStatus.FrameUsing = 2;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandMoveBack ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveBack Pressed" ) );

	eCharacterState = ECharacterStateInteraction::GuardStand;

	this->SetToRelativeLocationFrame ( FVector ( -15 , 0 , 0 ) , 5 );
	this->sFrameStatus.FrameUsing = 2;
}

void ACPP_CharacterPaul::CommandMoveBackDash ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveBackDash Pressed" ) );

	eCharacterState = ECharacterStateInteraction::Move;

	this->SetToRelativeLocationFrame ( FVector ( -120 , 0 , 0 ) , 15 );
	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgBackDash );

	this->sFrameStatus.FrameUsing = 20;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandJump ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJump Pressed" ) );
	
	eCharacterState = ECharacterStateInteraction::Air;

	this->SetToRelativeLocationFrame ( FVector (0 , 0 , 450 ) , 60 );
	this->bJumpping = true;
	this->sFrameStatus.FrameUsing = 54;
}

void ACPP_CharacterPaul::CommandJumpForward ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJumpForward Pressed" ) );

	this->SetToRelativeLocationFrame ( FVector ( 40 , 0 , 450 ) , 60 );
	this->bJumpping = true;
	this->sFrameStatus.FrameUsing = 54;
}

void ACPP_CharacterPaul::CommandJumpBackward ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJumpBackward Pressed" ) );

	this->SetToRelativeLocationFrame ( FVector ( -40 , 0 , 450 ) , 60 );
	this->bJumpping = true;
	this->sFrameStatus.FrameUsing = 54;
}

void ACPP_CharacterPaul::CommandJumpKneeKick ( )
{

	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJumpKneeKick Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackTop;

	SetActtacInfoSkell ( EDamagePointInteraction::Top , 5 , 10 , 6 , 0 , 8 , 1 , 8 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , 5 , 50 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 50 , 0 , 0 );
	sAttackInfo.KnockBackDefenceDir = this->RelativePointVector ( 30 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	sAttackInfo.debugColor = FColor ( 255 , 128 , 128 );
	// 추후 애니메이션 있을 시 애니메이션 이동으로 변경
	this->SetToRelativeLocationFrame ( FVector ( 30 , 0 , 300 ) , 25 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgJumpKneeKick );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLefthand;
	sAttackInfo.uHitSound = this->uSoundPaulWeakHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandJumpAxeKick ( )
{

	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJumpKneeKick Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackTop;

	SetActtacInfoSkell ( EDamagePointInteraction::Top , 5 , 10 , 6 , 0 , 8 , 1 , 8 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 110 , 5 , 50 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 50 , 0 , 0 );
	sAttackInfo.KnockBackDefenceDir = this->RelativePointVector ( 30 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	sAttackInfo.debugColor = FColor ( 255 , 128 , 128 );
	// 추후 애니메이션 있을 시 애니메이션 이동으로 변경
	this->SetToRelativeLocationFrame ( FVector ( 30 , 0 , 300 ) , 25 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgJumpAxeKick );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLefthand;
	sAttackInfo.uHitSound = this->uSoundPaulWeakHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandMoveLateralUpDash ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralPlus Pressed" ) );

	eCharacterState = ECharacterStateInteraction::Move;

	sAttackInfo.DamagePoint = EDamagePointInteraction::Special;
	sAttackInfo.DamageAmount = 0;

	this->sAttackInfo.ActionFrame = 1;

	if (this->player1)
		this->SetToRelativeLocationFrame (FVector( 50 , -80 , 0 ), 10);
	else
		this->SetToRelativeLocationFrame ( FVector ( 50 , 80 , 0 ) , 10 );

	// 애니매이션 실행 부분 있으면 만들기
	//PlayMontageFrameSystem ( uMtgMoveLateral );

	this->sFrameStatus.FrameUsing = 20;
}

void ACPP_CharacterPaul::CommandMoveLateralUpLoop( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralUpLoop Pressed" ) );

	eCharacterState = ECharacterStateInteraction::Move;

	sAttackInfo.DamagePoint = EDamagePointInteraction::Special;
	sAttackInfo.DamageAmount = 0;

	if ( this->player1 )
		this->SetToRelativeLocationFrame ( FVector ( 10 , -16 , 0 ) , 5 );
	else
		this->SetToRelativeLocationFrame ( FVector ( 10 , 16 , 0 ) , 5 );


	// 애니매이션 실행 부분 없음

	this->sFrameStatus.FrameUsing = 2;
}

void ACPP_CharacterPaul::CommandDownCrouch ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandDownCrouch Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::GuardSit;
	this->bCrouched = true;
	//this->GetCapsuleComponent ( )->SetWorldScale3D ( FVector ( 0.1 , 0.6 , 0.1 ) );
	//this->uCharacterMesh->SetRelativeLocation ( FVector ( 0 , 0 , -130 ) );
	this->sFrameStatus.FrameUsing = 1;
	this->iCurrFrame = 0;
}

void ACPP_CharacterPaul::CommandDownForwardCrouch ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandDownForwardCrouch Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::Sit;
	this->bCrouched = true;
	this->SetToRelativeLocationFrame ( FVector ( 3 , 0 , 0 ) , 3 );
	this->sFrameStatus.FrameUsing = 1;
	this->iCurrFrame = 0;
}
void ACPP_CharacterPaul::CommandSlidingDash ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandDownForwardCrouch Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::Move;
	this->bCrouched = false;
	this->SetToRelativeLocationFrame ( FVector ( 120 , 0 , 0 ) , 13);
	this->sFrameStatus.FrameUsing = 1;
	this->iCurrFrame = 0;

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgSlidingDash );
}
void ACPP_CharacterPaul::CommandDownBackCrouch ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandDownBackCrouch Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::GuardSit;
	this->bCrouched = true;
	this->SetToRelativeLocationFrame ( FVector ( -3 , 0 , 0 ) , 3 );
	this->sFrameStatus.FrameUsing = 1;
	this->iCurrFrame = 0;
}

// 사용하지 않는다
void ACPP_CharacterPaul::CommandUpCrouch ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandUpCrouch Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::Up;

	this->sFrameStatus.FrameUsing = 1;
}

void ACPP_CharacterPaul::CommandMoveLateralDownDash ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralDown Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::Move;

	sAttackInfo.DamagePoint = EDamagePointInteraction::Special;
	sAttackInfo.DamageAmount = 0;

	this->sAttackInfo.ActionFrame = 1;


	if (this->player1)
		this->SetToRelativeLocationFrame ( FVector ( 50 , 80 , 0 ) , 10 );
	else
		this->SetToRelativeLocationFrame ( FVector ( 50 , -80 , 0 ) , 10 );

	// 애니매이션 실행 부분 있으면 만들기
	PlayMontageFrameSystem ( uMtgMoveLateral );

	this->sFrameStatus.FrameUsing = 20;
}
void ACPP_CharacterPaul::CommandMoveLateralDownLoop ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandMoveLateralDown Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::Move;

	sAttackInfo.DamagePoint = EDamagePointInteraction::Special;
	sAttackInfo.DamageAmount = 0;

	if ( this->player1 )
		this->SetToRelativeLocationFrame ( FVector ( 10 , 16 , 0 ) , 5 );
	else
		this->SetToRelativeLocationFrame ( FVector ( 10 , -16 , 0 ) , 5 );


	// 애니매이션 실행 부분 없음

	this->sFrameStatus.FrameUsing = 2;
}

void ACPP_CharacterPaul::CommandLeadJab ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandLeadJab Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackTop;

	SetActtacInfoSkell ( EDamagePointInteraction::Top , 5 , 12 , 10 , 0 , 8 , 1 , 8 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 90 , 5 , 50 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 50 , 0 , 0 );
	sAttackInfo.KnockBackDefenceDir = this->RelativePointVector ( 30 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수
	
	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	sAttackInfo.debugColor = FColor ( 255 , 128 , 128 );
	// 추후 애니메이션 있을 시 애니메이션 이동으로 변경
	this->SetToRelativeLocationFrame( FVector(30, 0, 0) , 5);	

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLeadJab );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLefthand;
	sAttackInfo.uHitSound = this->uSoundPaulWeakHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandCrossStaight ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandCrossStaight Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackTop;

	SetActtacInfoSkell ( EDamagePointInteraction::Top , 5 , 10 , 12 , 0 , 6 , 0 , 6 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 105 , -5 , 30 ) ;
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 50 , 0 , 0 );
	sAttackInfo.KnockBackDefenceDir = this->RelativePointVector ( 30 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 35 , 0 , 0 ) , 5 );


	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( this->uMtgCrossStaight );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitRightHand;
	sAttackInfo.uHitSound = this->uSoundPaulWeakHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
	
}

void ACPP_CharacterPaul::CommandJingun ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJingun Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 14 , 15 , 13 , 0 , -1 , -12 , -1 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 100 , 5 , 60 ) ;
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 80 , 0 , 0 );
	sAttackInfo.KnockBackDefenceDir = this->RelativePointVector ( 30 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 15 , 0 , 0 ) , 5 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgJingun );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLeftFoot;
	sAttackInfo.uHitSound = this->uSoundPaulWeakHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;;
}

void ACPP_CharacterPaul::CommandHighKick ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJingun Pressed" ) );

	this->eCharacterState = ECharacterStateInteraction::AttackTop;

	SetActtacInfoSkell ( EDamagePointInteraction::Top , 17 , 12 , 10 , 0 , 14 , 4 , 57 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 85 , -5 , 60 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 30 , -5 , 0 );
	sAttackInfo.KnockBackDefenceDir = this->RelativePointVector ( 20 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 5 , 0 , 0 ) , 5 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgHighKick );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitRightFoot;
	sAttackInfo.uHitSound = this->uSoundPaulWeakHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandBungGuan ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandBungGuan Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;
	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 17 , 20, 20 , 0 ,0 , 0 ,0 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 60 ) ;
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 500 , 0 , 20 );
	sAttackInfo.KnockBackDefenceDir = this->RelativePointVector ( 40 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0.5;
	sAttackInfo.cameraZoom = 0.2;
	sAttackInfo.cameraDelay = 0.4;

	this->SetToRelativeLocationFrame ( FVector ( 50 , 0 , 0 ) , 5 );
	sAttackInfo.hitMontage = uMtgFallBackHitForBungGuan;
	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgBungGuan );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLefthand;
	sAttackInfo.uHitSound = this->uSoundPaulWeakHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandJinJee ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandJinJee Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 20 , 5 , 18 , 0 , 15 , -14 , 0 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 70 , -5 , 10 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 10 , 0 , 300 );
	sAttackInfo.KnockBackDefenceDir = this->RelativePointVector ( 20 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 10 , 0 , 0 ) , 5 );


	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLeadJab );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLefthand;
	sAttackInfo.uHitSound = this->uSoundPaulStrongHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandSitJab( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandSitJab Pressed" ) );

	this->bCrouched = true;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 15 , 6 , 12 , 0 , 10 , -5 , 6 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 90 , -5 , 10 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 120 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 2 , 0 , 0 ) , 3 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgSitJab );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLefthand;
	sAttackInfo.uHitSound = this->uSoundPaulWeakHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandSitSpineKick ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandSitSpineKick Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackLower;

	SetActtacInfoSkell ( EDamagePointInteraction::Lower , 15 , 12 , 8 , 0 , 10 , -5 , 6 );


	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 120 , -5 , -50 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 140 , -5 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 0 , 0 , 0 ) , 3 );


	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgSitSpineKick );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLeftFoot;
	sAttackInfo.uHitSound = this->uSoundPaulWeakHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandBullA ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandBullA Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 13 , 12 , 15, -8 , 42 , -5 , 42 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 10 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 100 , 0 , 450 );
	sAttackInfo.KnockBackFallingDirection = this->RelativePointVector ( 110 , 0 , 300 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 10 , 0 , 0 ) , 3 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgBullA );
	sAttackInfo.niagaraSystemArray.Add(this->uNS_HitEffect);

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitRightHand;
	sAttackInfo.uHitSound = this->uSoundPaulStrongHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandLeftTiger ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandLeftTiger Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 15 , 11 , 15 , 0, 8 , 8 , 42 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 10 ) ;
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 110 , 0 , 300 );
	sAttackInfo.KnockBackFallingDirection = this->RelativePointVector ( 110 , 0 , 300 );
	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 30 , 0 , 0 ) , 3 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLeftTiger );
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitRightHand;
	sAttackInfo.uHitSound = this->uSoundPaulStrongHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandSangBong1 ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandSangBong1 Pressed" ) );

	UE_LOG ( LogTemp , Warning , TEXT ( "CommandSangBong1 Pressed" ) );
	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 12 , 18 , 6 , 0 , 8 , 8 , 42 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 90 , -5 , 40 ) ;
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 130 , 0 , 0 );
	sAttackInfo.KnockBackFallingDirection = this->RelativePointVector ( 110 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0.0;
	sAttackInfo.cameraZoom = 0.0;
	sAttackInfo.cameraDelay = 0.0;

	this->SetToRelativeLocationFrame ( FVector ( 40 , 0 , 0 ) , 5 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgSangBong1 );
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLeftFoot;
	sAttackInfo.uHitSound = this->uSoundPaulStrongHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandSangBong2 ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandSangBong2 Pressed" ) );

	UE_LOG ( LogTemp , Warning , TEXT ( "CommandSangBong2 Pressed" ) );
	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 15 , 9 , 27 , 0 , 8 , 8 , 42 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 50 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 130 , 0 , 0 );
	sAttackInfo.KnockBackFallingDirection = this->RelativePointVector ( 110 , 0 , 0 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 3.1;
	sAttackInfo.cameraZoom = 0.9;
	sAttackInfo.cameraDelay = 0.3;

	this->SetToRelativeLocationFrame ( FVector ( 100 , 0 , 0 ) , 7 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgSangBong2 );
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLefthand;
	sAttackInfo.uHitSound = this->uSoundPaulStrongHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandBackJilPung ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandBackJilPung Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 15 , 18 , 20 , 0 , 8 , 8 , 42 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 10 ) ;
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 130 , 0 , 300 );
	sAttackInfo.KnockBackFallingDirection = this->RelativePointVector ( 110 , 0 , 300 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 30 , 0 , 0 ) , 3 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgBackJilPung );
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLeftFoot;
	sAttackInfo.uHitSound = this->uSoundPaulStrongHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandMtgLuOu1 ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandCombo1 Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackLower;

	SetActtacInfoSkell ( EDamagePointInteraction::Lower , 15 , 18 , 20 , 0 , 8 , 8 , 42 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , -40 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 130 , 0 , 300 );
	sAttackInfo.KnockBackFallingDirection = this->RelativePointVector ( 110 , 0 , 300 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 30 , 0 , 0 ) , 3 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLuOu1 );
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitLeftFoot;
	sAttackInfo.uHitSound = this->uSoundPaulStrongHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandMtgLuOu2 ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandCombo2 Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 15 , 18 , 20 , 0 , 8 , 8 , 42 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 10 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 100 , 0 , 300 );
	sAttackInfo.KnockBackFallingDirection = this->RelativePointVector ( 110 , 0 , 300 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 50 , 0 , 0 ) , 3 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLuOu2 );
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitRightHand;
	sAttackInfo.uHitSound = this->uSoundPaulStrongHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

void ACPP_CharacterPaul::CommandMtgLuOu3 ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandCombo3 Pressed" ) );

	this->bCrouched = false;

	this->eCharacterState = ECharacterStateInteraction::AttackMiddle;

	SetActtacInfoSkell ( EDamagePointInteraction::Middle , 15 , 18 , 20 , 0 , 8 , 8 , 42 );

	sAttackInfo.skellEffectLocation = this->RelativePointVector ( 80 , -5 , 10 );
	sAttackInfo.KnockBackDirection = this->RelativePointVector ( 130 , 0 , 300 );
	sAttackInfo.KnockBackFallingDirection = this->RelativePointVector ( 110 , 0 , 300 );

	this->SetAttackInfoOwnerOpposite ( ); // 내부 owner frame opposite frame 자동 세팅용 함수

	sAttackInfo.cameraShake = 0;
	sAttackInfo.cameraZoom = 0;
	sAttackInfo.cameraDelay = 0;

	this->SetToRelativeLocationFrame ( FVector ( 70 , 0 , 0 ) , 4 );

	// 애니매이션 실행 부분
	PlayMontageFrameSystem ( uMtgLuOu3 );
	sAttackInfo.niagaraSystemArray.Add ( this->uNS_HitEffect );

	// 사운드 추가
	sAttackInfo.uMovementSound = this->uSoundHitRightFoot;
	sAttackInfo.uHitSound = this->uSoundPaulStrongHit;
	sAttackInfo.uDefenceSound = this->uSoundGuard;

	this->sFrameStatus.FrameUsing = sAttackInfo.ActionFrame + sAttackInfo.RetrieveFrame;
}

float ACPP_CharacterPaul::GetZValue ( )
{
	
	FHitResult data;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor ( this );
	collisionParams.AddIgnoredActor ( this->aOpponentPlayer);
	bool hit = GetWorld()->LineTraceSingleByChannel(data, RootArrowComp->GetComponentLocation() , RootArrowComp->GetComponentLocation ( ) + FVector::UpVector * -1000, ECollisionChannel::ECC_WorldStatic , collisionParams );

	if (hit)
		return data.Distance;
	return 0;
}


bool ACPP_CharacterPaul::CommandAllStop ( )
{
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "CommandAllStop Pressed" ) );
	
	sAttackInfo.ActionFrame = -1;
	this->sFrameStatus.FrameUsing = 1;
	return 0;
}





/************************************************************************/
/*                    애니메이션을 위한  작업입니다                     */
/************************************************************************/

void ACPP_CharacterPaul::AnimationFrame ( )
{
	if (bFalling){
		this->StopAnimMontage();
		this->eCharacterState = ECharacterStateInteraction::Down;
	}
	if (bFalling && this->fFallingValue <= 1.0f )
	{
		this->fFallingValue += 0.06f;
	}else if (! bFalling && this->fFallingValue > 0.0f ){
		this->fFallingValue -= 0.06f;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Falling value %f : is true? %d"), fFallingValue, bFalling);
	if ( !this->ToLocationFrame.IsEmpty ( ) )
	{
		FVector dir = this->ToLocationFrame.Pop ( );
		locationlenght = dir;
		this->GetCapsuleComponent( )->AddRelativeLocation( locationlenght );
	}
	else
	{
		locationlenght *= 0;
	}
}

void ACPP_CharacterPaul::SetToRelativeLocationFrame ( FVector dir , int32 frame )
{
	FVector relativeDir = RelativePointVector ( dir.X , dir.Y , dir.Z );
	FVector	frameDir = (relativeDir) / frame;
	this->ToLocationFrame.Empty ( );
	for ( int32 i = 0; i < frame; i++ )
		this->ToLocationFrame.Add ( frameDir );
}

void ACPP_CharacterPaul::SetToLocationFrame ( FVector dir , int32 frame )
{
	FVector	frameDir = dir / frame;
	this->ToLocationFrame.Empty ( );
	for ( int32 i = 0; i < frame; i++ )
		this->ToLocationFrame.Add ( frameDir );
}

void ACPP_CharacterPaul::SetToLocationPoint ( float x , float y , float z )
{
	FVector moveToPoint = this->GetActorLocation ( ) +
		(
			this->GetActorForwardVector ( ) * x +
			this->GetActorRightVector ( ) * y +
			this->GetActorUpVector ( ) * z
		);
	this->ToLocation = moveToPoint;

	this->ToLocationFrame.Push ( FVector ( 1 , 1 , 1 ) );
}

void ACPP_CharacterPaul::SetToLocationPoint ( FVector vector )
{
	FVector moveToPoint = this->GetActorLocation ( ) +
		(
			this->GetActorForwardVector ( ) * vector.X +
			this->GetActorRightVector ( ) * vector.Y +
			this->GetActorUpVector ( ) * vector.Z
		);
	this->ToLocation = moveToPoint;
}


void ACPP_CharacterPaul::SetToWorldLocationPoint ( FVector vector )
{
	this->ToLocation = vector;
}


FVector ACPP_CharacterPaul::RelativePointVector ( float x , float y , float z )
{
	FVector relativePoint =
		(
			this->GetActorForwardVector ( ) * x +
			this->GetActorRightVector ( ) * y +
			this->GetActorUpVector ( ) * z
		);

	return relativePoint;
}

bool ACPP_CharacterPaul::PlayMontageFrameSystem ( UAnimMontage* montage )
{
	if ( !uAnim || !montage )
		return false;
	if ( DebugMode )
		UE_LOG ( LogTemp , Warning , TEXT ( "Montage Start" ) );
	UCPP_PualAnimInstance* tempAnim = Cast<UCPP_PualAnimInstance> ( uAnim );
	tempAnim->Montage_Play ( montage );
	return true;
}




FCommandTreeNode* ACPP_CharacterPaul::CreateCommandTree ( int32 keyValue , int32 timingStart , int32 timingEnd , void (ACPP_CharacterPaul::* fptr)(), bool loopCommand )
{
	FCommandTreeNode* NewCommand = new FCommandTreeNode ( );

	NewCommand->keyValue = keyValue;
	//NewCommand->PrevTrees;
	NewCommand->NextTrees = TMap<int32 , FCommandTreeNode*> ( );
	NewCommand->timingStart = timingStart;
	NewCommand->timingEnd = timingEnd;
	NewCommand->action.BindUObject ( this , fptr );
	NewCommand->loopCommand = loopCommand;
	return NewCommand;
}

FCommandTreeNode* ACPP_CharacterPaul::AddCommandTree ( TMap<int32 , FCommandTreeNode*>& CurrCommandTree , int32 keyValue , int32 timingStart , int32 timingEnd , void(ACPP_CharacterPaul::* fptr)(), bool loopCommand )
{
	//  음 빈거 확인해야 하는데 잘안됨 확인할 필요있음
	if ( &CurrCommandTree && CurrCommandTree.Find ( keyValue ) )
	{
		if ( DebugMode )
			UE_LOG ( LogTemp , Warning , TEXT ( "Error: befor add CommandTree was settinged!! %i" ) , keyValue );

		return CurrCommandTree[keyValue];
	}
	CurrCommandTree.Add ( keyValue , this->CreateCommandTree ( keyValue , timingStart , timingEnd , fptr, loopCommand ) );

	CurrCommandTree[keyValue]->PrevKeys = { keyValue };
	CurrCommandTree[keyValue]->NextKeys.Add ( keyValue );
	return CurrCommandTree[keyValue];
}

FCommandTreeNode* ACPP_CharacterPaul::AddCommandBaseTree ( TArray<int> arrayTreeCommand , int32 keyValue , int32 timingStart , int32 timingEnd , void(ACPP_CharacterPaul::* fptr)(), bool loopCommand )
{
	TMap<int32 , FCommandTreeNode*> temp = mBaseCommandTree;
	TMap<int32 , FCommandTreeNode*> tempParant = mBaseCommandTree;
	int lastkey = 0;

	for ( int commandkey : arrayTreeCommand )
	{
		tempParant = temp;
		if ( !temp.Find ( commandkey ) )
		{
			if ( DebugMode )
				UE_LOG ( LogTemp , Error , TEXT ( "[SetParentReLinkTree] No Tree Command " ) );
			return nullptr;
		}
		temp = temp[commandkey]->NextTrees;
		lastkey = commandkey;
	}
	if ( tempParant[lastkey]->NextTrees.Find ( keyValue ) )
	{
		if ( DebugMode )
			UE_LOG ( LogTemp , Warning , TEXT ( "Error: befor add CommandTree was settinged!! %i" ) , keyValue );
		return tempParant[lastkey]->NextTrees[lastkey];
	}
	tempParant[lastkey]->PrevKeys = arrayTreeCommand;
	tempParant[lastkey]->NextKeys.Add ( keyValue );
	tempParant[lastkey]->NextTrees.Add ( keyValue , this->CreateCommandTree ( keyValue , timingStart , timingEnd , fptr, loopCommand ) );

	return tempParant[lastkey]->NextTrees[keyValue];
}


bool ACPP_CharacterPaul::SetSelfReLinkTree ( TArray<int32> arrayTreeCommand )
{
	TMap<int32 , FCommandTreeNode*> temp = mBaseCommandTree;
	TMap<int32 , FCommandTreeNode*> tempParant = mBaseCommandTree;
	int lastkey = 0;
	for ( int commandkey : arrayTreeCommand )
	{
		tempParant = temp;
		if ( !temp.Find ( commandkey ) )
		{
			if ( DebugMode )
				UE_LOG ( LogTemp , Error , TEXT ( "[SetParentReLinkTree] No Tree Command " ) );
			return false;

		}
		temp = temp[commandkey]->NextTrees;
		lastkey = commandkey;
	}
	tempParant[lastkey]->PrevKeys = arrayTreeCommand;
	tempParant[lastkey]->NextKeys.Add ( lastkey );
	tempParant[lastkey]->NextTrees.Add ( lastkey , tempParant[lastkey] );
	return true;
}

bool ACPP_CharacterPaul::SetLinkTree ( TArray<int32> TargetTree , TArray<int32> ConnectTree )
{
	FCommandTreeNode* tempTarget;
	FCommandTreeNode* tempConnect;
	FCommandTreeNode* ConnectParant;
	int32 ConnectLastKey;

	tempTarget = mBaseCommandTree[0];
	for ( int32 commandkey : TargetTree )
	{
		tempTarget = tempTarget->NextTrees[commandkey];
	}

	tempConnect = mBaseCommandTree[0];
	for ( int32 commandkey : ConnectTree )
	{
		ConnectParant = tempConnect;
		tempConnect = tempConnect->NextTrees[commandkey];
		ConnectLastKey = commandkey;
	}
	tempTarget->NextTrees.Add ( ConnectLastKey , ConnectParant );

	return true;
}



/**
 * @title Frame 시스템을 위한 함수입니다.
 */
void ACPP_CharacterPaul::FrameSystem ( )
{
	this->fCurrTimeForFrame = 0;

	// 맞음으로 인한 모든 행동 불가 액션취소
	if ( sFrameStatus.FrameBlockUsing > 0 )
	{
		sAttackInfo.ActionFrame = -1;
		return;
	}
	// 인풋이 있을 경우 상대를 바라본다 
	if ( currKeyValue )
	{
		if ( this->GetZValue ( ) < this->fHeightValue )
			this->bFalling = false;
		FRotator Lookrotation = UKismetMathLibrary::FindLookAtRotation ( this->GetActorLocation ( ) , this->aOpponentPlayer->GetActorLocation ( ) );
		Lookrotation.Pitch = this->GetActorRotation ( ).Pitch;
		this->SetActorRotation ( Lookrotation );
	}
	if ( this->fFallingValue > 0.1f )
		return;

	// 액션 프레임 발동
	if ( sAttackInfo.ActionFrame == 0 )
		this->CommentHitFrameExecute ( );

	// 다음행동 예약
	// 예약된 다음 행동 실행
	PlayNextAction ( );
}
bool ACPP_CharacterPaul::PlayNextAction ( )
{
	if ( currKeyValue != 0 && this->GameModeMH->Player1 == this )
		DebugMode = 0;

	if ( sFrameStatus.FrameUsing < 0 && (sNextCommand ) )
	{
		iCurrFrame = 0;
		if ( sCurrCommand ) {
			sCurrCommand->action.Execute ( );

			// 소리 추가
			if ( this->sAttackInfo.uMovementSound )
				UGameplayStatics::PlaySound2D ( GetWorld ( ) , this->sAttackInfo.uMovementSound );
			this->sAttackInfo.uMovementSound = nullptr;
			if ( sNextCommand )
				sCurrCommand = sNextCommand;
			else
				sCurrCommand = mBaseCommandTree[0];
			if (sCurrCommand->loopCommand)
				sNextCommand = sCurrCommand;
			else
				sNextCommand = nullptr;
		}
		else {
			mBaseCommandTree[0]->action.Execute ( );
			sCurrCommand = mBaseCommandTree[0];
		}
		sCurrCommandKeys = sCurrCommand->NextKeys;
	}
	return false;
}


// 스킬 인포를 정의하기 위한 함수
void ACPP_CharacterPaul::SetActtacInfoSkell ( EDamagePointInteraction damagePointInteraction ,
	int32 damageAmount = 0 , int32 actionFrame = 0 , int32 retrieveFrame = 0 ,
	int32 missFrame = 0 , int32 hitFrame = 0 , int32 grardFrame = 0 , int32 counterFrame = 0 )
{
	sAttackInfo.DamagePoint = damagePointInteraction;
	sAttackInfo.DamageAmount = damageAmount;

	sAttackInfo.ActionFrame = actionFrame;
	sAttackInfo.RetrieveFrame = retrieveFrame;

	sAttackInfo.MissFrame = missFrame;
	sAttackInfo.HitFrame = hitFrame;
	sAttackInfo.GrardFrame = grardFrame;
	sAttackInfo.CounterFrame = counterFrame;

	this->SetAttackInfoOwnerOpposite ( );
}

// 자동 계산 함수
void ACPP_CharacterPaul::SetAttackInfoOwnerOpposite ( )
{
	//공격자
	sAttackInfo.OwnerHitFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.HitFrame < 0 ? sAttackInfo.HitFrame * -1 : 0);
	sAttackInfo.OwnerGuardFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.GrardFrame < 0 ? sAttackInfo.GrardFrame * -1 : 0);
	sAttackInfo.OwnerCounterFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.CounterFrame < 0 ? sAttackInfo.CounterFrame * -1 : 0);
	sAttackInfo.OwnerMissFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.MissFrame < 0 ? sAttackInfo.MissFrame * -1 : 0);

	//수비자
	sAttackInfo.OppositeHitFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.HitFrame > 0 ? sAttackInfo.HitFrame : 0);
	sAttackInfo.OppositeGuardFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.GrardFrame > 0 ? sAttackInfo.GrardFrame : 0);
	sAttackInfo.OppositeCounterFrame = sAttackInfo.RetrieveFrame + (sAttackInfo.CounterFrame > 0 ? sAttackInfo.CounterFrame : 0);
}

/************************************************************************/
/*               맞는 것을 정의한 공간 입니다                           */
/************************************************************************/
bool ACPP_CharacterPaul::HitDecision ( FAttackInfoInteraction attackInfoHit , ACPP_Tekken8CharacterParent* ownerHitPlayer )
{
	float fallingHeight = this->GetZValue ( );

	if ( aMainCamera )
		aMainCamera->RequestZoomEffect ( attackInfoHit.skellEffectLocation , attackInfoHit.cameraZoom , attackInfoHit.cameraShake , attackInfoHit.cameraDelay );
	else
		UE_LOG ( LogTemp , Warning , TEXT ( "is Emtpy " ) );
	if ( fallingHeight < 100 && attackInfoHit.DamagePoint == EDamagePointInteraction::Top && this->eCharacterState == ECharacterStateInteraction::GuardStand )
	{

		this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeGuardFrame;
		this->SetToLocationFrame ( attackInfoHit.KnockBackDefenceDir , attackInfoHit.OppositeGuardFrame );
		// defense animation 추가하기
		PlayMontageFrameSystem ( uMtgDefence );
		// 디펜스 파티클

		// 소리 추가
		UGameplayStatics::PlaySound2D ( GetWorld ( ) , this->sAttackInfo.uDefenceSound );
		if ( uNS_DefenceEffect && this->Hp > 0 )
			UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , uNS_DefenceEffect , attackInfoHit.skellEffectLocation );
		return false;
	}
	if ( fallingHeight < 100 && attackInfoHit.DamagePoint == EDamagePointInteraction::Middle && this->eCharacterState == ECharacterStateInteraction::GuardStand )
	{
		this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeGuardFrame;
		this->SetToLocationFrame ( attackInfoHit.KnockBackDefenceDir , attackInfoHit.OppositeGuardFrame );

		//LaunchCharacter ( (attackInfoHit.KnockBackDirection - this->GetActorLocation ( )) * 2 , true , true );
		// defense animation 추가하기
		if ( this->bCrouched )
			PlayMontageFrameSystem ( uMtgSitDefence );
		else
			PlayMontageFrameSystem ( uMtgDefence );

		// 소리 추가
		UGameplayStatics::PlaySound2D ( GetWorld ( ) , this->sAttackInfo.uDefenceSound );

		if ( uNS_DefenceEffect && this->Hp > 0 )
			UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , uNS_DefenceEffect , attackInfoHit.skellEffectLocation );

		return false;
	}
	if ( fallingHeight < 100 && attackInfoHit.DamagePoint == EDamagePointInteraction::Lower && this->eCharacterState == ECharacterStateInteraction::GuardSit )
	{
		this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeGuardFrame;
		this->SetToLocationFrame ( attackInfoHit.KnockBackDefenceDir , attackInfoHit.OppositeGuardFrame );

		//LaunchCharacter ( (attackInfoHit.KnockBackDirection - this->GetActorLocation ( )) * 2 , true , true );
		// 
		// defense animation 추가하기
		PlayMontageFrameSystem ( uMtgSitDefence );

		// 소리 추가
		UGameplayStatics::PlaySound2D ( GetWorld ( ) , this->sAttackInfo.uDefenceSound );

		if ( uNS_DefenceEffect && this->Hp > 0 )
			UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , uNS_DefenceEffect , attackInfoHit.skellEffectLocation );

		return false;
	}
	if (attackInfoHit.KnockBackDirection.Z > 10)
		this->bFalling = true;
	if ( fallingHeight > fHeightValue  + 10)
	{
		this->bFalling = true;
		this->GetMovementComponent()->Velocity = FVector(0,0,0);

		this->GetCapsuleComponent()->AddForce(FVector(0,0,400),FName("pelvis"), true);
		attackInfoHit.KnockBackDirection.X = 0;
		attackInfoHit.KnockBackDirection.Y = 0;
		attackInfoHit.KnockBackDirection.Z = 0;
		attackInfoHit.OppositeHitFrame += 10;
		this->uAnim->StopAllMontages ( 0.01f );
		this->ToLocationFrame.Empty();
	}
	
	this->sFrameStatus.FrameBlockUsing = attackInfoHit.OppositeHitFrame;
	this->SetToLocationFrame ( attackInfoHit.KnockBackDirection , attackInfoHit.OppositeHitFrame );

	iCurrFrame = 0;
	// heart animation 추가하기
	if ( this->Hp > 0 )
	{
		if ( attackInfoHit.hitMontage != nullptr ) //내가 준 몽타지 실행
		{
			this->uAnim->StopAllMontages ( 0.1f );
			PlayAnimMontage ( attackInfoHit.hitMontage );
		}
		else if ( attackInfoHit.DamagePoint == EDamagePointInteraction::Lower )
			PlayMontageFrameSystem ( uMtgSitHit );
		else
			PlayMontageFrameSystem ( uMtgIdleHit );
	}
	// 소리 추가
	UGameplayStatics::PlaySound2D ( GetWorld ( ) , this->sAttackInfo.uHitSound );

	// UI hit newHp 전달하기
	this->Hp -= attackInfoHit.DamageAmount;
	this->GameModeMH->UpdatePlayerHP ( this , this->Hp );

	// 좀 있다 이동 시키기
	this->eCharacterState = ECharacterStateInteraction::HitGround;
	//camera 효과 추가하기s

	//나이아가라 시스템
	if ( false == attackInfoHit.niagaraSystemArray.IsEmpty ( ) )
	{
		for ( auto* niagara : attackInfoHit.niagaraSystemArray )
		{
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , niagara , attackInfo.skellEffectLocation );
			// Niagara 시스템을 스폰하고 컴포넌트를 가져옴
			UNiagaraComponent* niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation (
				this ,
				niagara ,  // Niagara 시스템의 참조
				attackInfoHit.skellEffectLocation ,
				FRotator::ZeroRotator ,
					FVector ( 1.0f ) ,  // 기본 스케일
					true ,  // AutoDestroy를 true로 설정
					true ,  // AutoActivate를 true로 설정
					ENCPoolMethod::None // 풀링 방식을 None으로 설정
			);
			// 자동으로 파괴되도록 설정 (한 번만 실행 후 제거)
			if ( niagaraComponent )
			{
				niagaraComponent->SetWorldScale3D ( FVector ( 0.1f ) );
				niagaraComponent->SetAutoDestroy ( true );
			}
		}
		UE_LOG ( LogTemp , Warning , TEXT ( "test : %d" ) , attackInfoHit.niagaraSystemArray.Num() );
		attackInfoHit.niagaraSystemArray.Empty ( );
	}
	//파티클 시스템
	if ( false == attackInfoHit.particleSystemArray.IsEmpty ( ) )
	{
		for ( auto* particle : attackInfoHit.particleSystemArray )
		{
			UGameplayStatics::SpawnEmitterAtLocation ( GetWorld ( ) , particle , attackInfoHit.skellEffectLocation );
		}
	}
	// 	if ( uNS_DefenceEffect )
	// 		UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , uNS_HitEffect , attackInfoHit.skellEffectLocation );

	this->CommandAllStop ( );
	return true;
}

void  ACPP_CharacterPaul::CommentHitFrameExecute ( )
{
	float radius = 20.0f;
	// 스페셜 스킬 이동기 등
	if ( sAttackInfo.DamagePoint == EDamagePointInteraction::Special )
		return;

	// 맞는 부분 실행
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn) );
	TArray<AActor*> ignoreActors;
	ignoreActors.Init ( this , 1 );
	TArray<AActor*> outActors;
	sAttackInfo.skellEffectLocation += this->GetActorLocation ( );
	FVector sphereSpawnLocation = sAttackInfo.skellEffectLocation;
	UClass* seekClass = ACPP_Tekken8CharacterParent::StaticClass ( );

	// 맞은거 판별
	bool hit = UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );;
	if ( hit )
	{
		for ( AActor* hitActor : outActors )
		{
			if ( hitActor->IsA<ACPP_Tekken8CharacterParent> ( ) )
			{
				ACPP_Tekken8CharacterParent* hitCharacter = Cast<ACPP_Tekken8CharacterParent> ( hitActor );
				//때려보기
				if ( hitCharacter->HitDecision ( sAttackInfo , this ) )
					sFrameStatus.FrameBlockUsing = sAttackInfo.OwnerHitFrame;
				else
					sFrameStatus.FrameBlockUsing = sAttackInfo.OwnerGuardFrame;
			}
		}
	}
	else
	{
		// 아무도 안맞았을때
		sFrameStatus.FrameBlockUsing = sAttackInfo.OwnerMissFrame;
	}
	sAttackInfo.niagaraSystemArray.Empty();
	if ( sAttackInfo.DamagePoint == EDamagePointInteraction::Top )
		sAttackInfo.debugColor = FColor ( 255 , 0 , 0 );
	else if ( sAttackInfo.DamagePoint == EDamagePointInteraction::Middle )
		sAttackInfo.debugColor = FColor ( 255 , 125 , 0 );
	else if ( sAttackInfo.DamagePoint == EDamagePointInteraction::Lower )
		sAttackInfo.debugColor = FColor ( 0 , 0 , 255 );

	if ( DebugMode )
		DrawDebugSphere ( GetWorld ( ) , sAttackInfo.skellEffectLocation , radius , 26 , sAttackInfo.debugColor , false , 1.0f );
}


FInputKey ACPP_CharacterPaul::GetCurrInputKey ( )
{
	return this->CurrInputKey;
}

int32 ACPP_CharacterPaul::GetCurrInputKeyValue ( )
{
	int32 KeyValue =
		//this->CurrInputKey.bKey1		* 0b0000000000000001 +
		this->CurrInputKey.bKey2 * 0b0000000000000010 +
		//this->CurrInputKey.bKey3		* 0b0000000000000100 +
		this->CurrInputKey.bKey4 * 0b0000000000001000 +
		//this->CurrInputKey.bKey5		* 0b0000000000010000 +
		this->CurrInputKey.bKey6 * 0b0000000000100000 +
		//this->CurrInputKey.bKey7		* 0b0000000001000000 +
		this->CurrInputKey.bKey8 * 0b0000000010000000 +
		//this->CurrInputKey.bKey9		* 0b0000000100000000 +
		this->CurrInputKey.bLeftPunch * 0b0000001000000000 +
		this->CurrInputKey.bRightPunch * 0b0000010000000000 +
		this->CurrInputKey.bLeftKick * 0b0000100000000000 +
		this->CurrInputKey.bRightKick * 0b0001000000000000;

	return KeyValue;
}

bool ACPP_CharacterPaul::SetCurrInputKeyValue ( int32 value )
{
	//this->CurrInputKey.bKey1		= value & 0b0000000000000001;
	this->CurrInputKey.bKey2 = value & 0b0000000000000010;
	//this->CurrInputKey.bKey3		= value & 0b0000000000000100;
	this->CurrInputKey.bKey4 = value & 0b0000000000001000;
	//this->CurrInputKey.bKey5		= value & 0b0000000000010000;
	this->CurrInputKey.bKey6 = value & 0b0000000000100000;
	//this->CurrInputKey.bKey7		= value & 0b0000000001000000;
	this->CurrInputKey.bKey8 = value & 0b0000000010000000;
	//this->CurrInputKey.bKey9		= value & 0b0000000100000000;
	this->CurrInputKey.bLeftPunch = value & 0b0000001000000000;
	this->CurrInputKey.bRightPunch = value & 0b0000010000000000;
	this->CurrInputKey.bLeftKick = value & 0b0000100000000000;
	this->CurrInputKey.bRightKick = value & 0b0001000000000000;

	return true;
}

bool ACPP_CharacterPaul::SetCurrInputKey ( FInputKey value )
{
	this->CurrInputKey = value;
	return true;
}

int32 ACPP_CharacterPaul::InputKeyValue ( int ArrowKey , bool LeftArm , bool RightArm , bool LeftKick , bool RightKick )
{
	int32 inputValue = 0;

	inputValue = (ArrowKey ? 1 << (ArrowKey - 1) : 0) +
		LeftArm * 0b0000001000000000 +
		RightArm * 0b0000010000000000 +
		LeftKick * 0b0000100000000000 +
		RightKick * 0b0001000000000000;
	return inputValue;
}