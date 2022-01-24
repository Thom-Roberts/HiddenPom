// Fill out your copyright notice in the Description page of Project Settings.


#include "PomPlayerState.h"

#include "MyGameState.h"

APomPlayerState::APomPlayerState()
{
	craftPoints = 0;
	clearScoreIncrease = 15;
	movesUntilNextRowSpawnList = { 4, 4, 4, 4, 3, 3, 3, 3, 2 };
	movesIndex = 0;
	currentMoveCount = 0;
}

void APomPlayerState::HandlePomLanded_Implementation()
{
	currentMoveCount++;
}

void APomPlayerState::HandleNewRowSpawn_Implementation()
{
	currentMoveCount = 0;
	movesIndex++;
	if(movesIndex == movesUntilNextRowSpawnList.Num())
		movesIndex--;

	Cast<AMyGameState>(GetWorld()->GetGameState());
}
