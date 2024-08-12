// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Tekken8CharacterParent.h"
#include "Animation/AnimMontage.h"
#include "AICharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIStateComboLaserAttack.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

void UAICharacterAnimInstance::UpdateProperties ( )
{
    if ( owner == nullptr )
        owner = Cast<AAICharacter> ( TryGetPawnOwner ( ) );	// 소유자의 Pawn 를 가져온다.
    if ( owner )
    {

        // 공중에 있는지
        bIsInAir = owner->GetMovementComponent ( )->IsFalling ( );
        // Z축이 필요없기 때문에 Z는 0.f로 처리해서 속력를 구한다
        FVector velocity = owner->GetVelocity ( );
        velocityZ = velocity.Z;
        velocity = FVector ( velocity.X , velocity.Y , 0.f );
        movementSpeed = FVector ( velocity.X , velocity.Y , 0.f ).Size ( );

        // AnimInstance에 있는 CalculateDriection() 메소드를 통해 방향을 구한다.
        direction = CalculateDirection ( velocity , owner->GetActorRotation());
        
		if ( bStateWalkBack && GetInstanceForMontage (walkBackMontage)!=nullptr &&GetInstanceForMontage ( walkBackMontage )->GetBlendTime ( ) < 0.1f )
			bStateWalkBack = false;
        //bStateWalkBack = //실행되고 있을때
    }
}

void UAICharacterAnimInstance::NativeUpdateAnimation ( float DeltaSeconds )
{
    Super::NativeUpdateAnimation ( DeltaSeconds );
    UpdateProperties();

    //if ( Montage_IsPlaying ( walkBackMontage ) )
    //{
    //    //owner->GetMesh ( )->DetachFromParent();
    //    if ( BeforeLocation == NowLocation )
    //    {
    //        //원점과 중앙좌표의 오차를 저장
    //        NowLocation = owner->GetActorLocation ( ) - owner->GetMesh ( )->GetBoneTransform ( 0 ).GetLocation ( );
    //    }
    //    else
    //    {
    //        //이전의 좌표를 기억
    //        BeforeLocation = NowLocation;
    //        //원점과 중앙좌표의 오차를 저장
    //        NowLocation = owner->GetActorLocation ( ) - owner->GetMesh ( )->GetBoneTransform ( 0 ).GetLocation ( );
    //        FVector v = (BeforeLocation - NowLocation);
    //        owner->SetActorLocation ( owner->GetActorLocation ( ) + v * DeltaSeconds );

    //        //Z값은 언리얼의 Y좌표.캡슐에 지장이 생기니 X Y값만 조정한다
    //        owner->GetMesh ( )->MoveComponent ( FVector ( NowLocation.X* DeltaSeconds , NowLocation.Y* DeltaSeconds , 0 ) , owner->GetActorRotation() , false );
    //    }
    //}
}

void UAICharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if ( owner == nullptr )
	{
		owner = Cast<AAICharacter>(TryGetPawnOwner ( ));	// 소유자의 Pawn 를 가져온다.
	}
    if ( !OnMontageEnded.IsAlreadyBound ( this , &UAICharacterAnimInstance::HandleOnMontageEnded ) )
    OnMontageEnded.AddDynamic ( this , &UAICharacterAnimInstance::HandleOnMontageEnded );
}

void UAICharacterAnimInstance::PlayMontageAtFrameRate ( UAnimMontage* montage , int32 nFrames , float frameRate )
{
    if ( !montage ) return;

    // 몽타주 길이 (초)
    float montageLength = montage->GetPlayLength ( );

    // 목표 시간 (초)
    float targetDuration = nFrames / frameRate;

    // 재생 속도 계산
    float playRate = montageLength / targetDuration;

    // 몽타주 실행
    Montage_Play ( montage , playRate );
}

