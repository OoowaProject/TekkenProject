// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCameraLookPoint.h"

// Sets default values
AMainCameraLookPoint::AMainCameraLookPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCameraLookPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCameraLookPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

