// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/PPMenuSystemInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "PPGameInstance.generated.h"

class UPPMainMenu;
class IOnlineSubsystem;



/**
 * 
 */
UCLASS()														/* That mean we add interface to this class */
class PUZZLEPLATFORMS_API UPPGameInstance : public UGameInstance , public IPPMenuSystemInterface
{
	GENERATED_BODY()

	
public:


	UPPGameInstance();	
	
	void Init() override;

	/* Exec do available to call from console*/
	UFUNCTION(Exec)
	void Host_Interface() override;

	UFUNCTION(Exec)
	void Join_Interface(const FString& Adress) override;

	UFUNCTION()
	void ExitGame_Interface() override;

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION()
	void SessionCreated(FName SessionName, bool Succes);

	UFUNCTION()
	void SessionIsOver(FName SessionName, bool Succes);

	UFUNCTION()
	void CreateSession();

	UFUNCTION()
	void SessionFindComplete(bool Succes);


protected:

	//	Create pointer on class reference
	UPROPERTY()
	TSubclassOf<UPPMainMenu> MainMenuClass;
	
	//	Pointer to OnlineSystem interface
	IOnlineSubsystem* PPOnlineSubsystem;

	//	SharedPointer to SessioInterface
	IOnlineSessionPtr SessionInterface;

	// New shared pointer
	TSharedPtr<FOnlineSessionSearch> SessionSerchPtr;

		

private:

	UPPMainMenu* MainMenu;

};