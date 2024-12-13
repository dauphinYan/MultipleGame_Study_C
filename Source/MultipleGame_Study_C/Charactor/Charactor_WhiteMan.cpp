// Fill out your copyright notice in the Description page of Project Settings.


#include "Charactor_WhiteMan.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "MultipleGame_Study_C/Component/CombatComponent.h"
#include "MultipleGame_Study_C/Weapon/Weapon.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AnimInstance_WhiteMan.h"


ACharactor_WhiteMan::ACharactor_WhiteMan()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//Initial character camera
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraSpringArm->SetupAttachment(GetMesh());
	CameraSpringArm->TargetArmLength = 600.f;
	CameraSpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);
	

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void ACharactor_WhiteMan::BeginPlay()
{
	Super::BeginPlay();

}

void ACharactor_WhiteMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);
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
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ACharactor_WhiteMan::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ACharactor_WhiteMan::AimButtonReleased);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharactor_WhiteMan::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACharactor_WhiteMan::FireButtonReleased);
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

void ACharactor_WhiteMan::PlayFireMontage(bool bIsAiming)
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr)
		return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
		FName SectionName;
		SectionName = bIsAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
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

void ACharactor_WhiteMan::Jump()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
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
	if (Combat)
	{
		Combat->SetAiming(true);
	}
}

void ACharactor_WhiteMan::AimButtonReleased()
{
	if (Combat)
	{
		Combat->SetAiming(false);
	}
}

void ACharactor_WhiteMan::FireButtonPressed()
{
	if (Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void ACharactor_WhiteMan::FireButtonReleased()
{
	if (Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void ACharactor_WhiteMan::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr)
		return;
	FVector velocity = GetVelocity();
	velocity.Z = 0.f;
	float speed = velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (speed == 0.f && !bIsInAir)
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		bUseControllerRotationYaw = false;
	}

	if (speed > 0.f || bIsInAir)
	{
		StartAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
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

void ACharactor_WhiteMan::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
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

bool ACharactor_WhiteMan::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool ACharactor_WhiteMan::IsAiming()
{
	return (Combat && Combat->bIsAiming);
}
