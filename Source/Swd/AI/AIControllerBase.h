#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "Swd/Character/SwdCharacter.h"
#include "Swd/Interfacces/AI/StatefulAI.h"
#include "AIControllerBase.generated.h"

struct FAIStimulus;
class AAICharacter;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class AAIManager;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class SWD_API AAIControllerBase : public AAIController, public IStatefulAI
{
	GENERATED_BODY()

public:
	AAIControllerBase();

	UPROPERTY(BlueprintReadOnly)
	AAIManager* AIManager = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float DetectionLevel = 1.f;

	UPROPERTY(BlueprintReadOnly)
	float TimeStampWhenLastSensed = 0.f;

	void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	UPROPERTY(transient)
	UBehaviorTreeComponent* BTC;

	UPROPERTY(transient)
	UBlackboardComponent* BBC;

	UPROPERTY(BlueprintReadWrite)
	class AAICharacterBase* Agent = nullptr;

	uint8 EnemyKeyId;
	uint8 LocationKeyId;
	uint8 ContactKeyId;

	virtual UBlackboardComponent* GetBBC() override;

protected:
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	virtual void Tick(float DeltaSeconds) override;

	/*
	 * Will be called everytime actor sense someone or stop sensing it
	 */
	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	/** A Sight Sense config for our AI */
	UAISenseConfig_Sight* Sight;

	UAISenseConfig_Hearing* Hearing;

	FTimerHandle DetectionTimer;

	void UpdateDetectionLevel();

	// AActor* Target = nullptr;

	// FVector LastStimulusLocation = FVector::ZeroVector;

	/**
	 *
	 * Will hold the list of actors that are currently in sensed area
	 */
	TArray<AActor*> SensedActors;

	/**
	 * Will add actors to SensedActors list if actor just sensed and remove it from list when actor stopped sensing
	 */
	void ManageSensedActor(AActor* SensedActor);

private:
	bool ShouldStartDetection();

	void StartDetection();
	void SwitchToAIState(EAIState State);

	bool HaveHostileInSenseArea();

	/*
	 * Distance threshold at which the rate of detection will change (closer the target - faster the detection)
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Emeny Detection")
	float DistanceForRateOfDetectionChange = 200.f;
	/*
	 * Rate at which the detection level will increment
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Emeny Detection")
	float RateOfDetection = 1.f;

	/*
	 * Rate at witch the detection will slow down, if the distance to target is greater then DistanceForRateOfDetectionChange
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Emeny Detection")
	float RateOfDetectionIncrement = 1.f;
	/*
	 * Detection level at which the AI state will switch to Alerted
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Emeny Detection")
	float SwitchToAlertDetectionLevel;
	/*
	 * Distance to hostile at which the AI state will switch to Attack
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), Category="Emeny Detection")
	float SwitchToAttackDistance = 200;
	/*
	 * Max level of detection possible. Of reached - the AI state will switch to Attack
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="Emeny Detection")
	float MaxDetectionLevel = 10.f;

	float CalculateDetectionLevelIncrement();

	ASwdCharacter* ClosestHostile = nullptr;
};
