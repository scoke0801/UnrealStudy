// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "DTWidgetPath.generated.h"

USTRUCT(BlueprintType)
struct FBSWIdgetPath : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Path"))
	FString WidgetName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Path"))
	FSoftObjectPath Path;
};
 