// Fill out your copyright notice in the Description page of Project Settings.


#include "PPMovingPlatform.h"
#include "DrawDebugHelpers.h"
#include "Kismet//KismetMathLibrary.h"


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
				
	}


}



void APPMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	 if (TargetLocation != FVector(0.f) && HasAuthority())
	 {
		 //	Find direction between two vectors. Necessary normalize it
		 FVector DirectionToMove = (TargetLocation - DirectionToMove).GetSafeNormal();		
		 AddActorLocalOffset(DirectionToMove * Speed * DeltaSeconds, true);

		 /* We can convert vector in local coordinate system into global coordinate system. Example below
		 FVector GlobalDirection = GetTransform().TransformPosition(DirectionToMove);
		 */
		 		 
		 // UE_LOG(LogTemp, Warning, TEXT("Mooooving!"));
	 }

		 //DrawDebugString(GetWorld(), GetActorLocation(), "Not Replicated",nullptr,FColor::White,0.f);
	  

}


