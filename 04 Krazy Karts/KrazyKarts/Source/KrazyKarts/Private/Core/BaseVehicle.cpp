// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BaseVehicle.h"

#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Engine/Engine.h"



// Sets default values
ABaseVehicle::ABaseVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SkelMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkelMeshComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(SkelMeshComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	
}

// Called when the game starts or when spawned
void ABaseVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	 FVector DeltaLocation = Velocity * DeltaTime;

	 AddActorWorldOffset(DeltaLocation, true);



	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::SanitizeFloat(ForwarAxisValue));

}

// Called to bind functionality to input
void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseVehicle::MoveForward);

	
	
}

void ABaseVehicle::MoveForward(float InValue)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, "Forward value is:" + FString::SanitizeFloat(InValue));


	Velocity = GetActorForwardVector()* Speed * InValue;

}

