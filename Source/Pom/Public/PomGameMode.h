// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PomBase.h"
#include "PomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class POM_API APomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APomGameMode();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float baseMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currentMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveSpeedIncrease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float heldDownMoveSpeed;
};
