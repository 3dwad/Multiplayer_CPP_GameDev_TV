// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BaseVehicle.h"
#include "KKReplecationComponent.generated.h"

struct FHermiteCubicSpline
{
	FVector StartLocation, StartDerivative, TargetLocation, TargetDerivative;

	FVector InterpolateLocation(float InLerpRatio)
	{
		return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, InLerpRatio);
	}
	FVector InterpolateDerivative(float InLerpRatio)
	{
		return FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, InLerpRatio);
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UKKReplecationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKKReplecationComponent();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void ClearUnacknowledgedMoves(FKKVehicleMove InLastMove);
 
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FKKVehicleMove InMove);
 	
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
 	FKKVehicleServerState ServerState;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdate;
	FTransform ClientStart;
	FVector ClientStartVelocity;
	void ClientTick(float DeltaTime);	

	void CreateSpline(FHermiteCubicSpline& InSpline, float InVelocityToDerivative);
	void InterpolateLocation(FHermiteCubicSpline& InSpline, float InLerpRatio);
	void InterpolateVelocity(FHermiteCubicSpline& Spline, float LerpRatio, float VelocityToDerivative);
	void InterpolateRotation(float InSlerpRatio);


	float ClientSimulatedTime;


	/* This function called when come update from server*/
	UFUNCTION()
 		void OnRep_ServerState(); 

	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();
	

 	TArray<FKKVehicleMove> UnacknowledgedMoves;
		
private:

	ABaseVehicle* OwnerActor;

	void UpdateServerState(FKKVehicleMove InMove);
};
