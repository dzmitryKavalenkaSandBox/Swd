#include "StatefulAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Swd/Swd.h"
#include "Swd/AI/AIControllerBase.h"
#include "Swd/Character/AICharacterBase.h"
#include "Swd/Utils/Logger.h"

void IStatefulAIController::UpdateAIState(EAIState State)
{
	if (!IsStateEquals(State))
	{
		ULogger::Log(ELogLevel::INFO, TEXT("Changing AI state to ") + UEnum::GetValueAsString(State));
		GetBBC()->SetValueAsEnum(BBKeys::AIState, (uint8)State);
	}
	AAIControllerBase* AIController = Cast<AAIControllerBase>(this);
	if (!AIController) return;
	AAICharacterBase* AICharacter = AIController->Agent;
	UAICharacterData* CharacterData = Cast<UAICharacterData>(AICharacter->CharacterData);
	if (!CharacterData) return;

	switch (State)
	{
	case EAIState::Alerted:
		if (CharacterData->AlertedAnimInstanceClass)
		{
			AICharacter->GetMesh()->SetAnimInstanceClass(CharacterData->AlertedAnimInstanceClass);
			AICharacter->GetCharacterMovement()->MaxWalkSpeed = 230;
		}
		break;
	case EAIState::Idle:
		if (CharacterData->AnimInstanceClass)
		{
			AICharacter->GetMesh()->SetAnimInstanceClass(CharacterData->AnimInstanceClass);
			AICharacter->InitialMovementSetUp();
		}
		break;
	default: break;
	}
}

UBlackboardComponent* IStatefulAIController::GetBBC()
{
	return nullptr;
}

bool IStatefulAIController::IsStateEquals(EAIState State)
{
	return GetBBC()->GetValueAsEnum(BBKeys::AIState) == (uint8)State;
}
