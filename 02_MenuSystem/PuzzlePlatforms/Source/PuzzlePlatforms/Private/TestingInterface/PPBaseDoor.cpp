// Fill out your copyright notice in the Description page of Project Settings.


#include "TestingInterface/PPBaseDoor.h"
#include "Components/StaticMeshComponent.h"
#include "TestingInterface/PPButton.h"


// Sets default values
APPBaseDoor::APPBaseDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MainMesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APPBaseDoor::BeginPlay()
{
	Super::BeginPlay();

	LinkedButton->SetInterface(this);
	
}

// Called every frame
void APPBaseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPBaseDoor::OpenDoor_Interface()
{

	UE_LOG(LogTemp, Warning, TEXT("Interface Called!"));

	OpenDoor();

}

