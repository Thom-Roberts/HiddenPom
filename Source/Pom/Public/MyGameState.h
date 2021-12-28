// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PomBase.h"
#include "PlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameState.generated.h"

struct ArrayIndex
{
	int row, column;
};

/**
 * 
 */
UCLASS()
class POM_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMyGameState();
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void SpawnNewPom();

	UFUNCTION(BlueprintCallable)
	void SetPomColorPosition(int row, int column, PomColors color, APomBase* pom);

	UFUNCTION(BlueprintCallable)
	void Lose();

	UFUNCTION(BlueprintCallable)
	void Restart();
	
	UFUNCTION(BlueprintCallable)
	void ClearPoms();
	void CheckPom(int row, int column, PomColors& colorToMatch, TArray<ArrayIndex>& matchedIndices);

private:
	void ResetArrays();

private:
	FTransform m_spawnPosition;
	bool m_gameOver;
	TArray<TArray<APomBase*>> m_poms;
	TArray<TArray<PomColors>> m_pomColors;
	TArray<TArray<bool>> m_shouldPositionBeCleared;
	TArray<TArray<bool>> m_explored;
	const int COLUMN_COUNT = 5;
	const int COUNT_TO_CLEAR = 3;
	
	FActorSpawnParameters m_spawnParameters;
};
