// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AICharacterController.h"
#include "AICharacterAnimInstance.h"
#include "AIStateBackDash.h"
#include "AIStateRun.h"
#include "AIStateWalkBack.h"
#include "AIStateWalkForward.h"
#include "AIStateIdle.h"
#include "Components/CapsuleComponent.h"
#include "AIStateAttackLF.h"
#include "AIStateAttackRH.h"
#include "Components/SphereComponent.h"
#include "AIStateHit.h"
#include "AIStateComboLaserAttack.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "AIStateHitFalling.h"
#include "AIStateBound.h"
#include "GameMode_MH.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "AIStateWalkCross.h"
#include "AIStateAttackLH.h"
#include "AIStateKnockDown.h"
#include "BrainComponent.h"
#include "GameMode_MH.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> meshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Jaebin/Kazuya/T-Pose_Final/T-Pose2_UE.T-Pose2_UE'" ) );
	if ( meshFinder.Succeeded ( ) )
	{
		GetMesh ( )->SetSkeletalMesh (meshFinder.Object );
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> animFinder ( TEXT ( "/Script/Engine.AnimBlueprint'/Game/LSJ/Blueprint/ABP_AICharacter3.ABP_AICharacter3_C'" ) );
	if ( animFinder.Succeeded ( ) )
	{
		GetMesh ( )->SetAnimInstanceClass ( animFinder.Class );
	}
	GetMesh ( )->SetRelativeScale3D ( FVector ( 0.1f , 0.1f , 0.1f ) );
	GetMesh( )->SetRelativeLocation(FVector( -20.0 ,0,-90.f));
	GetMesh ( )->SetRelativeRotation ( FRotator (  0, -90.f , 0 ) );

	collisionLH = CreateDefaultSubobject<USphereComponent> ( TEXT ( "collisionLH" ) );
	collisionRH = CreateDefaultSubobject<USphereComponent> ( TEXT ( "collisionRH" ) );
	collisionLF = CreateDefaultSubobject<USphereComponent> ( TEXT ( "collisionLF" ) );
	collisionRF = CreateDefaultSubobject<USphereComponent> ( TEXT ( "collisionRF" ) );
	collisionLH->SetupAttachment(GetMesh(),TEXT("middle_02_l" ));
	collisionLH->SetSphereRadius(92.f);
	collisionLH->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	collisionRH->SetupAttachment ( GetMesh ( ) ,TEXT ( "middle_02_r" ));
	collisionRH->SetSphereRadius ( 92.f );
	collisionRH->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	collisionLF->SetupAttachment ( GetMesh ( ) , TEXT ( "ball_l" ) );
	collisionLF->SetSphereRadius ( 92.f );
	collisionLF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	collisionRF->SetupAttachment ( GetMesh ( ) , TEXT ( "ball_r" ) );
	collisionRF->SetSphereRadius ( 92.f );
	collisionRF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	//몸체 콜리전
	collisionTop = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "collisionTop" ) );
	collisionTop->SetupAttachment ( GetMesh ( ) , TEXT ( "head" ) );
	collisionMiddle = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "collisionMiddle" ) );
	collisionMiddle->SetupAttachment ( GetMesh ( ) , TEXT ( "spine_01" ) );
	collisionLower = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "collisionLower" ) );
	collisionLower->SetupAttachment ( GetMesh ( ) , TEXT ( "pelvis" ) );
	collisionTop->SetBoxExtent ( FVector ( 311.232643 , 166.634513 , 240.066655 ) );
	collisionMiddle->SetBoxExtent ( (FVector ( 311.232643 , 315.176916 , 264.956523 )) );
	collisionMiddle->SetUsingAbsoluteRotation ( true );
	collisionMiddle->SetRelativeLocation ( FVector ( 0,0 , 99.999999 ) );
	collisionLower->SetBoxExtent ( FVector ( 319.717793 , 321.694333 ,336.888169 ) );
	collisionLower->SetUsingAbsoluteRotation ( true );
	collisionLower->SetRelativeLocation ( FVector ( 0.092165 , 531.000000 , 117.000000 ) );
	//collisionBody = CreateDefaultSubobject<UCapsuleComponent> ( TEXT ( "collisionBody" ) );
	//collisionBody->SetupAttachment ( GetMesh ( ) , TEXT ( "spine_01" ) );
	//콜리전 설정
	//플레이어랑만 충돌

	stateBackDash = CreateDefaultSubobject<UAIStateBackDash>(TEXT("stateBackDash"));
	stateBackDash->SetStateOwner(this);
	stateRun = CreateDefaultSubobject<UAIStateRun> ( TEXT ( "stateRun" ) );
	stateRun->SetStateOwner ( this );
	stateWalkBack = CreateDefaultSubobject<UAIStateWalkBack> ( TEXT ( "stateWalkBack" ) );
	stateWalkBack->SetStateOwner ( this );
	stateWalkForward = CreateDefaultSubobject<UAIStateWalkForward> ( TEXT ( "stateWalkForward" ) );
	stateWalkForward->SetStateOwner ( this );
	stateIdle = CreateDefaultSubobject<UAIStateIdle> ( TEXT ( "stateIdle" ) );
	stateIdle->SetStateOwner ( this );
	stateAttackLF = CreateDefaultSubobject<UAIStateAttackLF> ( TEXT ( "stateAttackLF" ) );
	stateAttackLF->SetStateOwner ( this );
	stateAttackRH = CreateDefaultSubobject<UAIStateAttackRH> ( TEXT ( "stateAttackRH" ) );
	stateAttackRH->SetStateOwner ( this );
	stateAttackLH = CreateDefaultSubobject<UAIStateAttackLH> ( TEXT ( "stateAttackLH" ) );
	stateAttackLH->SetStateOwner ( this );
	stateHit=CreateDefaultSubobject<UAIStateHit> ( TEXT ( "stateHit" ) );
	stateHit->SetStateOwner ( this );
	stateHitFalling = CreateDefaultSubobject<UAIStateHitFalling> ( TEXT ( "stateHitFalling" ) );
	stateHitFalling->SetStateOwner ( this );
	stateBound = CreateDefaultSubobject<UAIStateBound> ( TEXT ( "stateBound" ) );
	stateBound->SetStateOwner ( this );
	stateComboLaserAttack = CreateDefaultSubobject<UAIStateComboLaserAttack> ( TEXT ( "stateComboLaserAttack" ));
	stateComboLaserAttack->SetStateOwner ( this );
	stateWalkCross = CreateDefaultSubobject<UAIStateWalkCross> ( TEXT ( "stateWalkCross" ) );
	stateWalkCross->SetStateOwner ( this );
	stateKnockDown = CreateDefaultSubobject<UAIStateKnockDown> ( TEXT ( "stateKnockDown" ) );
	stateKnockDown->SetStateOwner ( this );
	//콤보 공격 정보
	FAttackInfoInteraction attack1;
	attack1.KnockBackDirection = FVector (250.f,0.f,0.f); //-0.5 뒤로 밀려난다 5*50 = 250.0f
	attack1.DamageAmount = 10;
	attack1.DamagePoint = EDamagePointInteraction::Middle;
	attack1.HitFrame= 13;
	attack1.RetrieveFrame = 11;

	attack1.OwnerGuardFrame = -12;
	attack1.OppositeHitFrame = 101;
	attack1.OppositeCounterFrame = 101;
	FAttackInfoInteraction attack2;
	attack2.KnockBackDirection = FVector ( 1000.f * 50.f , 0.f , 10.f * 50.f); //-11.f , 0.f , 10.f 
	attack2.DamageAmount = 12;
	attack2.DamagePoint = EDamagePointInteraction::Middle;
	attack2.HitFrame = 49;
	attack2.RetrieveFrame = 75;
	attack2.OwnerGuardFrame = -12;
	attack2.OppositeHitFrame = 101;
	attack2.OppositeCounterFrame = 101;
	FAttackInfoInteraction attack3;
	attack3.KnockBackDirection = FVector ( 200.f , 0.f , 0.f ); //-0.5 보다 적게 예상 4*
	attack3.DamageAmount = 5;
	attack3.DamagePoint = EDamagePointInteraction::Middle;
	attack3.HitFrame = 49;
	attack3.RetrieveFrame = 75;
	attack3.OwnerGuardFrame = -12;
	attack3.OppositeHitFrame = 101;
	attack3.OppositeCounterFrame = 101;
	FAttackInfoInteraction attack4;
	attack4.KnockBackDirection = FVector ( 150.f , 0.f , 0.f ); //-0.5 보다 적게 예상 3*
	attack4.DamageAmount = 8;
	attack4.DamagePoint = EDamagePointInteraction::Middle;
	attack4.HitFrame = 49;
	attack4.RetrieveFrame = 75;
	attack4.OwnerGuardFrame = -12;
	attack4.OppositeHitFrame = 101;
	attack4.OppositeCounterFrame = 101;
	FAttackInfoInteraction attack5;
	attack5.KnockBackDirection = FVector ( 200.f , 0.f , 40.f ); // 2,0,4
	attack5.DamageAmount = 18;
	attack5.DamagePoint = EDamagePointInteraction::Middle;
	attack5.HitFrame = 49;
	attack5.RetrieveFrame = 75;
	attack5.OwnerGuardFrame = -12;
	attack5.OppositeHitFrame = 101;
	attack5.OppositeCounterFrame = 101;
	FAttackInfoInteraction attack6;
	attack6.KnockBackDirection = FVector ( 200.f , 0.f , 50.f ); //-0.5 보다 적게 예상 3*
	attack6.DamageAmount = 25;
	attack6.DamagePoint = EDamagePointInteraction::Middle;
	attack6.HitFrame = 49;
	attack6.RetrieveFrame = 75;
	attack6.OwnerGuardFrame = -12;
	attack6.OppositeHitFrame = 101;
	attack6.OppositeCounterFrame = 101;
	FAttackInfoInteraction attack7;
	attack7.KnockBackDirection = FVector ( 300.f , 0.f , 40.f ); //-0.5 보다 적게 예상 3*
	attack7.DamageAmount = 5;
	attack7.DamagePoint = EDamagePointInteraction::Middle;
	attack7.HitFrame = 49;
	attack7.RetrieveFrame = 75;
	attack7.OwnerGuardFrame = -12;
	attack7.OppositeHitFrame = 101;
	attack7.OppositeCounterFrame = 101;
	FAttackInfoInteraction attack8;
	attack8.KnockBackDirection = FVector ( 300.f , 0.f , 100.f ); // 2,0,4
	attack8.DamageAmount = 8;
	attack8.DamagePoint = EDamagePointInteraction::Middle;
	attack8.HitFrame = 49;
	attack8.RetrieveFrame = 75;
	attack8.OwnerGuardFrame = -12;
	attack8.OppositeHitFrame = 101;
	attack8.OppositeCounterFrame = 101;
	FAttackInfoInteraction attack9;
	attack9.KnockBackDirection = FVector ( 300.f , 0.f , 50.f ); //-0.5 보다 적게 예상 3*
	attack9.DamageAmount = 25;
	attack9.DamagePoint = EDamagePointInteraction::Middle;
	attack9.HitFrame = 49;
	attack9.RetrieveFrame = 75;
	attack9.OwnerGuardFrame = -12;
	attack9.OppositeHitFrame = 101;
	attack9.OppositeCounterFrame = 101;
	stateComboLaserAttack->attackInfoArray.Add(attack1);
	stateComboLaserAttack->attackInfoArray.Add(attack2);
	stateComboLaserAttack->attackInfoArray.Add(attack3);
	stateComboLaserAttack->attackInfoArray.Add(attack4);
	stateComboLaserAttack->attackInfoArray.Add(attack5);
	stateComboLaserAttack->attackInfoArray.Add(attack6);
	stateComboLaserAttack->attackInfoArray.Add(attack7);
	stateComboLaserAttack->attackInfoArray.Add(attack8);
	stateComboLaserAttack->attackInfoArray.Add(attack9);

	FAttackInfoInteraction attackRHMiddle;
	attackRHMiddle.KnockBackDirection = FVector ( 200.f , 0.f , 00.f ); //-0.5 보다 적게 예상 3*
	attackRHMiddle.KnockBackFallingDirection = FVector ( 100.f , 0.f , 10.f ); // 맞았을때
	attackRHMiddle.DamageAmount = 23;
	attackRHMiddle.DamagePoint = EDamagePointInteraction::Middle;
	attackRHMiddle.HitFrame = 20; //HitFrame
	attackRHMiddle.RetrieveFrame = 19; //회수Frame
	//내가 손해면 - 상대가 손해면 +
	attackRHMiddle.HitFrame = 60;
	attackRHMiddle.GrardFrame = -12; 
	attackRHMiddle.CounterFrame = 60; 
	attackRHMiddle.MissFrame = 0;
	SetAttackInfoOwnerOpposite(attackRHMiddle); //나머지값 넣기
	stateAttackRH->attackInfoArray.Add ( attackRHMiddle );

	//내가 손해면 - 상대가 손해면 +
	FAttackInfoInteraction attackTopLH;
	attackTopLH.KnockBackDirection = FVector ( 200.f , 0.f , 0.f ); // 가드했을때
	attackTopLH.KnockBackFallingDirection = FVector ( 50.f , 0.f , 0.f ); // 맞았을때
	attackTopLH.DamageAmount = 5;
	attackTopLH.DamagePoint = EDamagePointInteraction::Top;
	attackTopLH.HitFrame = 10; //HitFrame
	attackTopLH.RetrieveFrame = 20; //회수Frame
	attackTopLH.GrardFrame = 1;
	attackTopLH.HitFrame = 8;
	attackTopLH.CounterFrame = 8;
	attackTopLH.MissFrame = 0;
	SetAttackInfoOwnerOpposite ( attackTopLH );
	stateAttackLH->attackInfoArray.Add ( attackTopLH );

	//내가 손해면 - 상대가 손해면 +
	FAttackInfoInteraction attackLowerLF;
	attackLowerLF.KnockBackDirection = FVector ( 200.f , 0.f , 0.f ); // 가드했을때
	attackLowerLF.KnockBackFallingDirection = FVector ( 50.f , 0.f , 0.f ); // 맞았을때
	attackLowerLF.DamageAmount = 7;
	attackLowerLF.DamagePoint = EDamagePointInteraction::Lower;
	attackLowerLF.HitFrame = 12; //HitFrame
	attackLowerLF.RetrieveFrame = 15; //회수Frame
	attackLowerLF.GrardFrame = -13;
	attackLowerLF.HitFrame = -2;
	attackLowerLF.CounterFrame = -2;
	attackLowerLF.MissFrame = 0;
	SetAttackInfoOwnerOpposite ( attackLowerLF );
	stateAttackLF->attackInfoArray.Add ( attackLowerLF );
	
	//이펙트
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE ( TEXT ( "/Script/Niagara.NiagaraSystem'/Game/Jaebin/Effects/Hit_High.Hit_High'" ) );
	if ( NE.Succeeded ( ) )
	{
		niagaraFXSystem = NE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> basicAttack1FXSystemFinder ( TEXT ( "/Script/Niagara.NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Blink_Fire.NS_Blink_Fire'" ) );
	if ( basicAttack1FXSystemFinder.Succeeded ( ) )
	{
		basicAttack1FXSystem = basicAttack1FXSystemFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> basicAttack2FXSystemFinder ( TEXT ( "/Script/Niagara.NiagaraSystem'/Game/MegaMagicVFXBundle/VFX/MagicalExplosionsVFX/VFX/FlameBlast/Systems/N_FlameBlast.N_FlameBlast'" ) );
	if ( basicAttack2FXSystemFinder.Succeeded ( ) )
	{
		basicAttack2FXSystem = basicAttack2FXSystemFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> blockedFXSystemFinder ( TEXT ( "/Script/Niagara.NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Sparks/NS_Sparks_26.NS_Sparks_26'" ) );
	if ( blockedFXSystemFinder.Succeeded ( ) )
	{
		blockedFXSystem = blockedFXSystemFinder.Object;
	}
	//사운드
	ConstructorHelpers::FObjectFinder<USoundBase> attackLFSFVFinder ( TEXT ( "/Script/Engine.SoundWave'/Game/LSJ/Sound/AttackLF.AttackLF'" ) );
	if ( attackLFSFVFinder.Succeeded ( ) )
	{
		attackLFSFV = attackLFSFVFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> attackLHSFVFinder ( TEXT ( "/Script/Engine.SoundWave'/Game/LSJ/Sound/AttackLH.AttackLH'" ) );
	if ( attackLHSFVFinder.Succeeded ( ) )
	{
		attackLHSFV = attackLHSFVFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> attackRFSFVFinder ( TEXT ( "/Script/Engine.SoundWave'/Game/LSJ/Sound/AttackRF.AttackRF'" ) );
	if ( attackRFSFVFinder.Succeeded ( ) )
	{
		attackRFSFV = attackRFSFVFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> attackRHSFVFinder ( TEXT ( "/Script/Engine.SoundWave'/Game/LSJ/Sound/AttackRH.AttackRH'" ) );
	if ( attackRHSFVFinder.Succeeded ( ) )
	{
		attackRHSFV = attackRHSFVFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> guardSFVFinder ( TEXT ( "/Script/Engine.SoundWave'/Game/LSJ/Sound/Guard.Guard'" ) );
	if ( guardSFVFinder.Succeeded ( ) )
	{
		guardSFV = guardSFVFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> hitLastSFVFinder ( TEXT ( "/Script/Engine.SoundWave'/Game/LSJ/Sound/HitLast.HitLast'" ) );
	if ( hitLastSFVFinder.Succeeded ( ) )
	{
		hitLastSFV = hitLastSFVFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> hitStrongSFVFinder ( TEXT ( "/Script/Engine.SoundWave'/Game/LSJ/Sound/HitStrong.HitStrong'" ) );
	if ( hitStrongSFVFinder.Succeeded ( ) )
	{
		hitStrongSFV = hitStrongSFVFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> hitWeakSFVFinder ( TEXT ( "/Script/Engine.SoundWave'/Game/LSJ/Sound/HitWeak.HitWeak'" ) );
	if ( hitWeakSFVFinder.Succeeded ( ) )
	{
		hitWeakSFV = hitWeakSFVFinder.Object;
	}
	//중력적용
	//GetCharacterMovement()->bApplyGravityWhileJumping = true;
	//앞으로만 이동되게 하는 설정
	//GetCharacterMovement ( )->bOrientRotationToMovement = true;
	eCharacterState = ECharacterStateInteraction::Idle;

	AIControllerClass = AAICharacterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAICharacter::StartPlayer ( )
{
	if ( aiController->BrainComponent )
	{
		//중지
		//서있고 멈춰있는거

		//게임시작
		//캐릭터가 죽거나 시간제한이 걸렸을때
		blackboardComp->SetValueAsBool ( TEXT ( "IsStart" ) , false );

	}
}

void AAICharacter::EndPlayer ( )
{
	animInstance->StopAllMontages ( 0.0f );
	animInstance->InitializeAnimation ( );
	animInstance->bDie = false;

	blackboardComp->SetValueAsBool ( TEXT ( "IsStart" ) , true );
	blackboardComp->SetValueAsBool ( TEXT ( "IsHitFalling" ) , false );
	blackboardComp->SetValueAsBool ( TEXT ( "IsHit" ) , false );
	blackboardComp->SetValueAsBool ( TEXT ( "IsBound" ) , false );
	isResume = true;
	isPause = false;
}

void AAICharacter::SetAttackInfoOwnerOpposite ( FAttackInfoInteraction& attackInfo )
{
	//공격자
	attackInfo.OwnerHitFrame = attackInfo.RetrieveFrame + (attackInfo.HitFrame < 0 ? attackInfo.HitFrame * -1 : 0);
	attackInfo.OwnerGuardFrame = attackInfo.RetrieveFrame + (attackInfo.GrardFrame < 0 ? attackInfo.GrardFrame * -1 : 0);
	attackInfo.OwnerCounterFrame = attackInfo.RetrieveFrame + (attackInfo.CounterFrame < 0 ? attackInfo.CounterFrame * -1 : 0);
	attackInfo.OwnerMissFrame = attackInfo.RetrieveFrame + (attackInfo.MissFrame < 0 ? attackInfo.MissFrame * -1 : 0);

	//수비자
	attackInfo.OppositeHitFrame = attackInfo.RetrieveFrame + (attackInfo.HitFrame > 0 ? attackInfo.HitFrame : 0);
	attackInfo.OppositeGuardFrame = attackInfo.RetrieveFrame + (attackInfo.GrardFrame > 0 ? attackInfo.GrardFrame : 0);
	attackInfo.OppositeCounterFrame = attackInfo.RetrieveFrame + (attackInfo.CounterFrame > 0 ? attackInfo.CounterFrame : 0);
}
// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	stateWalkForward->SetComponentTickEnabled ( false );
	stateWalkBack->SetComponentTickEnabled ( false );
	animInstance = Cast<UAICharacterAnimInstance> (GetMesh()->GetAnimInstance());
	if ( animInstance )
	{
		if(stateAttackLF->attackInfoArray.IsValidIndex ( 0 ))
			stateAttackLF->attackInfoArray[0].hitMontage=animInstance->hitLowerRFMontage;
		if ( stateAttackRH->attackInfoArray.IsValidIndex ( 0 ) )
			stateAttackRH->attackInfoArray[0].hitMontage = animInstance->hitFallingRHMontage;
		//animInstance->OnMontageEnded.AddDynamic ( this , &AAICharacter::HandleOnMontageEnded );
	}
	
	//콜리전
	//if ( IsPlayer1 )
	//	GetCapsuleComponent ( )->SetCollisionProfileName ( FName ( TEXT ( "Player1Capsule" ) ) );
	//else
	//	GetCapsuleComponent ( )->SetCollisionProfileName ( FName ( TEXT ( "Player2Capsule" ) ) );

	//GetRootComponent()->SetCollisionName
	//FTimerHandle handle;
	//GetWorld ( )->GetTimerManager ( ).SetTimer ( handle ,FTimerDelegate::CreateLambda ([this]() {
	//	ChangeState ( stateWalkBack );
	//	}) , 10.0f ,false);
	collisionLF->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnCollisionLFBeginOverlap );
	collisionRF->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnCollisionRFBeginOverlap );
	collisionLH->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnCollisionLHBeginOverlap );
	collisionRH->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnCollisionRHBeginOverlap );
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic ( this , &AAICharacter::OnHitBeginOverlap );
	
	// Blackboard 컴포넌트에 접근하여 값을 설정
	aiController = Cast<AAICharacterController> ( GetController ( ) );
	check(aiController);
	//카메라 좌측인지 오른쪽인지 체크
	float cameraDirection = FVector::DotProduct ( UGameplayStatics::GetPlayerPawn ( GetWorld ( ) , 0 )->GetActorRightVector ( ) , GetActorLocation ( ) );
	if ( cameraDirection > 0 )
	{
		startDirection *= .0;
		aiController->SetBehaviorTree ( 1 );
	}
	else
	{
	
		aiController->SetBehaviorTree ( 2 );
	}
	blackboardComp = aiController->GetBlackboardComponent ( );
	check ( blackboardComp );

	gameMode = Cast<AGameMode_MH> ( UGameplayStatics::GetGameMode ( GetWorld ( ) ) );

	previousLocation = GetActorLocation ( );
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ( gameMode )
	{
		if ( gameMode->CurrentState == EGameState::RoundStart && !isPause )
		{
			//서있게
			ExitCurrentState ( ECharacterStateInteraction::Idle );
			SetStateIdle ( );
			blackboardComp->SetValueAsBool ( TEXT ( "IsStart" ) , false );
			animInstance->StopAllMontages ( 0.0f );
			animInstance->InitializeAnimation ( );
			animInstance->bDie = false;
			animInstance->bFalling = false;
			animInstance->bKnockDown= false;
			blackboardComp->SetValueAsBool ( TEXT ( "IsStart" ) , false );
			blackboardComp->SetValueAsBool ( TEXT ( "IsHitFalling" ) , false );
			blackboardComp->SetValueAsBool ( TEXT ( "IsHit" ) , false );
			blackboardComp->SetValueAsBool ( TEXT ( "IsBound" ) , false );
			isPause = true;
		}
		else if ( gameMode->CurrentState == EGameState::InProgress && isPause )
		{
			//움직이게
			ExitCurrentState ( ECharacterStateInteraction::Idle );
			SetStateIdle ( );
			blackboardComp->SetValueAsBool ( TEXT ( "IsStart" ) , false );
			animInstance->StopAllMontages ( 0.0f );
			animInstance->InitializeAnimation ( );
			animInstance->bDie = false;

			blackboardComp->SetValueAsBool ( TEXT ( "IsStart" ) , false );
			blackboardComp->SetValueAsBool ( TEXT ( "IsHitFalling" ) , false );
			blackboardComp->SetValueAsBool ( TEXT ( "IsHit" ) , false );
			blackboardComp->SetValueAsBool ( TEXT ( "IsBound" ) , false );
			blackboardComp->SetValueAsBool ( TEXT ( "IsStart" ) , true );
			isPause = false;
		}
		else if ( bIsDead )
		{
			//멈추게
			ExitCurrentState ( ECharacterStateInteraction::Idle );
			SetStateIdle ( );
			blackboardComp->SetValueAsBool ( TEXT ( "IsStart" ) , false );
			bIsDead = false;
		}
	}
	
	//if ( bIsDead )
	//{
	//	PauseAI ( ); 
	//	//최소 한번
	//	//라운드 끝에 한번
	//}
	//else
	//{
	//	ResumeAI ( );
	//	//라운드 시작 n초뒤 한번
	//}

	//현재 이동 속도
	FVector currentLocation = GetActorLocation ( );
	currentVelocity = (currentLocation - previousLocation) / DeltaTime;
	previousLocation = currentLocation;

	UpdateState( DeltaTime );
	
	

	//회전
	if ( bLookTarget )
	{
		targetRotator.Roll = GetActorRotation ( ).Roll;
		targetRotator.Pitch = GetActorRotation ( ).Pitch;
		SetActorRotation ( FMath::RInterpTo ( GetActorRotation ( ) , targetRotator , DeltaTime , 50.0f ) );
		if ( bLookTarget && FMath::Abs ( targetRotator.Yaw - GetActorRotation ( ).Yaw ) < 0.1 )
			bLookTarget = false;
	}

	if ( GetCharacterMovement ( )->IsFalling ( ) )
	{
		FVector Gravity = FVector ( 0 , 0 , -980 ); // 기본 중력 값
		AddMovementInput ( Gravity * DeltaTime,true);
	}

	//누워있을때 바닥인지 감지하는 line trace
	FHitResult hit;
	FVector traceStart = GetMesh ( )->GetBoneLocation ((TEXT ( "spine_01" )));//TEXT ( "spine_01" ) );
	FVector traceEnd = GetMesh ( )->GetBoneLocation ( (TEXT ( "spine_01" )) ) + GetCapsuleComponent ( )->GetUpVector () * -30.0f;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor ( this );
	GetWorld ( )->LineTraceSingleByChannel ( hit , traceStart , traceEnd , ECollisionChannel::ECC_Visibility , queryParams );
	DrawDebugLine ( GetWorld ( ) , traceStart , traceEnd , hit.bBlockingHit ? FColor::Blue : FColor::Red , false , .1f , 0 , 10.0f );
	if ( hit.bBlockingHit  )// && false ==IsValid ( hit.GetActor()) )
	{
		if ( animInstance )
			animInstance->bOnGround = true;
		//UE_LOG ( LogTemp , Log , TEXT ( "Trace hit actor: %s" ) , *hit.GetActor ( )->GetName ( ) );
	}

	if ( nullptr != aOpponentPlayer )
	{
		FString state = "";
		switch ((int) aOpponentPlayer->eCharacterState )
		{
		case 0:
			state = "move";
			break;
		case 1:
			state = "Idle";
			break;
		case 2:
			state = "AttackLower";
			break;
		case 3:
			state = "AttackMiddle";
			break;
		case 4:
			state = "AttackTop";
			break;
		case 5:
			state = "Air";
			break;
		case 6:
			state = "HitGround";
			break;
		case 7:
			state = "HitFalling";
			break;
		case 8:
			state = "GuardStand";
			break;
		case 9:
			state = "GuardSit";
			break;
		case 10:
			state = "Sit";
			break;
		case 11:
			state = "Down";
			break;
		case 12:
			state = "Up";
			break;
		case 13:
			state = "WalkForward";
			break;
		case 14:
			state = "WalkBack";
			break;
		case 15:
			state = "Run";
			break;
		case 16:
			state = "BackDash";
			break;
		case 17:
			state = "Turn";
			break;
		}
		//GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "eCharacterState : %s " ) , *state) );

	}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::ChangeState ( IAIStateInterface* NewState )
{
	//hit 했을때 필요할거 같다.
	//if ( NewState == stateHit && currentState != stateHit ) {
	//	currentState->Exit ( );
	//}
	preState = currentState;
	currentState = NewState;

	if ( currentState ) {
		currentState->Enter (animInstance);
	}
}

void AAICharacter::UpdateState(const float& deltatime)
{
	if ( currentState )
		currentState->Execute ( deltatime );
}

void AAICharacter::ExitCurrentState ( ECharacterStateInteraction state)
{
	if ( currentState ) {
		currentState->Exit ( );
	}
	//if( state == ECharacterStateInteraction::HitFalling )
		//ChangeState(stateIdle);
}

int8 AAICharacter::ChangeAttackMotionDependingOpponentState ( )
{
	check ( aOpponentPlayer );
	int8 randomAttackIndex;
	if ( aOpponentPlayer->eCharacterState == ECharacterStateInteraction::Air )
	{
		//공중 상태일때 상단공격
		randomAttackIndex = 1;
	}
	else if (aOpponentPlayer->eCharacterState == ECharacterStateInteraction::Sit )
	{
		//앉은 상태일때 중단 혹은 하단 공격
		randomAttackIndex = (int8)blackboardComp->GetValueAsEnum ( TEXT ( "ERandomAttack" ) );
		if( randomAttackIndex == 1 )
			randomAttackIndex = FMath::RandRange ( 2 , 3 );
	}
	else
	{
		//이전에 랜덤으로 선택된 공격
		randomAttackIndex= (int8)blackboardComp->GetValueAsEnum ( TEXT ( "ERandomAttack" ) );
		//스탠드 상태일때 랜덤 공격
		//randomAttackIndex = FMath::RandRange ( 1 , 3 );
	}
	//test
	//randomAttackIndex = 2;
	return randomAttackIndex;
}

float AAICharacter::GetBTWDistance ( )
{
	if ( nullptr == aOpponentPlayer )
		return 0;
	if ( nullptr == GetMesh ( ) )
		return 0;
	float distanceBTW = FVector::Dist ( aOpponentPlayer->GetMesh ( )->GetBoneLocation ( (TEXT ( "head" )) ) , GetMesh ( )->GetBoneLocation ( (TEXT ( "head" )) ) );
	distanceBTW -= 80;
	UE_LOG ( LogTemp , Error , TEXT ( "%f" ) , distanceBTW );
	return distanceBTW;
}

void AAICharacter::SetStateIdle ( )
{
	ChangeState ( stateIdle );
}

void AAICharacter::OnAttackCollisionLF ( )
{
	if ( IsAttacked || currentState == stateIdle )
		return;
	//collisionLF->SetCollisionEnabled ( ECollisionEnabled::QueryOnly);
	float radius = 40.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn ) );
	TArray<AActor*> ignoreActors;
	ignoreActors.Init ( this , 1 );
	TArray<AActor*> outActors;
	FVector sphereSpawnLocation = collisionLF->GetComponentLocation ( );
	UClass* seekClass = ACPP_Tekken8CharacterParent::StaticClass ( );
	bool hit = UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );;
	FAttackInfoInteraction hitInfo = SendAttackInfo ( );
	if ( hit )
	{
		for ( AActor* hitActor : outActors )
		{
			if ( hitActor->IsA<ACPP_Tekken8CharacterParent> ( ) )
			{
				ACPP_Tekken8CharacterParent* hitCharacter = Cast<ACPP_Tekken8CharacterParent> ( hitActor );
			
				if ( currentState->GetAttackCount ( ) == 0 )
				{
					//사운드
					UGameplayStatics::PlaySound2D ( GetWorld ( ) , attackLFSFV );
				}
			
				hitInfo.skellEffectLocation = sphereSpawnLocation;
				//공격 결과 blackboardComp에 넣기 
				blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , aOpponentPlayer->HitDecision ( hitInfo , this ) ? 1 : 0 ); //0 : hit - EAttackResult

				//DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
				IsAttacked = true;
			}
		}
	}
	else
	{
		// 아무도 안맞았을때
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , 2 ); //2 : Miss - EAttackResult
	}
}

void AAICharacter::OnAttackCollisionRF ( )
{
	if ( IsAttacked || currentState == stateIdle )
		return;
	//collisionRF->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	float radius = 20.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn ) );
	TArray<AActor*> ignoreActors;
	ignoreActors.Init ( this , 1 );
	TArray<AActor*> outActors;
	FVector sphereSpawnLocation = collisionRF->GetComponentLocation ( );
	UClass* seekClass = ACPP_Tekken8CharacterParent::StaticClass ( );
	bool hit = UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );;
	FAttackInfoInteraction hitInfo = SendAttackInfo ( );
	if ( hit )
	{
		for ( AActor* hitActor : outActors )
		{
			if ( hitActor->IsA<ACPP_Tekken8CharacterParent> ( ) )
			{
				ACPP_Tekken8CharacterParent* hitCharacter = Cast<ACPP_Tekken8CharacterParent> ( hitActor );
				if ( currentState->GetAttackCount ( ) == 0 )
					//사운드
					UGameplayStatics::PlaySound2D ( GetWorld ( ) , attackRFSFV );

				hitInfo.skellEffectLocation = sphereSpawnLocation;
				//공격 결과 blackboardComp에 넣기 
				blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , aOpponentPlayer->HitDecision ( hitInfo , this ) ? 1 : 0 ); //0 : hit - EAttackResult

				//DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
				IsAttacked = true;
			}
		}
	}
	else
	{
		// 아무도 안맞았을때
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , 2 ); //2 : Miss - EAttackResult
	}
}

