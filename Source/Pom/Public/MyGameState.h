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

public:
	UPROPERTY(EditDefaultsOnly, Category="Paramters")
	TSubclassOf<AActor> PomClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP")
	FTransform m_spawnPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP")
	int32 COUNT_TO_CLEAR = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP")
	int32 COLUMN_COUNT = 5;
	
private:
	void ResetArray(TArray<TArray<bool>>& arr);
	bool TestIfArrayIsValid(TArray<TArray<bool>> arr, int32 row, int32 col);
	
private:
	bool m_gameOver;
	TArray<TArray<APomBase*>> m_poms;
	TArray<TArray<PomColors>> m_pomColors;
	TArray<TArray<bool>> m_shouldPositionBeCleared;
	TArray<TArray<bool>> m_explored;
	
	TArray<bool> m_falseArray;
	
	FActorSpawnParameters m_spawnParameters;
};
