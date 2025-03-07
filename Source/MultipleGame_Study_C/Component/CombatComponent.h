// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MultipleGame_Study_C/HUD/HUD_Character.h"
#include "MultipleGame_Study_C/Weapon/WeaponTypes.h"
#include "MultipleGame_Study_C/CharacterTypes/CombatState.h"
#include "CombatComponent.generated.h"

class AWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLEGAME_STUDY_C_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ACharactor_WhiteMan;

public:
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(AWeapon* WeaponToEquip);
protected:
	virtual void BeginPlay() override;

	void SetAiming(bool bAiming);

	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bAiming);

	void FireButtonPressed(bool bPressed);

	void Fire();

	UFUNCTION(Server, Reliable)
	void Server_Fire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire(const FVector_NetQuantize& TraceHitTarget);

	void SetHUDCrosshairs(float DeltaTime);
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);


public:
	void Reload();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

protected:
	UFUNCTION(Server, Reliable)
	void Server_Reload();

	void HandleReload();

	int32 AmountToReload();

private:
	class ACharactor_WhiteMan* Character_WhiteMan;
	class APlayerController_Character* PlayerController;
	class AHUD_Character* HUD;

	FHUDPackage HUDPackage;
	float CrosshairsVelocityFactor;
	float CrosshairsInAirFactor = 0.f;
	float CrosshairsAimFactor;
	float CrosshairsShootFactor;

	float DefaultFOV;
	float CurrentFOV;

	UPROPERTY(EditAnywhere)
	float ZoomFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UPROPERTY(Replicated)
	bool bIsAiming;

	UPROPERTY(EditAnyWhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnyWhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	FTimerHandle FireTimer;

	bool bCanFire = true;
	bool CanFire();

	void StartFireTimer();
	void FireTimerFinished();

	FHitResult HitResult;

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxCarriedAmmo = 500;

	UPROPERTY(EditDefaultsOnly)
	int32 StartingAirAmmo = 30;

	UPROPERTY(EditDefaultsOnly)
	int32 StartingRocketAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 StartingPistolAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 StartingSMGAmmo = 60;

	UPROPERTY(EditDefaultsOnly)
	int32 StartingShotGunAmmo = 15;

	UPROPERTY(EditDefaultsOnly)
	int32 StartingSniperRifleAmmo = 15;

	UPROPERTY(EditDefaultsOnly)
	int32 StartingGrenadeLauncherAmmo = 0;

	void InitializeCarriedAmmo();
	void UpdateAmmoValues();
};
