// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"
#include "MultipleGame_Study_C/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	//if (Character_WhiteMan)
	//{
	//	Character_WhiteMan->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	//}

}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
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