// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Character.h"
#include "PlayerController_Character.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"

void AGameMode_Character::PlayerEliminated(ACharactor_WhiteMan* ElimmedCharacter, APlayerController_Character* VictimController, APlayerController_Character* AttackerController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Multicast_Elim();
	}
}
