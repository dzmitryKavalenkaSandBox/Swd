#include "AIControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Hearing.h"
#include "Kismet/KismetmathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SmartObjects/SmartObject.h"
#include "Swd/Swd.h"
#include "Swd/Character/AICharacterBase.h"


AAIControllerBase::AAIControllerBase()
{
	BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("behaviorTree Compponent"));
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AAICharacterBase* Chr = Cast<AAICharacterBase>(InPawn);
	if (Chr != nullptr && Chr->TreeAsset != nullptr)
	{
		// Initialize the Blackboard
		BBC->InitializeBlackboard(*Chr->TreeAsset->BlackboardAsset);

		// Set Blackboard Key IDs
		EnemyKeyId = BBC->GetKeyID(BBKeys::TargetActor);
		LocationKeyId = BBC->GetKeyID(BBKeys::MoveToLocation);
		ContactKeyId = BBC->GetKeyID(BBKeys::Contact);

		// Start The BehaviorTree.
		BTC->StartTree(*Chr->TreeAsset);
	}
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (!Agent)
	{
		AAICharacterBase* Chr = Cast<AAICharacterBase>(GetPawn());
		if (!Chr)
		{return;}
		Agent = Chr;
		Agent->ControllerRef = this;
	}

	if (Agent->SmartObject)
	{
		FGameplayTag SubTag;
		BTC->SetDynamicSubtree(SubTag, Agent->SmartObject->SubTree);
	}
}
