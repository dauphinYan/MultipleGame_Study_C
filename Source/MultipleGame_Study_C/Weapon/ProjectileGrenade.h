// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileGrenade.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLEGAME_STUDY_C_API AProjectileGrenade : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectileGrenade();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

private:
	UPROPERTY(EditDefaultsOnly)
	USoundWave* BounceSound;
};
