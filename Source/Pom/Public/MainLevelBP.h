// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PomBase.h"
#include "Engine/LevelScriptActor.h"
#include "MainLevelBP.generated.h"

/**
 * 
 */
UCLASS()
class POM_API AMainLevelBP : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<TEnumAsByte<PomColor>> generateRow();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 columnCount = 5;
};
