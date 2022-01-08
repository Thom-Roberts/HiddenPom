// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PomPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class POM_API APomPlayerState : public APlayerState
{
	GENERATED_BODY()

	APomPlayerState();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 craftPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float baseMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveSpeedIncrease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float heldDownMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 movesUntilNextRowSpawn;
private:
	
};
