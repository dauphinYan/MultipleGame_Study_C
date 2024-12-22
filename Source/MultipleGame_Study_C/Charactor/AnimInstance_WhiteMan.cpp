// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_WhiteMan.h"
#include "Charactor_WhiteMan.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MultipleGame_Study_C/CharacterTypes/CombatState.h"

void UAnimInstance_WhiteMan::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character_WhiteMan = Cast<ACharactor_WhiteMan>(TryGetPawnOwner());
}

void UAnimInstance_WhiteMan::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Character_WhiteMan == nullptr)
	{
		Character_WhiteMan = Cast<ACharactor_WhiteMan>(TryGetPawnOwner());
		if (Character_WhiteMan == nullptr)
			return;
	}

	FVector Velocity = Character_WhiteMan->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	bIsInAir = Character_WhiteMan->GetMovementComponent()->IsFalling();
	bIsAccelerating = Character_WhiteMan->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 ? true : false;
	bIsCrouching = Character_WhiteMan->bIsCrouched;
	bWeaponEquipped = Character_WhiteMan->IsWeaponEquipped();
	bIsAiming = Character_WhiteMan->IsAiming();
	TurningInPlace = Character_WhiteMan->GetTurningInPlace();
	bElimmed = Character_WhiteMan->IsElimmed();

	FRotator AimRotation = Character_WhiteMan->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Character_WhiteMan->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;
	CharactorRotationLastFrame = CharactorRotation;
	CharactorRotation = Character_WhiteMan->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharactorRotation, CharactorRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = Character_WhiteMan->GetAO_Yaw();
	AO_Pitch = Character_WhiteMan->GetAO_Pitch();

	bUseFABRIK = Character_WhiteMan->GetCombatState() != ECombatState::ECS_Reloading;
}
