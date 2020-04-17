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
void UKKReplecationComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	/* IMPORTANT! Super must be called, otherwise we will be marked as simulated proxy*/
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKKReplecationComponent, ServerState);
}



// Called every frame
void UKKReplecationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FKKVehicleMove LastMove = OwnerActor->KKMovementComponent->GetLastMove();


	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove);
	}

	/* We are the server and in control of the pawn*/
	if (OwnerActor->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		UpdateServerState(LastMove);
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		ClientTick(DeltaTime);
	}

	// ...
}

void UKKReplecationComponent::ClientTick(float DeltaTime)
{

	ClientTimeSinceUpdate += DeltaTime;

	if (ClientTimeBetweenLastUpdate < KINDA_SMALL_NUMBER) return;

	FHermiteCubicSpline Spline;

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdate;
	float SlerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdate;
	float VelocityToDerivative = ClientTimeBetweenLastUpdate * 100;

	CreateSpline(Spline,VelocityToDerivative);	
	InterpolateLocation(Spline, LerpRatio);
	InterpolateVelocity(Spline, LerpRatio, VelocityToDerivative);
	InterpolateRotation(SlerpRatio);
}

void UKKReplecationComponent::InterpolateVelocity(FHermiteCubicSpline& Spline, float LerpRatio, float VelocityToDerivative)
{
	FVector NewDerivative = Spline.InterpolateDerivative(LerpRatio);
	FVector NewVelocity = NewDerivative / VelocityToDerivative;
	OwnerActor->KKMovementComponent->SetVelocity(NewVelocity);
}

void UKKReplecationComponent::InterpolateRotation(float InSlerpRatio)
{
	FQuat TargetRotation = ServerState.Transform.GetRotation();
	FQuat StartRotation = ClientStart.GetRotation();
	FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, InSlerpRatio);

	if (OwnerActor->MeshOffsetComponent)
	{
		OwnerActor->MeshOffsetComponent->SetWorldRotation(NewRotation);
	}
}

void UKKReplecationComponent::CreateSpline(FHermiteCubicSpline& InSpline, float InVelocityToDerivative)
{
	InSpline.TargetLocation = ServerState.Transform.GetLocation();	
	InSpline.StartLocation = ClientStart.GetLocation();	
	InSpline.StartDerivative = ClientStartVelocity * InVelocityToDerivative;
	InSpline.TargetDerivative = ServerState.Velocity * InVelocityToDerivative;
}

void UKKReplecationComponent::InterpolateLocation(FHermiteCubicSpline& InSpline, float InLerpRatio)
{
	FVector NewLocation = InSpline.InterpolateLocation(InLerpRatio);
	if (OwnerActor->MeshOffsetComponent)
	{
		OwnerActor->MeshOffsetComponent->SetWorldLocation(NewLocation);
	}

}

void UKKReplecationComponent::OnRep_ServerState()
{
	UE_LOG(LogTemp, Warning, TEXT("Update from server"));

	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy:
		AutonomousProxy_OnRep_ServerState();
		break;
	case ROLE_SimulatedProxy:
		SimulatedProxy_OnRep_ServerState();
		break;
	default:
		break;
	}
}

void UKKReplecationComponent::SimulatedProxy_OnRep_ServerState()
{
	if (!OwnerActor->KKMovementComponent) return;

	ClientTimeBetweenLastUpdate = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;
	
	if (OwnerActor->MeshOffsetComponent)
	{
		ClientStart.SetLocation(OwnerActor->MeshOffsetComponent->GetComponentLocation());
		ClientStart.SetRotation(OwnerActor->MeshOffsetComponent->GetComponentQuat());
	}

	
	ClientStartVelocity = OwnerActor->KKMovementComponent->Velocity;

	OwnerActor->SetActorTransform(ServerState.Transform);
}

void UKKReplecationComponent::AutonomousProxy_OnRep_ServerState()
{
	OwnerActor->SetActorTransform(ServerState.Transform);
	OwnerActor->KKMovementComponent->Velocity = ServerState.Velocity;

	ClearUnacknowledgedMoves(ServerState.LastMove);

	for (const FKKVehicleMove& CurrentMove : UnacknowledgedMoves)
	{
		OwnerActor->KKMovementComponent->SimulateMove(CurrentMove);
	}
}

void UKKReplecationComponent::UpdateServerState(FKKVehicleMove InMove)
{

	ServerState.LastMove = InMove;
	ServerState.Transform = OwnerActor->GetActorTransform();
	ServerState.Velocity = OwnerActor->KKMovementComponent->Velocity;

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
	ClientSimulatedTime += InMove.DeltaTime;
	
	OwnerActor->KKMovementComponent->SimulateMove(InMove);

	UpdateServerState(InMove);
}

bool UKKReplecationComponent::Server_SendMove_Validate(FKKVehicleMove InMove)
{

	float ProopsedTime = ClientSimulatedTime + InMove.DeltaTime;
	bool ClientNotRunningAhead = ProopsedTime < GetWorld()->TimeSeconds;

	if (!ClientNotRunningAhead)
	{
		UE_LOG(LogTemp,Error,TEXT("Client is running too fast"))
		return false;
	}
	if (!InMove.IsValid())
	{
		return false;
	}
	return InMove.IsValid();

}


