// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Character.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "MultipleGame_Study_C/GamePlay/PlayerController_Character.h"
#include "Net/UnrealNetwork.h"

void APlayerState_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerState_Character, Defeats);
}

void APlayerState_Character::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	Character = Character == nullptr ? Cast<ACharactor_WhiteMan>(GetPawn()) : Character;
	if (Character)
	{
		PlayerController = PlayerController == nullptr ? Cast<APlayerController_Character>(Character->Controller) : PlayerController;
		if (PlayerController)
		{
			PlayerController->SetHUDScore(GetScore());
		}
	}
}

void APlayerState_Character::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character == nullptr ? Cast<ACharactor_WhiteMan>(GetPawn()) : Character;
	if (Character)
	{
		PlayerController = PlayerController == nullptr ? Cast<APlayerController_Character>(Character->Controller) : PlayerController;
		if (PlayerController)
		{
			PlayerController->SetHUDDefeats(Defeats);
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

void APlayerState_Character::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<ACharactor_WhiteMan>(GetPawn()) : Character;
	if (Character)
	{
		PlayerController = PlayerController == nullptr ? Cast<APlayerController_Character>(Character->Controller) : PlayerController;
		if (PlayerController)
		{
			PlayerController->SetHUDDefeats(Defeats);
		}
	}
}
