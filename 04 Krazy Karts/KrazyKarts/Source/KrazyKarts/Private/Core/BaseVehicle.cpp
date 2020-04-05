// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BaseVehicle.h"

#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"



// Sets default values
ABaseVehicle::ABaseVehicle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBoxComponent->SetupAttachment(RootComponent);

	SkelMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkelMeshComponent->SetupAttachment(CollisionBoxComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(SkelMeshComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

}

// Called when the game starts or when spawned
void ABaseVehicle::BeginPlay()
{
	Super::BeginPlay();

}

FString GetEnumText(ENetRole InRole)
{
	switch (InRole)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "Error";
	}
}

// Called every frame
void ABaseVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	Force = GetActorForwardVector() * MaxDrivingForce * Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * DeltaTime;
	
	ApplyRotation(DeltaTime);

	
	UpdateLocationFromVelocity(DeltaTime);	

	DrawDebugString(GetWorld(), FVector(0, 0, 200), GetEnumText(GetLocalRole()), this, FColor::Green,DeltaTime);
}

FVector ABaseVehicle::GetAirResistance()
{
	float Speed = Velocity.SizeSquared();

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, "Velocity value is:" + FString::SanitizeFloat(Speed));

	return Speed * DragCoefficient * - Velocity.GetSafeNormal();	
}

FVector ABaseVehicle::GetRollingResistance()
{
	float GravityStrenght = -GetWorld()->GetGravityZ() / 100;

	float NormalForce = Mass * GravityStrenght;

	return -Velocity.GetSafeNormal() *RolingResistanceCoefficient * NormalForce;
}

void ABaseVehicle::UpdateLocationFromVelocity(float DeltaTime)
{

	FVector DeltaLocation = Velocity * 100 * DeltaTime;
	FHitResult Result;
	AddActorWorldOffset(DeltaLocation, true, &Result);

	if (Result.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}

}

void ABaseVehicle::ApplyRotation(float DeltaTime)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(),Velocity) * DeltaTime;

	float RotationAngle = DeltaLocation / MinimumTurningRadius * SteeringThrow;

	FQuat RotationDelta(GetActorUpVector(), RotationAngle);


	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

// Called to bind functionality to input
void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseVehicle::MoveRight);
}

void ABaseVehicle::MoveForward(float InValue)
{
	Throttle = InValue;
	Server_MoveForward(InValue);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, "Forward value is:" + FString::SanitizeFloat(InValue));
}


void ABaseVehicle::Server_MoveForward_Implementation(float InValue)
{		
		Throttle = InValue;	
}

bool ABaseVehicle::Server_MoveForward_Validate(float InValue)
{
	return FMath::Abs(InValue) <= 1;	
	
}

void ABaseVehicle::MoveRight(float InValue)
{
	SteeringThrow = InValue;
	Server_MoveRight(InValue);
}

void ABaseVehicle::Server_MoveRight_Implementation(float InValue)
{
	SteeringThrow = InValue;
}

bool ABaseVehicle::Server_MoveRight_Validate(float InValue)
{
	return FMath::Abs(InValue) <= 1;
}
