// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);
	if (!HasAuthority())
		return;
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("Muzzle"));

	//print socket name
	USkeletalMeshComponent* WeaponMesh1 = GetWeaponMesh();
	if (WeaponMesh1 && WeaponMesh1->SkeletalMesh)
	{
		const TArray<USkeletalMeshSocket*>& AllSockets = WeaponMesh1->SkeletalMesh->GetSkeleton()->Sockets;
		for (const USkeletalMeshSocket* Socket : AllSockets)
		{
			if (Socket)
			{
				UE_LOG(LogTemp, Log, TEXT("Socket Name: %s, Bone Name: %s"),
					*Socket->SocketName.ToString(),
					*Socket->BoneName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponMesh or its SkeletalMesh is invalid!"));
	}

	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();
		if (ProjectileClass && InstigatorPawn)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = InstigatorPawn;
				World->SpawnActor<AProjectile>(
					ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams
				);

			}
		}
	}
}
