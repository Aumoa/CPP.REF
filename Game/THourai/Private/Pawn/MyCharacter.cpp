// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Pawn/MyCharacter.h"

#include "Components/SpringArmComponent.h"
#include "Components/CameraComponent.h"
#include "Components/InputComponent.h"

AMyCharacter::AMyCharacter()
	: velocity(Vector3::Zero)

	, bWKeyDown(false)
	, bSKeyDown(false)
	, bAKeyDown(false)
	, bDKeyDown(false)
{
	PrimaryActorTick.bCanEverTick = true;

	auto* springArm = AddComponent<SpringArmComponent>();
	auto* camera = AddComponent<CameraComponent>();

	springArm->AttachToComponent(RootComponent);
	camera->AttachToSocket(springArm, SpringArmComponent::SocketName);

	springArm->SpringArmLength = 5.0f;
}

AMyCharacter::~AMyCharacter()
{

}

void AMyCharacter::Tick(Seconds deltaTime)
{
	Super::Tick(deltaTime);

	UpdateMovement();

	Vector3 delta = velocity * deltaTime.Value;
	RootComponent->Location += delta;
}

void AMyCharacter::SetupPlayerInputComponent(InputComponent* inPlayerInput)
{
	inPlayerInput->GetKeyActionBinder(EKey::W, EKeyEvent::Pressed).AddRaw(this, &AMyCharacter::MovementKeyPressed);
	inPlayerInput->GetKeyActionBinder(EKey::W, EKeyEvent::Released).AddRaw(this, &AMyCharacter::MovementKeyPressed);
	inPlayerInput->GetKeyActionBinder(EKey::A, EKeyEvent::Pressed).AddRaw(this, &AMyCharacter::MovementKeyPressed);
	inPlayerInput->GetKeyActionBinder(EKey::A, EKeyEvent::Released).AddRaw(this, &AMyCharacter::MovementKeyPressed);
	inPlayerInput->GetKeyActionBinder(EKey::S, EKeyEvent::Pressed).AddRaw(this, &AMyCharacter::MovementKeyPressed);
	inPlayerInput->GetKeyActionBinder(EKey::S, EKeyEvent::Released).AddRaw(this, &AMyCharacter::MovementKeyPressed);
	inPlayerInput->GetKeyActionBinder(EKey::D, EKeyEvent::Pressed).AddRaw(this, &AMyCharacter::MovementKeyPressed);
	inPlayerInput->GetKeyActionBinder(EKey::D, EKeyEvent::Released).AddRaw(this, &AMyCharacter::MovementKeyPressed);
}

void AMyCharacter::UpdateMovement()
{
	velocity = 0;

	if (bWKeyDown)
	{
		velocity.X += 1.0f;
	}
	if (bSKeyDown)
	{
		velocity.X -= 1.0f;
	}
	if (bAKeyDown)
	{
		velocity.Y -= 1.0f;
	}
	if (bDKeyDown)
	{
		velocity.Y += 1.0f;
	}

	float speed = 1.0f;
	velocity *= speed;
}

void AMyCharacter::MovementKeyPressed(EKey inKey, EKeyEvent inKeyEvent)
{
	auto UpdateKey = [inKeyEvent]()
	{
		return inKeyEvent == EKeyEvent::Pressed;
	};

	switch (inKey)
	{
	case EKey::W:
		bWKeyDown = UpdateKey();
		break;
	case EKey::A:
		bAKeyDown = UpdateKey();
		break;
	case EKey::S:
		bSKeyDown = UpdateKey();
		break;
	case EKey::D:
		bDKeyDown = UpdateKey();
		break;
	}
}