// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"	

class USpringArmComponent;
class UCameraComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUWeaponComponent* WeaponComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (ClampMin = "0"))
	float LandedAnim = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (ClampMin = "0"))
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (ClampMin = "0"))
	FVector2D LandedDamageVelocity = FVector2D(800.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (ClampMin = "0"))
	FVector2D LandedDamage = FVector2D(1.0f, 100.0f);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void AddControllerYawInput(float Val) override;
	virtual void Jump() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool CanLandedBlock = true;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool InputBlocked = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsMovingForward = false;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsMoving() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;


private:
	FTimerHandle UnblockMovementHandle;
	bool WantsToRun = false;

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void OnStartRunning();
	void OnStopRunning();

	void OnDeath();
	void OnHealthChanged(float Health, float HealthDelta);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

	void UnblockMovement();
};
