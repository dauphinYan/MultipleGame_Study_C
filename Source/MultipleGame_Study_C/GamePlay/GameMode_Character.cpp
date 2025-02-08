// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Character.h"
#include "PlayerController_Character.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "MultipleGame_Study_C/GamePlay/PlayerState_Character.h"

AGameMode_Character::AGameMode_Character()
{
	bDelayedStart = true;
}

void AGameMode_Character::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AGameMode_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
}

void AGameMode_Character::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController_Character* Player = Cast<APlayerController_Character>(*It);
		if (Player)
		{
			Player->OnMatchStateSet(MatchState);
		}
	}
}

void AGameMode_Character::PlayerEliminated(ACharactor_WhiteMan* ElimmedCharacter, APlayerController_Character* VictimController, APlayerController_Character* AttackerController)
{
	APlayerState_Character* AttackerPlayerState = AttackerController ? Cast<APlayerState_Character>(AttackerController->PlayerState) : nullptr;
	APlayerState_Character* VictimPlayerState = VictimController ? Cast<APlayerState_Character>(VictimController->PlayerState) : nullptr;

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->AddToScore(1.f);
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}

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
