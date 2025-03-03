#include "AmmoPickup.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "MultipleGame_Study_C/Component/CombatComponent.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ACharactor_WhiteMan* WhiteMan = Cast<ACharactor_WhiteMan>(OtherActor);
	if (WhiteMan)
	{
		UCombatComponent* Combat = WhiteMan->GetCombat();
		if (Combat)
		{
			Combat->PickupAmmo(WeaponType, AmmoAmount);
		}
	}
	Destroy();
}
