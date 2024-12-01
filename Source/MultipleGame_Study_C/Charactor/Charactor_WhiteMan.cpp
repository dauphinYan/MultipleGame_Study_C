// Fill out your copyright notice in the Description page of Project Settings.


#include "Charactor_WhiteMan.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "MultipleGame_Study_C/Component/CombatComponent.h"
#include "MultipleGame_Study_C/Weapon/Weapon.h"
#include "Net/UnrealNetwork.h"


ACharactor_WhiteMan::ACharactor_WhiteMan()
{
	PrimaryActorTick.bCanEverTick = false;

	//Initial character camera
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraSpringArm->SetupAttachment(GetMesh());
	CameraSpringArm->TargetArmLength = 600.f;
	CameraSpringArm->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);
}

void ACharactor_WhiteMan::BeginPlay()
{
	Super::BeginPlay();


}

void ACharactor_WhiteMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharactor_WhiteMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Initialize character movement
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACharactor_WhiteMan::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACharactor_WhiteMan::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnLeft"), this, &ACharactor_WhiteMan::TurnLeft);
	PlayerInputComponent->BindAxis(TEXT("TurnUp"), this, &ACharactor_WhiteMan::TurnUp);
	//Initialize character action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharactor_WhiteMan::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACharactor_WhiteMan::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ACharactor_WhiteMan::EquipButtonPressed);
}

void ACharactor_WhiteMan::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ACharactor_WhiteMan, OverlappingWeapon, COND_OwnerOnly);
}

void ACharactor_WhiteMan::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Combat)
	{
		Combat->Character_WhiteMan = this;
	}
}

void ACharactor_WhiteMan::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ACharactor_WhiteMan::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ACharactor_WhiteMan::TurnLeft(float Value)
{
	AddControllerYawInput(Value);
}

void ACharactor_WhiteMan::TurnUp(float Value)
{
	AddControllerPitchInput(-Value);
}

void ACharactor_WhiteMan::CrouchButtonPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ACharactor_WhiteMan::EquipButtonPressed()
{
	if (Combat)
	{
		if (HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			Server_EquipButtonPressed();
		}
	}
}

void ACharactor_WhiteMan::Server_EquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ACharactor_WhiteMan::AimButtonPressed()
{
}

void ACharactor_WhiteMan::AimButtonReleased()
{
}

void ACharactor_WhiteMan::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ACharactor_WhiteMan::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}
