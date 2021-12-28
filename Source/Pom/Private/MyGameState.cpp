// Fill out your copyright notice in the Description page of Project Settings.
#include "MyGameState.h"



AMyGameState::AMyGameState()
{
	m_spawnPosition.SetComponents(
		FVector().ToOrientationQuat(),
		FVector(-20, 0, 550),
		FVector(1)
	);
	m_gameOver = false;

	m_spawnParameters = FActorSpawnParameters();
	m_spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
}

void AMyGameState::BeginPlay()
{
	SpawnNewPom();
}

void AMyGameState::SpawnNewPom()
{
	const auto spawnedActor = GetWorld()->SpawnActor(APomBase::StaticClass(), &m_spawnPosition, m_spawnParameters);
	if(IsValid(spawnedActor))
	{
		const auto playerPawn = Cast<APlayerBase>(
			UGameplayStatics::GetPlayerPawn(GetWorld(), 0)
			);
		playerPawn->SetCurrentPom(Cast<APomBase>(spawnedActor));
	}
	// Could not spawn
	else
	{
		Lose();
	}
}

void AMyGameState::SetPomColorPosition(int row, int column, PomColors color, APomBase* pom)
{
	while(m_pomColors.Num() <= row)
	{
		TArray<PomColors> temp;
		temp.Init(PomColors::None, COLUMN_COUNT);
		m_pomColors.Add(temp);
	}
	if(m_pomColors[row].Num() < COLUMN_COUNT)
		m_pomColors[row].Init(PomColors::None, COLUMN_COUNT);
	m_pomColors[row][column] = color;

	while(m_poms.Num() <= row)
	{
		TArray<APomBase*> temp;
		temp.Init(nullptr, COLUMN_COUNT);
		m_poms.Add(temp);
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
	ResetArrays();
	TArray<ArrayIndex> matchedIndices;
	// See if poms should have themselves removed
	for(int i = 0; i < m_pomColors.Num(); ++i)
	{
		const auto currentRow = m_pomColors[i];
		for(int j = 0; j < currentRow.Num(); ++j)
		{
			if(m_pomColors[i][j] == PomColors::None)
				continue;
			matchedIndices.Empty();
			CheckPom(i, j, m_pomColors[i][j], matchedIndices);
			if(matchedIndices.Num() >= COUNT_TO_CLEAR)
			{
				for (auto& matchedIndex : matchedIndices)
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
			}
		}
	}
}

void AMyGameState::CheckPom(int row, int column, PomColors& colorToMatch, TArray<ArrayIndex>& matchedIndices)
{
	// We've already tested this spot
	if(m_explored[row][column])
		return;
		
	m_explored[row][column] = true;
	
	if(m_pomColors[row][column] == PomColors::None)
		return;
	
	// Test different directions
	const PomColors currentColor = m_pomColors[row][column];
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

void AMyGameState::ResetArrays()
{
	m_shouldPositionBeCleared.Empty();
	m_explored.Empty();
	
	const int maxHeight = m_poms.Num();
	m_shouldPositionBeCleared.Init(
		TArray<bool>(false, COLUMN_COUNT),
		maxHeight
	);
	m_explored.Init(
		TArray<bool>(false, COLUMN_COUNT),
		maxHeight
	);
}

