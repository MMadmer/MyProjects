// Sky Faller. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKYFALLER_API USFHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USFHealthComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float Health = 100.0f;

	virtual void BeginPlay() override;

	bool IsDead() const;
		
};
