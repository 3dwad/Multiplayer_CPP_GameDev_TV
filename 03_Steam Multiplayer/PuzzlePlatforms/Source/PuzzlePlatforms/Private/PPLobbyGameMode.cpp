// Fill out your copyright notice in the Description page of Project Settings.


#include "PPLobbyGameMode.h"
#include "GameMapsSettings.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "PPGameInstance.h"


void APPLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);

			
		PlayerCount++;

		UE_LOG(LogTemp, Warning, TEXT("New player is connect. Total player is: %d"), PlayerCount);

		/* Start timer to travel when players more then 3*/
		if (PlayerCount==3)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ready to start!"));						
			
			GetWorldTimerManager().SetTimer(TimerToStartGame,this, &APPLobbyGameMode::TravelFromLobby,5.f);	
			
		}
}

void APPLobbyGameMode::Logout(AController* Exiting)
{

	Super::Logout(Exiting);

	PlayerCount--;

	/* If Players less the 3, clear timer*/
	if (PlayerCount<3 && TimerToStartGame.IsValid())
	{

		GetWorldTimerManager().ClearTimer(TimerToStartGame);

	}


	UE_LOG(LogTemp, Warning, TEXT("Player is leave. Total player is: %d"), PlayerCount);

}

void APPLobbyGameMode::TravelFromLobby()
{
	/* Enable seamless travel*/
	bUseSeamlessTravel = true;
	GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/MainMap?listen");

	UPPGameInstance* GameInstance = Cast<UPPGameInstance>(GetGameInstance());

	/* Mark session as started. Exclude from searching result*/
	if (GameInstance)
	{
		GameInstance->StartSession();
	}
}