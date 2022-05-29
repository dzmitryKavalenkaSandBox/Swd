#include "T_SetAnimationState.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Swd/AI/AIControllerBase.h"
#include "Swd/Character/AICharacterBase.h"
#include "Swd/Components/Modular/LockOnTargetModule/LockOnTargetComponent.h"


UT_SetAnimationState::UT_SetAnimationState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = TEXT("Set Animation State");
	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}

EBTNodeResult::Type UT_SetAnimationState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* MyController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	AAICharacterBase* AICharacter = Cast<AAICharacterBase>(MyController->GetPawn());
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	ASwdCharacter* EnemyActor = Cast<ASwdCharacter>(MyBlackboard->GetValueAsObject(BBKeys::ClosestHostile));
	if (MyController && AICharacter)
	{
		if (!AnimationStates.IsEmpty())
		{
			// Relaxed state management
			if (AnimationStates.Find(EAnimationState::Idle))
			{
				if (AnimationStates[EAnimationState::Idle])
				{
					MyController->UpdateAIState(EAIState::Idle);
				}
				else
				{
					MyController->UpdateAIState(EAIState::OnDuty);
				}
			}
			if (AnimationStates.Find(EAnimationState::OnDuty))
			{
				if (AnimationStates[EAnimationState::OnDuty])
				{
					AICharacter->Ready();
					MyController->UpdateAIState(EAIState::OnDuty);
				}
				else
				{
					AICharacter->AtEase();
					MyController->UpdateAIState(EAIState::Idle);
				}
			}
			if (AnimationStates.Find(EAnimationState::Focus))
			{
				if (AnimationStates[EAnimationState::Focus])
				{
					if (FocusTarget)
					{
						AICharacter->LockOnTargetComponent->SetTargetToLockOn(FocusTarget);
					}
					else if (EnemyActor)
					{
						AICharacter->LockOnTargetComponent->SetTargetToLockOn(EnemyActor);
					}
				}
				else
				{
					AICharacter->LockOnTargetComponent->SetTargetToLockOn(nullptr);
				}
			}
			// if (AnimationStates.Find(EAnimationState::Combat))
			// {
			// 	AICharacter->ToggleCombat(AnimationStates[EAnimationState::Combat]);
			// }
			//
			// if (AnimationStates.Find(EAnimationState::Sprinting))
			// {
			// 	AICharacter->ToggleSprinting(AnimationStates[EAnimationState::Sprinting]);
			// }
		}
		// MyController->ClearFocus(EAIFocusPriority::LastFocusPriority);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
