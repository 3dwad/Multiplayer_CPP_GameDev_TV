// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "PPMovingPlatform.h"

// Sets default values
APPPlatformTrigger::APPPlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(GetRootComponent()); 

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPPlatformTrigger::TriggerOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APPPlatformTrigger::TriggerOverlapEnd);

}

// Called when the game starts or when spawned
void APPPlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPPlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPPlatformTrigger::TriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	UE_LOG(LogTemp, Warning,TEXT("Activated"));
	
	/*	For each loop old method
	for (int index = 0; index < ControlledPlatforms.Num(); index++)
	{
		APPMovingPlatform* Element = ControlledPlatforms[index];
		Element->bCanMove = true;		
	}
	*/

	/* For each loop new method*/
	for (APPMovingPlatform* Element : ControlledPlatforms)
	{
		Element->AddActiveTrigger();
	}



}

void APPPlatformTrigger::TriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	UE_LOG(LogTemp, Warning, TEXT("Deactivated"));
	
	/* For each loop new method*/
	for (APPMovingPlatform* Element : ControlledPlatforms)
	{
		Element->RemoveActiveTrigger();
	}


}

