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
//
// UAnimMontage* USwdGameUtils::GetRandomMontage(UDataTable* DataTable, FName RowName, FString AnimAction)
// {
// 	static const FString ContextString(TEXT("Data table anim montage context"));
// 	FPlayerAnimMontage* AminMontage = DataTable->FindRow<FPlayerAnimMontage>(
// 		RowName,
// 		ContextString,
// 		true
// 	);
// 	if (AminMontage)
// 	{
// 		FString MontageSection;
// 		if (AminMontage->AnimSectionCount == 1)
// 		{
// 			MontageSection = AnimAction;
// 		}
// 		else
// 		{
// 			int MontageSectionIndex = rand() % AminMontage->AnimSectionCount + 1;
// 			MontageSection = AnimAction + FString::FromInt(MontageSectionIndex);
// 		}
// 		USwdGameUtils::PlayAnimMontage(Character, AminMontage, 1.f, FName(*MontageSection));
// 	}
// }
