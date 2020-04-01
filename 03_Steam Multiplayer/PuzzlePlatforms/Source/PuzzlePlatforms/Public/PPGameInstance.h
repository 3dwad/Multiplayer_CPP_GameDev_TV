// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/PPMenuSystemInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PPGameInstance.generated.h"

class UPPMainMenu;
class IOnlineSubsystem;
class UPPSessionAdressRow;



/**
 * 
 */
UCLASS()														/* That mean we add interface to this class */
class PUZZLEPLATFORMS_API UPPGameInstance : public UGameInstance , public IPPMenuSystemInterface
{
	GENERATED_BODY()

	
public:


	UPPGameInstance(const FObjectInitializer& ObjectInitializer);
	
	void Init() override;

	/* Exec do available to call from console*/
	UFUNCTION(Exec)
	void Host_Interface() override;

	UFUNCTION(Exec)
	void Join_Interface() override;

	UFUNCTION(Exec)
	void OK_Interface() override;

	UFUNCTION()
	void ExitGame_Interface() override;

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMenuWidget();
	
	void SessionCreated(FName SessionName, bool Succes);
	
	void SessionIsOver(FName SessionName, bool Succes);

	UFUNCTION()
	void CreateSession();
	
	void SessionFindComplete(bool Succes);

	UFUNCTION(Exec)
	void CreateSessionRowWidget(FText SessionName, uint32 Index);

	UFUNCTION()
	void SetSelectedIndex(uint32 InIndex);
	
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);




protected:

	//	Create pointer on class reference
	UPROPERTY()
	TSubclassOf<UPPMainMenu> MainMenuClass;

	UPROPERTY()
	TSubclassOf<UPPSessionAdressRow> SessionAdressClass;
	
	//	Pointer to OnlineSystem interface
	IOnlineSubsystem* PPOnlineSubsystem;

	//	SharedPointer to SessioInterface
	IOnlineSessionPtr SessionInterface;

	// Shared pointer to SearchSession
	TSharedPtr<FOnlineSessionSearch> SessionSerchPtr;

		

private:

	// Pointer to MainMenuWidget
	UPPMainMenu* MainMenuWidgaet;

	// Pointer to SessionRow widget
	UPPSessionAdressRow* SessionAdressWidget;

	uint32 SelectedIndex;
		

};
