#include "SmartObject.h"

#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"


ASmartObject::ASmartObject()
{
	PrimaryActorTick.bCanEverTick = false;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());

	FacingDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Facing Direction"));
	FacingDirection->SetupAttachment(Billboard);
}

void ASmartObject::BeginPlay()
{
	Super::BeginPlay();
}

