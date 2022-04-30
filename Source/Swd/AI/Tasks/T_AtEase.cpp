// Fill out your copyright notice in the Description page of Project Settings.


#include "T_AtEase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/Swd.h"
#include "Swd/AI/AIControllerBase.h"
#include "Swd/Character/AICharacterBase.h"

UT_AtEase::UT_AtEase()
{
	bCreateNodeInstance = true;
	NodeName = TEXT("At Ease");
}

EBTNodeResult::Type UT_AtEase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto AI = Cast<AAIControllerBase>(OwnerComp.GetAIOwner())->Agent)
	{
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKeys::ReadyForCombat))
		{
			AI->ManageCombatState(false);
			AI->EquipSheathWeapon();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKeys::ReadyForCombat, false);
		}
	}
	return EBTNodeResult::Succeeded;
}
