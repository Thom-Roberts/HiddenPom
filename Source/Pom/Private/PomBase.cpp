// Fill out your copyright notice in the Description page of Project Settings.


#include "PomBase.h"

// Sets default values
APomBase::APomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void APomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

PomColor APomBase::GetPomColor_Implementation()
{
	return pomColor;
}

void APomBase::SetPosition_Implementation(FVector position)
{
	SetActorLocation(position);
}

