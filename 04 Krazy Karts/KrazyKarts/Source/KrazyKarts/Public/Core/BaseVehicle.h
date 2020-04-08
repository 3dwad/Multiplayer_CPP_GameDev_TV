// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseVehicle.generated.h"


class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;


USTRUCT()
struct FVehicleMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		float Throttle;

	UPROPERTY()
		float SteeringThrow;

	UPROPERTY()
		float DeltaTime;

	UPROPERTY()
		float Time;

};


USTRUCT()
struct FVehicleServerState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FVehicleMove LastMove;

	UPROPERTY()
		FVector Velocity;

	UPROPERTY()
		FTransform Transform;

	UPROPERTY()
		float ServerTime;
};



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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FVehicleServerState ServerState;
/*

	UPROPERTY(Replicated)
	FVehicleMove VehicleMove;

*/

	UPROPERTY(Replicated, BlueprintReadOnly)
		FVector Velocity;

/*
	UPROPERTY(ReplicatedUsing = OnRep_RepTransform)
		FTransform ReplicatedTransform;
*/

	/* This function called when come update from server*/
	UFUNCTION()
		void OnRep_ServerState();

	UPROPERTY(BlueprintReadOnly)
		FVector Force;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetAirResistance();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetRollingResistance();


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void MoveForward(float InValue);
	void MoveRight(float InValue);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FVehicleMove InMove);


private:

	void SimulateMove(const FVehicleMove InMove);

	FVehicleMove CreateMove(float InDeltaTime);

	void UpdateLocationFromVelocity(float DeltaTime);

	void ApplyRotation(float DeltaTime, float InSteeringThrow);

	void ClearUnacknowledgedMoves(FVehicleMove InLastMove);

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


		TArray<FVehicleMove> UnacknowledgedMoves;
	
};
