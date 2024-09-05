// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CWBSTestData.generated.h"


/** Please add a struct description */
USTRUCT(BlueprintType)
struct FBSTestData
{
	GENERATED_BODY()
	
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "BoolVar", MakeStructureDefaultValue = "False"))
	bool BoolVar;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IntVar", MakeStructureDefaultValue = "0"))
	int32 IntVar;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IntVarList"))
	TArray<int32> IntVarList;
};