void AAICharacter::OnAttackCollisionLH ( )
{
	if ( IsAttacked || currentState == stateIdle )
		return;
	//collisionLH->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );

	float radius = 20.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn ) );
	TArray<AActor*> ignoreActors;
	ignoreActors.Init ( this , 1 );
	TArray<AActor*> outActors;
	FVector sphereSpawnLocation = collisionLH->GetComponentLocation();
	UClass* seekClass = ACPP_Tekken8CharacterParent::StaticClass ( );
	bool hit = UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );;
	FAttackInfoInteraction hitInfo = SendAttackInfo ( );
	if ( hit )
	{
		for ( AActor* hitActor : outActors )
		{
			if ( hitActor->IsA<ACPP_Tekken8CharacterParent> ( ) )
			{
				ACPP_Tekken8CharacterParent* hitCharacter = Cast<ACPP_Tekken8CharacterParent> ( hitActor );
				if ( currentState->GetAttackCount ( ) == 0 )
					//사운드
					UGameplayStatics::PlaySound2D ( GetWorld ( ) , attackLHSFV );

				hitInfo.skellEffectLocation = sphereSpawnLocation;
				//공격 결과 blackboardComp에 넣기 
				blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , aOpponentPlayer->HitDecision ( hitInfo , this ) ? 1 : 0 ); //0 : hit - EAttackResult

				//DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
				IsAttacked = true;
			}
		}
	}
	else
	{
		// 아무도 안맞았을때
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , 2 ); //2 : Miss - EAttackResult
	}
}

