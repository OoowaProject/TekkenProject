// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
#include "CPP_Tekken8CharacterParent.h"
#include "AICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FOnHit );
UCLASS()
class LSJ_API AAICharacter : public ACPP_Tekken8CharacterParent
{
	GENERATED_BODY()

	UPROPERTY (EditDefaultsOnly)
	class UAICharacterAnimInstance * animInstance;

	class IAIStateInterface* currentState;

	UPROPERTY ()
	class UAIStateBackDash* stateBackDash;
	UPROPERTY ( )
	class UAIStateRun* stateRun;
	UPROPERTY ( )
	class UAIStateWalkBack* stateWalkBack;
	UPROPERTY ( )
	class UAIStateWalkForward* stateWalkForward;
	UPROPERTY ( )
	class UAIStateHitFalling* stateHitFalling;
	UPROPERTY ( )
	class UAIStateBound* stateBound;
	UPROPERTY ( )
	class UAIStateAttackLF* stateAttackLF;
	UPROPERTY ( )
	class UAIStateAttackRH* stateAttackRH;
	UPROPERTY ( )
	class UAIStateIdle* stateIdle;
	UPROPERTY ( )
	class UAIStateHit* stateHit;
	UPROPERTY ( )
	class UAIStateAttackLH* stateAttackLH;
	UPROPERTY ( )
	class UAIStateKnockDown* stateKnockDown;
	UPROPERTY ( )
	class UAIStateComboLaserAttack* stateComboLaserAttack;
	UPROPERTY ( )
	class UAIStateWalkCross* stateWalkCross;
	bool IsPlayer1;

	class AAICharacterController* aiController;
	class UBlackboardComponent* blackboardComp;

	void SetAttackInfoOwnerOpposite ( FAttackInfoInteraction& attackInfo );

public:
	// Sets default values for this character's properties
	AAICharacter();

	//시작움직임설정
	virtual void StartPlayer ( );
	virtual void EndPlayer ( );

