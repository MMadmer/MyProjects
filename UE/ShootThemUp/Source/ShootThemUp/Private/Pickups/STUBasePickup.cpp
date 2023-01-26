// Shoot Them Up game. All Rights Reserved.


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

ASTUBasePickup::ASTUBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);
}

void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
	check(CollisionComponent);

	StartLocation = GetActorLocation();
	GenerateRotationYaw();
}

void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement();
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Pawn = Cast<APawn>(OtherActor);
	if (GivePickupTo(Pawn))
	{
		PickapWasTaken();
	}
}

bool ASTUBasePickup::GivePickupTo(APawn* Player)
{
	return false;
}

void ASTUBasePickup::PickapWasTaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
	}

	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
	GenerateRotationYaw();

	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
}

void ASTUBasePickup::GenerateRotationYaw()
{
	const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;

	RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}

void ASTUBasePickup::HandleMovement()
{
	if (!GetWorld()) return;

	float Time = GetWorld()->GetTimeSeconds();
	if (RotationYaw != 0.0f)
	{
		AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));
	}
	if (Amplitude != 0.0f)
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation.Z = StartLocation.Z + Amplitude * FMath::Sin(Frequency * Time);
		SetActorLocation(CurrentLocation);
	}
}
