// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Tekken8CharacterParent.h"

// Sets default values
ACPP_Tekken8CharacterParent::ACPP_Tekken8CharacterParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACPP_Tekken8CharacterParent::BeginPlay()
{
	Super::BeginPlay();
}

bool ACPP_Tekken8CharacterParent::HitDecision ( FAttackInfoInteraction attackInfo , ACPP_Tekken8CharacterParent* ownerHitPlayer )
{
	return false;
}

bool ACPP_Tekken8CharacterParent::SetDead(bool value)
{
	this->bIsDead;
	return this->bIsDead;
}

