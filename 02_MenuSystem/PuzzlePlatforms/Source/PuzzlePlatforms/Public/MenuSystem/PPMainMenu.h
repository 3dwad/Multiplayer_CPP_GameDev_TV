// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/PPMenuSystemInterface.h"
#include "PPMainMenu.generated.h"


class UButton;
class UWidgetSwitcher;
class UEditableTextBox;


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

	UPROPERTY(meta = (BindWidget))
	UButton* Cancel;

	UPROPERTY(meta = (BindWidget))
	UButton* OK;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* EnterBox;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;



	bool Initialize() override;
	

	void SetMenuInterface(IPPMenuSystemInterface* Interface);

	void Setup();

	void Hide();

	

protected:

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void OnCancelClicked();

	UFUNCTION()
	void OnOkClicked();

	UFUNCTION()
	void SetMenuSwitcher(int32 Index);

	UFUNCTION()
	void OnBoxTextChanged(const FText& Text);


private:

	FString IPAdress;

	IPPMenuSystemInterface* MenuInterface;


};
