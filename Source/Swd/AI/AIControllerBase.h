#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIManager.h"
#include "Perception/AIPerceptionComponent.h"
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
	float DetectionLevel = 0.f;

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

	float Rate = 1.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float DetectionThreshold = 10.f;

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
	
	bool HaveHostileInSenseArea();
};