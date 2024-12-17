// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Character.h"
#include "MultipleGame_Study_C/HUD/HUD_Character.h"
#include "MultipleGame_Study_C/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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