// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseVehicle.generated.h"


class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;

UCLASS()
class KRAZYKARTS_API ABaseVehicle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseVehicle();

	UPROPERTY(VisibleAnywhere, Category = "VehicleSettings")
	USkeletalMeshComponent* SkelMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "CameraSettings")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "CameraSettings")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "VehicleSettings")
	UBoxComponent* CollisionBoxComponent;

	

	float ForwarAxisValue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly)
	FVector Force;

	UFUNCTION(BlueprintCallable ,BlueprintPure)
	FVector GetAirResistance();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetRollingResistance();
	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float InValue);

	void MoveRight(float InValue);


private:

	

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


	float Throttle;
	float SteeringThrow;



	void UpdateLocationFromVelocity(float DeltaTime);

	void ApplyRotation(float DeltaTime);
};
