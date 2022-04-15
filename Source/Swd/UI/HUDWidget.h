#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UObject/Object.h"
#include "HUDWidget.generated.h"

class UProgressBar;

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

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
};
