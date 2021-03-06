// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Pickups.h"
#include "ItemPickup.generated.h"

/**
 * 
 */
UCLASS()
class TOYGAME_API AItemPickup : public APickups
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AItemPickup();

	virtual void OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	
};
