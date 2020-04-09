// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/KKMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/Engine.h"


// Sets default values for this component's properties
UKKMovementComponent::UKKMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/* Get Owner Actor*/
	if (GetOwner()) OwnerActor = GetOwner();

	// ...
}


// Called when the game starts
void UKKMovementComponent::BeginPlay()
{
	Super::BeginPlay();


	// ...
	
}


// Called every frame
void UKKMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// ...
}


FVector UKKMovementComponent::GetAirResistance()
{
	float Speed = Velocity.SizeSquared();

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, "Velocity value is:" + FString::SanitizeFloat(Speed));

	return Speed * DragCoefficient * -Velocity.GetSafeNormal();
}

FVector UKKMovementComponent::GetRollingResistance()
{
	float GravityStrenght = -GetWorld()->GetGravityZ() / 100;

	float NormalForce = Mass * GravityStrenght;

	return -Velocity.GetSafeNormal() * RolingResistanceCoefficient * NormalForce;

}

FKKVehicleMove UKKMovementComponent::CreateMove(float InDeltaTime)
{
	FKKVehicleMove VehicleMove;
	VehicleMove.DeltaTime = InDeltaTime;
	VehicleMove.SteeringThrow = SteeringThrow;
	VehicleMove.Throttle = Throttle;	
	VehicleMove.Time = GetWorld()->GetTimeSeconds();
		
	return VehicleMove;
}

void UKKMovementComponent::SimulateMove(const FKKVehicleMove InMove)
{
	Force = OwnerActor->GetActorForwardVector() * MaxDrivingForce * InMove.Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * InMove.DeltaTime;

	ApplyRotation(InMove.DeltaTime, InMove.SteeringThrow);

	UpdateLocationFromVelocity(InMove.DeltaTime);
}

void UKKMovementComponent::ApplyRotation(float DeltaTime, float InSteeringThrow)
{

	float DeltaLocation = FVector::DotProduct(OwnerActor->GetActorForwardVector(), Velocity) * DeltaTime;

	float RotationAngle = DeltaLocation / MinimumTurningRadius * InSteeringThrow;

	FQuat RotationDelta(OwnerActor->GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	OwnerActor->AddActorWorldRotation(RotationDelta);

}

void UKKMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector DeltaLocation = Velocity * 100 * DeltaTime;
	FHitResult Result;
	OwnerActor->AddActorWorldOffset(DeltaLocation, true, &Result);

	if (Result.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}

}

void UKKMovementComponent::SetThrottle(float InValue)
{
	
	Throttle = InValue;
}

void UKKMovementComponent::SetSteeringThrow(float InValue)
{
	SteeringThrow = InValue;
}

