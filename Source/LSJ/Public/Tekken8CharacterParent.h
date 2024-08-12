// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tekken8CharacterParent.generated.h"

//캐릭터 상태
UENUM()
enum class ECharacterState
{
	Idle, //별표상태
	//Attack, //공격상태 //상대가 헛발질 할때의 상황을 가져올 수 있나?
	//상단 중단 하단 가드하기 위해 Attack 위치가 필요하다
	AttackLower, 
	AttackMiddle,
	AttackTop, 
	HitGround , //지상에서 Hit 상태 //
	HitFalling , //공중에서 Hit 상태 //trace?
	Air, //공중상태
	GuardStand, //서있는 가드상태 
	GuardSit, //앉은 가드상태 
	Sit, //앉아있는 상태 
	Down, //넘어져있는상태
	Turn , //횡이동 상태 //어디로 횡이동 하는지 알아야하나?

	Up, //일어서고 있는 상태 Down -> Up, Sit -> Up 둘다 포함
	WalkForward, //앞으로 걷고 있는 상태
	WalkBack,//뒤로 걷고 있는 상태
	Run, //앞으로 달리는 상태
	BackDash, //뒤로 대쉬한 상태
};

//공격 범위
UENUM()
enum class EDamagePoint 
{	
	Lower, //하단
	Middle, //중단
	Top //상단
};

//TArray<FDirectionConnections> AttackPackage
// AttackPackage ridjap(damagePoint,,.....);
//AttackPackage.Add( )
//공격 정보
USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_USTRUCT_BODY()

	//상단 중단 하단
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamagePoint DamagePoint;

	//넉백 수치 ( -1 , 1, z) : z = 높이 띄우는 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector KnockBackDirection;

	//데미지량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageAmount;

	//공격하는데 걸리는 프레임
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HitFrame;

	//현재 공격이 가드 되었을 때 공격하는 캐릭터가 얻거나 잃는 프레임 : -값이 잃는 프레임
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Grd;

	//현재 공격이 적중 되었을 때 공격하는 캐릭터가 얻는 프레임 : -값이 잃는 프레임
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NH;

	//가드 가능한가? true : 가드 불가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGaurdBreaker;

	FAttackInfo ()
	{
		DamagePoint = EDamagePoint::Lower;
		KnockBackDirection = FVector::ZeroVector;
		DamageAmount = 0;
		HitFrame = 0;
		Grd = 0;
		NH = 0;
		bGaurdBreaker = false;
	}
	FAttackInfo (EDamagePoint pDamagePoint, FVector pKnockBackDirection, int16 pDamageAmount, int8 pHitFrame, int8 pGrd, int8 pNH, bool pbGaurdBreaker)
	{
		DamagePoint = pDamagePoint;
		KnockBackDirection = pKnockBackDirection;
		DamageAmount = pDamageAmount;
		HitFrame = pHitFrame;
		Grd = pGrd;
		NH = pNH;
		bGaurdBreaker = pbGaurdBreaker;
	}
};



UCLASS()
class LSJ_API ATekken8CharacterParent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATekken8CharacterParent();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
