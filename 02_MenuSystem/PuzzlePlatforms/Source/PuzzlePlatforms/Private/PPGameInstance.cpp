// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "PPPlatformTrigger.h"


UPPGameInstance::UPPGameInstance()
{
	//	Get reference from BPClass
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (MainMenuBPClass.Class != NULL)
	{		
		//	Fill pointer
		MainMenuClass = MainMenuBPClass.Class;
		
	}
	
	
}

void UPPGameInstance::Init()
{


	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MainMenuClass->GetName());

}


void UPPGameInstance::LoadMenu()
{

	/* Create widget, add to viewport and fill pointer*/
	UUserWidget* MenuWidget = CreateWidget(this, MainMenuClass);

	if (MenuWidget->IsInViewport()==false)
	{
		MenuWidget->AddToViewport();

		APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
		if (PlayerController)
		{
			/* Create input data. Set input data settings*/

			FInputModeUIOnly MyInputData;			
			MyInputData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			
			/* Set input mode with input data*/
			PlayerController->SetInputMode(MyInputData);

			/* Show mouse cursor*/
			GetFirstLocalPlayerController(GetWorld())->bShowMouseCursor = true;
		
		}		

	}
}


void UPPGameInstance::Host()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Hosting!");

	GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/MainMap?listen");


}

void UPPGameInstance::Join(const FString& IPAdress)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Connect to: " + IPAdress + " was successful");

	APlayerController* L_Playercontroller = (GetFirstLocalPlayerController());

	if (L_Playercontroller)
	{
		L_Playercontroller->ClientTravel(IPAdress, ETravelType::TRAVEL_Absolute);

	}
	

}