void AAICharacter::OnAttackCollisionRH ( )
{
	if ( IsAttacked || currentState == stateIdle)
		return;
	//collisionRH->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	float radius = 20.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECollisionChannel::ECC_Pawn ) );
	TArray<AActor*> ignoreActors;
	ignoreActors.Init ( this , 1 );
	TArray<AActor*> outActors;
	FVector sphereSpawnLocation = collisionRH->GetComponentLocation ( );
	UClass* seekClass = ACPP_Tekken8CharacterParent::StaticClass ( );
	bool hit = UKismetSystemLibrary::SphereOverlapActors ( GetWorld ( ) , sphereSpawnLocation , radius , traceObjectTypes , seekClass , ignoreActors , outActors );;
	FAttackInfoInteraction hitInfo = SendAttackInfo ( );
	if ( hit  )
	{
		for ( AActor* hitActor : outActors )
		{
			if ( hitActor->IsA<ACPP_Tekken8CharacterParent> ( ) )
			{
				ACPP_Tekken8CharacterParent* hitCharacter = Cast<ACPP_Tekken8CharacterParent> ( hitActor );
				if ( currentState->GetAttackCount ( ) == 0 )
					//사운드
					UGameplayStatics::PlaySound2D ( GetWorld ( ) , attackRHSFV );

				hitInfo.skellEffectLocation = sphereSpawnLocation;
				//공격 결과 blackboardComp에 넣기 
				blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , aOpponentPlayer->HitDecision ( hitInfo , this ) ? 1 : 0 ); //0 : hit - EAttackResult

				//DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
				IsAttacked = true;
			}
		}
	}
	else
	{
		// 아무도 안맞았을때
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , 2 ); //2 : Miss - EAttackResult
	}
}

