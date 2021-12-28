// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PomBase.generated.h"

UCLASS()
class POM_API APomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APomBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

UENUM(Blueprintable)
enum PomColors
{
	None     UMETA(DisplayName = "None"),
	Red      UMETA(DisplayName = "Red"),
	Yellow   UMETA(DisplayName = "Yellow"),
	Green	 UMETA(DisplayName = "Green"),
	Blue	 UMETA(DisplayName = "Blue")
};