// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPPGameInstance : public UGameInstance
{
	GENERATED_BODY()

	
public:


	UPPGameInstance();
	

	
	void Init() override;

	/* Exec do available to call from console*/
	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& IPAdress);

};
