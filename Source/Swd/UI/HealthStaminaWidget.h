#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthStaminaWidget.generated.h"

class UProgressBar;

UCLASS()
class SWD_API UHealthStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* StaminaBar;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
};
