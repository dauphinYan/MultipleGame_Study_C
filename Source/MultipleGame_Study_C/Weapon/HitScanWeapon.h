// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLEGAME_STUDY_C_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& HitTarget) override;

protected:
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget);
	void WeaponTraceHit(const FVector& Start, const FVector& HitTarget, FHitResult& OutHit);

	UPROPERTY(EditDefaultsOnly)
	float DistanceToSphere = 800.f;

	UPROPERTY(EditDefaultsOnly)
	float SphereRadius = 75.f;

	UPROPERTY(EditDefaultsOnly)
	bool bUseScatter = false;

private:
	UPROPERTY(EditDefaultsOnly)
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* BeamParticles;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* HitSound;
};
