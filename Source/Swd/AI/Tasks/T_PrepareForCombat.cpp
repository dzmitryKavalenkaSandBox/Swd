// Fill out your copyright notice in the Description page of Project Settings.


#include "T_PrepareForCombat.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/AI/AIControllerBase.h"
#include "Swd/Character/AICharacterBase.h"

UT_PrepareForCombat::UT_PrepareForCombat()
{
	bCreateNodeInstance = true;
	NodeName = TEXT("Prepare For Combat");
}


EBTNodeResult::Type UT_PrepareForCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto AI = Cast<AAIControllerBase>(OwnerComp.GetAIOwner())->Agent)
	{
		if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKeys::ReadyForCombat))
		{
			AI->ManageCombatState(true);
			AI->EquipSheathWeapon();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKeys::ReadyForCombat, true);
		}
	}
	return EBTNodeResult::Succeeded;
}
