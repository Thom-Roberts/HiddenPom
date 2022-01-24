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
	APomBase* SpawnNewPomInPreviewPosition();

	UFUNCTION(BlueprintCallable)
	void SetActivePom(UPARAM(DisplayName="Success") bool& success, APomBase* newPom = nullptr, UPARAM(DisplayName="Player Index")int32 playerIndex =0);
	
	UFUNCTION(BlueprintCallable)
	void SetPomColorPosition(int row, int column, PomColor color, APomBase* pom);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Lose();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Restart();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<APomBase*> SpawnPreviewRow();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnInitialRows();

	UFUNCTION(BlueprintCallable)
	void MovePreviewRowUp();
	
	UFUNCTION(BlueprintCallable)
	// Returning like this let's me change what the return looks like in BluePrints
	void ClearPoms(UPARAM(DisplayName="Groups Cleared") int32& countCleared);
	void CheckPom(int row, int column, PomColor& colorToMatch, TArray<ArrayIndex>& matchedIndices);
	
	UFUNCTION(BlueprintCallable)
	void MovePomsDown();

	UFUNCTION(BlueprintCallable)
	void IncreaseScoreBonus();
	
	UFUNCTION(BlueprintCallable)
	void ResetScoreBonus();

public:
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	TSubclassOf<AActor> PomClass;
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	TSubclassOf<AActor> SinglePomClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform m_previewPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform m_activePosition;
	UPROPERTY(EditAnywhere)
	float m_rowPreviewZPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 COUNT_TO_CLEAR = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 COLUMN_COUNT = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ROWS_TO_SPAWN = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_gameOver;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APomBase* nextPom = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<APomBase*> m_previewRowPoms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dropping")
	float m_currentMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dropping")
	float m_maxMoveSpeed;
	
	// The value that is added to the 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 clearScoreIncrease = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 m_currentClearScoreValue;
	
private:
	void ResetArray(TArray<TArray<bool>>& arr);
	bool TestIfArrayIsValid(TArray<TArray<bool>> arr, int32 row, int32 col);
	FVector CreatePomPositionVector(int32 row, int32 column);
	void ClearPomFromPosition(int32 row, int32 column);
	bool WouldPomsBeCleared();
	// Utility function for the GenerateInitialRow function
	PomColor GenerateRandomColor(PomColor colorToExclude = PomColor::None);
	
private:
	TArray<TArray<APomBase*>> m_poms;
	TArray<TArray<PomColor>> m_pomColors;
	TArray<TArray<bool>> m_shouldPositionBeCleared;
	TArray<TArray<bool>> m_explored;

	// Used for populating initial setting of arrays
	TArray<bool> m_falseArray;
	TArray<APomBase*> m_nullptrArray;
    uint32 m_initialClearScoreValue = 10;
	
	FActorSpawnParameters m_spawnParameters;
};
