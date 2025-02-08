// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_Character.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLEGAME_STUDY_C_API APlayerController_Character : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDTime();
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	virtual float GetServerTime();
	virtual void ReceivedPlayer() override;
	void CheckTimeSync(float DeltaTime);
	void OnMatchStateSet(FName State);
protected:
	virtual void BeginPlay() override;

	void PollInit();


	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(float TimeOfClientRequest);

	UFUNCTION(Client, Reliable)
	void Client_ReportServerTime(float TimeOfClientRequest, float TimeServerReceiveClientRequest);

	float ClientServerDelta = 0.f;

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSysncRunningTime = 0.f;
private:
	class AHUD_Character* CharacterHUD;
	class ACharactor_WhiteMan* Character_WhiteMan;

	float MatchTime = 120.f;
	uint32 CountdownInt = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
};
