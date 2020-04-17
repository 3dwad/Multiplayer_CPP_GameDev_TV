// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "BaseVehicle.generated.h"


class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UKKMovementComponent;
class UKKReplecationComponent;


USTRUCT()
struct FKKVehicleMove
{
	GENERATED_BODY()

		UPROPERTY()
		float Throttle;

	UPROPERTY()
		float SteeringThrow;

	UPROPERTY()
		float DeltaTime;

	UPROPERTY()
		float Time;

	bool IsValid() const
	{
		return FMath::Abs(Throttle) <= 1 && FMath::Abs(SteeringThrow) <= 1;
	}
};


USTRUCT()
struct FKKVehicleServerState
{
	GENERATED_BODY()

		UPROPERTY()
		FKKVehicleMove LastMove;

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

	UPROPERTY(VisibleAnywhere, Category = "VehicleSettings")
	USceneComponent* MeshOffsetComponent;

	UPROPERTY(VisibleAnywhere, Category = "MoveSettings")
		UKKMovementComponent* KKMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "ReplicationSettings")
 		UKKReplecationComponent* KKReplicationComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float InValue);
	void MoveRight(float InValue);

private:

};
