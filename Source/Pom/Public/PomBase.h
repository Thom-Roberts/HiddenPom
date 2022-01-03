// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PomBase.generated.h"

UENUM(BlueprintType)
enum PomColor
{
	None     UMETA(DisplayName = "None"),
	Red      UMETA(DisplayName = "Red"),
	Yellow   UMETA(DisplayName = "Yellow"),
	Green	 UMETA(DisplayName = "Green"),
	Blue	 UMETA(DisplayName = "Blue")
};

UCLASS()
class POM_API APomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APomBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BecomeInactive();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPosition(FVector position);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetPomColor(PomColor color);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	PomColor GetPomColor();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void FadeThenDestroy();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_shouldTriggerOverlaps = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<PomColor> pomColor;
};

