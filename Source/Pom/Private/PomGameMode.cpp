// Fill out your copyright notice in the Description page of Project Settings.


#include "PomGameMode.h"

APomGameMode::APomGameMode()
{
	baseMoveSpeed = 20;
	currentMoveSpeed = baseMoveSpeed;
	moveSpeedIncrease = 20;
	maxMoveSpeed = 400;
	heldDownMoveSpeed = 800;
}

