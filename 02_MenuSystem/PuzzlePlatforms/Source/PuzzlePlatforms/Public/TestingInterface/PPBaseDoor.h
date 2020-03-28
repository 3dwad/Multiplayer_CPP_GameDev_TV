// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestingInterface/PPGameplayInterface.h"
#include "PPBaseDoor.generated.h"


class UStaticMeshComponent;
class APPButton;


UCLASS()
class PUZZLEPLATFORMS_API APPBaseDoor : public AActor, public IPPGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPBaseDoor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere)
	APPButton* LinkedButton;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void OpenDoor_Interface() override;


	UFUNCTION(BlueprintImplementableEvent)
	void OpenDoor();


};
