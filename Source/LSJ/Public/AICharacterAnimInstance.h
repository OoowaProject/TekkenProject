// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AICharacterAnimInstance.generated.h"

/**
 * 
 */

UCLASS()
class LSJ_API UAICharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
				UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* attackLowerLFMontage;

			UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* crossWalkClockwiseMontage;
				UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* crossWalkCounterclockwiseMontage;
			UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* uppercutLHMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* walkForwardMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* walkBackMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* idleMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* attackRHMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* attackLHMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* attackLFMontage;

	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* boundMontage;
	//콤보 레이저 몽타주 
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* comboLaserMontage;
	UPROPERTY ( EditAnyWhere , BlueprintReadOnly , Category = Move , Meta = (AllowPrivateAccess = true) )
	UAnimMontage* knockDownMontage;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AAICharacter* owner;
	UPROPERTY( EditAnyWhere , BlueprintReadOnly , Category = Move , Meta = (AllowPrivateAccess = true) )
	bool bStateWalkForward;
	UPROPERTY( EditAnyWhere , BlueprintReadOnly , Category = Move , Meta = (AllowPrivateAccess = true) )
	bool bStateWalkBack;
	FVector BeforeLocation;
	FVector NowLocation;
	
	bool OnLog = false;
protected:
	UFUNCTION(BlueprintCallable, Category = "Update Properties")
	virtual void UpdateProperties();
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;
	virtual void NativeInitializeAnimation ( ) override;

public:
	virtual void PlayMontageAtFrameRate ( UAnimMontage* montage , int32 nFrames , float frameRate );
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* hitFallingRHMontage;
	UPROPERTY ( EditAnyWhere , BlueprintReadOnly , Category = Move )
	UAnimMontage* hitKnockDownMontage;
	UPROPERTY ( EditAnyWhere , BlueprintReadOnly , Category = Move )
	UAnimMontage* hitTopMontage;
	UPROPERTY ( EditAnyWhere , BlueprintReadOnly , Category = Move )
	UAnimMontage* hitMiddleMontage;
	UPROPERTY ( EditAnyWhere , BlueprintReadOnly , Category = Move )
	UAnimMontage* hitLowerRFMontage;
	UPROPERTY ( EditAnyWhere , BlueprintReadOnly , Category = Move )
	UAnimMontage* hitFallingMontage;
	UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* hitFallingTurnMontage;
		UPROPERTY(EditAnyWhere , BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* standUpMontage;
	UAICharacterAnimInstance();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float movementSpeed;

	UPROPERTY( VisibleAnywhere , BlueprintReadOnly )
	float direction;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite )
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite )
	float velocityZ;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite )
	bool bFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite )
	bool bRun;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite )
	bool bDie;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite )
	bool bKnockDown;
	 // Animation Montage가 끝났을 때 호출될 함수
    UFUNCTION()
    virtual void HandleOnMontageEnded(class UAnimMontage* Montage, bool bInterrupted);
	virtual void PlayComboLaserMontage ( );
	virtual void PlayKnockDownMontage ( );
	virtual void PlayBoundMontage ( );

	virtual void PlayCrossWalkClockwiseMontage ( );
	virtual void PlayCrossWalkCounterclockwiseMontage ( );
	virtual void PlayHitFallingMontage (float nFrame, float frameRate);
	virtual void PlayHitFallingTurnMontage ( float nFrame , float frameRate );
	virtual void PlayHitTopMontage ( float nFrame , float frameRate );
	virtual void PlayHitMiddleMontage ( float nFrame , float frameRate );
	virtual void PlayerWalkForwardMontage();
	virtual void PlayerWalkBackMontage();
	virtual void PlayerBackDashMontage();
	virtual void PlayeAttackRHMontage ( );
	virtual void PlayeAttackLHMontage ( );
	virtual void PlayeAttackLFMontage ( );
	virtual void PlayeAttackLowerLFMontage ( );
	virtual void PlayerIdleMontage( );
	virtual bool StateWalkForward ( ) const { return bStateWalkForward; }
	virtual void StateWalkForward ( bool val ) { bStateWalkForward = val; }
	virtual bool StateWalkBack ( ) const { return bStateWalkBack; }
	virtual void StateWalkBack ( bool val ) { bStateWalkBack = val; }

	//notify
	UFUNCTION ()
	void AnimNotify_LookTarget ();
	UFUNCTION ( )
	void AnimNotify_Move ( );
	UFUNCTION ( )
	void AnimNotify_MoveEnd ( );
	UFUNCTION ( )
	void AnimNotify_WalkMoveEnd ( );
	UFUNCTION ( )
	void AnimNotify_Laser ( );

	float GetCurrentMontageTime ( );
	//laser effect
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraSystem* laserFXSystem;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraComponent* laserFXComponent;
};
