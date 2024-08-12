// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIStateInterface.h"
#include "AIStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FOnStateWalkForwardComplete );



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LSJ_API UAIStateComponent : public UActorComponent , public IAIStateInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIStateComponent();
	TArray<struct FAttackInfoInteraction> attackInfoArray;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	class AAICharacter* owner;
	FRotator toLookTargetRotator;
public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void AddAttackCount ( int32 amount );
	virtual int32 GetAttackCount ( );
	virtual void Enter (class UAICharacterAnimInstance* pAnimInstance );
	virtual void Execute ( const float& deltatime );
	virtual void Exit ( );
	virtual void SetStateOwner (class AAICharacter* pOwner);
	virtual void ToLookTargetRotate( const float& deltaTime );
		UPROPERTY(BlueprintAssignable)
    FOnStateWalkForwardComplete OnStateComplete;
};
