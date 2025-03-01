
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class MULTIPLEGAME_STUDY_C_API AProjectile : public AActor
{
	GENERATED_BODY()

public:

	AProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* TrailSystem;

	void SpawnTrailSystem();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;

	void StartDestroyTimer();
	void DestroyTimerFinished();

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Tracer;

	class UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditDefaultsOnly)
	float DestroyTime = 3.f;
};
