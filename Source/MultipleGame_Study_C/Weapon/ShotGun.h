// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLEGAME_STUDY_C_API AShotGun : public AHitScanWeapon
{
	GENERATED_BODY()
protected:
	virtual void Fire(const FVector& HitTarget) override;

private:
	UPROPERTY(EditDefaultsOnly)
	uint32 NumberOfPellets = 10;
};