UAICharacterAnimInstance::UAICharacterAnimInstance ( )
{
    static ConstructorHelpers::FObjectFinder <UAnimMontage> hitFallingRHMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/A_FallBack_Mid_Roll2_Montage.A_FallBack_Mid_Roll2_Montage'" ) ); //"/Script/Engine.AnimSequence'/Game/Jaebin/Kazuya/Walk_Forward/Walking_Anim.Walking_Anim'" ) );
    if ( hitFallingRHMontageFinder.Succeeded ( ) )
        hitFallingRHMontage = hitFallingRHMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage> hitKnockDownMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/FallBack2_Montage.FallBack2_Montage'" ) ); //"/Script/Engine.AnimSequence'/Game/Jaebin/Kazuya/Walk_Forward/Walking_Anim.Walking_Anim'" ) );
    if ( hitKnockDownMontageFinder.Succeeded ( ) )
        hitKnockDownMontage = hitKnockDownMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage> knockDownMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Laying1_Montage.Laying1_Montage'" ) ); //"/Script/Engine.AnimSequence'/Game/Jaebin/Kazuya/Walk_Forward/Walking_Anim.Walking_Anim'" ) );
    if ( knockDownMontageFinder.Succeeded ( ) )
        knockDownMontage = knockDownMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage> standUpMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/A_StandUp_Front2_Montage.A_StandUp_Front2_Montage'" ) ); //"/Script/Engine.AnimSequence'/Game/Jaebin/Kazuya/Walk_Forward/Walking_Anim.Walking_Anim'" ) );
    if ( standUpMontageFinder.Succeeded ( ) )
        standUpMontage = standUpMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage> hitLowerRFMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/A_HitLeg_R_IP2_Montage.A_HitLeg_R_IP2_Montage'" ) ); //"/Script/Engine.AnimSequence'/Game/Jaebin/Kazuya/Walk_Forward/Walking_Anim.Walking_Anim'" ) );
    if ( hitLowerRFMontageFinder.Succeeded ( ) )
       hitLowerRFMontage = hitLowerRFMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage> walkForwardMontageFinder
    ( TEXT ("/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Step_Forward1_Montage.Step_Forward1_Montage'")); //"/Script/Engine.AnimSequence'/Game/Jaebin/Kazuya/Walk_Forward/Walking_Anim.Walking_Anim'" ) );
    if ( walkForwardMontageFinder.Succeeded ( ) )
        walkForwardMontage = walkForwardMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage> walkBackMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalStep_Backward_Anim1_Montage.FinalStep_Backward_Anim1_Montage'" ) );
    if ( walkBackMontageFinder.Succeeded ( ) )
        walkBackMontage= walkBackMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>idleMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/KB_Idle_T1_Montage.KB_Idle_T1_Montage'" ) );
    if ( idleMontageFinder.Succeeded ( ) )
       idleMontage = idleMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>attackRHMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/AM_A_PunchBody_R_M2_Montage.AM_A_PunchBody_R_M2_Montage'" ) );
    if ( attackRHMontageFinder.Succeeded ( ) )
        attackRHMontage = attackRHMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>attackLHMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Combo1/AM_KB_Cross-L_S_2_Montage.AM_KB_Cross-L_S_2_Montage'" ) );
    if ( attackLHMontageFinder.Succeeded ( ) )
        attackLHMontage = attackLHMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>attackLFMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Kicking_Anim1_Montage.Kicking_Anim1_Montage'" ) );
    if ( attackLFMontageFinder.Succeeded ( ) )
        attackLFMontage = attackLFMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>hitTopMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Hit_Face_Montage.Hit_Face_Montage'" ) );
    //( TEXT ( "/Script/Engine.AnimMontage'/Game/Kyoulee/character/Ani/Montage/Hit_Face_Montage.Hit_Face_Montage'" ) );
    if ( hitTopMontageFinder.Succeeded ( ) )
        hitTopMontage = hitTopMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>hitMiddleMontageFinder
    //( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Kicking_Anim1_Montage.Kicking_Anim1_Montage'" ) );
    (TEXT ( "/Script/Engine.AnimMontage'/Game/Kyoulee/character/Ani/Montage/Hit_Face_Montage.Hit_Face_Montage'" ));
    if ( hitMiddleMontageFinder.Succeeded ( ) )
        hitMiddleMontage = hitMiddleMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>uppercutLHMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/KB_Uppercut-LB_S_3_Montage.KB_Uppercut-LB_S_3_Montage'" ) );
    if ( uppercutLHMontageFinder.Succeeded ( ) )
        uppercutLHMontage = uppercutLHMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>comboLaserMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Combo1/Combo1.Combo1'" ) );
    if ( comboLaserMontageFinder.Succeeded ( ) )
        comboLaserMontage = comboLaserMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>hitFallingMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/FallBack1_Montage.FallBack1_Montage'" ) );
    if ( hitFallingMontageFinder.Succeeded ( ) )
        hitFallingMontage = hitFallingMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>hitFallingTurnMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/Combo1/A_FallBack_Twist_R2_Montage.A_FallBack_Twist_R2_Montage'" ) );
    if ( hitFallingTurnMontageFinder.Succeeded ( ) )
        hitFallingTurnMontage = hitFallingTurnMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>boundMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/NewAnimation6_Montage.NewAnimation6_Montage'" ) );
    if ( boundMontageFinder.Succeeded ( ) )
        boundMontage = boundMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>crossWalkClockwiseMontageFinder 
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/KB_Pivot-L_T1_Montage.KB_Pivot-L_T1_Montage'" ) );
    if ( crossWalkClockwiseMontageFinder.Succeeded ( ) )
        crossWalkClockwiseMontage = crossWalkClockwiseMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>crossWalkCounterclockwiseMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/KB_Pivot-R_T1_Montage.KB_Pivot-R_T1_Montage'" ) );
    if ( crossWalkCounterclockwiseMontageFinder.Succeeded ( ) )
        crossWalkCounterclockwiseMontage = crossWalkCounterclockwiseMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder <UAnimMontage>attackLowerLFMontageFinder
    ( TEXT ( "/Script/Engine.AnimMontage'/Game/LSJ/Animation/FinalAnimation/AM_LowKick_Left1_Montage.AM_LowKick_Left1_Montage'" ) );
    if ( attackLowerLFMontageFinder.Succeeded ( ) )
        attackLowerLFMontage = attackLowerLFMontageFinder.Object;
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE ( TEXT ( "/Script/Niagara.NiagaraSystem'/Game/Jaebin/Effects/Laser.Laser'" ) );
    if ( NE.Succeeded ( ) )
    {
        laserFXSystem = NE.Object;
    }
}

