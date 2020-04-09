// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BaseVehicle.h"

#include "KKReplecationComponent.generated.h"




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
 
	/* This function called when come update from server*/
	UFUNCTION()
 		void OnRep_ServerState();
 
 	TArray<FKKVehicleMove> UnacknowledgedMoves;
		
private:

	ABaseVehicle* OwnerActor;
};
