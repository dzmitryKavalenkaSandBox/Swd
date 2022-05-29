#include "T_SetRandomMoveToLocationAroundTarget.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/Swd.h"

UT_SetRandomMoveToLocationAroundTarget::UT_SetRandomMoveToLocationAroundTarget()
{
	NodeName = TEXT("Set Random Move To Around Provided Location");
}

EBTNodeResult::Type UT_SetRandomMoveToLocationAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                                        uint8* NodeMemory)
{
	FVector OriginLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	FNavLocation TargetLocation;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem->GetRandomPointInNavigableRadius(
		OriginLocation, RadiusAroundPLayer, TargetLocation, nullptr))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKeys::MoveToLocation, TargetLocation.Location);
	}
	return EBTNodeResult::Succeeded;
}
