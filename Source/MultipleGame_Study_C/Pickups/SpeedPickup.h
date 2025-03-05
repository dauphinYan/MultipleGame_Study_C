// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "SpeedPickup.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLEGAME_STUDY_C_API ASpeedPickup : public APickup
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float BaseSpeedBuff = 1600.f;
	
	UPROPERTY(EditDefaultsOnly)
	float CrouchSpeedBuff = 850.f;

	UPROPERTY(EditDefaultsOnly)
	float SpeedBuffTime = 10.f;
};
