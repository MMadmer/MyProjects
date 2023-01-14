// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBaseWeapon();

	virtual void StartFire();
	virtual void StopFire();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (ClapMin = "0.0"))
	float TraceMaxDistance = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageAmount = 10.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewTotation) const;
	FVector GetMuzzleWorldLocation() const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
	void MakeDamage(const FHitResult& HitResult);
	
};
