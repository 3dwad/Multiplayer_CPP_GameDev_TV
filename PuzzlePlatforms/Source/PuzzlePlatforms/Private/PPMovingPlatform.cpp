// Fill out your copyright notice in the Description page of Project Settings.


#include "PPMovingPlatform.h"
#include "DrawDebugHelpers.h"
#include "Kismet//KismetMathLibrary.h"
#include "Engine/Engine.h"


APPMovingPlatform::APPMovingPlatform()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SetMobility(EComponentMobility::Movable);


}


void APPMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	//	Enable replication and move replication. Call only on server
	if (HasAuthority())
	{
		
		SetReplicates(true);
		SetReplicateMovement(true);

		GlobalStartLocation = GetActorLocation();
		GlobalFinishLocation = GetTransform().TransformPosition(TargetLocation);
	}

}






void APPMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		float DistanceToFinish = (GlobalFinishLocation - GetActorLocation()).Size();

		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, "Distance To Finish:" + FString::SanitizeFloat(DistanceToFinish));

		if (DistanceToFinish < 10.f)
		{
			ChangeDirection();

			UE_LOG(LogTemp, Warning, TEXT("Change Direction Called"));
		}

	}	


	 if (TargetLocation != FVector(0.f) && HasAuthority())
	 {		 
		 //	Find direction between two vectors. Necessary normalize it
		 FVector DirectionToMove = (GlobalFinishLocation - GlobalStartLocation).GetSafeNormal();		
		 AddActorWorldOffset(DirectionToMove * Speed * DeltaSeconds, false);  		 
		 		
	 }  
}

//	Swap Start and Finish vectors
void APPMovingPlatform::ChangeDirection()
{
	
	FVector Temp;

	Temp = GlobalStartLocation;
	GlobalStartLocation = GlobalFinishLocation;
	GlobalFinishLocation = Temp;	

}

