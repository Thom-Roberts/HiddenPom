// Fill out your copyright notice in the Description page of Project Settings.
#include "MyGameState.h"

AMyGameState::AMyGameState()
{
	m_spawnPosition.SetComponents(
		FQuat::MakeFromEuler(FVector(0)),
		FVector(-20, 0, 550),
		FVector(1)
	);
	m_gameOver = false;

	m_spawnParameters = FActorSpawnParameters();
	m_spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	m_falseArray.Init(false, COLUMN_COUNT);
	m_nullptrArray.Init(nullptr, COLUMN_COUNT);
}

void AMyGameState::BeginPlay()
{
	SpawnNewPom();
}

void AMyGameState::SpawnNewPom()
{
	const auto spawnedActor = GetWorld()->SpawnActor(PomClass, &m_spawnPosition, m_spawnParameters);
	if(IsValid(spawnedActor))
	{
		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		APlayerBase* castPawn = Cast<APlayerBase>(playerPawn);
		castPawn->SetCurrentPom(Cast<APomBase>(spawnedActor));
	}
	// Could not spawn
	else
	{
		Lose();
	}
}

void AMyGameState::SetPomColorPosition(int row, int column, PomColor color, APomBase* pom)
{
	while(m_pomColors.Num() <= row)
	{
		TArray<PomColor> temp;
		temp.Init(PomColor::None, COLUMN_COUNT);
		m_pomColors.Add(temp);
	}
	if(m_pomColors[row].Num() < COLUMN_COUNT)
		m_pomColors[row].Init(PomColor::None, COLUMN_COUNT);
	m_pomColors[row][column] = color;

	while(m_poms.Num() <= row)
	{
		m_poms.Add(m_nullptrArray);
	}
	if(m_poms[row].Num() < COLUMN_COUNT)
		m_poms[row].Init(nullptr, COLUMN_COUNT);
	m_poms[row][column] = pom;
}

void AMyGameState::Lose()
{
	m_gameOver = true;
#if UE_BUILD_DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::White, TEXT("Lose"));
	#endif
}

void AMyGameState::Restart()
{
	m_gameOver = false;
}

void AMyGameState::ClearPoms()
{
	// Initialize arrays to false
	ResetArray(m_shouldPositionBeCleared);
	
	TArray<ArrayIndex> matchedIndices;
	// See if poms should have themselves removed
	for(int i = 0; i < m_pomColors.Num(); ++i)
	{
		const auto currentRow = m_pomColors[i];
		for(int j = 0; j < currentRow.Num(); ++j)
		{
			if(m_pomColors[i][j] == PomColor::None)
				continue;
			matchedIndices.Empty();
			ResetArray(m_explored);
			CheckPom(i, j, m_pomColors[i][j], matchedIndices);
			if(matchedIndices.Num() >= COUNT_TO_CLEAR)
			{
				for (const auto& matchedIndex : matchedIndices)
				{
					m_shouldPositionBeCleared
						[matchedIndex.row]
						[matchedIndex.column] = true;
				}
			}
		}
	}

	// Remove all marked poms
	for(int i = 0; i < m_shouldPositionBeCleared.Num(); ++i)
	{
		const auto currentRow = m_shouldPositionBeCleared[i];
		for(int j = 0; j < currentRow.Num(); ++j)
		{
			if(currentRow[j])
			{
				GetWorld()->DestroyActor(m_poms[i][j]);
#if UE_BUILD_DEBUG
				auto message = TEXT("Deleting pom at position %d %d", i, j);
				GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::White, message);
#endif
				m_poms[i][j] = nullptr;
				m_pomColors[i][j] = PomColor::None;
			}
		}
	}
}

void AMyGameState::CheckPom(int row, int column, PomColor& colorToMatch, TArray<ArrayIndex>& matchedIndices)
{
	if(!TestIfArrayIsValid(m_explored, row, column))
		return;
	// We've already tested this spot
	if(m_explored[row][column])
		return;
	
	m_explored[row][column] = true;

	if(m_shouldPositionBeCleared[row][column])
		return;
	
	if(m_pomColors[row][column] == PomColor::None)
		return;
	
	// Test different directions
	const PomColor currentColor = m_pomColors[row][column];
	if(currentColor == colorToMatch)
		matchedIndices.Add({
			row, column
		});
	
	if(column > 0)
		CheckPom(row, column - 1, colorToMatch, matchedIndices);
	if(column < COLUMN_COUNT)
		CheckPom(row, column + 1, colorToMatch, matchedIndices);
	if(row > 0)
		CheckPom(row - 1, column, colorToMatch, matchedIndices);
	if(row < m_pomColors.Num())
		CheckPom(row + 1, column, colorToMatch, matchedIndices);
}

void AMyGameState::ResetArray(TArray<TArray<bool>>& arr)
{
	const int maxHeight = m_poms.Num();

	arr.Empty();
	while(arr.Num() < maxHeight)
		arr.Add(m_falseArray);
}

bool AMyGameState::TestIfArrayIsValid(TArray<TArray<bool>> arr, int32 row, int32 col)
{
	const int32 rowCount = arr.Num() - 1;

	const bool invalidRow = rowCount < row || row < 0;
	const bool invalidCol = col < 0 || col > COLUMN_COUNT - 1;

	return !invalidRow && !invalidCol;
}
