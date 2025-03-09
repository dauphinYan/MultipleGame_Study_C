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

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	bool bUseServerSideRewind = false;

	UPROPERTY()
	class APlayerController_Character* CharacterOwnerController;

	UPROPERTY()
	class ACharactor_WhiteMan* OwnerCharacter;

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

	UPROPERTY(EditDefaultsOnly)
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACasing> CasingClass;

	UPROPERTY(EditDefaultsOnly)
	int32 Ammo;

	UFUNCTION(Client, Reliable)
	void Client_UpdateAmmo(int32 ServerAmmo);

	UFUNCTION(Client, Reliable)
	void Client_AddAmmo(int32 AmmoToAdd);

	void SpendRound();

	UPROPERTY(EditDefaultsOnly)
	int32 MaxCapacity;

	//The number of unprocessed server requests for Ammo.
	int32 Sequence = 0;

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
	FORCEINLINE int32 GetDamage() const { return Damage; }
};