void AAICharacter::OffAttackCollisionLF ( )
{
	collisionLF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	//currentState->AddAttackCount ( 1 );
	IsAttacked = false;
}

void AAICharacter::OffAttackCollisionRF ( )
{
	collisionRF->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	//currentState->AddAttackCount ( 1 );
	IsAttacked = false;
}

void AAICharacter::OffAttackCollisionLH ( )
{
	collisionLH->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	//currentState->AddAttackCount ( 1 );
	IsAttacked = false;
}

void AAICharacter::OffAttackCollisionRH ( )
{
	collisionRH->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	//currentState->AddAttackCount(1);
	//GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "OffAttackCollisionRH : %f " ) , FVector::Dist(GetMesh()->GetBoneLocation ((TEXT("head"))), collisionRH->GetComponentLocation())));
	IsAttacked = false;
}

void AAICharacter::OnHitBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	//if ( SweepResult.GetActor ( ) == aOpponentPlayer )
	//{
	//	aOpponentPlayer->HitDecision ( SendAttackInfo ( ) , this );
	//	DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
	//	IsAttacked = true;
	//}
}

void AAICharacter::OnCollisionLHBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	if (SweepResult.GetActor() == aOpponentPlayer)
	{
		//공격 결과 blackboardComp에 넣기 
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , 0 ); //0 : hit - EAttackResult

		FAttackInfoInteraction hitInfo = SendAttackInfo ( );
		hitInfo.skellEffectLocation = collisionLH->GetComponentLocation();
		//공격 결과 blackboardComp에 넣기 
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , aOpponentPlayer->HitDecision ( hitInfo , this )?1:0 ); //0 : hit - EAttackResult

		//DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
		IsAttacked = true;
	}

	//DrawDebugSphere(GetWorld(),SweepResult.ImpactPoint,92.0f,2,FColor::Blue,false,5.f);
}

