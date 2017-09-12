// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups.h"
#include "Components/PrimitiveComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Components/BoxComponent.h"
#include "ToyGamePlayerController.h"
#include "ToyGameCharacter.h"
#include "GameFramework/Actor.h"

// Sets default values
APickups::APickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickupRoot"));
	RootComponent = PickupRoot;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBox"));
	PickupBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickupBox->bGenerateOverlapEvents = true;
	PickupBox->OnComponentBeginOverlap.AddDynamic(this, &APickups::OnPlayerEnterPickupBox);
	PickupBox->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

void APickups::OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto PlayerCharacter = Cast<AToyGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter)
	{
		// Add a brick to the character
		PlayerCharacter->AddBrick();

		// Destroy the pickup on collision
		Destroy();
	}
}
