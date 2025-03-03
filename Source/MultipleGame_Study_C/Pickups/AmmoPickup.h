#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "MultipleGame_Study_C/Weapon/WeaponTypes.h"
#include "AmmoPickup.generated.h"

UCLASS()
class MULTIPLEGAME_STUDY_C_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditDefaultsOnly)
	int32 AmmoAmount = 30;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;
};
