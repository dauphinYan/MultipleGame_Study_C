// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Character.h"
#include "MultipleGame_Study_C/HUD/HUD_Character.h"
#include "MultipleGame_Study_C/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"

void APlayerController_Character::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACharactor_WhiteMan* Character_WhiteMan = Cast <ACharactor_WhiteMan>(InPawn);
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
