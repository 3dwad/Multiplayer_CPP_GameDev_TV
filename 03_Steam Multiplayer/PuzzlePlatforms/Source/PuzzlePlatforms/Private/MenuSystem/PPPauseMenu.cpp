// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/PPPauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"






bool UPPPauseMenu::Initialize()
{

	bool Succes = Super::Initialize();

	if (!Succes)
	{
		return false;
	}

	else
	{
		CancelButton->OnClicked.AddDynamic(this, &UPPPauseMenu::Hide);
		MainMenuButton->OnClicked.AddDynamic(this, &UPPPauseMenu::ToMainMenu);

	}

	return true;

}

void UPPPauseMenu::Setup()
{
	AddToViewport();

	APlayerController* PlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
	if (PlayerController)
	{
		/* Create input data. Set input data settings*/

		FInputModeGameAndUI MyInputData;
		MyInputData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		MyInputData.SetWidgetToFocus(TakeWidget());

		/* Set input mode with input data*/
		PlayerController->SetIgnoreMoveInput(true);
		
		PlayerController->SetInputMode(MyInputData);

		/* Show mouse cursor*/
		PlayerController->bShowMouseCursor = true;
	}

}

void UPPPauseMenu::Hide()
{

	APlayerController* PlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());

	if (PlayerController)
	{

		FInputModeGameOnly MyInputMode;
		PlayerController->SetInputMode(MyInputMode);
		PlayerController->SetIgnoreMoveInput(false);
		

		PlayerController->bShowMouseCursor = false;

		RemoveFromParent();
	}
}

void UPPPauseMenu::ToMainMenu()
{

	Hide();
	APlayerController* PlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());

	if (PlayerController)
	{

		PlayerController->ClientTravel("MainMenu", ETravelType::TRAVEL_Absolute);
		
	}
	//	UGameplayStatics::OpenLevel(this, "MainMenu");

}
