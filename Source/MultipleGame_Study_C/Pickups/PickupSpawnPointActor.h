#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPointActor.generated.h"

UCLASS()
class MULTIPLEGAME_STUDY_C_API APickupSpawnPointActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupSpawnPointActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class APickup>> PickupClasses;

	UPROPERTY()
	APickup* SpawnedPickup;

	void SpawnPickup();
	UFUNCTION()
	void StartSpawnPickupTimer(AActor* DestroyedActor);
	void SpawnPickupTimerFinished();


private:
	FTimerHandle SpawnPickupTimer;

	UPROPERTY(EditDefaultsOnly)
	float SpawnPickupTimeMin;
	UPROPERTY(EditDefaultsOnly)
	float SpawnPickupTimeMax;
};
