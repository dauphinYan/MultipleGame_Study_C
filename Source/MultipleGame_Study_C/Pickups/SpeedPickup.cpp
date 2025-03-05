#include "SpeedPickup.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "MultipleGame_Study_C/Component/BuffComponent.h"

void ASpeedPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ACharactor_WhiteMan* Character = Cast<ACharactor_WhiteMan>(OtherActor);
	if (Character)
	{
		UBuffComponent* Buff = Character->GetBuff();
		if (Buff)
		{
			Buff->BuffSpeed(BaseSpeedBuff, CrouchSpeedBuff, SpeedBuffTime);
			
		}
	}
	Destroy();
}