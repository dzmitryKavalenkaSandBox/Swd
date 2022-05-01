// Fill out your copyright notice in the Description page of Project Settings.


#include "AIManager.h"

#include "AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Swd/Swd.h"
#include "Swd/Character/AICharacterBase.h"
#include "Swd/Utils/SwdGameUtils.h"

AAIManager::AAIManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAIManager::CoverFire(bool ProvideCoverFire, AAICharacterBase* Instgtr)
{
	if (!Instgtr || Defenders.Find(Instgtr) < 0) return;

	if (ProvideCoverFire)
	{
		for (auto& Agnt : Defenders)
		{
			if (Agnt != Instgtr && Agnt->ControllerRef->BBC->GetValueAsEnum("CombatState") == (uint8)
				ECombatState::HoldCover)
			{
				AgentProvidingCoverFire = Agnt;
				Agnt->ControllerRef->BBC->SetValueAsBool("ShootFromCover", true);
				break;
			}
		}
		return;
	}

	if (AgentProvidingCoverFire->ControllerRef->BBC->GetValueAsEnum("CombatState") == (uint8)ECombatState::HoldCover)
		AgentProvidingCoverFire->ControllerRef->BBC->SetValueAsBool("ShootFromCover", false);
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	CreateAgentsList();
	UpdateCombatRole();
}

bool AAIManager::IsAnyAgentEngaged()
{
	bool IsEngaged = false;

	for (auto& AIController : Agents)
	{
		if (AIController->BBC->GetValueAsBool(BBKeys::Contact))
		{
			IsEngaged = true;
			break;
		}

		if (AIController->BBC->GetValueAsObject(BBKeys::BestTarget))
		{
			IsEngaged = true;
			break;
		}

		if (UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - AIController->TimeStampWhenLastSensed <
			MaxStimulusTime_Combat)
		{
			IsEngaged = true;
			break;
		}
	}
	return IsEngaged;
}

void AAIManager::RunCombatLoop()
{
	if (IsAnyAgentEngaged())
	{
		//Approach
		if (Defenders.Num() > 0)
		{
			Defenders[ApproacherIndex]->ControllerRef->BBC->SetValueAsEnum(
				BBKeys::CombatState, (uint8)ECombatState::ApproachingCover
			);
			CoverFire(true, Defenders[ApproacherIndex]);
			ApproacherIndex = (ApproacherIndex + 1 <= Defenders.Num() - 1) ? ApproacherIndex += 1 : 0;
		}

		return;
	}

	NotifyAllAgentsAIState(EAIState::LostEnemy);
	GetWorldTimerManager().ClearTimer(CombatTimer);
}

void AAIManager::RunSearchTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
	                                 FString::Printf(
		                                 TEXT("ElapsedTime Is : %f"),
		                                 UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) -
		                                 TimeStampWhenStartedSearching));

	if (UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - TimeStampWhenStartedSearching < MaxStimulusTime_Search)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(SearchTimer);
	NotifyAllAgentsAIState(EAIState::Idle);
}

void AAIManager::CreateAgentsList()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIControllerBase::StaticClass(), Actors);

	for (auto& GenericController : Actors)
	{
		AAIControllerBase* AIController = Cast<AAIControllerBase>(GenericController);
		if (AIController && AIController->Agent && AIController->Agent->Faction == Faction)
		{
			Agents.AddUnique(AIController);
			AIController->AIManager = this;
		}
	}
}

// currently sets attack to all the agents, should probably make it more intelligent
void AAIManager::NotifyAllAgentsAIState(EAIState State)
{
	for (auto& AIController : Agents)
	{
		IsAnyAgentEngaged()
			? AIController->UpdateAIState(EAIState::Attack)
			: AIController->UpdateAIState(State);
		AIController->BBC->SetValueAsVector(BBKeys::LastLocationOfClosestHostile, LastStimulusLocation);
	}

	if (State == EAIState::Attack)
	{
		GetWorldTimerManager().ClearTimer(SearchTimer);
		GetWorldTimerManager().SetTimer(CombatTimer, this, &AAIManager::RunCombatLoop, ApproachDelay, true);
		return;
	}

	if (State == EAIState::Search)
	{
		TimeStampWhenStartedSearching = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
		GetWorldTimerManager().SetTimer(SearchTimer, this, &AAIManager::RunSearchTimer, 1.f, true);
		return;
	}
}

void AAIManager::RemoveAgent(AAIControllerBase* ControllerToRemove)
{
	int32 Index = Agents.Find(ControllerToRemove);
	if (Index < 0)
	{
		return;
	}

	Agents.RemoveSingle(ControllerToRemove);
	UpdateCombatRole();
}

void AAIManager::UpdateCombatRole()
{
	for (auto& AIController : Agents)
	{
		if (AIController->Agent->CombatRole == ECombatRole::Defender)
		{
			Defenders.AddUnique(AIController->Agent);
		}
	}
}
