// Fill out your copyright notice in the Description page of Project Settings.


#include "PPGameInstance.h"
#include "Engine/Engine.h"


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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Fak eah!");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Second message!");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Third message!");

}

void UPPGameInstance::Join(const FString& IPAdress)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Connect to: " + IPAdress + " was successful");

	
	

}

