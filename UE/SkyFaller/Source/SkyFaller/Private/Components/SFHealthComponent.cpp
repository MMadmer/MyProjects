// Sky Faller. All rights reserved.


#include "Components/SFHealthComponent.h"
#include "SFCoreTypes.h"

USFHealthComponent::USFHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USFHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

bool USFHealthComponent::IsDead() const
{
	return Health <= 0.0f;
}
