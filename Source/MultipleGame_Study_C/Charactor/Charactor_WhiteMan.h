// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Charactor_WhiteMan.generated.h"

UCLASS()
class MULTIPLEGAME_STUDY_C_API ACharactor_WhiteMan : public ACharacter
{
	GENERATED_BODY()

public:
	ACharactor_WhiteMan();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnLeft(float Value);
	void TurnUp(float Value);

	void CrouchButtonPressed();
	void EquipButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
	class AWeapon* OverlappingWeapon;

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
};
