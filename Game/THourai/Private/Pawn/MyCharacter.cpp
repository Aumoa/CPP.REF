// Copyright 2020 Aumoa.lib. All right reserved.

#include "Pawn/MyCharacter.h"

#include "Components/SpringArmComponent.h"
#include "Components/CameraComponent.h"
#include "Components/InputComponent.h"

AMyCharacter::AMyCharacter()
	: velocity(Vector3::Zero)
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

void AMyCharacter::MovementKeyPressed(EKey inKey, EKeyEvent inKeyEvent)
{
	auto GetSpeed = [inKeyEvent]()
	{
		return inKeyEvent == EKeyEvent::Pressed ? 1.0f : 0.0f;
	};

	TH_LOG(LogTH, Verbose, L"speed: {0}", GetSpeed());

	switch (inKey)
	{
	case EKey::W:
		velocity.X = GetSpeed();
		break;
	case EKey::A:
		velocity.Y = -GetSpeed();
		break;
	case EKey::S:
		velocity.X = -GetSpeed();
		break;
	case EKey::D:
		velocity.Y = GetSpeed();
		break;
	}
}