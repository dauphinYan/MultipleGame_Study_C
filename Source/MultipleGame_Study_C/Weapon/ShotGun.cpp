// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

void AShotGun::Fire(const FVector& HitTarget)
{
	/*Super::Fire(HitTarget);*/

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = TraceEndWithScatter(Start, HitTarget);


	}
}
