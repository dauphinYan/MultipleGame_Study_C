// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Character.h"
#include "PlayerController_Character.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void AGameMode_Character::PlayerEliminated(ACharactor_WhiteMan* ElimmedCharacter, APlayerController_Character* VictimController, APlayerController_Character* AttackerController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void AGameMode_Character::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*>PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}
