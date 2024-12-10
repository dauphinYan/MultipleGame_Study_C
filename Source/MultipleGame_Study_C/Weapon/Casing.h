// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class MULTIPLEGAME_STUDY_C_API ACasing : public AActor
{
	GENERATED_BODY()
	
public:	
	ACasing();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CasingMesh;

	UPROPERTY(EditAnyWhere)
	float ShellEjectionImpulse;

	UPROPERTY(EditAnyWhere)
	class USoundCue* ShellSound;
};
