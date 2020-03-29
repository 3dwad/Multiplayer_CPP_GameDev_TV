// Fill out your copyright notice in the Description page of Project Settings.


#include "TestingInterface/PPButton.h"
#include "Components/BoxComponent.h"
#include "TestingInterface/PPGameplayInterface.h"

// Sets default values
APPButton::APPButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Trigger->SetupAttachment(GetRootComponent());
	
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);	

}

// Called when the game starts or when spawned
void APPButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPButton::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("Overlap!"));
	

	if (GameplayInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("Try Call Interface"));

		GameplayInterface->OpenDoor_Interface();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LinkedDoor is nulptr!"));
		return;
	}
			
}

void APPButton::SetInterface(IPPGameplayInterface* Interface)
{

	this->GameplayInterface = Interface;

}

