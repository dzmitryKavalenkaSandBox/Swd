#include "StatefulAI.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/Swd.h"
#include "Swd/Utils/Logger.h"

void IStatefulAI::UpdateAIState(EAIState State)
{
	if (!IsStateEquals(State))
	{
		ULogger::Log(ELogLevel::INFO, TEXT("Changing AI state to ") + UEnum::GetValueAsString(State));
		GetBBC()->SetValueAsEnum(BBKeys::AIState, (uint8)State);
	}
}

UBlackboardComponent* IStatefulAI::GetBBC()
{
	return nullptr;
}

bool IStatefulAI::IsStateEquals(EAIState State)
{
	return GetBBC()->GetValueAsEnum(BBKeys::AIState) == (uint8)State;
}
