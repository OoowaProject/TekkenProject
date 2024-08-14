// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_IFCharacterInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPP_IFCharacterInteraction : public UInterface
{
	GENERATED_BODY()
};

enum eHitDecision
{
	Top,
	Middle,
	Lower,
	Special,
	Grap,
	GuardBreak,
	Other
};

/**
 * @Title 키에대한 입력처리를 위한 스트럭트입니다.
 */
USTRUCT(Atomic, BlueprintType)
struct FSkellInfo
{
	GENERATED_BODY()
public:

	/**
	 * Skell Info
	 */
	// 공격 분류
	eHitDecision skellHitDecision = eHitDecision::Top;

	// 데미지
	int32 skellDamage = 0;
	
	// Effect Location
	FVector skellEffectLocation = FVector(0);

	// 넉백
	FVector skellNuckbuck = FVector ( 0 );

	/**
	 * Camera Send Info
	 */
	 float cameraShake = 0;
	 float cameraZoom = 0;
	 // skellEffectLocation; 상위 참조
 };
 



/**
 * @Title 캐릭터의 상호작용을 위한 InterFace입니다.
 * @brie 히트에 대한 처리를 준비하고 있습니다. 
 */
class KYOULEE_API ICPP_IFCharacterInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	
	/**
	 * @brie 현재 체력을 확인하기위한 함수
	 */
	virtual int32 GetCurrentHp() const = 0;

	/**
	 * @func HitDecision
	 * @param (eHitDecision hitPosition, float Damage)
	 * @brie충돌에 대한 위치와 데미지 처리를 위한 가상함수
	 * @return bool
	 * 반환에 true 맞은것 false 막은것
	 */
	virtual bool HitDecision(eHitDecision hitPosition, float Damage) const = 0;

};
