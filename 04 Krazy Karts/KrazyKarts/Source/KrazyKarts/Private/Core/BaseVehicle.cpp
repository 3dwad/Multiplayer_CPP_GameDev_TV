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
#include "Net/UnrealNetwork.h"



// Sets default values
ABaseVehicle::ABaseVehicle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/* Enable replicated*/
	bReplicates = true;

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

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
}

/* For each variable we must create macros in this function*/
void ABaseVehicle::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	/* IMPORTANT! Super must be called, otherwise we will be marked as simulated proxy*/
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseVehicle, ServerState);
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

	if (GetLocalRole()==ROLE_AutonomousProxy)
	{
		FVehicleMove VehicleMove = CreateMove(DeltaTime);
		SimulateMove(VehicleMove);

		UnacknowledgedMoves.Add(VehicleMove);
		Server_SendMove(VehicleMove);
	}

	/* We are the server and in control of the pawn*/
	if (GetLocalRole()==ROLE_Authority && GetRemoteRole()==ROLE_SimulatedProxy)
	{
		FVehicleMove VehicleMove = CreateMove(DeltaTime);
		Server_SendMove(VehicleMove);
	}

	if (GetLocalRole()==ROLE_SimulatedProxy)
	{
		SimulateMove(ServerState.LastMove);
	}

	DrawDebugString(GetWorld(), FVector(0, 0, 200), GetEnumText(GetLocalRole()), this, FColor::Green, DeltaTime);

}

FVehicleMove ABaseVehicle::CreateMove(float InDeltaTime)
{
	FVehicleMove VehicleMove;
	VehicleMove.DeltaTime = InDeltaTime;
	VehicleMove.SteeringThrow = SteeringThrow;
	VehicleMove.Throttle = Throttle;
	VehicleMove.Time = GetWorld()->TimeSeconds;

	return VehicleMove;
}

void ABaseVehicle::OnRep_ServerState()
{
	UE_LOG(LogTemp, Warning, TEXT("Update from server"));

	SetActorTransform(ServerState.Transform);
	Velocity = ServerState.Velocity;

	ClearUnacknowledgedMoves(ServerState.LastMove);

	for (const FVehicleMove& CurrentMove : UnacknowledgedMoves)
	{

		SimulateMove(CurrentMove);
	}
}

void ABaseVehicle::ClearUnacknowledgedMoves(FVehicleMove InLastMove)
{

	TArray<FVehicleMove> NewMoves;

	for (const FVehicleMove& CurrentMove : UnacknowledgedMoves)
	{
		if (CurrentMove.Time > InLastMove.Time)
		{
			NewMoves.Add(CurrentMove);
		}
	}
	UnacknowledgedMoves = NewMoves;
}


void ABaseVehicle::SimulateMove(const FVehicleMove InMove)
{
	Force = GetActorForwardVector() * MaxDrivingForce * InMove.Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * InMove.DeltaTime;

	ApplyRotation(InMove.DeltaTime,InMove.SteeringThrow);

	UpdateLocationFromVelocity(InMove.DeltaTime);
}

FVector ABaseVehicle::GetAirResistance()
{
	float Speed = Velocity.SizeSquared();

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, "Velocity value is:" + FString::SanitizeFloat(Speed));

	return Speed * DragCoefficient * -Velocity.GetSafeNormal();
}

FVector ABaseVehicle::GetRollingResistance()
{
	float GravityStrenght = -GetWorld()->GetGravityZ() / 100;

	float NormalForce = Mass * GravityStrenght;

	return -Velocity.GetSafeNormal() * RolingResistanceCoefficient * NormalForce;
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

void ABaseVehicle::ApplyRotation(float DeltaTime, float InSteeringThrow)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;

	float RotationAngle = DeltaLocation / MinimumTurningRadius * InSteeringThrow;

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
}


void ABaseVehicle::Server_SendMove_Implementation(FVehicleMove InMove)
{
	SimulateMove(InMove);

	ServerState.LastMove = InMove;
	ServerState.Transform = GetActorTransform();
	ServerState.Velocity = Velocity;
}

bool ABaseVehicle::Server_SendMove_Validate(FVehicleMove InMove)
{
	return true;
}

void ABaseVehicle::MoveRight(float InValue)
{
	SteeringThrow = InValue;
}
