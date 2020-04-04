// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/PPMainMenu.h"
#include "Components/Button.h"
#include "PPGameInstance.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "Components/PanelWidget.h"
#include "MenuSystem/PPSessionAdressRow.h"

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
	QuitButton->OnClicked.AddDynamic(this, &UPPMainMenu::OnQuitClicked);
	HostToCreate->OnClicked.AddDynamic(this, &UPPMainMenu::HostToCreateClicked);
	HostToMain->OnClicked.AddDynamic(this, &UPPMainMenu::HostToMainClicked);


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


void UPPMainMenu::AddChildToScrollBox(UWidget* NewChild)
{

	ScrollBoxWithSessions->AddChild(NewChild);

}

void UPPMainMenu::ClearScrollBoxChildrens()
{

	ScrollBoxWithSessions->ClearChildren();
}



void UPPMainMenu::OnHostClicked()
{
	SetMenuSwitcher(2);
}

void UPPMainMenu::HostToCreateClicked()
{

	UE_LOG(LogTemp, Warning, TEXT("Host called"))

		//	Thaw we can call function from interface
		if (MenuInterface)
		{
			FString Name = ServerName->GetText().ToString();
			if (Name.IsEmpty())
			{
				ServerName->SetHintText(FText::FromString("Enter name!"));
			}
			else
			{
				MenuInterface->Host_Interface(Name);
			}			
		}




}

void UPPMainMenu::HostToMainClicked()
{
	SetMenuSwitcher(0);
}

void UPPMainMenu::OnOkClicked()
{
	//	Thaw we can call function from interface
	if (MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("MenuInterface is valid"));
		MenuInterface->OK_Interface();
	}
}



void UPPMainMenu::OnQuitClicked()
{
	//	Thaw we can call function from interface
	if (MenuInterface)
	{
		MenuInterface->ExitGame_Interface();
	}
}

void UPPMainMenu::SetMenuSwitcher(int32 Index)
{

	UE_LOG(LogTemp, Warning, TEXT("SetMenuSwitcher() is called"))

		MenuSwitcher->SetActiveWidgetIndex(Index);
}



void UPPMainMenu::ResetServerRowsSelected()
{
	TArray<UWidget*> AllRows = ScrollBoxWithSessions->GetAllChildren();

	for (UWidget* CurrentRow : AllRows)
	{

		UPPSessionAdressRow* RowAfterCast = Cast<UPPSessionAdressRow>(CurrentRow);
		if (RowAfterCast)
		{
			RowAfterCast->isSelected = false;
		}
	}
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

			MenuInterface->Join_Interface();

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MenuSwitcher is not valid!"))
		}
}


