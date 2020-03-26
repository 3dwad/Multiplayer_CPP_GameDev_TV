// Fill out your copyright notice in the Description page of Project Settings.


#include "PPGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"


UPPGameInstance::UPPGameInstance()
{

	UE_LOG(LogTemp,Warning,TEXT("This Log from Constructor!"))

}

void UPPGameInstance::Init()
{

	UE_LOG(LogTemp, Warning, TEXT("This Log from Init()!"))

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

