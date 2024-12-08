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
	}

}



void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		Server_Fire(HitResult.ImpactPoint);
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
		FVector End = Start + CrosshairWorldDirection * 80000.f;
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
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