void UAICharacterAnimInstance::HandleOnMontageEnded ( UAnimMontage* Montage , bool bInterrupted )
{

    
    // Montage가 끝났을 때의 처리 로직
    if ( bInterrupted )
    {
    // Animation Montage가 정상적으로 끝나지 않고 중간에 인터럽트되었습니다. 인터럽트는 다른 애니메이션이 재생되었거나, 명시적으로 중단되는 등의 이유로 발생할 수 있습니다.

     /*   if ( Montage == hitTopMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitGround );
        }*/
    }
    else
    {
    // Animation Montage가 정상적으로 끝났습니다.
        if ( Montage == crossWalkCounterclockwiseMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::Move );
            //owner->SetActorLocation ( owner->GetActorLocation ( ) + BeforeLocation - NowLocation );
        }
        else
        if ( Montage == crossWalkClockwiseMontage)
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::Move );
            //owner->SetActorLocation ( owner->GetActorLocation ( ) + BeforeLocation - NowLocation );
        }
        else
        if ( Montage == walkForwardMontage)
        {
           owner->ExitCurrentState ( ECharacterStateInteraction::Move);
            //owner->SetActorLocation ( owner->GetActorLocation ( ) + BeforeLocation - NowLocation );
        }
        else
        if ( Montage == walkBackMontage )
        {
            //owner->SetActorLocation ( owner->GetActorLocation ( ) + BeforeLocation - NowLocation );
            //if ( OnLog ) UE_LOG ( LogTemp , Warning , TEXT ( "walkBackMontage walkBackMontage %s" ) , *Montage->GetName ( ) );
        }
        else
        if ( Montage == attackLFMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
        }
        else
		if ( Montage == attackLHMontage )
		{
			owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
		}
        else
            if ( Montage == attackRHMontage )
            {
                owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
            }

        else
            if ( Montage == uppercutLHMontage )
            {
                owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
            }
        else
        if ( Montage == comboLaserMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
        }
        else if ( Montage == hitMiddleMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitGround );
        }
        else if ( Montage == hitTopMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitGround );
        }
		 else if ( Montage == hitFallingMontage )
		 {
			 owner->ExitCurrentState ( ECharacterStateInteraction::HitFalling );
		 }
         else if ( Montage == hitFallingTurnMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitFalling );
        }
         else if ( Montage == boundMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::HitFalling );
        }
         else if ( Montage == attackLowerLFMontage )
        {
            owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
        }
         else
        {
           owner->ExitCurrentState ( ECharacterStateInteraction::AttackLower );
        }
    }
}

