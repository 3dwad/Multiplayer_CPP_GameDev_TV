// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatforms/PuzzlePlatformsGameMode.h"
#include "PPLobbyGameMode.generated.h"

class UGameMapsSettings;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API APPLobbyGameMode : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()
	

public:


	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

	void TravelFromLobby();

	int16 PlayerCount = 0;

	FTimerHandle TimerToStartGame;

	
};
