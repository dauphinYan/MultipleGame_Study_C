// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLEGAME_STUDY_C_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectileRocket();

protected:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& Hit) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* TrailSystem;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* RocketMesh;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditDefaultsOnly)
	float DestroyTime = 3.f;
};
