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
	
	
	UFUNCTION(BlueprintCallable)
	void SpawnNewPom();

	UFUNCTION(BlueprintCallable)
	void SetPomColorPosition(int row, int column, PomColor color, APomBase* pom);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Lose();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Restart();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnRow();
	
	UFUNCTION(BlueprintCallable)
	void ClearPoms();
	void CheckPom(int row, int column, PomColor& colorToMatch, TArray<ArrayIndex>& matchedIndices);

public:
	UPROPERTY(EditDefaultsOnly, Category="Paramters")
	TSubclassOf<AActor> PomClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform m_spawnPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 COUNT_TO_CLEAR = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 COLUMN_COUNT = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ROWS_TO_SPAWN = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_gameOver;
	
private:
	void ResetArray(TArray<TArray<bool>>& arr);
	bool TestIfArrayIsValid(TArray<TArray<bool>> arr, int32 row, int32 col);
	
private:
	
	TArray<TArray<APomBase*>> m_poms;
	TArray<TArray<PomColor>> m_pomColors;
	TArray<TArray<bool>> m_shouldPositionBeCleared;
	TArray<TArray<bool>> m_explored;
	
	TArray<bool> m_falseArray;
	TArray<APomBase*> m_nullptrArray;

	FActorSpawnParameters m_spawnParameters;
};
