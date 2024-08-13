// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPP_PualAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class KYOULEE_API UCPP_PualAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation ( ) override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	float fHorizontal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	float fVertical;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	bool	bCrouch;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	bool	bFalling;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	float	fFallingValue;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	bool	bNuckDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	bool bDead;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	class ACPP_CharacterPaul* aOwner;

	UFUNCTION ( )
	void HandleOnMontageEnded ( UAnimMontage* Montage , bool bInterrupted );
};
