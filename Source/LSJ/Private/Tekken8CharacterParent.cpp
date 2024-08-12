// Fill out your copyright notice in the Description page of Project Settings.


#include "Tekken8CharacterParent.h"

// Sets default values
ATekken8CharacterParent::ATekken8CharacterParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATekken8CharacterParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATekken8CharacterParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATekken8CharacterParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

