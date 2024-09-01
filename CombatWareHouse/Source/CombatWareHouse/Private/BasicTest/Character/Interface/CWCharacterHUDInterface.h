// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CWCharacterHUDInterface.generated.h"

class UCWUIHUDIngame;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCWCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ICWCharacterHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetupHUDWIdget(UCWUIHUDIngame* InHUDWidget) = 0;
};
