// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPButton.generated.h"

class UBoxComponent;
class IPPGameplayInterface;
class APPBaseDoor;

UCLASS()
class PUZZLEPLATFORMS_API APPButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPButton();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Trigger;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void SetInterface(IPPGameplayInterface* Interface);

private:

	IPPGameplayInterface* GameplayInterface;

};
