#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIControllerBase.generated.h"

struct FAIStimulus;
class AAICharacter;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class AAIManager;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class SWD_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase();

	UPROPERTY(BlueprintReadOnly)
	class AAIManager* AIManager = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float DetectionLevel = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TimeStamp = 0.f;

	void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	UPROPERTY(transient)
	UBehaviorTreeComponent* BTC;

	UPROPERTY(transient)
	UBlackboardComponent* BBC;

	UPROPERTY(BlueprintReadWrite)
	class AAICharacterBase* Agent;

	uint8 EnemyKeyId;
	uint8 LocationKeyId;
	uint8 ContactKeyId;

protected:
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	/** A Sight Sense config for our AI */
	UAISenseConfig_Sight* Sight;
	
	UAISenseConfig_Hearing* Hearing;

	FTimerHandle DetectionTimer;

	void SetDetectionLevel();

	float Rate = 1.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float DetectionThreshold = 5.f;

	AActor* Target = nullptr;

	FVector LastStimulusLocation = FVector::ZeroVector;
};
