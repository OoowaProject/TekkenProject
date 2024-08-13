// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_Tekken8CharacterParent.h"
#include "PlayerCameraPawn.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "CPP_CharacterPaul.generated.h"

// 델리게이트
DECLARE_DELEGATE ( FActoin );

USTRUCT(Atomic, BlueprintType)
struct FInputKey
{
	GENERATED_BODY()
public:
	//방향키
	bool bKey1 : 1;		// 아래 왼쪽
	bool bKey2 : 1;		// 아래
	bool bKey3 : 1;		// 아래 오른쪽
	bool bKey4 : 1;		// 중간 왼쪽
	bool bKey6 : 1;		// 중간 오른쪽
	bool bKey7 : 1;		// 위 왼쪽
	bool bKey8 : 1;		// 위
	bool bKey9 : 1;		// 위 오른쪽
	// 액션키
	bool bLeftPunch : 1;	// 왼쪽 펀치
	bool bRightPunch : 1;	// 오른쪽 펀치
	bool bLeftKick : 1;		// 왼쪽 킥
	bool bRightKick : 1;	// 오른쪽 킥
};


/**
 * @Title 키에대한 입력처리를 위한 스트럭트입니다.
 */
USTRUCT(Atomic, BlueprintType)
struct FCommandTreeNode
{
	GENERATED_BODY()
public:
	int32 keyValue;							// 이기술의 키
	TArray<int32> PrevKeys;					// 이전 기술 키들
	TArray<int32> NextKeys;					// 다음 기술 키들
	TMap<int32, FCommandTreeNode*> NextTrees;	// 다음 가능기술 목록
	int32 timingStart;						// 시작시간
	int32 timingEnd;						// 끝시간
	int32 timingAction;						// 발동 시간
	FActoin action;							// 발동 함수
	bool loopCommand;						// 반복 함수인지 체크합니다
 };


/**
 * @Title 키에대한 입력처리를 위한 스트럭트입니다.
 */
USTRUCT(Atomic, BlueprintType)
struct FFrameStatus
{
	GENERATED_BODY()
public:
	int32 FrameUsing = 0;			// 사용중인 프레임 딜레이입니다. (0일시 기본으로 동작이 초기화 됩니다.)
	int32 FrameBlockUsing = 0;		// 막힌 프레임 정보입니다. ( 이프레임 동안은 동작을 할 수 없습니다.)
 };

UCLASS()
class KYOULEE_API ACPP_CharacterPaul : public ACPP_Tekken8CharacterParent
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_CharacterPaul();


	UPROPERTY ( EditDefaultsOnly , Category = "DebugMode" )
	bool DebugMode = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay ( ) override;
	
	/**
	 * @title Character에 대한 기본정보
	 * @brief 히트의 충돌체 카메라 효과를 위한 작업입니다.
	 */
	class USkeletalMeshComponent* uCharacterMesh;

	bool player1;

	/**
	 * @title Camera Effect
	 * @brief 히트의 충돌체 카메라 효과를 위한 작업입니다.
	 */
	class AGameMode_MH* GameModeMH;

private:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent ( class UInputComponent* PlayerInputComponent ) override;

	// Called every frame
	virtual void Tick ( float DeltaTime ) override;



