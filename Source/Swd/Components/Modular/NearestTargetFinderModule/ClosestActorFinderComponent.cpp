#include "ClosestActorFinderComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Swd/Utils/Logger.h"


UClosestActorFinderComponent::UClosestActorFinderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UClosestActorFinderComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<APawn>(GetOwner());
}


AActor* UClosestActorFinderComponent::GetNearestTarget() const
{
	TArray<AActor*> FoundActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	TArray<FHitResult> Hits;

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Owner->GetActorLocation(),
		Owner->GetActorLocation(),
		RadiusOfSearch,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hits,
		true
	);
	for (FHitResult Hit : Hits)
	{
		FoundActors.Add(Hit.GetActor());
	}

	return GetClosestActorOutOfList(Owner->GetActorLocation(), FoundActors);
}

AActor* UClosestActorFinderComponent::GetClosestActorOutOfList(FVector SourceLocation, TArray<AActor*> Actors) const
{
	if (Actors.Num() <= 0)
	{
		return nullptr;
	}

	AActor* ClosestActor = nullptr;
	float CurrentClosestDistance = TNumericLimits<float>::Max();

	for (int i = 0; i < Actors.Num(); i++)
	{
		float distance = FVector::DistSquared(SourceLocation, Actors[i]->GetActorLocation());
		if (distance < CurrentClosestDistance)
		{
			CurrentClosestDistance = distance;
			ClosestActor = Actors[i];
		}
	}
	return ClosestActor;
}
