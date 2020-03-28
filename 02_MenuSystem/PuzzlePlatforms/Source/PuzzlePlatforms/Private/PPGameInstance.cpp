// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/PPMainMenu.h"

#include "PPPlatformTrigger.h"


UPPGameInstance::UPPGameInstance()
{
	//	Get reference from BPClass
	ConstructorHelpers::FClassFinder<UPPMainMenu> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (MainMenuBPClass.Class != NULL)
	{		
		//	Fill pointer
		MainMenuClass = MainMenuBPClass.Class;
		
	}	
	else
	{

	}

}

void UPPGameInstance::Init()
{


	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MainMenuClass->GetName());

}


void UPPGameInstance::LoadMenu()
{

	/* Create widget, add to viewport and fill pointer*/
	 MainMenu = CreateWidget<UPPMainMenu>(this, MainMenuClass);

	if (MainMenu)
	{		
		/* Call setup*/
		MainMenu->Setup();

		/* Sen self in widget to get access to function in interface*/
		MainMenu->SetMenuInterface(this);
	}
	
}


void UPPGameInstance::Host_Interface()
{
	
	if (MainMenu)
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Hosting!");

		GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/MainMap?listen");

		MainMenu->Hide();

	}	
}

void UPPGameInstance::Join_Interface(const FString& Adress)
{

	UE_LOG(LogTemp, Warning, TEXT("Enter string is: %s"), *Adress);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Connect to: " + Adress + " was successful");

	APlayerController* L_Playercontroller = (GetFirstLocalPlayerController());

	if (L_Playercontroller)
	{
		L_Playercontroller->ClientTravel(Adress, ETravelType::TRAVEL_Absolute);

		MainMenu->Hide();
	}
	

}



