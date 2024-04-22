// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Player/MMCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameObject/Player/MMCharacterControlData.h"

AMMCharacterPlayer::AMMCharacterPlayer()
{
	//Camera 
	_cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamearaBoom"));
	_cameraBoom->SetupAttachment(RootComponent);
	_cameraBoom->TargetArmLength = 400.0f;
	_cameraBoom->bUsePawnControlRotation = true;

	_followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCam"));
	_followCamera->SetupAttachment(_cameraBoom, USpringArmComponent::SocketName);
	_followCamera->bUsePawnControlRotation = false;

}

void AMMCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	// PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMMCharacterPlayer::Attack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMMCharacterPlayer::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight") , this, &AMMCharacterPlayer::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMMCharacterPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMMCharacterPlayer::LookUp);
}

void AMMCharacterPlayer::SetCharacterControlData(const UMMCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	_cameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	_cameraBoom->SetRelativeRotation( CharacterControlData->RelativeRotation);
	_cameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	_cameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	_cameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	_cameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	_cameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
} 
void AMMCharacterPlayer::BeginPlay()
{
	Super::BeginPlay(); 
}

void AMMCharacterPlayer::UpDown(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AMMCharacterPlayer::LeftRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void AMMCharacterPlayer::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AMMCharacterPlayer::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}
