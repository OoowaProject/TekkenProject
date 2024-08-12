// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateIdle.h"
#include "AICharacterAnimInstance.h"
#include "AIStateComponent.h"
#include "AICharacter.h"
void UAIStateIdle::BeginPlay ( )
{
	Super::BeginPlay();
}

void UAIStateIdle::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter(pAnimInstance);
	//animInstace->PlayerIdleMontage();
}

void UAIStateIdle::Execute ( const float& deltatime )
{

}

void UAIStateIdle::Exit ( )
{
	//Super::Exit();
}
