// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_JMH.h"

// Sets default values
APlayer_JMH::APlayer_JMH()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayer_JMH::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer_JMH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayer_JMH::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

