// Shoot Them Up game. All Rights Reserved.


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ASTUAIController::StaticClass();

	bUseControllerRotationYaw = false;
	auto MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->bUseControllerDesiredRotation = true;
		MovementComponent->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}
