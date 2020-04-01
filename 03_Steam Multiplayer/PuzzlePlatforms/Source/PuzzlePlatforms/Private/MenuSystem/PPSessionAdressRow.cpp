// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/PPSessionAdressRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PPGameInstance.h"


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

void UPPSessionAdressRow::Setup(UPPGameInstance* InGameInstance, FText InSessionName, uint32 InIndex)
{

	PPGameInstance = InGameInstance;
	SessionName->SetText(InSessionName);
	SessionIndex = InIndex;

}

void UPPSessionAdressRow::ButtonClicked()
{
	PPGameInstance->SetSelectedIndex(SessionIndex);	

}
