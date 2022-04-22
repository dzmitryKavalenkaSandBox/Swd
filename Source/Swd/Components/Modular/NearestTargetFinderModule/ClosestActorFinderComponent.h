#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClosestActorFinderComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UClosestActorFinderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UClosestActorFinderComponent();

	UFUNCTION(BlueprintCallable)
	AActor* GetClosestTarget() const;

	AActor* GetClosestActorOutOfList(FVector SourceLocation, TArray<AActor*> Actors) const;

protected:
	virtual void BeginPlay() override;

private:
	APawn* Owner = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float RadiusOfSearch = 750.f;
};
