// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_IFCharacterInteraction.h"
#include "PlayerCameraPawn.h"
#include "CPP_Tekken8CharacterParent.generated.h"

//캐릭터 상태
UENUM()
enum class ECharacterStateInteraction
{
	Move, // 이동할때
	Idle, //별표상태
	//Attack, //공격상태 //상대가 헛발질 할때의 상황을 가져올 수 있나?
	//상단 중단 하단 가드하기 위해 Attack 위치가 필요하다
	AttackLower, 
	AttackMiddle,
	AttackTop, 
	Air, //공중상태
	HitGround, //지상에서 Hit 상태 //
	HitFalling, //공중에서 Hit 상태 //trace?
	GuardStand, //서있는 가드상태
	GuardSit, //앉은 가드상태
	Sit, //앉아있는 상태
	Down, //넘어져있는상태
	Up, //일어서고 있는 상태 Down -> Up, Sit -> Up 둘다 포함
	WalkForward, //앞으로 걷고 있는 상태
	WalkBack,//뒤로 걷고 있는 상태
	Run, //앞으로 달리는 상태
	BackDash, //뒤로 대쉬한 상태
	Turn, //횡이동 상태 //어디로 횡이동 하는지 알아야하나?
};

//공격 범위
UENUM()
enum class EDamagePointInteraction
{
	Top ,
	Middle ,
	Lower ,
	Special ,
	Grap ,
	GuardBreak ,
	Other
};


//TArray<FDirectionConnections> AttackPackage
// AttackPackage ridjap(damagePoint,,.....);
//AttackPackage.Add( )
//공격 정보
USTRUCT( Atomic, BlueprintType)
struct FAttackInfoInteraction
{
	GENERATED_BODY ( );
	//상단 중단 하단 그랩 특수 가드불가
public:
	EDamagePointInteraction DamagePoint;


	int32 DamageAmount;			// 공격 데미지랑
	int32 HitFrame;				// 공격이 발생하는 시간
	int32 AnimEndFrame;			// 공격 애니메이션이 끝나는 시간

	int32 ActionFrame;
	int32 RetrieveFrame;

	int32 MissFrame;			// 빛나감
	int32 GrardFrame;			// 공격이 막힘
	int32 CounterFrame;			// 카운터 발생



	FVector skellEffectLocation = FVector ( 0 );	// Effect Location 스킬 이펙트 및 콜리션 위치
	FVector KnockBackDirection = FVector ( 0 );						// 넉백 수치 ( -1 , 1, z) : z = 높이 띄우는 값
	FVector KnockBackFallingDirection = FVector ( 0 );
	FVector KnockBackDefenceDir = FVector ( 0 );
	/**
	 * OwerFrame 스킬을 쓴 주체가 된다
	 * @note 현재 공격이 가드  되었을 때 공격하는 캐릭터가 얻거나 잃는 프레임 : -값이 잃는 프레임
	 * @param 
	 * hit 맞히면 프레임 이득을 얻는다 
	 * Guard 상대가 막았다는 신호로 프레임 손해를 본다
	 */
	int32 OwnerHitFrame = 0; 
	int32 OwnerGuardFrame = 0;
	int32 OwnerCounterFrame = 0;
	int32 OwnerMissFrame = 0;
	


	/**
	 * oppositeFrame 
	 * @note 현재 공격이 가드  되었을 때 공격하는 캐릭터가 얻거나 잃는 프레임 : -값이 잃는 프레임
	 * @param 
	 * hit 맞으면 프레임 손해를 얻는다 
	 * Guard 공격을 막았다는 신호로 프레임 이득을 본다
	 */
	int32 OppositeHitFrame = 0;
	int32 OppositeGuardFrame = 0;
	int32 OppositeCounterFrame = 0;


	//가드 가능한가? true : 가드 불가능
	bool bGaurdBreaker;

	/**
	 * Camera Send Info
	 * 카메라에 전달할 인자들 입니다.
	 */
	float cameraShake = 0;
	float cameraZoom = 0;
	float cameraDelay = 0;
	// skellEffectLocation; 상위 참조
	

// 
// 	FAttackInfoInteraction ()
// 	{
// 		DamagePoint = EDamagePointInteraction::Lower;
// 		KnockBackDirection = FVector::ZeroVector;
// 		DamageAmount = 0;
// 		HitFrame = 0;
// 		OwnerHitFrame = 0;
// 		OppositeHitFrame = 0;
// 		OppositeGuardFrame = 0;
// 		OwnerGuardFrame = 0;
// 		OwnerCounterFrame = 0;
// 		OppositeCounterFrame = 0;
// 		bGaurdBreaker = false;
// 	}
// 
// 	FAttackInfoInteraction (EDamagePointInteraction pDamagePoint, FVector pKnockBackDirection, int16 pDamageAmount, int8 pHitFrame, int8 pGrd, int8 pNH, bool pbGaurdBreaker)
// 	{
// 		DamagePoint = pDamagePoint;
// 		KnockBackDirection = pKnockBackDirection;
// 		DamageAmount = pDamageAmount;
// 		HitFrame = pHitFrame;
// 		OwnerGuardFrame = 0;
// 		OppositeGuardFrame = 0;
// 		bGaurdBreaker = pbGaurdBreaker;
// 	}

	FColor debugColor;
	//해당 공격받았을때 실행되는 몽타지
	class UAnimMontage* hitMontage=nullptr;
	//해당 공격이 가드되었을때 실행되는 몽타지
	class UAnimMontage* guardMontage=nullptr;

	class USoundBase* uMovementSound;
	class USoundBase* uHitSound;
	class USoundBase* uDefenceSound;

	

	//이펙트 
	TArray<class UNiagaraSystem*> niagaraSystemArray;
	TArray<class UParticleSystem*> particleSystemArray;
};

UCLASS()
class TEKKEN8_API ACPP_Tekken8CharacterParent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Tekken8CharacterParent();

	int32 Hp = 180;
	int32 MaxHp = 180;
	int32 RageGauge = 0;
	int32 HitGauge = 100;
	bool bIsDead = false;

	
	// SetCamera 
	APlayerCameraPawn* aMainCamera;
	
	/**
	 * @title Target Point
	 * @brief 상대에 대한 정보
	 */
	UPROPERTY(EditDefaultsOnly)
	ACPP_Tekken8CharacterParent* aOpponentPlayer;
	
	bool SetMainCamera ( APlayerCameraPawn* mainCamera )
	{
		if (!mainCamera)
			return true;
		this->aMainCamera = mainCamera;
		return true;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	ECharacterStateInteraction eCharacterState;
	FAttackInfoInteraction sAttackInfo;
	virtual	bool HitDecision ( FAttackInfoInteraction attackInfo , ACPP_Tekken8CharacterParent* ownerHitPlayer )  ;

	virtual bool SetDead(bool value);
};
