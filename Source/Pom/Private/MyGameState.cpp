// Fill out your copyright notice in the Description page of Project Settings.
#include "MyGameState.h"

AMyGameState::AMyGameState()
{
	m_previewPosition.SetComponents(
	FQuat::MakeFromEuler(FVector(0)),
	FVector(-20, 0, 550),
	FVector(1)
	);
	m_activePosition.SetComponents(
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

APomBase* AMyGameState::SpawnNewPomInPreviewPosition()
{
	const auto spawnedActor = GetWorld()->SpawnActor(PomClass, &m_previewPosition, m_spawnParameters);
	if(!IsValid(spawnedActor))
	{
		printf("Failed to spawn actor in preview position. This should never happen");
		return nullptr;
	}
	return Cast<APomBase>(spawnedActor);
}

// TODO: Implement preview pom
void AMyGameState::SetActivePom(bool& success, APomBase* newPom)
{
	if(newPom == nullptr)
		newPom = nextPom;
	const bool moved = newPom->SetActorLocation(m_activePosition.GetLocation());
	if(!moved)
	{
		success = false;
		return;
	}
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APlayerBase* castPawn = Cast<APlayerBase>(playerPawn);
	castPawn->SetCurrentPom(Cast<APomBase>(newPom));
	success = true;
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
		m_poms.Add(m_nullptrArray);
	
	if(m_poms[row].Num() < COLUMN_COUNT)
		m_poms[row].Init(nullptr, COLUMN_COUNT);
	m_poms[row][column] = pom;
}

void AMyGameState::Lose_Implementation()
{
	m_gameOver = true;
#if UE_BUILD_DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::White, TEXT("Lose"));
	#endif
}

void AMyGameState::Restart_Implementation()
{
	m_gameOver = false;
}

void AMyGameState::SpawnRow_Implementation()
{
	UWorld* world = GetWorld();
	const int32 xPos = -20;
	const int32 columnDistance = 100;
	FTransform spawnTransform;
	spawnTransform.SetComponents(
	FQuat::MakeFromEuler(FVector(0)),
	FVector(xPos, 0, 0),
	FVector(1)
	);
	
	for(int32 i = 0; i < COLUMN_COUNT; i++)
	{
		const int32 yOffset = (i - 2) * columnDistance;
		spawnTransform.SetLocation(
			CreatePomPositionVector(0, i)
		);
		AActor* spawnedPom = world->SpawnActorAbsolute(PomClass, spawnTransform);
		APomBase* pom = Cast<APomBase>(spawnedPom);
		pom->m_shouldTriggerOverlaps = true;
		pom->BecomeInactive();
	}
}

void AMyGameState::SpawnInitialRows_Implementation()
{
	UWorld* world = GetWorld();
	const int32 xPos = -20;
	const int32 columnDistance = 100;

	FTransform spawnTransform;
	spawnTransform.SetComponents(
	FQuat::MakeFromEuler(FVector(0)),
	FVector(xPos, 0, 0),
	FVector(1)
	);
	
	for(int32 i = 0; i < COLUMN_COUNT; i++)
	{
		spawnTransform.SetLocation(
			CreatePomPositionVector(0, i)
		);
		AActor* spawnedPom = world->SpawnActorAbsolute(PomClass, spawnTransform);
		APomBase* pom = Cast<APomBase>(spawnedPom);
		pom->m_shouldTriggerOverlaps = false;
		pom->BecomeInactive();
	}

	printf("Above would be cleared");
	// Ensure we don't have any initial clearings
	while(WouldPomsBeCleared())
	{
		// Randomize colors and try again
		for(int32 i = 0; i < m_poms.Num(); i++)
		{
			for(int32 j = 0; j < COLUMN_COUNT; j++)
			{
				PomColor color = GenerateRandomColor();
				m_poms[i][j]->SetPomColor(color);
				m_pomColors[i][j] = color;
			}
		}
	}
}

void AMyGameState::ClearPoms(int32& countCleared)
{
	// Initialize arrays to false
	ResetArray(m_shouldPositionBeCleared);
	
	TArray<ArrayIndex> matchedIndices;
	// See if poms should have themselves removed
	for(int i = 0; i < m_pomColors.Num(); i++)
	{
		const auto currentRow = m_pomColors[i];
		for(int j = 0; j < currentRow.Num(); j++)
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
	countCleared = 0;
	for(int i = 0; i < m_shouldPositionBeCleared.Num(); i++)
	{
		const auto currentRow = m_shouldPositionBeCleared[i];
		for(int j = 0; j < currentRow.Num(); j++)
		{
			if(currentRow[j])
			{
				m_poms[i][j]->FadeThenDestroy();
				ClearPomFromPosition(i, j);
				countCleared++;
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

	// This has already been targeted by a previous checkPom call
	if(m_shouldPositionBeCleared[row][column])
		return;
	
	const PomColor currentColor = m_pomColors[row][column];
	// Color doesn't match
	if(currentColor != colorToMatch)
		return;
	
	matchedIndices.Add({
		row, column
	});
	
	// Test different directions
	if(column > 0)
		CheckPom(row, column - 1, colorToMatch, matchedIndices);
	if(column < COLUMN_COUNT)
		CheckPom(row, column + 1, colorToMatch, matchedIndices);
	if(row > 0)
		CheckPom(row - 1, column, colorToMatch, matchedIndices);
	if(row < m_pomColors.Num())
		CheckPom(row + 1, column, colorToMatch, matchedIndices);
}

void AMyGameState::MovePomsDown()
{
	printf("Before move");
	// Adjust array
	for(int i = m_poms.Num() - 1; i > 0; i--)
	{
		TArray<APomBase*>& currentRow = m_poms[i];
		for(int j = 0; j < currentRow.Num(); j++)
		{
			if(!m_poms[i-1][j])
			{
				m_poms[i-1][j] = currentRow[j];
				m_pomColors[i-1][j] = m_pomColors[i][j];
				ClearPomFromPosition(i, j);
			}
		}		
	}

	printf("After move");
	// Adjust positions
	for(int i = 0; i < m_poms.Num(); i++)
	{
		TArray<APomBase*>& currentRow = m_poms[i];
		for(int j = 0; j < currentRow.Num(); j++)
		{
			if(currentRow[j] != nullptr)
				currentRow[j]->SetPosition(CreatePomPositionVector(i, j));
		}
	}
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

FVector AMyGameState::CreatePomPositionVector(int32 row, int32 column)
{
	const int32 rowDistance = 100;
	const int32 columnDistance = 100;
	const int32 xPosition = -20;
	const int32 yPosition = (column - 2) * columnDistance;
	return FVector(xPosition, yPosition, row * rowDistance);
}

void AMyGameState::ClearPomFromPosition(int32 row, int32 column)
{
	m_poms[row][column] = nullptr;
	m_pomColors[row][column] = PomColor::None;
}

bool AMyGameState::WouldPomsBeCleared()
{
	bool foundMatch = false;
	TArray<ArrayIndex> matchedIndices;

	// Initialize the cleared array (otherwise overflow will occur)
	ResetArray(m_shouldPositionBeCleared);

	// See if poms should have themselves removed
	for(int i = 0; i < m_pomColors.Num(); i++)
	{
		const auto currentRow = m_pomColors[i];
		for(int j = 0; j < currentRow.Num(); j++)
		{
			if(m_pomColors[i][j] == PomColor::None)
				continue;
			matchedIndices.Empty();
			ResetArray(m_explored);
			CheckPom(i, j, m_pomColors[i][j], matchedIndices);
			if(matchedIndices.Num() >= COUNT_TO_CLEAR)
			{
				foundMatch = true;
				break;
			}
		}
	}

	ResetArray(m_explored);
	return foundMatch;
}

PomColor AMyGameState::GenerateRandomColor(PomColor colorToExclude)
{
	// TODO: Improve this awful func :D
	TArray<PomColor> colors;
	if(colorToExclude != PomColor::Red)
		colors.Add(PomColor::Red);
	if(colorToExclude != PomColor::Yellow)
		colors.Add(PomColor::Yellow);
	if(colorToExclude != PomColor::Green)
		colors.Add(PomColor::Green);
	if(colorToExclude != PomColor::Blue)
		colors.Add(PomColor::Blue);
	
	const int32 randomIndex = FMath::RandRange(0, colors.Num() - 1);
	return colors[randomIndex];
}
