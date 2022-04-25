#include "AIControllerBase.h"

#include "AIManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "SmartObjects/SmartObject.h"
#include "Swd/Swd.h"
#include "Swd/Character/AICharacterBase.h"
#include "Swd/Utils/Logger.h"
#include "Swd/Utils/SwdGameUtils.h"


AAIControllerBase::AAIControllerBase()
{
	BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Compponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Componennt"));

	//Create a Sight And Hearing Sense
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	Sight->SightRadius = 2000.f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;

	Hearing->HearingRange = 2000.f;

	//Tell the senses to detect everything
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = true;

	//Register the sight sense to our Perception Component
	AIPerceptionComponent->ConfigureSense(*Sight);
	AIPerceptionComponent->ConfigureSense(*Hearing);
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AAICharacterBase* AICharacter = Cast<AAICharacterBase>(InPawn);

	if (AICharacter != nullptr && AICharacter->TreeAsset != nullptr)
	{
		Agent = AICharacter;
		Agent->ControllerRef = this;

		BBC->InitializeBlackboard(*AICharacter->TreeAsset->BlackboardAsset);

		EnemyKeyId = BBC->GetKeyID(BBKeys::TargetActor);
		LocationKeyId = BBC->GetKeyID(BBKeys::MoveToLocation);
		ContactKeyId = BBC->GetKeyID(BBKeys::Contact);

		BTC->StartTree(*AICharacter->TreeAsset);
	}
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnPerception);
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (!Agent)
	{
		AAICharacterBase* AICharacter = Cast<AAICharacterBase>(GetPawn());
		if (!AICharacter)
		{
			return;
		}
		Agent = AICharacter;
		Agent->ControllerRef = this;
	}

	if (Agent->SmartObject)
	{
		FGameplayTag SubTag;
		BTC->SetDynamicSubtree(SubTag, Agent->SmartObject->SubTree);
	}
}

void AAIControllerBase::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	ASwdCharacter* SensedCharacter = Cast<ASwdCharacter>(Actor);
	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass())
	{
		if (SensedCharacter && Agent->IsHostile(SensedCharacter))
		{
			// ULogger::Log(ELogLevel::INFO, TEXT("Setting contact"));
			BBC->SetValueAsBool(BBKeys::Contact, Stimulus.WasSuccessfullySensed());

			// Check to see if AI already attacking someone
			if (!IsStateEquals(EAIState::Attack))
			{
				Agent->GetCharacterMovement()->StopActiveMovement();
				// ULogger::Log(ELogLevel::INFO, FString("Setting target actor to: ") + FString(Actor->GetName()));
				BBC->SetValueAsObject(BBKeys::TargetActor, SensedCharacter);
			}

			Target = SensedCharacter;
			LastStimulusLocation = Stimulus.StimulusLocation;
			if (AIManager) AIManager->LastStimulusLocation = LastStimulusLocation;
			TimeStampWhenLastSensed = UKismetSystemLibrary::GetGameTimeInSeconds(Agent/*pass GetWorld() here*/);
		}

		if (!GetWorldTimerManager().IsTimerActive(DetectionTimer) && BBC->GetValueAsBool(BBKeys::Contact) &&
			(IsStateEquals(EAIState::Idle) || IsStateEquals(EAIState::Alerted) ||
				IsStateEquals(EAIState::Search)))
		{
			DetectionLevel = 0.f;
			Agent->UpdateWidgetVis(true);
			GetWorldTimerManager().SetTimer(DetectionTimer, this, &AAIControllerBase::SetDetectionLevel, Rate, true,
			                                0.f);
		}
		return;
	}

	if (IsStateEquals(EAIState::Attack)) return;

	// if (SensedCharacter && SensedCharacter->IsHostile(Agent))
	// {
	// 	BBC->SetValueAsEnum(BBKeys::AIState, (uint8)EAIState::Alerted);
	// 	BBC->SetValueAsVector(BBKeys::LastStimulusLocation, Stimulus.StimulusLocation);
	// }
}

void AAIControllerBase::SetDetectionLevel()
{
	if (!Target || !BBC->GetValueAsBool(BBKeys::Contact))
	{
		// if (!IsAIStateEquals(EAIState::Idle))
		// {
		// 	GetWorldTimerManager().ClearTimer(DetectionTimer);
		// 	Agent->UpdateWidgetVis(false);
		// 	return;
		// }

		if (DetectionLevel > 0.f)
		{
			DetectionLevel -= 1;
			return;
		}
		GetWorldTimerManager().ClearTimer(DetectionTimer);
		Agent->UpdateWidgetVis(false);
		UpdateAIState(EAIState::Idle);
		return;
	}

	const float Distance = GetPawn()->GetDistanceTo(Target);
	Rate = (Distance <= 500.f) ? 1.f : 2.f;
	DetectionLevel += 1;

	if (DetectionLevel >= DetectionThreshold)
	{
		if (AIManager) AIManager->NotifyAllAgentsAIState(EAIState::Attack);
		GetWorldTimerManager().ClearTimer(DetectionTimer);
		Agent->UpdateWidgetVis(false);
		return;
	}

	if (DetectionLevel >= DetectionThreshold / 2)
	{
		UpdateAIState(EAIState::Alerted);
		BBC->SetValueAsVector(BBKeys::LastStimulusLocation, LastStimulusLocation);
	}
}

UBlackboardComponent* AAIControllerBase::GetBBC()
{
	return BBC;
}