public:


	/**
	* @title 메인트리
	* @brief 시작 지점 트리를 가지고 있습니다.
	*	모든 스킬트리는 맵형식의 반복된 값을 가지고 있음으로 참고 바랍니다.
 	*/

	void SettingCommandTree ( );

	/************************************************************************/
	/*                커멘드 함수에 대한 정보                               */
	/************************************************************************/

	void CommandStarWaitNext ( );
	void CommandStarTest2 ( );
	////
	void CommandIdle ( );
	void CommandStar ( );
	void CommandEnd ( );
	void CommandMoveForward ( );
	void CommandMoveForwardLoop ( );
	void CommandMoveForwarDash();
	void CommandMoveBack ();
	void CommandMoveBackDash();
	void CommandJump ( );
	void CommandJumpForward ();
	void CommandMoveLateralUpDash ( );
	void CommandMoveLateralUpLoop ( );
	void CommandDownCrouch ( );
	void CommandUpCrouch ( );
	void CommandMoveLateralDownDash ( );
	void CommandMoveLateralDownLoop( );
	void CommandLeadJab ( );
	void CommandCrossStaight ( );
	void CommandJingun ( );
	void CommandHighKick ();
	void CommandSitJab ( );
	void CommandSitSpineKick ( );
	// 꽃
	void CommandBungGuan ( );
	
	//일어나며 커멘드
	void CommandJinJee ( );
	bool CommandAllStop ( );
	void CommentHitFrameExecute( );

	void CommandBullA ( );
	void CommandLeftTiger ( );
	void CommandBackJilPung ( );
	void CommandSangBong1 ( );
	void CommandSangBong2 ( );
	void CommandMtgLuOu1 ( );
	void CommandMtgLuOu2 ( );
	void CommandMtgLuOu3 ( );

	float GetZValue();

