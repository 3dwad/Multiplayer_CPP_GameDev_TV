// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BaseVehicle.h"
#include "KKMovementComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KRAZYKARTS_API UKKMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKKMovementComponent();

	UPROPERTY()
		FVector Velocity;

	UPROPERTY()
		FVector Force;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetAirResistance();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetRollingResistance();

	
	FKKVehicleMove GetLastMove();	

	void SetVelocity(FVector InVelocity) { Velocity = InVelocity; };

	void SimulateMove(const FKKVehicleMove InMove);

	void ApplyRotation(float DeltaTime, float InSteeringThrow);

	void UpdateLocationFromVelocity(float DeltaTime);

	void SetThrottle(float InValue);

	void SetSteeringThrow(float InValue);

	/* In kg*/
	UPROPERTY(EditDefaultsOnly, Category = "MovementSettings")
		float Mass = 1000.f;

	/* The force applied to the car when the throttle is fully down (Newton)*/
	UPROPERTY(EditDefaultsOnly, Category = "MovementSettings")
		float MaxDrivingForce = 10000.f;

	/* The Drag with air. Higher value mean more drag. kg/meter*/
	UPROPERTY(EditDefaultsOnly, Category = "MovementSettings")
		float DragCoefficient = 16.f;

	/* kg/meter*/
	UPROPERTY(EditDefaultsOnly, Category = "MovementSettings")
		float RolingResistanceCoefficient = 0.03f;

	UPROPERTY(EditDefaultsOnly, Category = "MovementSettings")
		float MinimumTurningRadius = 10.f;

private:

	float Throttle;
	float SteeringThrow;

	AActor* OwnerActor;

	FKKVehicleMove LastMove;

	FKKVehicleMove CreateMove(float InDeltaTime);
};
