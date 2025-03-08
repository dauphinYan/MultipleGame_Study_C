#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultipleGame_Study_C/CharacterTypes/TurningInPlace.h"
#include "MultipleGame_Study_C/Interface/InteractWithCrosshairsInterface.h"
#include "MultipleGame_Study_C/CharacterTypes/CombatState.h"
#include "Charactor_WhiteMan.generated.h"

UCLASS()
class MULTIPLEGAME_STUDY_C_API ACharactor_WhiteMan : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ACharactor_WhiteMan();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	void PlayFireMontage(bool bIsAiming);
	void PlayElimMontage();
	void PlayReloadMontage();

	void Elim();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Elim();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	void UpdateHealth();

	UPROPERTY()
	TMap<FName, class UBoxComponent*> HitCollisionBoxes;

protected:
	virtual void BeginPlay() override;
	void PollInit();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnLeft(float Value);
	void TurnUp(float Value);

	virtual void Jump() override;

	void CrouchButtonPressed();
	void EquipButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void ReloadButtonPressed();
	void PlayHitReactMontage();

	void AimOffset(float DeltaTime);

	UFUNCTION()
	void ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* head;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* pelvis;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* spine_03;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* chlf_r;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* chlf_l;

private:
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBuffComponent* Buff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ULagCompensationComponent* LagCompensation;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY()
	class APlayerController_Character* CharacterPlayerController;

	UPROPERTY()
	class APlayerState_Character* CharacterPlayerState;

	UFUNCTION(Server, Reliable)
	void Server_EquipButtonPressed();

	float AO_Yaw;
	//UPROPERTY(Replicated)
	float AO_Pitch;
	FRotator StartAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float  DeltaTime);

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	UPROPERTY(EditAnywhere, Category = "Player State")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_CurHealth, VisibleAnywhere, Category = "Player State")
	float CurHealth = 100.f;

	UFUNCTION()
	void OnRep_CurHealth(float LastHealth);

	bool bElimmed = false;
	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	void ElimTimerFinished();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> DefaultWeaponClass;

	void SpawnDefaultWeapon();

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; };
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; };
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; };
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return Camera; }
	FORCEINLINE bool IsElimmed()const { return bElimmed; }
	FORCEINLINE float GetCurHealth()const { return CurHealth; }
	FORCEINLINE void SetHealth(float Amount) { CurHealth = Amount; }
	FORCEINLINE float GetMaxHealth()const { return MaxHealth; }
	ECombatState GetCombatState() const;
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }
};
