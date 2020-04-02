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

	FText FullSessionName = FText::FromString(InHostName + "--" + InSessionName);
	SessionName->SetText(FullSessionName);
	SessionIndex = InIndex;
	PPGameInstance = InGameInstance;

	int32 CurPl = InCurentPlayers;
	int32 MaxPl = InMaxPlayers;
	FString PlayerCountStr = FString::FromInt(CurPl) + "/" + FString::FromInt(MaxPl);
	
	PlayerCount->SetText(FText::FromString(PlayerCountStr));
}

void UPPSessionAdressRow::ButtonClicked()
{

	PPGameInstance->MainMenuWidgaet->ResetServerRowsSelected();
	isSelected = true;
	PPGameInstance->SetSelectedIndex(SessionIndex);

}
