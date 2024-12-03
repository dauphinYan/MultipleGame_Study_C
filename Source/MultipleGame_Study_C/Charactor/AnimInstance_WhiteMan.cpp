// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_WhiteMan.h"
#include "Charactor_WhiteMan.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}
