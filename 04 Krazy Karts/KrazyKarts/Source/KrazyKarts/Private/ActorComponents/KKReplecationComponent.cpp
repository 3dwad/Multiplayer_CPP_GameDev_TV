// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/KKReplecationComponent.h"
#include "ActorComponents/KKMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UKKReplecationComponent::UKKReplecationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


 	if (GetOwner()) OwnerActor = Cast<ABaseVehicle>(GetOwner());
	// ...
}


// Called when the game starts
void UKKReplecationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

/* For each variable we must create macros in this function*/
void UKKReplecationComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	/* IMPORTANT! Super must be called, otherwise we will be marked as simulated proxy*/
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		
	DOREPLIFETIME(UKKReplecationComponent, ServerState);
   }


 
// Called every frame
void UKKReplecationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		FKKVehicleMove VehicleMove = OwnerActor->KKMovementComponent->CreateMove(DeltaTime);
		OwnerActor->KKMovementComponent->SimulateMove(VehicleMove);

		UnacknowledgedMoves.Add(VehicleMove);
 		Server_SendMove(VehicleMove);
	}

	/* We are the server and in control of the pawn*/
	if (GetOwnerRole() == ROLE_Authority && OwnerActor->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		FKKVehicleMove VehicleMove = OwnerActor->KKMovementComponent->CreateMove(DeltaTime);
		Server_SendMove(VehicleMove);
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		OwnerActor->KKMovementComponent->SimulateMove(ServerState.LastMove);	
	}


 	// ...
 }

void UKKReplecationComponent::OnRep_ServerState()
{
	UE_LOG(LogTemp, Warning, TEXT("Update from server"));

	OwnerActor->SetActorTransform(ServerState.Transform);
	OwnerActor->KKMovementComponent->Velocity = ServerState.Velocity;

	ClearUnacknowledgedMoves(ServerState.LastMove);

	for (const FKKVehicleMove& CurrentMove : UnacknowledgedMoves)
	{
		OwnerActor->KKMovementComponent->SimulateMove(CurrentMove);
 	}
 }

void UKKReplecationComponent::ClearUnacknowledgedMoves(FKKVehicleMove InLastMove)
{
	TArray<FKKVehicleMove> NewMoves;

	for (const FKKVehicleMove& CurrentMove : UnacknowledgedMoves)
	{
		if (CurrentMove.Time > InLastMove.Time)
		{
			NewMoves.Add(CurrentMove);
		}
	}
 	UnacknowledgedMoves = NewMoves;
 }



void UKKReplecationComponent::Server_SendMove_Implementation(FKKVehicleMove InMove)
{
	OwnerActor->KKMovementComponent->SimulateMove(InMove);

	ServerState.LastMove = InMove;
	ServerState.Transform = OwnerActor->GetActorTransform();
	ServerState.Velocity = OwnerActor->KKMovementComponent->Velocity;
}

bool UKKReplecationComponent::Server_SendMove_Validate(FKKVehicleMove InMove)
{
	return true;
  }


