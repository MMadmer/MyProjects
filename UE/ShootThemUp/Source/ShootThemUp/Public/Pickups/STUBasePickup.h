// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	

	ASTUBasePickup();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (ClapMin = "0.1"))
	float RespawnTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (ClapMin = "0.0"))
	float Amplitude = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (ClapMin = "0.0"))
	float Frequency = 2.0f;

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	float RotationYaw = 0.0f;
	FVector StartLocation;

	virtual bool GivePickupTo(APawn* Player);

	void PickapWasTaken();
	void Respawn();
	void GenerateRotationYaw();
	void HandleMovement();

};
