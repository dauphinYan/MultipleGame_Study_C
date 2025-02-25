// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_Max UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class MULTIPLEGAME_STUDY_C_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ShowPickupWidget(bool bShow);
	virtual void Fire(const FVector& HitTarget);
	void Droppped();
	void AddAmmo(int32 AmmoToAdd);

	UPROPERTY(EditAnyWhere, Category = "CrossHairs")
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnyWhere, Category = "CrossHairs")
	class UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnyWhere, Category = "CrossHairs")
	class UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnyWhere, Category = "CrossHairs")
	class UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnyWhere, Category = "CrossHairs")
	class UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, Category = "CrossHairs")
	float ZoomFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = "CrossHairs")
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(EditAnywhere)
	float FireDelay = 0.15f;

	UPROPERTY(EditAnywhere)
	bool bAutomatic = true;

	void SetHUDAmmo();

	UPROPERTY(EditAnywhere)
	class USoundCue* EquipSound;

protected:
	virtual void BeginPlay() override;

	// 重叠开始时调用
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 重叠结束时调用
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* PickupWidget;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere)
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(EditAnywhere)
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
	int32 Ammo;

	UFUNCTION()
	void OnRep_Ammo();

	void SpendRound();

	UPROPERTY(EditAnywhere)
	int32 MaxCapacity;

	UPROPERTY()
	class APlayerController_Character* CharacterOwnerController;

	UPROPERTY()
	class ACharactor_WhiteMan* OwnerCharacter;

	virtual void OnRep_Owner() override;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

public:
	FORCEINLINE void SetWeaponState(EWeaponState State);
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
	bool IsEmpty();
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMaxCapacity() const { return MaxCapacity; }
};
