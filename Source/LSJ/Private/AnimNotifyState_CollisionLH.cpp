// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_CollisionLH.h"
#include "AICharacter.h"
void UAnimNotifyState_CollisionLH::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration , const FAnimNotifyEventReference& EventReference )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration , EventReference );
	AAICharacter* enemy = Cast<AAICharacter> ( MeshComp->GetOwner ( ) );
	if ( nullptr == enemy )
		return;
	enemy->OnAttackCollisionLH ( );
}
void  UAnimNotifyState_CollisionLH::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , const FAnimNotifyEventReference& EventReference )
{
	Super::NotifyEnd ( MeshComp , Animation , EventReference );
	AAICharacter* enemy = Cast<AAICharacter> ( MeshComp->GetOwner ( ) );
	if ( nullptr == enemy )
		return;
	enemy->OffAttackCollisionLH ( );
}

