#include "StatefulAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/Swd.h"
#include "Swd/Utils/Logger.h"

void IStatefulAIController::UpdateAIState(EAIState State)
{
	if (!IsStateEquals(State))
	{
		ULogger::Log(ELogLevel::INFO, TEXT("Changing AI state to ") + UEnum::GetValueAsString(State));
		GetBBC()->SetValueAsEnum(BBKeys::AIState, (uint8)State);
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
