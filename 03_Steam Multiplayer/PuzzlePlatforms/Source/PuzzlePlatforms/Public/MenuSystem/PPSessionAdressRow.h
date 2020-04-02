// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPSessionAdressRow.generated.h"

class UButton;
class UTextBlock;
class UPPGameInstance;

/**
 *
 */

/* Create new structure with ServerData*/
USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	FString HostName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	
};


UCLASS()
class PUZZLEPLATFORMS_API UPPSessionAdressRow : public UUserWidget
{
	GENERATED_BODY()

public:

	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
		UButton* SessionButton;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* HostName;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayerCount;


	uint32 SessionIndex;

	UPROPERTY(BlueprintReadOnly)
		bool isSelected = false;

	FServerData Data;

	UPPGameInstance* PPGameInstance;

	UFUNCTION()
		void Setup(UPPGameInstance* InGameInstance, FString InSessionName, uint32 InIndex, FString InHostName, uint16 InMaxPlayers, uint16 InCurentPlayers);

	UFUNCTION()
		void ButtonClicked();

};
