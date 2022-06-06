#include "SwdGameUtils.h"

#include "Logger.h"
#include "Swd/Swd.h"


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

float USwdGameUtils::PlayAnimMontage(ASwdCharacter* AnimationOwner, UAnimMontage* AnimMontage,
                                     float InPlayRate,
                                     FName StartSectionName)
{
	if (AnimationOwner && AnimMontage)
	{
		return AnimationOwner->PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
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

void USwdGameUtils::PlayDataTableAnimation(
	ASwdCharacter* Character,
	UDataTable* DataTable,
	bool HasBlendedAnim,
	FString AnimActionName
)
{
	if (Character && DataTable)
	{
		ULogger::Log(ELogLevel::WARNING, FString("Animation Action Name ") + AnimActionName);
		static const FString ContextString(TEXT("Data table anim montage context"));
		FPlayerAnimMontage* AminMontage;
		FName RowName;
		if (Character->GetSpeed() <= 0 || !HasBlendedAnim)
		{
			ULogger::Log(ELogLevel::WARNING, TEXT("Playing Root Motion Animation"));
			Character->bIsAnimationBlended = false;
			RowName = FName(AnimActionName + FString("FullBody"));
		}
		else if (Character->GetSpeed() > 0 && HasBlendedAnim)
		{
			ULogger::Log(ELogLevel::WARNING, TEXT("Playing Blended animation"));
			Character->bIsAnimationBlended = true;
			RowName = FName(AnimActionName + FString("UpperBody"));
		}

		AminMontage = DataTable->FindRow<FPlayerAnimMontage>(
			RowName,
			ContextString,
			true
		);
		if (AminMontage)
		{
			FString MontageSection;
			if (AminMontage->AnimSectionCount == 1)
			{
				MontageSection = AnimActionName;
			}
			else
			{
				int MontageSectionIndex = rand() % AminMontage->AnimSectionCount + 1;
				MontageSection = AnimActionName + FString::FromInt(MontageSectionIndex);
			}
			USwdGameUtils::PlayAnimMontage(Character, AminMontage, 1.f, FName(*MontageSection));
		}
	}
}