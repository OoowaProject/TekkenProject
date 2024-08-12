// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MainCameraLookPoint.h"

// Sets default values
ACPP_MainCameraLookPoint::ACPP_MainCameraLookPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_MainCameraLookPoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_MainCameraLookPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

