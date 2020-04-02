// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/PPMainMenu.h"
#include "MenuSystem/PPSessionAdressRow.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PPPlatformTrigger.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"



const static FName SESSION_NAME = TEXT("MyAwesomeSession");



UPPGameInstance::UPPGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//	Get reference from BPClass
	ConstructorHelpers::FClassFinder<UPPMainMenu> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (MainMenuBPClass.Class != NULL)
	{
		//	Fill pointer
		MainMenuClass = MainMenuBPClass.Class;
	}


	//	Get reference from BPClass
	ConstructorHelpers::FClassFinder<UPPSessionAdressRow> SessionAdressBPClass(TEXT("/Game/MenuSystem/WBP_SessionAdressRow"));
	if (SessionAdressBPClass.Class != NULL)
	{
		//	Fill pointer
		SessionAdressClass = SessionAdressBPClass.Class;
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
		/* Bind to delegate OnFindSessionComplete*/
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPPGameInstance::SessionFindComplete);	
		/* Bind to delegate OnJoinSessionComplete*/
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPPGameInstance::OnJoinSessionComplete);		
	}
}

void UPPGameInstance::LoadMenuWidget()
{
	if (MainMenuClass)
	{
		/* Create widget, add to viewport and fill pointer*/
		MainMenuWidgaet = CreateWidget<UPPMainMenu>(this, MainMenuClass);
		if (MainMenuWidgaet)
		{
			/* Call setup*/
			MainMenuWidgaet->Setup();

			/* Send self in widget to get access to function in interface*/
			MainMenuWidgaet->SetMenuInterface(this);
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

		MainMenuWidgaet->Hide();
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
	/* Place to setup settings for Session*/
	FOnlineSessionSettings SessionSettings;
	SessionSettings.NumPublicConnections = MaxPlayersInSession;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;

	/* If steam enabled run internet session with lobby. Otherwise run LAN session*/
	if (PPOnlineSubsystem->GetSubsystemName()=="STEAM")
	{
		SessionSettings.bIsLANMatch = false;
	}
	else
	{
		SessionSettings.bIsLANMatch = true;
	}
	
	if (SessionInterface)
	{		
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
		for (int32 Index=0; Index<SearchResult.Num(); Index++)
		{
			FOnlineSessionSearchResult CurrentElement = SearchResult[Index];

			uint32 I = Index;
			UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *CurrentElement.GetSessionIdStr());

			/*Get info for ServerData struct*/
			FString SessionName = SESSION_NAME.ToString();
			FString HostName = CurrentElement.Session.OwningUserName;
			uint16 MaxPlayers = MaxPlayersInSession;
			uint16 CurrentPlayers = MaxPlayersInSession - CurrentElement.Session.NumOpenPublicConnections;	

			CreateSessionRowWidget(SessionName,I,HostName,MaxPlayers,CurrentPlayers);			
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Session find failed"));
	}
}

void UPPGameInstance::CreateSessionRowWidget(FString InSessionName, uint32 InIndex, FString InHostName, uint16 InMaxPlayers, uint16 InCurentPlayers)
{
	if (SessionAdressClass)
	{

		SessionAdressWidget = CreateWidget<UPPSessionAdressRow>(this, SessionAdressClass);

		SessionAdressWidget->Setup(this, InSessionName, InIndex, InHostName,InMaxPlayers,InCurentPlayers);		
		MainMenuWidgaet->AddChildToScrollBox(SessionAdressWidget);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionAdressClass not found"))
	}
	

}

void UPPGameInstance::SetSelectedIndex(uint32 InIndex)
{

	SelectedIndex = InIndex;

	int32 Index = SelectedIndex;

	UE_LOG(LogTemp, Warning, TEXT("Selected index is %s"), *FString::FromInt(Index));
}

void UPPGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{	

	FString ConnectionInfo;

	bool isStringSucces = SessionInterface->GetResolvedConnectString(SESSION_NAME, ConnectionInfo);

	if (isStringSucces)
	{

		APlayerController* L_Playercontroller = (GetFirstLocalPlayerController());

		if (L_Playercontroller)
		{
			L_Playercontroller->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);

			MainMenuWidgaet->Hide();
		}

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

void UPPGameInstance::Join_Interface()
{
	// Fill shared pointer to a new object
		
	SessionSerchPtr = MakeShareable(new FOnlineSessionSearch());
	
	/* Debug
	int32 SerchCount = SessionSerchPtr.GetSharedReferenceCount();

	UE_LOG(LogTemp, Warning, TEXT("searching sessions is: %s"), *FString::FromInt(SerchCount));
	*/

	//	Necessary check to valid 
	if (SessionSerchPtr)
	{
		SessionSerchPtr->MaxSearchResults = 100;			
		SessionSerchPtr->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);


		//	Convert Share pointer to share reference
		TSharedRef<FOnlineSessionSearch> SessionSerchRef = SessionSerchPtr.ToSharedRef();		
		
		SessionInterface->FindSessions(0, SessionSerchRef);

		/* Clear children and start find active sessions*/
		MainMenuWidgaet->ClearScrollBoxChildrens();

		UE_LOG(LogTemp, Warning, TEXT("Start find session"));
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TSharePtr is null"));
	}
}

void UPPGameInstance::OK_Interface()
{
	

	if (SessionInterface && SessionSerchPtr.IsValid())
	{

		int32 Index = SelectedIndex;

		UE_LOG(LogTemp, Warning, TEXT("Try Connect to %s"), *FString::FromInt(Index));

		SessionInterface->JoinSession(0, SESSION_NAME, SessionSerchPtr->SearchResults[SelectedIndex]);

	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("SessionSearchPtr is null"));
	}
	
}

void UPPGameInstance::ExitGame_Interface()
{


	UKismetSystemLibrary::QuitGame(this, GetFirstLocalPlayerController(), EQuitPreference::Quit, true);

}


