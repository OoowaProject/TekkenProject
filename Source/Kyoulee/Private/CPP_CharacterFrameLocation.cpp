// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CharacterFrameLocation.h"

// Sets default values for this component's properties
UCPP_CharacterFrameLocation::UCPP_CharacterFrameLocation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCPP_CharacterFrameLocation::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCPP_CharacterFrameLocation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