void AAICharacter::OnCollisionRHBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	if ( SweepResult.GetActor ( ) == aOpponentPlayer )
	{
		
		FAttackInfoInteraction hitInfo = SendAttackInfo ( );
		hitInfo.skellEffectLocation = collisionRH->GetComponentLocation ( );
		//공격 결과 blackboardComp에 넣기 
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , aOpponentPlayer->HitDecision ( hitInfo , this ) ); //0 : hit - EAttackResult
		//DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
		IsAttacked = true;
	}
	//카메라가 제자리로 안돌아간다
	//float InZoomAmount 어떤 값을 줘도 줌확대가 된다.
	// 확대할 위치 , 줌 정도 0.5 기본 , 흔들림정도 , 흔들림 시간
		//aiCharacter->aMainCamera->RequestZoomEffect(aiCharacter->GetActorLocation(),0.5f,1.0f,3.0f);
	//aMainCamera->RequestZoomEffect ( GetActorLocation ( ) , 0.5f , 100.0f , 0.5f );
	//DrawDebugSphere(GetWorld(),SweepResult.ImpactPoint,92.0f,2,FColor::Blue,false,5.f);
}

void AAICharacter::OnCollisionRFBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	if ( SweepResult.GetActor ( ) == aOpponentPlayer )
	{
		//공격 결과 blackboardComp에 넣기 
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , 0 ); //0 : hit - EAttackResult

		FAttackInfoInteraction hitInfo = SendAttackInfo ( );
		hitInfo.skellEffectLocation = collisionRF->GetComponentLocation ( );
		//공격 결과 blackboardComp에 넣기 
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , aOpponentPlayer->HitDecision ( hitInfo , this ) ); //0 : hit - EAttackResult

		//DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
		IsAttacked = true;
	}
}

