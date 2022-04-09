#include "AMDataTablePlayable.h"

#include "Swd/Utils/Logger.h"
#include "Swd/Utils/SwdGameUtils.h"


void IAMDataTablePlayable::PlayDataTableAnimation(
	ASwdCharacter* Character,
	UDataTable* DataTable,
	bool HasBlendedAnim,
	FString AnimActionName
)
{
	if (Character && DataTable)
	{
		static const FString ContextString(TEXT("Data table anim montage context"));
		FPlayerAnimMontage* AminMontage;
		FName RowName;
		if (Character->GetSpeed() <= 0 || !HasBlendedAnim)
		{
			Character->bIsAnimationBlended = false;
			RowName = FName(AnimActionName + FString("FullBody"));
		}
		else if (Character->GetSpeed() > 0 && HasBlendedAnim)
		{
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
