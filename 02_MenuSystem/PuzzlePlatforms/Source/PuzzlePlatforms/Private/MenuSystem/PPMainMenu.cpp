// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/PPMainMenu.h"
#include "Components/Button.h"
#include "PPGameInstance.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UPPMainMenu::Initialize()
{

	bool Succes = Super::Initialize();

	if (!Succes)
	{
		return false;
	}

	Host->OnClicked.AddDynamic(this, &UPPMainMenu::OnHostClicked);
	Join->OnClicked.AddDynamic(this, &UPPMainMenu::OnJoinClicked);
	Cancel->OnClicked.AddDynamic(this, &UPPMainMenu::OnCancelClicked);
	OK->OnClicked.AddDynamic(this, &UPPMainMenu::OnOkClicked);

	EnterBox->OnTextChanged.AddDynamic(this, &UPPMainMenu::OnBoxTextChanged);

	return true;

}

void UPPMainMenu::SetMenuInterface(IPPMenuSystemInterface* Interface)
{

	this->MenuInterface = Interface;
}

void UPPMainMenu::Setup()
{

	if (IsInViewport() == false)
	{
		AddToViewport();

		APlayerController* PlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
		if (PlayerController)
		{
			/* Create input data. Set input data settings*/

			FInputModeUIOnly MyInputData;
			MyInputData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);			

			/* Set input mode with input data*/
			PlayerController->SetInputMode(MyInputData);

			/* Show mouse cursor*/
			PlayerController->bShowMouseCursor = true;
		}


	}
}

void UPPMainMenu::Hide()
{
	
	APlayerController* PlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());

	if (PlayerController)
	{

		FInputModeGameOnly MyInputMode;
		PlayerController->SetInputMode(MyInputMode);

		PlayerController->bShowMouseCursor = false;

		RemoveFromParent();
	}

}

void UPPMainMenu::OnHostClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Host called"))

	//	Thaw we can call function from interface
	if (MenuInterface)
	{		
		MenuInterface->Host_Interface();		
	}
}

void UPPMainMenu::OnOkClicked()
{
	//	Thaw we can call function from interface
	if (MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("MenuInterface is valid"));
		MenuInterface->Join_Interface(IPAdress);
	}
}



void UPPMainMenu::SetMenuSwitcher(int32 Index)
{

	UE_LOG(LogTemp, Warning, TEXT("SetMenuSwitcher() is called"))

	MenuSwitcher->SetActiveWidgetIndex(Index);
}

void UPPMainMenu::OnBoxTextChanged(const FText& Text)
{
	IPAdress = Text.ToString();

}

void UPPMainMenu::OnCancelClicked()
{
	SetMenuSwitcher(0);
}

void UPPMainMenu::OnJoinClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Button clicked"))

	if (MenuSwitcher)
	{
		SetMenuSwitcher(1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MenuSwitcher is not valid!"))
	}
}


