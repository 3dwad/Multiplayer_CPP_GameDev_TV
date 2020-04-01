// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPSessionAdressRow.generated.h"

class UButton;
class UTextBlock;
class UPPGameInstance;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPPSessionAdressRow : public UUserWidget
{
	GENERATED_BODY()

public:

	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UButton* SessionButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionName;

	uint32 SessionIndex;

	UPROPERTY()
	UPPGameInstance* PPGameInstance;

	UFUNCTION()
	void Setup(UPPGameInstance* InGameInstance, FText InSessionName, uint32 InIndex);

	UFUNCTION()
	void ButtonClicked();
	
};
