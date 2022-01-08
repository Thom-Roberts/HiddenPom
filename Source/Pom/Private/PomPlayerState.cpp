// Fill out your copyright notice in the Description page of Project Settings.


#include "PomPlayerState.h"

APomPlayerState::APomPlayerState()
{
	craftPoints = 0;
	
	baseMoveSpeed = 20.0;
	moveSpeed = baseMoveSpeed;
	moveSpeedIncrease = 5.0;
	maxMoveSpeed = 200.0;
	heldDownMoveSpeed = 500.0;
	
	movesUntilNextRowSpawn = 3;
}
