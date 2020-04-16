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
#include "ActorComponents/KKMovementComponent.h"
#include "ActorComponents/KKReplecationComponent.h"


// Sets default values
ABaseVehicle::ABaseVehicle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/* Enable replicated*/
	bReplicates = true;
	SetReplicateMovement(false);

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBoxComponent->SetupAttachment(RootComponent);

	SkelMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkelMeshComponent->SetupAttachment(CollisionBoxComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(SkelMeshComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	KKMovementComponent = CreateDefaultSubobject<UKKMovementComponent>(TEXT("KKMovementComponent"));
	KKReplicationComponent = CreateDefaultSubobject<UKKReplecationComponent>(TEXT("KKReplicationComponent"));
	KKReplicationComponent->SetIsReplicated(true);

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

	DrawDebugString(GetWorld(), FVector(0, 0, 200), GetEnumText(GetLocalRole()), this, FColor::Green, DeltaTime);
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

	KKMovementComponent->SetThrottle(InValue);
}

void ABaseVehicle::MoveRight(float InValue)
{
	KKMovementComponent->SetSteeringThrow(InValue);
}
