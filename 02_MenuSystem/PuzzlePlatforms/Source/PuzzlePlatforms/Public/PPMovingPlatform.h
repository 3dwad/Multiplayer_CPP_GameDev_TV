// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PPMovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API APPMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
		

public:

	APPMovingPlatform();

	void Tick(float DeltaSeconds) override;

	void BeginPlay() override;


	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector TargetLocation;

		

protected:

	
	FVector GlobalStartLocation;
	FVector GlobalFinishLocation;

	int32 ActiveTriggers = 0;


public:

	UFUNCTION()
	void ChangeDirection();

	UFUNCTION()
	void AddActiveTrigger();

	UFUNCTION()
	void RemoveActiveTrigger();

};




