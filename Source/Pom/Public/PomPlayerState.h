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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandlePomLanded();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleNewRowSpawn();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Scoring")
	int32 craftPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scoring")
	int32 clearScoreIncrease;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Scoring")
	int32 m_currentClearScoreValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	TArray<int32> movesUntilNextRowSpawnList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	int32 movesIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spawning")
	int32 currentMoveCount = 0;
	
private:
	
};
