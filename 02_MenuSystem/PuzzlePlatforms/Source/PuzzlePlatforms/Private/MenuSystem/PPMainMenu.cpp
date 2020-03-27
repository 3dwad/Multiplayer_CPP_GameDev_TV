// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/PPMainMenu.h"
#include "Components/Button.h"
#include "PPGameInstance.h"

bool UPPMainMenu::Initialize()
{

	bool Succes = Super::Initialize();

	if (!Succes)
	{
		return false;
	}

	Host->OnClicked.AddDynamic(this, &UPPMainMenu::OnHostClicked);
	Join->OnClicked.AddDynamic(this, &UPPMainMenu::OnJoinClicked);

	return true;

}

void UPPMainMenu::OnHostClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Host called"))

	UPPGameInstance* PPGameInstance = Cast<UPPGameInstance>(GetGameInstance());
	PPGameInstance->Host();

}

void UPPMainMenu::OnJoinClicked()
{

	UPPGameInstance* PPGameInstance = Cast<UPPGameInstance>(GetGameInstance());
	PPGameInstance->Join("192.168.31.229");

}