void UAICharacterAnimInstance::PlayComboLaserMontage()
{
    Montage_Play(comboLaserMontage);
}
void UAICharacterAnimInstance::PlayKnockDownMontage ( )
{
    Montage_Play ( knockDownMontage );
}
void UAICharacterAnimInstance::PlayBoundMontage ( )
{
    FAlphaBlendArgs a;

    Montage_Play( boundMontage , 0.5f , EMontagePlayReturnType::MontageLength , 0.2f , false );
}

void UAICharacterAnimInstance::PlayCrossWalkClockwiseMontage ( )
{
    Montage_Play ( crossWalkClockwiseMontage );
}

void UAICharacterAnimInstance::PlayCrossWalkCounterclockwiseMontage ( )
{
    Montage_Play ( crossWalkCounterclockwiseMontage );
}

void UAICharacterAnimInstance::PlayHitFallingMontage ( float nFrame , float frameRate )
{
    //PlayMontageAtFrameRate( hitFallingMontage )
    Montage_Play ( hitFallingMontage , 0.5f);
}

void UAICharacterAnimInstance::PlayHitFallingTurnMontage ( float nFrame , float frameRate )
{
    Montage_Play ( hitFallingTurnMontage , 0.5f );
}

void UAICharacterAnimInstance::PlayHitTopMontage ( float nFrame , float frameRate )
{
    PlayMontageAtFrameRate ( hitTopMontage , nFrame , frameRate );
}

void UAICharacterAnimInstance::PlayHitMiddleMontage ( float nFrame , float frameRate )
{
    PlayMontageAtFrameRate ( hitMiddleMontage , nFrame , frameRate );
}

void UAICharacterAnimInstance::PlayerWalkForwardMontage ( )
{
   // NowLocation = owner->GetActorLocation ( );
   // BeforeLocation = NowLocation;
    Montage_Play( walkForwardMontage,1.0f);
}

void UAICharacterAnimInstance::PlayerWalkBackMontage ( )
{
//UAnimMontage* MontageToPlay, float InPlayRate/*= 1.f*/, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages
    Montage_Play (walkBackMontage,1.0f, EMontagePlayReturnType::MontageLength,0.0f,true);
}
void UAICharacterAnimInstance::PlayerBackDashMontage ( )
{
    //UAnimMontage* MontageToPlay, float InPlayRate/*= 1.f*/, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages
    Montage_Play ( walkBackMontage , 3.0f , EMontagePlayReturnType::MontageLength , 0.0f , true );
}
void UAICharacterAnimInstance::PlayeAttackRHMontage ( )
{
    Montage_Play ( attackRHMontage );
}
void UAICharacterAnimInstance::PlayeAttackLHMontage ( )
{
    Montage_Play ( attackLHMontage );
}
void UAICharacterAnimInstance::PlayeAttackLFMontage ( )
{
    Montage_Play ( attackLFMontage );
}

void UAICharacterAnimInstance::PlayeAttackLowerLFMontage ( )
{
    Montage_Play ( attackLowerLFMontage );
}

void UAICharacterAnimInstance::PlayerIdleMontage ( )
{
    //UAnimMontage* MontageToPlay, float InPlayRate/*= 1.f*/, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages
    Montage_Play (idleMontage);
}

void UAICharacterAnimInstance::AnimNotify_LookTarget ( )
{
    //회전
    FRotator lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , owner->aOpponentPlayer->GetActorLocation ( ) );
    owner->targetRotator = lookPlayerRotator;
    owner->bLookTarget = true;
}

