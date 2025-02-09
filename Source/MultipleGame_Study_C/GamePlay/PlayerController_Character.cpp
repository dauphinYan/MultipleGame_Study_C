// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Character.h"
#include "MultipleGame_Study_C/HUD/HUD_Character.h"
#include "MultipleGame_Study_C/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "Net/UnrealNetwork.h"
#include "MultipleGame_Study_C/GamePlay/GameMode_Character.h"

void APlayerController_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerController_Character, MatchState);
}

void APlayerController_Character::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Character_WhiteMan = Cast <ACharactor_WhiteMan>(InPawn);
	if (Character_WhiteMan)
	{
		SetHUDHealth(Character_WhiteMan->GetCurHealth(), Character_WhiteMan->GetMaxHealth());
	}
}

void APlayerController_Character::BeginPlay()
{
	Super::BeginPlay();

	CharacterHUD = Cast<AHUD_Character>(GetHUD());
}

void APlayerController_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
}

void APlayerController_Character::CheckTimeSync(float DeltaTime)
{
	TimeSysncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSysncRunningTime > TimeSyncFrequency)
	{
		Server_RequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSysncRunningTime = 0.f;
	}
}

void APlayerController_Character::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		CharacterHUD = CharacterHUD == nullptr ? Cast<AHUD_Character>(GetHUD()) : CharacterHUD;
		UE_LOG(LogTemp, Log, TEXT("Here1."));
		if (CharacterHUD)
		{
			CharacterHUD->AddCharacterOverlay();
			PollInit();
		}
	}
}

void APlayerController_Character::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		CharacterHUD = CharacterHUD == nullptr ? Cast<AHUD_Character>(GetHUD()) : CharacterHUD;
		if (CharacterHUD)
		{
			CharacterHUD->AddCharacterOverlay();
			PollInit();
		}
	}
}

float APlayerController_Character::GetServerTime()
{
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void APlayerController_Character::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		Server_RequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void APlayerController_Character::Server_RequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	Client_ReportServerTime_Implementation(TimeOfClientRequest, ServerTimeOfReceipt);
}

void APlayerController_Character::Client_ReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	//ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
	ClientServerDelta = FMath::Lerp(ClientServerDelta, CurrentServerTime - GetWorld()->GetTimeSeconds(), 0.1f);
}

void APlayerController_Character::SetHUDHealth(float Health, float MaxHealth)
{
	if (CharacterHUD == nullptr)
	{
		CharacterHUD = Cast<AHUD_Character>(GetHUD());
	}

	if (CharacterHUD && CharacterHUD->CharacterOverlay)
	{
		const float HealthPercent = Health / MaxHealth;
		CharacterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		CharacterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void APlayerController_Character::SetHUDScore(float Score)
{
	if (CharacterHUD == nullptr)
	{
		CharacterHUD = Cast<AHUD_Character>(GetHUD());
	}

	if (CharacterHUD && CharacterHUD->CharacterOverlay)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		CharacterHUD->CharacterOverlay->ScoreAmountText->SetText(FText::FromString(ScoreText));
	}
}

void APlayerController_Character::SetHUDDefeats(int32 Defeats)
{
	if (CharacterHUD == nullptr)
	{
		CharacterHUD = Cast<AHUD_Character>(GetHUD());
	}

	if (CharacterHUD && CharacterHUD->CharacterOverlay)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		CharacterHUD->CharacterOverlay->DefeatsAmountText->SetText(FText::FromString(DefeatsText));
	}
}

void APlayerController_Character::SetHUDWeaponAmmo(int32 Ammo)
{
	if (CharacterHUD == nullptr)
	{
		CharacterHUD = Cast<AHUD_Character>(GetHUD());
	}

	if (CharacterHUD && CharacterHUD->CharacterOverlay)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		CharacterHUD->CharacterOverlay->WeaponAmmoAmountText->SetText(FText::FromString(AmmoText));
	}
}

void APlayerController_Character::SetHUDCarriedAmmo(int32 Ammo)
{
	if (CharacterHUD == nullptr)
	{
		CharacterHUD = Cast<AHUD_Character>(GetHUD());
	}

	if (CharacterHUD && CharacterHUD->CharacterOverlay)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		CharacterHUD->CharacterOverlay->CarriedAmmoAmountText->SetText(FText::FromString(AmmoText));
	}
}

void APlayerController_Character::SetHUDMatchCountdown(float CountdownTime)
{
	if (CharacterHUD == nullptr)
	{
		CharacterHUD = Cast<AHUD_Character>(GetHUD());
	}

	if (CharacterHUD && CharacterHUD->CharacterOverlay)
	{
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		CharacterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

void APlayerController_Character::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (CharacterHUD && CharacterHUD->CharacterOverlay)
		{
			CharacterOverlay = CharacterHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				Character_WhiteMan = Cast <ACharactor_WhiteMan>(GetCharacter());
				if (Character_WhiteMan)
				{
					SetHUDHealth(Character_WhiteMan->GetCurHealth(), Character_WhiteMan->GetMaxHealth());
				}

			}
		}
	}

}

void APlayerController_Character::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetServerTime());
	if (CountdownInt != SecondsLeft)
	{
		SetHUDMatchCountdown(MatchTime - GetServerTime());
	}

	CountdownInt = SecondsLeft;
}


