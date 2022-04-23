#include "SwdGameUtils.h"

#include "Swd/Interfacces/DataTableAnimated.h"


USwdGameUtils::USwdGameUtils(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float USwdGameUtils::PlayAnimMontage(ASwdCharacter* AnimationOwner, FPlayerAnimMontage* AnimMontage,
                                     float InPlayRate,
                                     FName StartSectionName)
{
	if (AnimationOwner && AnimMontage)
	{
		return AnimationOwner->PlayAnimMontage(AnimMontage->Montage, InPlayRate, StartSectionName);
	}
	return 0.f;
}

AActor* USwdGameUtils::GetClosestActor(FVector SourceLocation, TArray<AActor*> Actors)
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