void UAICharacterAnimInstance::AnimNotify_Move ( )
{
    FVector Direction = owner->GetActorForwardVector ( );
    float Distance = 100.0f; // 한 걸음의 거리
    float  Duration = 1.0f; // 한 걸음의 시간
    switch ( owner->StateComboLaserAttack ( )->GetAttackCount ( ) )
    {
    case 0:
        Distance = 30.0f; // 한 걸음의 거리
        Duration = 0.5f; // 한 걸음의 시간
        owner->StateComboLaserAttack ( )->StepAndAttack ( Direction , Distance , Duration );
        break;
    case 1:
        Distance = 250.0f; // 한 걸음의 거리
        Duration = 1.0f;// 한 걸음의 시간
        owner->StateComboLaserAttack ( )->StepAndAttack ( Direction , Distance , Duration );
        break;
    case 2:
        Distance = 25.0f; // 한 걸음의 거리
        Duration = 0.7f;// 한 걸음의 시간
        owner->StateComboLaserAttack ( )->StepAndAttack ( Direction , Distance , Duration );
        break;
    case 3:
        Distance = 25.0f; // 한 걸음의 거리
        Duration = 0.7f;// 한 걸음의 시간
        owner->StateComboLaserAttack ( )->StepAndAttack ( Direction , Distance , Duration );
        break;
    case 4:
        Distance = 60.0f; // 한 걸음의 거리
        Duration = 1.f; // 한 걸음의 시간
        owner->StateComboLaserAttack ( )->StepAndAttack ( Direction , Distance , Duration );
        break;
    case 5:
        Distance = 60.0f; // 한 걸음의 거리
        Duration = 1.f; // 한 걸음의 시간
        owner->StateComboLaserAttack ( )->StepAndAttack ( Direction , Distance , Duration );
        break;
    case 6:
        Distance = 40.0f; // 한 걸음의 거리
        Duration = 1.f; // 한 걸음의 시간
        owner->StateComboLaserAttack ( )->StepAndAttack ( Direction , Distance , Duration );
        break;
    case 7:
        Distance = 40.0f; // 한 걸음의 거리
        Duration = 1.f; // 한 걸음의 시간
        owner->StateComboLaserAttack ( )->StepAndAttack ( Direction , Distance , Duration );
        break;
    case 8:
        Distance = 40.0f; // 한 걸음의 거리
        Duration = 1.f; // 한 걸음의 시간
        owner->StateComboLaserAttack ( )->StepAndAttack ( Direction , Distance , Duration );
        break;
    default:
        break;
    }
   
}

void UAICharacterAnimInstance::AnimNotify_MoveEnd ( )
{
    owner->StateComboLaserAttack ( )->FinishStep ();
}

void UAICharacterAnimInstance::AnimNotify_WalkMoveEnd ( )
{
    owner->ExitCurrentState ( ECharacterStateInteraction::Move );
}

void UAICharacterAnimInstance::AnimNotify_Laser ( )
{
    FVector start = owner->GetMesh()->GetSocketLocation("headSocket");
    FVector ForwardVector = owner->GetActorForwardVector ( );
    FRotator TiltedRotator = FRotator ( -15.f , 0 , 0 ); // Pitch를 -15도로 회전
    // 끝점 계산 (라인의 길이를 조절할 수 있습니다. 여기서는 1000 유닛)
    FVector end = start + (TiltedRotator.RotateVector ( ForwardVector ) * 1000.0f);

    FHitResult hitResult;
    FCollisionQueryParams collisionParams;
    collisionParams.AddIgnoredActor ( owner );

    DrawDebugLine ( GetWorld ( ) , start , end , FColor::Red , false , 1.0f );
    if ( GetWorld ( )->LineTraceSingleByChannel (
        hitResult ,
        start ,
        end ,
        ECC_Camera ,
        collisionParams ) )
    {
        if ( hitResult.GetActor ( ) == owner->aOpponentPlayer )
        {
            owner->aOpponentPlayer->HitDecision ( owner->SendAttackInfo ( ) , owner );
        }
    }
    laserFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld() , laserFXSystem , start,(end-start).Rotation());

}

float UAICharacterAnimInstance::GetCurrentMontageTime ( )
{
    return Montage_GetPosition(GetCurrentActiveMontage());
}