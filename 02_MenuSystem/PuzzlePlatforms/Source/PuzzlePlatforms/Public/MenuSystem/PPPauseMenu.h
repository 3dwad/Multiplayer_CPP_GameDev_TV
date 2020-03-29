// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPPauseMenu.generated.h"


class UButton;


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPPPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton;


	bool Initialize() override;

	UFUNCTION()
	void Setup();

	UFUNCTION()
	void Hide();

	UFUNCTION()
	void ToMainMenu();
};
