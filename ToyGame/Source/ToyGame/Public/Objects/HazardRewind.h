// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Pickups.h"
#include "HazardRewind.generated.h"

UCLASS()
class TOYGAME_API AHazardRewind : public APickups
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHazardRewind();

	virtual void OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

};
