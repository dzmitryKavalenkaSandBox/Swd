#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Swd/Character/SwdCharacter.h"
#include "HUDWidget.generated.h"

class UAttackBase;
class UProgressBar;
class UAttackData;

UCLASS()
class SWD_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	// UProgressBar* HealthBar;
	// UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	// UProgressBar* StaminaBar; 

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* LeftLeg;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* RightLeg;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* LeftHand;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* RightHand;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* LeftElbow;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* RightElbow;

	UFUNCTION()
	void OnRightElbowPressed();
	UFUNCTION()
	void OnLeftLegPressed();
	UFUNCTION()
	void OnRightLegPressed();
	UFUNCTION()
	void OnRightHandPressed();
	UFUNCTION()
	void OnLeftHandPressed();

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	ASwdCharacter* OwningPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UAttackData* LeftLegKickAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UAttackData* RightLegKickAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UAttackData* RightHandAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UAttackData* PommelAttack;
};
