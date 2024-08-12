// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChangeAttack.h"
#include "AICharacter.h"
#include "AIController.h"
#include "AIStateAttackLF.h"
#include "AIStateAttackRH.h"
#include "AIStateComboLaserAttack.h"
#include "AIStateAttackLH.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTTaskNode_ChangeAttack::UBTTaskNode_ChangeAttack ( )
{

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_ChangeAttack::ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory )
{
	EBTNodeResult::Type Result = Super::ExecuteTask ( OwnerComp , NodeMemory );
	AAIController* aiController = OwnerComp.GetAIOwner ( );
	if ( aiController )
	{
		owner = Cast<AAICharacter> ( aiController->GetPawn ( ) );
		if ( owner )
		{
			int8 attackIndex = owner->GetBlackboardComponent ( )->GetValueAsEnum ( TEXT ( "ERandomAttack" ));
			UAIStateComponent* stateComponent = nullptr;

			// 적절한 상태 클래스에 따라 상태 변경
			//attackIndex=3 : Lower, =2 : Middle, =1 : Top
			if ( attackIndex==3 || newStateClass == UAIStateAttackLF::StaticClass ( ) )
			{
				stateComponent = owner->GetAIStateAttackLF ( );
			}
			else if ( attackIndex == 2 || newStateClass == UAIStateAttackRH::StaticClass ( ) )
			{
				stateComponent = owner->GetAIStateAttackRH ( );
			}
			else if ( newStateClass == UAIStateComboLaserAttack::StaticClass ( ) )
			{
				stateComponent = owner->StateComboLaserAttack ( );
			}
			else if ( attackIndex == 1 || newStateClass == UAIStateAttackLH::StaticClass ( ) )
			{
				stateComponent = owner->GetAIStateAttackLH();
			}

			if ( stateComponent )
			{
				stateComponent->attackPoint = attackPoint;
				//랜덤 공격 일때 
				switch ( attackIndex )
				{
				case 1: //상단
					stateComponent->attackPoint = EAttackPoint::Top;
					break;
				case 2: //중단
					stateComponent->attackPoint = EAttackPoint::Middle;
					break;
				case 3: //하단
					stateComponent->attackPoint = EAttackPoint::Lower;
					break;
				default:
					break;
				}
				// 상태 완료시 호출될 델리게이트 바인딩
				if ( UAIStateAttackLF* stateAttackLF = Cast<UAIStateAttackLF> ( stateComponent ) )
				{
					if ( !stateAttackLF->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted ) )
						stateAttackLF->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted );
				}
				else if ( UAIStateAttackRH* stateAttackRH = Cast<UAIStateAttackRH> ( stateComponent ) )
				{
					if ( !stateAttackRH->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted ) )
						stateAttackRH->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted );
				}
				else if ( UAIStateComboLaserAttack* stateComboLaserAttack = Cast<UAIStateComboLaserAttack> ( stateComponent ) )
				{
					if ( !stateComboLaserAttack->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted ) )
						stateComboLaserAttack->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted );
				}
				else if ( UAIStateAttackLH* stateAttackLH = Cast<UAIStateAttackLH> ( stateComponent ) )
				{
					if ( !stateAttackLH->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted ) )
						stateAttackLH->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted );
				}

				bIsWaitingForState = true;
				cachedOwnerComp = &OwnerComp;
				owner->ChangeState ( Cast<IAIStateInterface> ( stateComponent ) );
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTaskNode_ChangeAttack::TickTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds )
{
	Super::TickTask ( OwnerComp , NodeMemory , DeltaSeconds );

	// 상태 완료
	if ( !bIsWaitingForState )
	{
		owner->GetBlackboardComponent ( )->SetValueAsEnum ( TEXT ( "ERandomAttack" ) , 0);
		FinishLatentTask ( OwnerComp , EBTNodeResult::Succeeded );
	}
}

void UBTTaskNode_ChangeAttack::OnStateCompleted ( )
{
	bIsWaitingForState = false;
}

