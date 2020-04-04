// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/PPSessionAdressRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PPGameInstance.h"
#include "MenuSystem/PPMainMenu.h"


bool UPPSessionAdressRow::Initialize()
{
	bool Succes = Super::Initialize();

	if (Succes)
	{		
		SessionButton->OnClicked.AddDynamic(this, &UPPSessionAdressRow::ButtonClicked);

		return true;
	}
	else
	{
		return false;
	}	

}

void UPPSessionAdressRow::Setup(UPPGameInstance* InGameInstance, FString InSessionName, uint32 InIndex, FString InHostName, uint16 InMaxPlayers, uint16 InCurentPlayers)
{
	/* Fill struct*/
	Data.Name = InSessionName;
	Data.HostName = InHostName;	
	Data.CurrentPlayers = InCurentPlayers;
	Data.MaxPlayers = InMaxPlayers;

	FText HostNameText = FText::FromString(InHostName);
	HostName->SetText(HostNameText);
	SessionIndex = InIndex;
	PPGameInstance = InGameInstance;

	FText ServerNameText = FText::FromString(InSessionName);

	UE_LOG(LogTemp, Error, TEXT("Server name is: %s"),*InSessionName);

	ServerName->SetText(ServerNameText);
		
	FString PlayerCountStr = FString::Printf(TEXT("%d/%d"), InCurentPlayers, InMaxPlayers);
	
	PlayerCount->SetText(FText::FromString(PlayerCountStr));
}

void UPPSessionAdressRow::ButtonClicked()
{

	PPGameInstance->MainMenuWidgaet->ResetServerRowsSelected();
	isSelected = true;
	PPGameInstance->SetSelectedIndex(SessionIndex);

}