void AAICharacter::OnCollisionLFBeginOverlap ( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if ( IsAttacked )
		return;
	if ( SweepResult.GetActor ( ) == aOpponentPlayer )
	{
		//공격 결과 blackboardComp에 넣기 
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , 0 ); //0 : hit - EAttackResult

		FAttackInfoInteraction hitInfo = SendAttackInfo ( );
		hitInfo.skellEffectLocation = collisionLF->GetComponentLocation ( );
		//공격 결과 blackboardComp에 넣기 
		blackboardComp->SetValueAsEnum ( TEXT ( "EAttackResult" ) , aOpponentPlayer->HitDecision ( hitInfo , this ) ); //0 : hit - EAttackResult

		//DrawDebugSphere ( GetWorld ( ) , collisionLH->GetComponentLocation ( ) , 20 , 26 , FColor ( 181 , 0 , 0 ) , true , 0.5f , 0 , 0.5f );
		IsAttacked = true;
	}
}
//void CheckCollision (bool guard, UBoxComponent* hitCollision )
//{
//	if ( guard )
//	{
//		if ( hitCollision == topCollision )
//		{
//			Play_Montage ( )
//		} 
//		else if ( hitCollision == topCollision )
//		{
//			Play_Montage ( )
//		}
//		else if ( hitCollision == topCollision )
//		{
//			Play_Montage ( )
//		}
//	}
//	else
//	{
//		if ( hitCollision == topCollision )
//		{
//			Play_Montage ( )
//		}
//		else if ( hitCollision == topCollision )
//		{
//			Play_Montage ( )
//		}
//		else if ( hitCollision == topCollision )
//		{
//			Play_Montage ( )
//		}
//	}
//}
//공격 받았을 때 //FAttackInfoInteraction attackInfo에서 공격받는 애니메이션 혹은 가드 애니메이션 전달 
bool AAICharacter::HitDecision ( FAttackInfoInteraction attackInfo , ACPP_Tekken8CharacterParent* ownerHitPlayer)
{
	//현재 상태와 공격정보를 확인해서 데미지 처리
	//공격 받았다면 Hit State 상태 처리 attackInfo를 전달 return true;
	//가드라면 ... return false;
	//attackInfo
	//if()
	//	CheckCollision ( bool guard , UBoxComponent * hitCollision )
	//else if()
	//	CheckCollision ( bool guard , UBoxComponent * hitCollision )
	//else
	//	CheckCollision ( bool guard , UBoxComponent * hitCollision )
	if ( blackboardComp )
	{
		preState = currentState;

		niagaraFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , niagaraFXSystem , attackInfo.skellEffectLocation);

		
		Hp = FMath::Clamp(Hp-attackInfo.DamageAmount,0.0f,MaxHp);

		gameMode = Cast<AGameMode_MH>(GetWorld()->GetAuthGameMode());
		if( gameMode )
			gameMode->UpdatePlayerHP(this,Hp);
		// 확대할 위치 , 줌 정도 0.5 기본 , 흔들림정도 , 흔들림 시간
		aMainCamera->RequestZoomEffect ( GetActorLocation ( ) , 0.5f , 10.0f , 0.3f );

		if ( false == attackInfo.niagaraSystemArray.IsEmpty ( ) )
		{
			for ( auto* niagara : attackInfo.niagaraSystemArray )
			{
				//UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , niagara , attackInfo.skellEffectLocation );
				// Niagara 시스템을 스폰하고 컴포넌트를 가져옴
				//UNiagaraComponent* niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation (
				//	this ,
				//	niagara ,  // Niagara 시스템의 참조
				//	attackInfo.skellEffectLocation,
				//	FRotator::ZeroRotator ,
				//		FVector ( 1.0f ) ,  // 기본 스케일
				//		true ,  // AutoDestroy를 true로 설정
				//		true ,  // AutoActivate를 true로 설정
				//		ENCPoolMethod::None // 풀링 방식을 None으로 설정
				//);

				// 자동으로 파괴되도록 설정 (한 번만 실행 후 제거)
				/*if ( niagaraComponent )
				{
					niagaraComponent->SetWorldScale3D ( FVector ( 0.1f ) );
					niagaraComponent->SetAutoDestroy ( true );
				}*/
			}
		}
		if ( false == attackInfo.particleSystemArray.IsEmpty ( ) )
		{
			for ( auto* particle : attackInfo.particleSystemArray )
			{
				UGameplayStatics::SpawnEmitterAtLocation ( GetWorld ( ) , particle , attackInfo.skellEffectLocation );
			}
		}
	

		//사운드
		UGameplayStatics::PlaySound2D ( GetWorld ( ) , hitWeakSFV );
		if ( currentState == stateKnockDown && stateKnockDown->isKnockDown )
		{
			//공격 받을 때 지연 프레임을 받아서 HitFalling 상태에 전달하고
			//HitFalling 상태에서 이전 상태가 stateKnockDown 이면 누워서 맞는 애니메이션을 지연 프레임만큼 실행하고
			//해당 애니메이션이 끝나면 stateKnockDown 에 지연이 끝났다는 것을 알리고 stateKnockDown 상태로 변경한다.
			ExitCurrentState ( ECharacterStateInteraction::HitGround );
			stateKnockDown->SetAttackInfo ( attackInfo );
			stateHitFalling->SetAttackInfo ( attackInfo );
			stateHitFalling->WasKnockDown = true;
			blackboardComp->SetValueAsBool ( TEXT ( "IsHitFalling" ) , true );
			eCharacterState = ECharacterStateInteraction::Sit;
		}
		else
		{
			if ( attackInfo.KnockBackDirection.Y > 0 || currentState == stateBound || currentState == stateHitFalling )
			{
				ExitCurrentState ( ECharacterStateInteraction::HitGround );
				if ( preState == stateHitFalling )
					stateHitFalling->WasHitFalling = true;
				stateHitFalling->SetAttackInfo ( attackInfo );
				blackboardComp->SetValueAsBool ( TEXT ( "IsHitFalling" ) , true );
			}
		
			/*else if ( )
			{
				stateBound->SetAttackInfo ( attackInfo );
				blackboardComp->SetValueAsBool ( TEXT ( "IsBound" ) , true );
			}*/
		
			else
			{
				ExitCurrentState ( ECharacterStateInteraction::HitGround );
				stateHit->SetAttackInfo ( attackInfo );
				blackboardComp->SetValueAsBool ( TEXT ( "IsHit" ) , true );
			}
		}
		OnHit.Broadcast ( );
		if ( Hp <= 0 )
		{
			bIsDead = true;
			animInstance->bDie = true;
		}
	}

	//ChangeState( stateHit );
	return true;
}

