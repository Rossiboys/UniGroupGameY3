// Fill out your copyright notice in the Description page of Project Settings.

#include "HazardRewind.h"
#include "Components/PrimitiveComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Components/BoxComponent.h"
#include "ToyGamePlayerController.h"
#include "ToyGameCharacter.h"
#include "GameFramework/Actor.h"

// Sets default values
AHazardRewind::AHazardRewind()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AHazardRewind::OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnPlayerEnterPickupBox(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	auto PlayerCharacter = Cast<AToyGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerCharacter)
	{
		PlayerCharacter->ActivateRewind();
	}
}
