// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMode_Character.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLEGAME_STUDY_C_API AGameMode_Character : public AGameMode
{
	GENERATED_BODY()


public:
	virtual void PlayerEliminated(class ACharactor_WhiteMan* ElimmedCharacter, class APlayerController_Character* VictimController, APlayerController_Character* AttackerController);
	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController);
};
