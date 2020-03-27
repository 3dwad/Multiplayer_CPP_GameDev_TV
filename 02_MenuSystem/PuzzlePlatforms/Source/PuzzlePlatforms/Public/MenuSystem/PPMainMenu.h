// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPMainMenu.generated.h"


class UButton;


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPPMainMenu : public UUserWidget
{
	GENERATED_BODY()
	

public:


	/* Bind to widget elements. !IMPORTANT! Name this pointer must be same as name into Widget Blueprint*/
	UPROPERTY(meta = (BindWidget))
	UButton* Host;

	UPROPERTY(meta = (BindWidget))
	UButton* Join;

	bool Initialize() override;
	

	

protected:

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();



private:

};
