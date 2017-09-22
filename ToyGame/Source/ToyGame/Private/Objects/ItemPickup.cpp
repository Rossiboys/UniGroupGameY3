// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemPickup.h"
#include "Components/PrimitiveComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Components/BoxComponent.h"
#include "ToyGamePlayerController.h"
#include "ToyGameCharacter.h"
#include "GameFramework/Actor.h"


AItemPickup::AItemPickup()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItemPickup::OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnPlayerEnterPickupBox(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	auto PlayerCharacter = Cast<AToyGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerCharacter)
	{
		PlayerCharacter->AddBrick();
	}
}