/**
 * @title : Interaction
 *
 */
	virtual bool HitDecision ( FAttackInfoInteraction attackInfo , ACPP_Tekken8CharacterParent* ownerHitPlayer ) override;

	UPROPERTY ( EditDefaultsOnly )
	class UBoxComponent* collisionTop;
	UPROPERTY ( EditDefaultsOnly )
	class UBoxComponent* collisionMiddle;
	UPROPERTY ( EditDefaultsOnly )
	class UBoxComponent* collisionLower;


	/**
	 * @title Particle Effect
	 * @brief 파티클 이펙트에 대한
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect" )
	UNiagaraSystem* uNS_HitEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect" )
	UNiagaraSystem* uNS_HitEffect2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect" )
	UNiagaraSystem* uNS_DefenceEffect;




	
	/************************************************************************/
	/*                    애니메이션을 위한  작업입니다                     */
	/************************************************************************/
	/**
	 * @title Next Location Data 처리
	 * @brief 애니메이션을 위한 작업입니다.
	 */
	UAnimInstance* uAnim ;
	FVector ToLocation;
	TArray<FVector> ToLocationFrame;

	bool bMoveTo = true;

	void AnimationFrame ( );

	void SetToRelativeLocationFrame ( FVector dir , int32 frame );
	void SetToLocationFrame ( FVector dir , int32 frame );

	void SetToLocationPoint ( float x , float y , float z );
	void SetToLocationPoint ( FVector vector );
	void SetToWorldLocationPoint ( FVector vector );

	FVector RelativePointVector ( float x , float y , float z );

	UFUNCTION ( )
	bool PlayMontageFrameSystem (UAnimMontage* montage);

	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgMoveLateral;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgLeadJab;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgCrossStaight;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgJingun;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgHighKick;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgBungGuan;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgJinJee;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgSitJab;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgSitSpineKick;
	// Defense
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgDefence;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgSitDefence;
	// Hit
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgIdleHit;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgSitHit;
	
	// Skell
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgBullA;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgLeftTiger;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgBackJilPung;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgSangBong1;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgSangBong2;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgLuOu1;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgLuOu2;
	UPROPERTY(EditDefaultsOnly, Category="MontageList" )
	class UAnimMontage* uMtgLuOu3;


	/************************************************************************/
	/*                커멘드 크리에 대한 정보                               */
	/************************************************************************/
	/**
	 * @title Command Tree
	 * @brief 커멘트 트리에 대한 정보 및 세팅입니다.
	 */
	TMap<int32 , FCommandTreeNode*> mBaseCommandTree;
	TMap<int32 , FCommandTreeNode*> mCurrCommandTree;

	FCommandTreeNode* sCurrCommand;
	FCommandTreeNode* sNextCommand;
	FCommandTreeNode* sTempCommand;
	TArray<int32> sCurrCommandKeys;

	int32 prevKeyValue = 0;
	int32 currKeyValue = 0;
	int32 nextKeyValue = 0;

	FCommandTreeNode* CreateCommandTree ( int32 keyValue , int32 timingStart , int32 timingEnd , void (ACPP_CharacterPaul::* fptr)() );
	FCommandTreeNode* AddCommandTree ( TMap<int32 , FCommandTreeNode*>& CurrCommandTree , int32 keyValue , int32 timingEnd , int32 timingAction , void(ACPP_CharacterPaul::* fptr)() );
	FCommandTreeNode* AddCommandBaseTree ( TArray<int> arrayTreeCommand , int32 keyValue , int32 timingStart , int32 timingEnd , void(ACPP_CharacterPaul::* fptr)() );//, bool loopCommand );

	bool CheckKeyArray ( );

	// 노드를 다시 루프하기 위한 함수 입니다.
	bool SetSelfReLinkTree ( TArray<int32> arrayTreeCommand );
	bool SetLinkTree ( TArray<int32> TargetTree , TArray<int32> ConnectTree );


	// 커맨드 트리 나누기 필요



	/**
	 * @title Owner State
	 * @brief 상태 정보입니다.
	 */

	eHitDecision DefencePoint;

	bool bCrouched = 0;
	bool bJumpping = 0;
	bool bFalling = 0;
	bool bNuckDown = 0;
	bool bDead = 0;

	FVector locationlenght;

	float fFallingValue = 0;

	void SetActtacInfoSkell ( EDamagePointInteraction damagePointInteraction ,
	int32 damageAmount , int32 actionFrame , int32 retrieveFrame ,
	int32 missFrame , int32 hitFrame , int32 grardFrame , int32 counterFrame );
	void SetAttackInfoOwnerOpposite ( );


	/**
	 * @Title 프레임 시스템을 만듬니다.
	 * @brief 1초당 60프레임으로 실행 되는 시스템입니다.
	 */
	void FrameSystem ( );
	bool PlayNextAction ( );
	FFrameStatus sFrameStatus;
	float fFrameTime = (1 / 60) * 1000;
	float fCurrTimeForFrame = 60;
	int32 iCurrFrame = 0;
	
	/************************************************************************/
	/*                     기본적인 게임에 대한 설정                        */
	/************************************************************************/
	/**
	 * @title 기본적인 키에대한 정보입니다.
	 */
	FInputKey CurrInputKey;
	FInputKey PrevInputKey;

	int32 CurrInputMoveKey;
	int32 CurrInputActionKey;
	
	UFUNCTION ( )
	FInputKey GetCurrInputKey ( );
	UFUNCTION()
	int32 GetCurrInputKeyValue();
	UFUNCTION()
	bool SetCurrInputKeyValue(int32 value);
	UFUNCTION()
	bool SetCurrInputKey( FInputKey value);
	UFUNCTION()
	int32 InputKeyValue(int ArrowKey, bool LeftArm, bool RightArm, bool LeftKick, bool RightKick);	
	
	/**
	 * @title Sound
	 * @brief 사운드 대한 정보
	 */
	UPROPERTY(EditDefaultsOnly, Category="Sound" )
	class USoundBase* uSoundEndPaul;
	UPROPERTY(EditDefaultsOnly, Category="Sound" )
	class USoundBase* uSoundGuard;
	UPROPERTY(EditDefaultsOnly, Category="Sound" )
	class USoundBase* uSoundPaulLastHit;
	UPROPERTY(EditDefaultsOnly, Category="Sound" )
	class USoundBase* uSoundPaulStrongHit;
	UPROPERTY(EditDefaultsOnly, Category="Sound" )
	class USoundBase* uSoundPaulWeakHit;
	UPROPERTY(EditDefaultsOnly, Category="Sound" )
	class USoundBase* uSoundHitLeftFoot;
	UPROPERTY(EditDefaultsOnly, Category="Sound" )
	class USoundBase* uSoundHitRightFoot;
	UPROPERTY(EditDefaultsOnly, Category="Sound" )
	class USoundBase* uSoundHitLefthand;
	UPROPERTY(EditDefaultsOnly, Category="Sound" )
	class USoundBase* uSoundHitRightHand;
	
		
};	            
