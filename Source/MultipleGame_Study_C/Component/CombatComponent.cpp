// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "MultipleGame_Study_C/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/ActorComponent.h"
#include "MultipleGame_Study_C/GamePlay/PlayerController_Character.h"
#include "Camera/CameraComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 300.f;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character_WhiteMan)
	{
		Character_WhiteMan->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		if (Character_WhiteMan->GetFollowCamera())
		{
			DefaultFOV = Character_WhiteMan->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}

}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character_WhiteMan && Character_WhiteMan->IsLocallyControlled())
	{
		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
		TraceUnderCrosshairs(HitResult);
	}
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bIsAiming);
}

void UCombatComponent::OnRegister()
{
	Super::OnRegister();
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character_WhiteMan == nullptr || WeaponToEquip == nullptr)
		return;
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandRSocket = Character_WhiteMan->GetMesh()->GetSocketByName(FName("hand_r_socket"));
	if (HandRSocket)
	{
		HandRSocket->AttachActor(EquippedWeapon, Character_WhiteMan->GetMesh());
	}
	EquippedWeapon->SetOwner(Character_WhiteMan);
	Character_WhiteMan->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character_WhiteMan->bUseControllerRotationYaw = true;
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character_WhiteMan)
	{
		Character_WhiteMan->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character_WhiteMan->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr)
		return;

	if (bIsAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime, EquippedWeapon->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}
	if (Character_WhiteMan && Character_WhiteMan->GetFollowCamera())
	{
		Character_WhiteMan->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}

void UCombatComponent::SetAiming(bool bAiming)
{
	this->bIsAiming = bAiming;
	Server_SetAiming(bAiming);
	if (Character_WhiteMan)
	{
		Character_WhiteMan->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (bFireButtonPressed)
	{
		Server_Fire(HitResult.ImpactPoint);

		if (EquippedWeapon)
		{
			CrosshairsShootFactor = 1.f;
		}
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if (Character_WhiteMan)
		{
			float DistanceToCharacter = (Character_WhiteMan->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
			
		}

		FVector End = Start + CrosshairWorldDirection * 80000.f;
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>())
		{
			HUDPackage.CrosshairsColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairsColor = FLinearColor::White;
		}
	}
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (Character_WhiteMan == nullptr || Character_WhiteMan->GetController() == nullptr)
	{
		return;
	}

	if (PlayerController == nullptr)
	{
		PlayerController = Cast<APlayerController_Character>(Character_WhiteMan->GetController());
	}
	else
	{
		if (HUD == nullptr)
		{
			HUD = Cast<AHUD_Character>(PlayerController->GetHUD());
		}
		else
		{
			if (EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
			}
			else
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
			}

			FVector2D WalkSpeedRange(0.f, Character_WhiteMan->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = Character_WhiteMan->GetVelocity();
			Velocity.Z = 0.f;
			CrosshairsVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (Character_WhiteMan->GetMovementComponent()->IsFalling())
			{
				CrosshairsInAirFactor = FMath::FInterpTo(CrosshairsInAirFactor, 1.0f, DeltaTime, 15.0f);
			}
			else
			{
				CrosshairsInAirFactor = FMath::FInterpTo(CrosshairsInAirFactor, 0.f, DeltaTime, 30.0f);
			}
			if (bIsAiming)
			{
				CrosshairsAimFactor = FMath::FInterpTo(CrosshairsAimFactor, 0.58f, DeltaTime, 30.f);
			}
			else
			{
				CrosshairsAimFactor = FMath::FInterpTo(CrosshairsAimFactor, 0.f, DeltaTime, 30.f);
			}
			CrosshairsShootFactor = FMath::FInterpTo(CrosshairsShootFactor, 0.f, DeltaTime, 2.f);

			HUDPackage.CrosshairsSpreadScale = 0.4f + CrosshairsVelocityFactor + CrosshairsInAirFactor - CrosshairsAimFactor + CrosshairsShootFactor;
			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UCombatComponent::Multicast_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (EquippedWeapon == nullptr)
		return;
	if (Character_WhiteMan)
	{
		Character_WhiteMan->PlayFireMontage(bIsAiming);
		EquippedWeapon->Fire(TraceHitTarget);
	}
}

void UCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	Multicast_Fire(TraceHitTarget);
}

void UCombatComponent::Server_SetAiming_Implementation(bool bAiming)
{
	this->bIsAiming = bAiming;
	if (Character_WhiteMan)
	{
		Character_WhiteMan->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}
