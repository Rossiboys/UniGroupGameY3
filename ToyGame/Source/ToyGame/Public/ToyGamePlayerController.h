// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ToyGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOYGAME_API AToyGamePlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	class AToyGameCharacter* GetControlledPlayer() const;
	
	
};
