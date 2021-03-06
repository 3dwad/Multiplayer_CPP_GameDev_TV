// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPMenuSystemInterface.generated.h"

class UGameInstance;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPMenuSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUZZLEPLATFORMS_API IPPMenuSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* That we can create pure virtual function. Its mean we can not create implementation. Implementation will be in inherited classes
	/* This is interface function*/
	virtual void Host_Interface(FString Name) = 0;

	virtual void OK_Interface() = 0;

	virtual void Join_Interface() = 0;

	virtual void ExitGame_Interface() = 0;


};