FAttackInfoInteraction AAICharacter::SendAttackInfo ( )
{
	auto* state = Cast<UAIStateComponent>(currentState);
	// 적절한 상태 클래스에 따라 상태 변경
	if ( state->IsA(UAIStateAttackLF::StaticClass()))
	{

	}
	else if ( state->IsA ( UAIStateAttackRH::StaticClass()))
	{

	}
	else if( state->IsA ( UAIStateComboLaserAttack::StaticClass()))
	{
	
	}
	if ( state->GetAttackCount ( ) == 0 )
	{
		state->attackInfoArray[0].niagaraSystemArray.Add ( basicAttack1FXSystem );
		state->attackInfoArray[0].niagaraSystemArray.Add ( basicAttack2FXSystem );
	}
	return state->attackInfoArray[state->GetAttackCount ( )];
}

void AAICharacter::CurrentAttackCountToZero ( )
{
	currentState->AddAttackCount(-1*currentState->GetAttackCount());
}

void AAICharacter::LookTarget (const float& deltaTime)
{
	if(nullptr==aOpponentPlayer)
		return;
	FVector opponentPlayerRotator = aOpponentPlayer->GetMesh()->GetBoneLocation((TEXT("head")));
	opponentPlayerRotator.Z = GetActorLocation ( ).Z;
	FRotator lookRotator = (opponentPlayerRotator - GetActorLocation ( )).Rotation ( );

	GetCapsuleComponent ( )->SetRelativeRotation (  lookRotator );
	lookRotator.Yaw += 180 * startDirection;
	GetMesh ( )->SetRelativeRotation ( lookRotator );
	//SetActorRotation ( FMath::RInterpTo ( GetActorRotation ( ) , lookRotator , deltaTime , 20.0f ) );
}
void AAICharacter::LookTarget ( const float& deltaTime , FRotator lookRotator)
{
	GetCapsuleComponent ( )->SetRelativeRotation ( lookRotator );
	lookRotator.Yaw += 180 * startDirection;
	GetMesh ( )->SetRelativeRotation ( lookRotator );
	
	/*GetCapsuleComponent ( )->SetWorldRotation ( FMath::RInterpTo ( GetCapsuleComponent ( )->GetComponentRotation ( ) , lookRotator , deltaTime , 1.0f ) );
	lookRotator.Yaw += 180 * startDirection;
	GetMesh ( )->SetWorldRotation ( FMath::RInterpTo ( GetMesh ( )->GetComponentRotation ( ) , lookRotator , deltaTime , 1.0f ) );
	*/
	
	//SetActorRotation ( FMath::RInterpTo ( GetActorRotation ( ) , lookRotator , deltaTime , 20.0f ) );
}
