// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Character.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "MultipleGame_Study_C/GamePlay/PlayerController_Character.h"

void APlayerState_Character::AddToScore(float ScoreAmount)
{
	Score += ScoreAmount;
	Character = Character == nullptr ? Cast<ACharactor_WhiteMan>(GetPawn()) : Character;
	if (Character)
	{
		PlayerController = PlayerController == nullptr ? Cast<APlayerController_Character>(Character->Controller) : PlayerController;
		if (PlayerController)
		{
			PlayerController->SetHUDScore(Score);
		}
	}
}

void APlayerState_Character::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr ? Cast<ACharactor_WhiteMan>(GetPawn()) : Character;
	if (Character)
	{
		PlayerController = PlayerController == nullptr ? Cast<APlayerController_Character>(Character->Controller) : PlayerController;
		if (PlayerController)
		{
			PlayerController->SetHUDScore(Score);
		}
	}
}
