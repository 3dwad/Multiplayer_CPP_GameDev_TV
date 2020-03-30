// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/PPMainMenu.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PPPlatformTrigger.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"



const static FName SESSION_NAME = TEXT("MyAwesomeSession");


UPPGameInstance::UPPGameInstance()
{
	//	Get reference from BPClass
	ConstructorHelpers::FClassFinder<UPPMainMenu> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (MainMenuBPClass.Class != NULL)
	{
		//	Fill pointer
		MainMenuClass = MainMenuBPClass.Class;
	}


}

void UPPGameInstance::Init()
{
	//	Confirm MainMenuBPclass was found
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MainMenuClass->GetName());

	//	Get OnlineSubsystem
	PPOnlineSubsystem = IOnlineSubsystem::Get();
	if (PPOnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is valid! Found: %s"), *PPOnlineSubsystem->GetSubsystemName().ToString());

		//	Get SessionInterface
		SessionInterface = PPOnlineSubsystem->GetSessionInterface();

		/*	Bind to delegate OnCreateSession*/
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPPGameInstance::SessionCreated);
		/* Bind to delegate OnSessionEnd*/
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPPGameInstance::SessionIsOver);
		
		// Fill shared pointer to a new object
		SessionSerchPtr = MakeShareable(new FOnlineSessionSearch());
		

		//	Necessary check to valid 
		if (SessionSerchPtr)
		{
			//	Convert Share pointer to share reference
			TSharedRef<FOnlineSessionSearch> SessionSerchRef = SessionSerchPtr.ToSharedRef();
			SessionSerchPtr->bIsLanQuery = true;
			//SessionSerchPtr->QuerySettings.Set()
			
			SessionInterface->FindSessions(0, SessionSerchRef);

			UE_LOG(LogTemp, Warning, TEXT("Start find session"));

			/* Bind to delegate OnFindSessionComplete*/
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPPGameInstance::SessionFindComplete);
			
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TSharePtr is null"));
		}

	}
}

void UPPGameInstance::LoadMenuWidget()
{
	if (MainMenuClass)
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
}


/* Connect to session after creation completed. Via delegate*/
void UPPGameInstance::SessionCreated(FName SessionName, bool Succes)
{

	if (Succes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session %s was create succeful"), *SessionName.ToString())

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Hosting!");

		GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/MainMap?listen");

		MainMenu->Hide();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Session not created"));

	}

}

void UPPGameInstance::SessionIsOver(FName SessionName, bool Succes)
{
	if (Succes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session was over"));
		CreateSession();
	}
}

void UPPGameInstance::CreateSession()
{
		
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	

	if (SessionInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is valid!"));
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}

}

void UPPGameInstance::SessionFindComplete(bool Succes)
{
	if (Succes && SessionSerchPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session find succeful completed"));

		/* Get Array of founded sessions*/
		TArray<FOnlineSessionSearchResult> SearchResult;
		SearchResult = SessionSerchPtr->SearchResults;

		/* For each loop print Sessions ID*/
		for (FOnlineSessionSearchResult & Index : SearchResult)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *Index.GetSessionIdStr());
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Session find failed"));
	}


}

/* Find session per name and destroy if founded. Create Session*/
void UPPGameInstance::Host_Interface()
{

	if (SessionInterface->GetNamedSession(SESSION_NAME))
	{

		UE_LOG(LogTemp, Warning, TEXT("Try End Session"));
		SessionInterface->DestroySession(SESSION_NAME);

		return;
	}

	else
	{
		CreateSession();
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

void UPPGameInstance::ExitGame_Interface()
{


	UKismetSystemLibrary::QuitGame(this, GetFirstLocalPlayerController(), EQuitPreference::Quit, true);

}


