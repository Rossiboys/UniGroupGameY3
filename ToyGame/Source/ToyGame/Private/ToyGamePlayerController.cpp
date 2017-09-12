// Fill out your copyright notice in the Description page of Project Settings.

#include "ToyGamePlayerController.h"
#include "ToyGameCharacter.h"
#include "GameFramework/Pawn.h"

AToyGameCharacter* AToyGamePlayerController::GetControlledPlayer() const
{
	return Cast<AToyGameCharacter>(GetPawn());
}