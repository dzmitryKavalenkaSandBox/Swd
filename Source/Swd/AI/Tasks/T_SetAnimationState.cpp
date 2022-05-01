// Fill out your copyright notice in the Description page of Project Settings.


#include "T_SetAnimationState.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Swd/Character/AICharacterBase.h"
#include "Swd/Utils/Logger.h"


UT_SetAnimationState::UT_SetAnimationState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = TEXT("Set Animation State");
	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}

EBTNodeResult::Type UT_SetAnimationState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	AAICharacterBase* AICharacter = Cast<AAICharacterBase>(MyController->GetPawn());
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	ASwdCharacter* EnemyActor = Cast<ASwdCharacter>(MyBlackboard->GetValueAsObject(BBKeys::ClosestHostile));
	if (MyController && AICharacter)
	{
		if (!States.IsEmpty())
		{
			if (States.Find(EAnimationState::ArmSelf))
			{
				AICharacter->ToggleArmedState(States[EAnimationState::ArmSelf]);
			}
			if (States.Find(EAnimationState::Combat))
			{
				AICharacter->ToggleCombat(States[EAnimationState::Combat]);
			}
			if (States.Find(EAnimationState::Focus) && EnemyActor)
			{
				States[EAnimationState::Focus]
					? MyController->SetFocus(EnemyActor)
					: MyController->ClearFocus(EAIFocusPriority::LastFocusPriority);
				// AICharacter->ToggleADS(ADS);
				return EBTNodeResult::Succeeded;
			}
			if (States.Find(EAnimationState::Sprinting))
			{
				AICharacter->ToggleSprinting(States[EAnimationState::Sprinting]);
			}
		}
		// MyController->ClearFocus(EAIFocusPriority::LastFocusPriority);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