	//이펙트
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraSystem* niagaraFXSystem;
		UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraSystem* basicAttack1FXSystem;
		UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraSystem* basicAttack2FXSystem;
		UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraSystem* blockedFXSystem;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraComponent* niagaraFXComponent;
	// 상태 공격 객체에 대한 접근 메서드 추가
	class UAIStateAttackLF* GetAIStateAttackLF ( ) const { return stateAttackLF; }
	void StateAttackLF ( class UAIStateAttackLF* val ) { stateAttackLF = val; }
	class UAIStateAttackRH* GetAIStateAttackRH ( ) const { return stateAttackRH; }
	void StateAttackRH ( class UAIStateAttackRH* val ) { stateAttackRH = val; }
	class UAIStateAttackLH* GetAIStateAttackLH ( ) const { return stateAttackLH; }
	void StateAttackLH ( class UAIStateAttackLH* val ) { stateAttackLH = val; }
	class UAIStateComboLaserAttack* StateComboLaserAttack ( ) const { return stateComboLaserAttack; }
	void StateComboLaserAttack ( class UAIStateComboLaserAttack* val ) { stateComboLaserAttack = val; }
	//공격 콜리전
	UPROPERTY (EditDefaultsOnly)
	class USphereComponent* collisionLH;
	UPROPERTY ( EditDefaultsOnly )
	class USphereComponent* collisionRH;
	UPROPERTY ( EditDefaultsOnly )
	class USphereComponent* collisionLF;
	UPROPERTY ( EditDefaultsOnly )
	class USphereComponent* collisionRF;
	//몸체 콜리전
	UPROPERTY ( EditDefaultsOnly )
	class UBoxComponent* collisionTop;
	UPROPERTY ( EditDefaultsOnly )
	class UBoxComponent* collisionMiddle;
	UPROPERTY ( EditDefaultsOnly )
	class UBoxComponent* collisionLower;
	//UPROPERTY ( EditDefaultsOnly )
	//class UCapsuleComponent* collisionBody;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool isResume;
	bool isPause;
	AGameMode_MH* gameMode;
public:
	//사운드
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* attackLFSFV;
		UPROPERTY(EditDefaultsOnly)
	class USoundBase* attackRFSFV;
		UPROPERTY(EditDefaultsOnly)
	class USoundBase* attackLHSFV;
		UPROPERTY(EditDefaultsOnly)
	class USoundBase* attackRHSFV;
		UPROPERTY(EditDefaultsOnly)
	class USoundBase* guardSFV;
		UPROPERTY(EditDefaultsOnly)
	class USoundBase* hitLastSFV;
		UPROPERTY(EditDefaultsOnly)
	class USoundBase* hitStrongSFV;
			UPROPERTY(EditDefaultsOnly)
	class USoundBase* hitWeakSFV;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ChangeState (class IAIStateInterface* NewState );
	void UpdateState ( const float& deltatime );
	void ExitCurrentState (ECharacterStateInteraction state );
	//플레이어가 앉은 상태인지 공중상태인지를 판단하여 공격 모션이 달라진다. - AICharacter.cpp
	int8 ChangeAttackMotionDependingOpponentState ( );
	//상대 플레이어와의 거리
	float GetBTWDistance ( );
	// 상태 이동 객체에 대한 접근 메서드 추가
	void SetStateIdle();
	UAIStateKnockDown* GetAIStateKnockDown ( ) const { return stateKnockDown; }
	UAIStateWalkCross* GetAIStateWalkCross ( ) const { return stateWalkCross; }
	IAIStateInterface* GetCurrentState ( ) const { return currentState; }
	UAIStateIdle* GetAIStateIdle ( ) const { return stateIdle; }
	UAIStateWalkForward* GetAIStateWalkForward ( ) const { return stateWalkForward; }
	UAIStateWalkBack* GetAIStateWalkBack ( ) const { return stateWalkBack; }
	UAIStateRun* GetAIStateRun ( ) const { return stateRun; }
	UAIStateBackDash* GetAIStateBackDash ( ) const { return stateBackDash; }
	UAIStateHit* GetAIStateHit ( ) const { return stateHit; }
	UAIStateHitFalling* GetAIStateHitFalling ( ) const { return stateHitFalling; }
	UAIStateBound* GetAIStateBound ( ) const { return stateBound; }
	//공격 콜리전 켜기 끄기
	void OnAttackCollisionLF();
	void OnAttackCollisionRF ( );
	void OnAttackCollisionLH ( );
	void OnAttackCollisionRH ( );
	//공격 콜리전 끄기
	void OffAttackCollisionLF ( );
	void OffAttackCollisionRF ( );
	void OffAttackCollisionLH ( );
	void OffAttackCollisionRH ( );
	bool IsAttacked;

	//공격 콜리전 Overlap
	UFUNCTION()
	virtual void OnCollisionLHBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnCollisionRHBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnCollisionRFBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnCollisionLFBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual	bool HitDecision ( FAttackInfoInteraction attackInfo , ACPP_Tekken8CharacterParent* ownerHitPlayer);
	//공격 상태에 따른 공격정보 전달 함수
	//공격 상태에 몇번째 공격인지 count 변수로 구별하고 count에 해당하는 FAttackInfoInteraction정보를 전달한다.
	//count 변수는 콜리전을 끌때 count ++해서 공격상태 count에 전달한다
	virtual FAttackInfoInteraction SendAttackInfo();
	//공격 count를 0으로 만들기
	virtual void CurrentAttackCountToZero();
	//회전
	bool bLookTarget;
	FRotator targetRotator;
	virtual void LookTarget(const float& deltaTime);
	virtual void LookTarget ( const float& deltaTime , FRotator lookRotator );
	//상체 콜리전 Overlap
	UFUNCTION()
	virtual void OnHitBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//공격받았을때 델리게이트 
	UPROPERTY(BlueprintAssignable)
    FOnHit OnHit;
	UBlackboardComponent* GetBlackboardComponent()
	{
		return blackboardComp;
	};

	FVector direction;
};
