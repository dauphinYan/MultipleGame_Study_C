// Fill out your copyright notice in the Description page of Project Settings.


#include "Charactor_WhiteMan.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


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


